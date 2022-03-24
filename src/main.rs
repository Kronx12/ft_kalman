use nalgebra::{Vector3, Rotation3};
use std::net::UdpSocket;
use std::env;

use crate::utils::{identity, zeros, send_location, debug_matrix};

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

    let dt: f64 = 0.01;

    // Matrice qui relie les capteurs à la position
    let mut f = identity(6);
    f.set(0, 3, dt);
    f.set(1, 4, dt);
    f.set(2, 5, dt);

    // Matrice de transition
    let mut h = identity(6);
    h.set(3, 3, 1.);
    h.set(4, 4, 1.);
    h.set(5, 5, 1.);

    // Matrice de covariance du process noise
    let mut q = zeros((6, 6));
    q.set(0, 0, 0.1);
    q.set(1, 1, 0.1);
    q.set(2, 2, 0.1);
    q.set(3, 3, 0.011);
    q.set(4, 4, 0.011);
    q.set(5, 5, 0.011);

    // Matrice de covariance du bruit de mesure
    let r = identity(6);

    let mut filter = kalman_filter::KalmanFilter::new(f, None, h, Some(q), Some(r), Some(zeros((6, 6))), Some(zeros((6, 1))));

    let mut i = 0;
    let iterations = if args.len() > 2 { args[2].parse::<u32>().unwrap() } else { 1000 };

    let mut position = Vector3::new(0.0, 0.0, 0.0);
    let mut velocity = Vector3::new(0.0, 0.0, 0.0);
    let mut result = Vector3::new(0.0, 0.0, 0.0);

    unsafe {
        while i < iterations {
            let message = message::Message::from_socket(&socket);

            // Set position
            if i % 300 == 0 {
                position = message.position;
                result = position;
            }

            // Set velocity
            velocity = if i == 0 {
                Vector3::new(message.initial_speed / 3.6, 0.0, 0.0) + Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * dt
            } else {
                velocity + Rotation3::from_euler_angles(message.direction.x, message.direction.y, message.direction.z) * message.acceleration * dt
            };

            // Set measurement
            let mut measurement = zeros((6, 1));
            measurement.set(0, 0, position.x);
            measurement.set(1, 0, position.y);
            measurement.set(2, 0, position.z);
            measurement.set(3, 0, velocity.x);
            measurement.set(4, 0, velocity.y);
            measurement.set(5, 0, velocity.z);

            // Update the filter
            if i == 0 {
                filter.set_state(measurement);
            } else {
                let prediction = filter.predict(Some(0.0));
                result = Vector3::<f64>::new(*prediction.get(0, 0).unwrap(), *prediction.get(1, 0).unwrap(), *prediction.get(2, 0).unwrap());
                filter.update(measurement);
            }
            
            // Send location
            send_location(message.position, &socket);
            if i % 300 == 0 {
                println!("({}) => Prediction: {:?}", i, position);
                // send_location(position, &socket);
            } else {
                println!("({}) => Prediction: {:?}", i, result);
                // send_location(result, &socket);
            }

            i += 1;
        }
        i = 0;
        let mut y: Vec<f64> = Vec::new();
        while i < kalman_filter::HISTORY_FILTER_X.len() as u32 {
            y.push(i as f64 * 0.01);
            i += 1;
        }
        println!("{} {}", i as f64 * 0.01, kalman_filter::HISTORY_FILTER_X.last().unwrap());
        utils::plot("x_t.png", kalman_filter::HISTORY_FILTER_X.clone(), y.clone());
        utils::plot("real_x_t.png", message::HISTORY_POSITION_X.clone(), y.clone());
        utils::plot("y_t.png", kalman_filter::HISTORY_FILTER_Y.clone(), y.clone());
        utils::plot("real_y_t.png", message::HISTORY_POSITION_Y.clone(), y.clone());
        utils::plot("z_t.png", kalman_filter::HISTORY_FILTER_Z.clone(), y.clone());
        utils::plot("real_z_t.png", message::HISTORY_POSITION_Z.clone(), y.clone());
        utils::plot_splitted("splitted.png");
    }
}
