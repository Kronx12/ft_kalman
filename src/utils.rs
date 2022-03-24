use std::net::UdpSocket;
use nalgebra::Vector3;
use plotters::{prelude::*,coord::*};
use rand::Rng;
use simple_matrix::Matrix;
use std::fmt::Display;

use crate::message::*;
use crate::kalman_filter::*;

pub fn plot_on_root(root: DrawingArea<BitMapBackend, Shift>, x: Vec<f64>, y: Vec<f64>, label: &str) {
    root.fill(&WHITE).unwrap();

    let x_max: f64 = x.iter().cloned().fold(0./0., f64::max);
    let x_min: f64 = x.iter().cloned().fold(0./0., f64::min);
    let y_max: f64 = y.iter().cloned().fold(0./0., f64::max);
    let y_min: f64 = y.iter().cloned().fold(0./0., f64::min);

    let mut ctx = ChartBuilder::on(&root)
        .set_label_area_size(LabelAreaPosition::Left, 40)
        .set_label_area_size(LabelAreaPosition::Bottom, 40)
        .build_cartesian_2d(x_min..x_max, y_min..y_max)
        .unwrap();
    ctx.configure_mesh().draw().unwrap();
    let mut data: Vec<(f64, f64)> = Vec::<(f64, f64)>::new();
    for i in 0..x.len() - 1 { data.push((x[i], y[i])); }
    let mut rng = rand::thread_rng();
    ctx.draw_series(LineSeries::new(data, RGBColor(rng.gen(), rng.gen(), rng.gen())))
        .unwrap()
        .label(label);
    ctx.configure_series_labels()
        .border_style(&BLACK)
        .background_style(&WHITE.mix(0.8))
        .draw()
        .unwrap();
}

pub fn plot3d(root: DrawingArea<BitMapBackend, Shift>, x: Vec<f64>, y: Vec<f64>, z: Vec<f64>) {
    root.fill(&WHITE).unwrap();

    let x_max: f64 = x.iter().cloned().fold(0./0., f64::max);
    let x_min: f64 = x.iter().cloned().fold(0./0., f64::min);
    let y_max: f64 = y.iter().cloned().fold(0./0., f64::max);
    let y_min: f64 = y.iter().cloned().fold(0./0., f64::min);
    let z_max: f64 = z.iter().cloned().fold(0./0., f64::max);
    let z_min: f64 = z.iter().cloned().fold(0./0., f64::min);

    let mut chart = ChartBuilder::on(&root)
        .margin(20)
        .caption("3D Line", ("sans-serif", 40))
        .build_cartesian_3d(x_min..x_max, y_min..y_max, z_min..z_max)
        .unwrap();
    chart.with_projection(|mut pb| {
            pb.pitch = 0.8;
            pb.yaw = 0.5;
            pb.scale = 0.7;
            pb.into_matrix()
        });
    chart.configure_axes().draw().unwrap();

    let mut data: Vec<(f64, f64, f64)> = Vec::<(f64, f64, f64)>::new();
    for i in 0..x.len() - 1 {
        data.push((x[i], y[i], z[i]));
    }
    chart.draw_series(LineSeries::new(
        data.iter().map(|x| (x.0, x.1, x.2)),
        &RED
    )).unwrap();
}

pub fn plot(path: &str, x: Vec<f64>, y: Vec<f64>) {
    let root = BitMapBackend::new(path, (2000, 1000)).into_drawing_area();

    plot_on_root(root, x, y, path)
}

pub unsafe fn plot_splitted(path: &str) {
    let root = BitMapBackend::new(path, (2000, 1000)).into_drawing_area();
    
    let areas = root.split_evenly((2, 4));

    for (id, area) in areas.into_iter().enumerate() {
        if id == 0 { plot_on_root(area, HISTORY_POSITION_X.clone(), HISTORY_POSITION_Y.clone(), "X - Y"); }
        else if id == 1 { plot_on_root(area, HISTORY_POSITION_Y.clone(), HISTORY_POSITION_Z.clone(), "Y - Z"); }
        else if id == 2 { plot_on_root(area, HISTORY_POSITION_Z.clone(), HISTORY_POSITION_X.clone(), "Z - X"); }
        else if id == 3 { plot3d(area, HISTORY_POSITION_X.clone(), HISTORY_POSITION_Y.clone(), HISTORY_POSITION_Z.clone()); }
        else if id == 4 { plot_on_root(area, HISTORY_FILTER_X.clone(), HISTORY_FILTER_Y.clone(), "X - Y"); }
        else if id == 5 { plot_on_root(area, HISTORY_FILTER_Y.clone(), HISTORY_FILTER_Z.clone(), "Y - Z"); }
        else if id == 6 { plot_on_root(area, HISTORY_FILTER_Z.clone(), HISTORY_FILTER_X.clone(), "Z - X"); }
        else if id == 7 { plot3d(area, HISTORY_FILTER_X.clone(), HISTORY_FILTER_Y.clone(), HISTORY_FILTER_Z.clone()); }
    }
}

