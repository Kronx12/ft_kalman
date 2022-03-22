use crate::utils::send_location;
use crate::message::Message;

use std::net::UdpSocket;
use nalgebra::{
    Vector3,
    Rotation3,
};

pub static mut HISTORY_FILTER_X: Vec<f64> = Vec::<f64>::new();
pub static mut HISTORY_FILTER_Y: Vec<f64> = Vec::<f64>::new();
pub static mut HISTORY_FILTER_Z: Vec<f64> = Vec::<f64>::new();

pub struct KalmanFilter {
    pub position: Vector3<f32>,
    pub velocity: Vector3<f32>,
    pub i: i64,
}

impl KalmanFilter {
    pub fn default() -> KalmanFilter {
        KalmanFilter {
            position: Vector3::new(0.0, 0.0, 0.0),
            velocity: Vector3::new(0.0, 0.0, 0.0),
            i: 0,
        }
    }

    pub fn setup(&mut self, message: &Message) {
        self.position = message.position;
        self.velocity = Vector3::new(message.initial_speed / 3.6, 0.0, 0.0) +  Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * 0.01;
        unsafe { self.save(); }
    }

    pub fn update(&mut self, message: &Message) {
        self.velocity += Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * 0.01;
        if message.position_updated && self.i % 300 == 0 { self.position = message.position; }
        self.position += self.velocity * 0.01;
        unsafe { self.save(); }
        self.i += 1;
    }

    pub unsafe fn save(&mut self) {
        HISTORY_FILTER_X.push(self.position.x as f64);
        HISTORY_FILTER_Y.push(self.position.y as f64);
        HISTORY_FILTER_Z.push(self.position.z as f64);
    }

    // pub fn init(&mut self) {
    // }

    // pub fn predict(&mut self) {

    // }

    // pub fn compute_gain(&mut self) {

    // }

    // pub fn compute_estimation(&mut self) {

    //     // Update position and velocity with estimations
    // }

    // pub fn compute_covariance_error(&mut self) {

    // }

    #[allow(dead_code)]
    pub fn send(&mut self, socket: &UdpSocket) {
        send_location(self.position, socket);
    }
}
