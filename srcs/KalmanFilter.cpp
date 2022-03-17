//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter(Matrix initial_state, double dt) {
	_X_Current = initial_state;
	_P_Current = Matrix(6, 1);

	_H = Matrix::createIdentity(6);
	std::cout << "Here" << std::endl;
	
	double f_values[6][6] = {
		{1, 0, 0, dt, 0, 0},
		{0, 1, 0, 0, dt, 0},
		{0, 0, 1, 0, 0, dt},
		{0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 1},
		};
	_F = Matrix(f_values[0], 6, 6);

	_Q = Matrix::createIdentity(6);
	_Q(0, 0) = 0.1;
	_Q(1, 1) = 0.1;
	_Q(2, 2) = 0.1;
	_Q(3, 3) = 0.00101;
	_Q(4, 4) = 0.00101;
	_Q(5, 5) = 0.00101;

	_R = Matrix::createIdentity(6);
	_R(0, 0) = 0.1;
	_R(1, 1) = 0.1;
	_R(2, 2) = 0.1;
	_R(3, 3) = 0.00101;
	_R(4, 4) = 0.00101;
	_R(5, 5) = 0.00101;
}

void KalmanFilter::save() {
	Px.push_back(_X_Current(0, 0));
	Py.push_back(_X_Current(1, 0));
	Pz.push_back(_X_Current(2, 0));

	Vx.push_back(_X_Current(3, 0));
	Vy.push_back(_X_Current(4, 0));
	Vz.push_back(_X_Current(5, 0));

	T.push_back(0.01);
}

Matrix KalmanFilter::predict() {
	// Make kalman filter
	_X_Current = _F * _X_Current;
	save();

	_P_Current = _F * _P_Current * _F.transpose() + _Q;
	_Z = _H * _X_Current;
	return _X_Current;
}

Matrix KalmanFilter::update(Matrix z) {	
	Matrix S = _H * _P_Current * _H.transpose() + _R;
	Matrix K = _P_Current * _H.transpose() * S.inverse();
	_X_Current = _X_Current + K * (z - _H * _X_Current);
	save();

	Matrix I = Matrix::createIdentity(6);
	_P_Current = (I - K * _H) * _P_Current;
	return _X_Current;
}

Matrix KalmanFilter::getState() {
	return _X_Current;
}

// Destructor
KalmanFilter::~KalmanFilter() {}
