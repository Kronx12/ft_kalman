use crate::utils::{
    identity,
    zeros,
    mul_by_scalar,
    inverse, debug_matrix,
};
use simple_matrix::Matrix;

pub static mut HISTORY_FILTER_X: Vec<f64> = Vec::<f64>::new();
pub static mut HISTORY_FILTER_Y: Vec<f64> = Vec::<f64>::new();
pub static mut HISTORY_FILTER_Z: Vec<f64> = Vec::<f64>::new();

pub struct KalmanFilter {
    pub n: usize,
    pub m: usize,


    pub f: Matrix<f64>,
    pub h: Matrix<f64>,
    pub b: Matrix<f64>,
    pub q: Matrix<f64>,
    pub r: Matrix<f64>,
    pub p: Matrix<f64>,

    pub x: Matrix<f64>,
}

impl KalmanFilter {
    pub fn default() -> KalmanFilter {
        KalmanFilter {
            n: 9,
            m: 1,
            f: identity(9),
            h: identity(1),
            b: zeros((9, 1)),
            q: identity(9),
            r: identity(1),
            p: identity(9),
            x: zeros((9, 1)),
        }
    }

    pub fn new( val_f: Matrix<f64>, val_b: Option<Matrix<f64>>, val_h: Matrix<f64>, val_q: Option<Matrix<f64>>, val_r: Option<Matrix<f64>>, val_p: Option<Matrix<f64>>, x0: Option<Matrix<f64>>) -> KalmanFilter {
        let mut kf = KalmanFilter::default();
        kf.n = val_f.rows();
        kf.m = val_h.cols();
        
        kf.f = val_f;
        kf.h = val_h;

        kf.b = val_b.unwrap_or(zeros((kf.n, 1)));
        kf.q = val_q.unwrap_or(identity(kf.n));
        kf.r = val_r.unwrap_or(identity(kf.m));
        kf.p = val_p.unwrap_or(identity(kf.n));

        kf.x = x0.unwrap_or(zeros((kf.n, 1)));

        return kf;
    }

    pub fn set_state(&mut self, x: Matrix<f64>) {
        self.x = x;
    }

    pub fn predict(&mut self, u: Option<f64>) -> Matrix<f64> {
        // println!("Before");
        // debug_matrix(&self.x);
        self.x = self.f.clone() * self.x.clone() + mul_by_scalar(&self.b, u.unwrap_or(0.));
        // println!("After");
        // debug_matrix(&self.x);
        self.p = self.f.clone() * self.p.clone() * self.f.transpose() + self.q.clone();

        unsafe {
            HISTORY_FILTER_X.push(*self.x.get(0, 0).unwrap());
            HISTORY_FILTER_Y.push(*self.x.get(1, 0).unwrap());
            HISTORY_FILTER_Z.push(*self.x.get(2, 0).unwrap());
        }

        return self.x.clone();
    }

    pub fn update(&mut self, z: Matrix<f64>) -> Matrix<f64> {
        let y = z - self.h.clone() * self.x.clone();
        let s = self.h.clone() * self.p.clone() * self.h.transpose() + self.r.clone();
        let k = self.p.clone() * self.h.transpose() * inverse(&s);
        self.x = self.x.clone() + k.clone() * y;
        let i = identity(self.n);
        self.p = (i.clone() - k.clone() * self.h.clone()) * self.p.clone() * (i.clone() - k.clone() * self.h.clone()).transpose() + k.clone() * self.r.clone() * k.transpose();
        return self.x.clone();
    }
}
