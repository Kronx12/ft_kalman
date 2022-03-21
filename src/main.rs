use std::net::UdpSocket;
use std::env;

mod utils;
mod message;
mod kalman_filter;

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
    
    let mut filter = kalman_filter::KalmanFilter::default();

    // Receive a response from the server
    let mut i = 0;
    while i < 10000 {
        let mut message = message::Message::from_socket(&socket);

        if i == 0 {
            filter.setup(message, &socket);
        } else {
            filter.update(message);
        }
        filter.send(&socket);
        i += 1;
    }
}
