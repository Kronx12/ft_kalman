use crate::utils::send_location;
use std::net::UdpSocket;
use poloto::prelude::*;
use std::env;
use nalgebra::Rotation3;
use nalgebra::Vector3;
use std::fs;

mod utils;
mod message;

fn main() {
    let gaussian = |sigma: f64, mu: f64| {
        use std::f64::consts::TAU;
        let s = sigma.powi(2);
        let k = (sigma * TAU).sqrt().recip();
        move |x: f64| (-0.5 * (x - mu).powi(2) / s).exp() * k
    };

    let range = poloto::range_iter([-5.0, 5.0], 200);

    let g1 = gaussian(1.0, 0.0);
    let g2 = gaussian(0.5, 0.0);
    let g3 = gaussian(0.3, 0.0);

    use poloto::build::line;
    let l1 = line("σ = 1.0", range.clone().map(|x| [x, g1(x)]));
    let l2 = line("σ = 0.5", range.clone().map(|x| [x, g2(x)]));
    let l3 = line("σ = 0.3", range.clone().map(|x| [x, g3(x)]));

    let canvas = poloto::render::canvas();
    let plotter = canvas
        .build_with(plots!(l1, l2, l3), [], [0.0])
        .plot("gaussian", "x", "y");
    fs::write("./graph.svg", poloto::disp(|a| plotter.simple_theme(a))).expect("Unable to write file");
    
    // Extract the port from the command line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Usage: {} <port>", args[0]);
        return;
    }
    let ip = "127.0.0.1";
    let port = args[1].parse::<u16>().unwrap();
    let server_address = format!("{}:{}", ip, port);
    let client_address = format!("{}:{}", ip, port + 1);

    println!("Server run on {:?}", server_address);
    println!("Client run on {:?}", client_address);

    // Create a socket
    let socket = UdpSocket::bind(client_address).expect("couldn't bind to client address");

    // Connect to the server
    socket.connect(server_address).expect("couldn't connect to server");

    // Send a handshake to the server
    let handshake = "READY";
    socket.send(handshake.as_bytes()).expect("couldn't send message");

    // Receive a response from the server
    let mut dt = 0;
    let mut velocity = Vector3::new(0.0, 0.0, 0.0);
    let mut position = Vector3::new(0.0, 0.0, 0.0);
    loop {
        let msg = message::Message::from_socket(&socket);
        if dt == 0 {
            velocity = Vector3::new(msg.initial_speed, 0.0, 0.0);
            position = msg.position;
        } else {
            velocity = Rotation3::from_euler_angles(msg.direction[0], msg.direction[1], msg.direction[2]) * velocity;
            position += (velocity + msg.acceleration * 0.01) * 0.01;
        }
        //println!("Message = {}", msg.fmt());
        println!("Final position = {:?}", position);
        send_location(position, &socket);
        dt += 1;
        println!("dt = {}", dt);
    }
}
