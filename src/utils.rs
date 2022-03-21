use std::net::UdpSocket;
use nalgebra::Vector3;

pub fn recv_message(socket: &UdpSocket) -> String {
    let mut buf = [0; 1024];
    let (amt, _src) = socket.recv_from(&mut buf).expect("couldn't receive message");
    let response = String::from_utf8_lossy(&buf[..amt]).to_string();
    return response;
}

pub fn send_location(v: Vector3<f32>, socket: &UdpSocket) {
    let msg = format!("{} {} {}", v[0], v[1], v[2]);
    // println!("SEND LOCATION : {}", msg);
    socket.send(msg.as_bytes()).expect("couldn't send message");
}