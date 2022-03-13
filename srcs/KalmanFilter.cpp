//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter() {
	_X_Current = Matrix(3, 3);
	_P_Current = Matrix::createIdentity(3);

	_X_Previous = Matrix(3, 3);
	_P_Previous = Matrix::createIdentity(3);
	
	_A = Matrix::createIdentity(3);

	_Q = Matrix::createIdentity(3);
	_Q(0, 0) = 0.001;
	_Q(1, 1) = 0.01;
	_Q(2, 2) = 0.1;

	_w = Matrix::createIdentity(3);
	_w(0, 0) = 0.001;
	_w(1, 1) = 0.01;
	_w(2, 2) = 0.1; 
}

Matrix KalmanFilter::predict(Matrix state, double dt) {

	_X_Previous = _X_Current;
	_P_Previous = _P_Current;


	double f_values[9][9] = {
		{1, 0, 0, dt, 0, 0, dt * dt * 0.5, 0, 0},
		{0, 1, 0, 0, dt, 0, 0, dt * dt * 0.5, 0},
		{0, 0, 1, 0, 0, dt, 0, 0, dt * dt * 0.5},
		{0, 0, 0, 1, 0, 0, dt, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, dt, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 1}
		};
	_F = Matrix(f_values[0], 9, 9);

	std::cout << "F : " << _F << std::endl;
	std::cout << "State : " << state << std::endl;

	_X_Current = _F * state;
	std::cout << "Dot : " << _X_Current << std::endl;

	return Matrix();
}

// Destructor
KalmanFilter::~KalmanFilter() {}
