use std::net::UdpSocket;
use std::env;

use crate::utils::send_location;

mod utils;
mod message;
mod kalman_filter;

fn main() {
    // Extract the port from the command line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("Usage: {} <port> [iterations]", args[0]);
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
    let mut j = 0;
    let iterations = if args.len() > 2 { args[2].parse::<u32>().unwrap() } else { 1000 };
    unsafe {
        while j < iterations {
            println!("{}", j);
            let message = message::Message::from_socket(&socket);
            if j == 0 { filter.setup(&message); }
            else { filter.update(&message); }
            filter.send(&socket);
            // send_location(message.position, &socket);
            println!("{}", filter.position);
            j += 1;
        }
        filter.debug();
        utils::plot_splitted("splitted.png");
    }
}
