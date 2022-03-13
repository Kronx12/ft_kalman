//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter(Matrix initial_state, double dt) {
	_X_Current = initial_state;
	_P_Current = Matrix(9, 1);
	
	_H = Matrix::createIdentity(9);
	// _R = Matrix::createIdentity(9);
	
	double f_values[9][9] = {
		{1, 0, 0, dt, 0, 0, dt * dt * 0.5, 0, 0},
		{0, 1, 0, 0, dt, 0, 0, dt * dt * 0.5, 0},
		{0, 0, 1, 0, 0, dt, 0, 0, dt * dt * 0.5},
		{0, 0, 0, 1, 0, 0, dt, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, dt, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, dt},
		{0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 1}
		};
	_F = Matrix(f_values[0], 9, 9);

	_Q = Matrix::createIdentity(9);
	_Q(0, 0) = 0.1;
	_Q(1, 1) = 0.1;
	_Q(2, 2) = 0.1;
	_Q(3, 3) = 0.01;
	_Q(4, 4) = 0.01;
	_Q(5, 5) = 0.01;
	_Q(6, 6) = 0.001;
	_Q(7, 7) = 0.001;
	_Q(8, 8) = 0.001;

	_R = Matrix::createIdentity(9);
	_R(0, 0) = 0.1;
	_R(1, 1) = 0.1;
	_R(2, 2) = 0.1;
	_R(3, 3) = 0.01;
	_R(4, 4) = 0.01;
	_R(5, 5) = 0.01;
	_R(6, 6) = 0.001;
	_R(7, 7) = 0.001;
	_R(8, 8) = 0.001;
}

Matrix KalmanFilter::predict() {
	_X_Current = _F * _X_Current;
	_P_Current = _F * _P_Current * _F.transpose() + _Q;
	_Z = _H * _X_Current;
	return _X_Current;
}

Matrix KalmanFilter::update(Matrix z) {
	Matrix S = _H * _P_Current * _H.transpose() + _R;
	Matrix K = _P_Current * _H.transpose() * S.inverse();
	_X_Current = _X_Current + K * (z - _H * _X_Current);
	Matrix I = Matrix::createIdentity(9);
	_P_Current = (I - K * _H) * _P_Current;
	return _X_Current;
}

void KalmanFilter::forceState(Matrix state) {
	_X_Current = state;
	_P_Current = Matrix(9, 1);
	uptodate = 1;
}

// Destructor
KalmanFilter::~KalmanFilter() {}
