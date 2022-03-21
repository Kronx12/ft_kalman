use crate::utils::recv_message;
use std::net::UdpSocket;
use nalgebra::{
    Vector3
};

pub struct Message {
    pub position_updated: bool,
    pub position: Vector3<f32>,
    pub acceleration: Vector3<f32>,
    pub direction: Vector3<f32>,
    pub initial_speed: f32,
}

impl Message {
    pub fn default() -> Message {
        Message {
            position_updated: false,
            position: Vector3::new(0.0, 0.0, 0.0),
            acceleration: Vector3::new(0.0, 0.0, 0.0),
            direction: Vector3::new(0.0, 0.0, 0.0),
            initial_speed: 0.0,
        }
    }

    pub fn fmt(&self) -> String {
        format!("P(x,y,z) = {} A(x,y,z) = {} D(x,y,z) = {} S = {}",
            self.position,
            self.acceleration,
            self.direction,
            self.initial_speed
        )
    }

    pub fn from_socket(socket: &UdpSocket) -> Message {
        let mut msg = Message::default();
        let mut buf: String = recv_message(socket);

        while !(buf.contains("MSG_END")) {
            let splitted = buf.split("\n").collect::<Vec<&str>>();

            if buf.contains("POSITION") {
                msg.position = Vector3::new(
                    splitted[1].parse::<f32>().unwrap(), 
                    splitted[2].parse::<f32>().unwrap(), 
                    splitted[3].parse::<f32>().unwrap());
                msg.position_updated = true;
            } else if buf.contains("ACCELERATION") {
                msg.acceleration = Vector3::new(
                    splitted[1].parse::<f32>().unwrap(), 
                    splitted[2].parse::<f32>().unwrap(),
                    splitted[3].parse::<f32>().unwrap());
            } else if buf.contains("DIRECTION") {
                msg.direction = Vector3::new(
                    splitted[1].parse::<f32>().unwrap(), 
                    splitted[2].parse::<f32>().unwrap(), 
                    splitted[3].parse::<f32>().unwrap());
            } else if buf.contains("SPEED") {
                msg.initial_speed = splitted[1].parse::<f32>().unwrap();
            }
            buf = recv_message(socket);
        }
        return msg;
    }
}