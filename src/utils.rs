use std::net::UdpSocket;
use nalgebra::Vector3;
use plotters::{prelude::*,coord::*};
use rand::Rng;

use crate::message::*;
use crate::kalman_filter::*;

// pub fn distrub_plot(base_path: &str, datas: Vec<(Vec<f64>, Vec<f64>)>) {
//     let mut j = 0;
//     for i in datas {
//         let mut v: Vec<(Vec<f64>, Vec<f64>)> = Vec::<(Vec<f64>, Vec<f64>)>::new();
//         v.push(i);
//         let path = format!("{}_{}.png", base_path, j);
//         plot(&path , v);
//         j += 1;
//     }
// }

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

// pub fn plot(path: &str, datas: Vec<(Vec<f64>, Vec<f64>)>) {
//     let root_area = BitMapBackend::new(path, (1280, 1280)).into_drawing_area();
//     plot_on_root(root_area, datas);
// }

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

pub fn send_location(v: Vector3<f32>, socket: &UdpSocket) {
    let msg = format!("{} {} {}", v[0], v[1], v[2]);
    // println!("SEND LOCATION : {}", msg);
    socket.send(msg.as_bytes()).expect("couldn't send message");
}