pub fn recv_message(socket: &UdpSocket) -> String {
    let mut buf = [0; 1024];
    let (amt, _src) = socket.recv_from(&mut buf).expect("couldn't receive message");
    let response = String::from_utf8_lossy(&buf[..amt]).to_string();
    return response;
}

pub fn send_location(v: Vector3<f64>, socket: &UdpSocket) {
    let msg = format!("{} {} {}", v[0], v[1], v[2]);
    // println!("SEND LOCATION : {}", msg);
    socket.send(msg.as_bytes()).expect("couldn't send message");
}

#[allow(dead_code)]
pub fn debug_matrix<T: Display>(m: &Matrix<T>) {
    let mut lines: Vec<String> = Vec::new();

    // get max number of digits
    let mut max_digits: usize = 0;
    for i in 0..m.rows() {
        for j in 0..m.cols() {
            let digits = m.get(i, j).unwrap().to_string().len();
            if digits > max_digits {
                max_digits = digits;
            }
        }
    }
    for x in 0..m.rows() {
        let mut line = "│".to_string();
        for y in 0..m.cols() {
            line = format!("{} {:w$}", line, m.get(x, y).unwrap(), w = max_digits);
        }
        line = format!("{} │", line);
        lines.push(line);
    }
    let width = lines[0].chars().count() - 2;
    println!("┌{:w$}┐", "", w = width);
    for i in lines.iter() {
        println!("{}", i);
    }
    println!("└{:w$}┘", "", w = width);
}

pub fn identity(size: usize) -> Matrix<f64> {
    let mut m = Matrix::<f64>::new(size, size);
    for i in 0..size {
        m.set(i, i, 1.);
    }
    return m;
}

pub fn zeros(size: (usize, usize)) -> Matrix<f64> {
    let mut m = Matrix::<f64>::new(size.0, size.1);
    for i in 0..size.0 {
        for j in 0..size.1 {
            m.set(i, j, 0.);
        }
    }
    return m;
}

pub fn mul_by_scalar(m: &Matrix<f64>, s: f64) -> Matrix<f64> {
    let mut result = Matrix::<f64>::new(m.rows(), m.cols());
    for i in 0..m.rows() {
        for j in 0..m.cols() {
            result.set(i, j, m.get(i, j).unwrap() * s);
        }
    }
    return result;
}

// Calculate a inverse of a matrix
pub fn inverse(m: &Matrix<f64>) -> Matrix<f64> {
    // Only on square matrix
    let identity = identity(m.rows());
    let mut merge = Matrix::<f64>::new(m.rows(), m.cols() * 2);
    for i in 0..m.rows() {
        for j in 0..m.cols() {
            merge.set(i, j, *m.get(i, j).unwrap());
            merge.set(i, j + m.cols(), *identity.get(i, j).unwrap());
        }
    }
    // Apply reduced row echelon form
    let rref_matrix = rref(&merge);
    // Extract the inverse
    let mut inverse = Matrix::<f64>::new(m.rows(), m.cols());
    for i in 0..m.rows() {
        for j in 0..m.cols() {
            inverse.set(i, j, *rref_matrix.get(i, j + m.cols()).unwrap());
        }
    }
    return inverse;
}

pub fn rref(m: &Matrix<f64>) -> Matrix<f64> {
    let mut result = Matrix::<f64>::new(m.rows(), m.cols());
    for i in 0..m.rows() {
        for j in 0..m.cols() {
            result.set(i, j, *m.get(i, j).unwrap());
        }
    }
    let mut pivot = 0;
    for i in 0..m.rows() {
        // Find the pivot
        let mut pivot_value = *result.get(i, pivot).unwrap();
        let mut pivot_row = i;
        for j in i..m.rows() {
            if pivot_value.abs() < result.get(j, pivot).unwrap().abs() {
                pivot_value = *result.get(j, pivot).unwrap();
                pivot_row = j;
            }
        }

        // Swap rows
        if pivot_row != i {
            for j in 0..m.cols() {
                let tmp = *result.get(i, j).unwrap();
                result.set(i, j, *result.get(pivot_row, j).unwrap());
                result.set(pivot_row, j, tmp);
            }
        }
        // Divide by pivot
        if pivot_value != 0. {
            for j in 0..m.cols() {
                result.set(i, j, result.get(i, j).unwrap() / pivot_value);
            }
        }
        // Subtract from rows below
        for j in i + 1..m.rows() {
            let factor = *result.get(j, pivot).unwrap();
            for k in 0..m.cols() {
                result.set(j, k, result.get(j, k).unwrap() - factor * result.get(i, k).unwrap());
            }
        }
        pivot += 1;
    }
    return result;
}
