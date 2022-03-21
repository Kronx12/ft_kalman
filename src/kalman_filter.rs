use crate::utils::send_location;
use crate::message::Message;

use std::net::UdpSocket;
use nalgebra::{
    Vector3,
    Rotation3
};

pub struct KalmanFilter {
    pub position: Vector3<f32>,
    pub velocity: Vector3<f32>,

    pub position_history: Vec<Vector3<f32>>,
    pub velocity_history: Vec<Vector3<f32>>,
    pub acceleration_history: Vec<Vector3<f32>>,
}

impl KalmanFilter {
    pub fn default() -> KalmanFilter {
        KalmanFilter {
            position: Vector3::new(0.0, 0.0, 0.0),
            velocity: Vector3::new(0.0, 0.0, 0.0),

            position_history: Vec::new(),
            velocity_history: Vec::new(),
            acceleration_history: Vec::new(),
        }
    }

    pub fn setup(&mut self, message: Message, socket: &UdpSocket) {
        self.position = message.position;
        self.velocity = Vector3::new(message.initial_speed / 3.6, 0.0, 0.0) + Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * 0.01;
    }

    pub fn update(&mut self, message: Message) {
        self.position_history.push(self.position);
        self.velocity_history.push(self.velocity);
        self.acceleration_history.push(message.acceleration);

        self.velocity += Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * 0.01;
        // if message.position_updated { self.position = message.position; }
        self.position += self.velocity * 0.01;
    }

    pub fn send(&mut self, socket: &UdpSocket) {
        send_location(self.position, socket);
    }
}
