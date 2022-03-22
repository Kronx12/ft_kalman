use std::net::UdpSocket;
use nalgebra::Vector3;
use plotters::{prelude::*, style::RGBAColor};
use rand::Rng;

pub fn plot(path: &str, datas: Vec<(Vec<f64>, Vec<f64>)>) {
    let root_area = BitMapBackend::new(path, (1280, 1280)).into_drawing_area();

    root_area.fill(&WHITE).unwrap();

    let mut x_max: f64 = datas[0].0[0];
    let mut x_min: f64 = datas[0].0[0];
    let mut y_max: f64 = datas[0].1[0];
    let mut y_min: f64 = datas[0].1[0];

    for set in datas.clone() {
        if set.0.iter().cloned().fold(0./0., f64::max) > x_max { x_max = set.0.iter().cloned().fold(0./0., f64::max); }
        if set.0.iter().cloned().fold(0./0., f64::min) < x_min { x_min = set.0.iter().cloned().fold(0./0., f64::min); }
        if set.1.iter().cloned().fold(0./0., f64::max) > y_max { y_max = set.1.iter().cloned().fold(0./0., f64::max); }
        if set.1.iter().cloned().fold(0./0., f64::min) < y_min { y_min = set.1.iter().cloned().fold(0./0., f64::min); }
    }

    println!("{}", x_min);
    println!("{}", x_max);
    println!("{}", y_min);
    println!("{}", y_max);

    let mut ctx = ChartBuilder::on(&root_area)
        .set_label_area_size(LabelAreaPosition::Left, 40)
        .set_label_area_size(LabelAreaPosition::Bottom, 40)
        .caption("Legend", ("sans-serif", 40))
        .build_cartesian_2d(x_min..x_max, y_min..y_max)
        .unwrap();

    ctx.configure_mesh().draw().unwrap();

    for set in datas {
        let mut data: Vec<(f64, f64)> = Vec::<(f64, f64)>::new();
        for i in 0..set.0.len() - 1 {
            data.push((set.0[i], set.1[i]));
        }
    
        let mut rng = rand::thread_rng();
        ctx.draw_series(LineSeries::new(data.iter().map(|x| (x.0, x.1)), RGBColor(rng.gen(), rng.gen(), rng.gen())))
            .unwrap()
            .label("Curve");
    }

    ctx.configure_series_labels()
        .border_style(&BLACK)
        .background_style(&WHITE.mix(0.8))
        .draw()
        .unwrap();
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