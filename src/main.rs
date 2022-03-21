use crate::utils::send_location;

use std::net::UdpSocket;
use std::env;
use nalgebra::{
    Rotation3,
    Vector3,
};

mod utils;
mod message;

fn main() {
    // Extract the port from the command line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Usage: {} <port>", args[0]);
        return;
    }
    let server_address = format!("127.0.0.1:{}", args[1].parse::<u16>().unwrap());
    let client_address = "127.0.0.1:34567";
    println!("Server run on {:?}", server_address);

    // Create a socket and connect to the server
    let socket = UdpSocket::bind(client_address).expect("couldn't bind to client address");
    socket.connect(server_address).expect("couldn't connect to server");
    
    // Send a handshake to the server
    let handshake = "READY";
    socket.send(handshake.as_bytes()).expect("couldn't send message");
    
    // Receive a response from the server
    let mut i = 0;
    let mut velocity = Vector3::new(0.0, 0.0, 0.0);
    let mut position = Vector3::new(0.0, 0.0, 0.0);
    while i < 10000 {
        let mut message = message::Message::from_socket(&socket);

        if i == 0 {
            velocity = Vector3::new(message.initial_speed / 3.6, 0.0, 0.0) + Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z).transform_vector(&message.acceleration) * 0.01; // TODO: Fix this
            position = message.position;
        } else {
            velocity += Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z).transform_vector(&message.acceleration) * 0.01;
        }

        if !(message.position.x == 0.0 && message.position.y == 0.0 && message.position.z == 0.0) {
            position = message.position;
            println!("I = {}", i);
            println!("{}", message.fmt());
        }


        position += velocity * 0.01;
        send_location(position, &socket);
        i += 1;
    }
}
