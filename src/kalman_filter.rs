use nalgebra::{
    Vector3
};

pub struct KalmanFilter {
    pub position: Vector3<f32>,
    pub acceleration: Vector3<f32>,
    pub direction: Vector3<f32>,
    pub speed: Vector3<f32>,
    pub position_history: Vector3<Vector3<f32>>,
    pub acceleration_history: Vector3<Vector3<f32>>,
    pub direction_history: Vector3<Vector3<f32>>,
}

impl KalmanFilter {
    pub fn default() -> KalmanFilter {
        KalmanFilter {
            position: Vector3::new(0.0, 0.0, 0.0),
            acceleration: Vector3::new(0.0, 0.0, 0.0),
            direction: Vector3::new(0.0, 0.0, 0.0),
            speed: Vector3::new(0.0, 0.0, 0.0),
        }
    }

    pub fn new(position: Vector3<f32>, acceleration: Vector3<f32>, direction: Vector3<f32>, speed: Vector3<f32>) -> KalmanFilter {
        KalmanFilter {
            position: position,
            acceleration: acceleration,
            direction: direction,
            speed: speed,
        }
    }

    pub fn fmt(&self) -> String {
        format!("P(x,y,z) = {} A(x,y,z) = {} D(x,y,z) = {} S(x,y,z) = {}",
            self.position,
            self.acceleration,
            self.direction,
            self.speed
        )
    }

    pub fn save(&mut self) {
        self.position_history.push(self.position);
        self.acceleration_history.push(self.acceleration);
        self.direction_history.push(self.direction);
    }
}