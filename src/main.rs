use std::net::UdpSocket;
use std::env;

mod utils;
mod message;
mod kalman_filter;

fn main() {
    // X-Axis data
    let mut datas: Vec::<(Vec<f64>, Vec<f64>)> = Vec::<(Vec<f64>, Vec<f64>)>::new();

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
    
    let mut filter = kalman_filter::KalmanFilter::default();

    // Receive a response from the server
    let mut i = 0.0;
    let mut j = 0;

    let mut position_x: Vec::<f64> = Vec::<f64>::new();
    let mut position_y: Vec::<f64> = Vec::<f64>::new();
    let mut position_z: Vec::<f64> = Vec::<f64>::new();

    while j < 500 {
        // println!("I = {}", i);
        let message = message::Message::from_socket(&socket);

        if i == 0.0 { filter.setup(message); }
        else { filter.update(message); }
        
        filter.send(&socket);

        println!("{}", filter.position.x);
        position_x.push(filter.position.x as f64);
        position_y.push(filter.position.y as f64);
        position_z.push(filter.position.z as f64);
        i += 0.01;
        j += 1;
        println!("{}", i);
    }
    datas.push((position_x.clone(), position_y.clone()));
    utils::plot("graph.png", datas);
}
