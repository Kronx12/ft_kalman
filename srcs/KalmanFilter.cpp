//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter() {
	_X_Current = new Matrix(3, 3);
	_P_Current = Matrix::identity(3, 3, 1);

	_X_Previous = new Matrix(3, 3);
	_P_Previous = Matrix::identity(3, 3, 1);
	
	_A = Matrix::identity(3, 3, 1);

	_Q = Matrix::identity(3, 3, 1);
	_Q->set(0, 0, 0.001);
	_Q->set(1, 1, 0.01);
	_Q->set(2, 2, 0.1);

	_w = Matrix::identity(3, 3, 1);
	_w->set(0, 0, 0.001);
	_w->set(1, 1, 0.01);
	_w->set(2, 2, 0.1); 
}

Matrix *KalmanFilter::predict(Matrix *state, double dt) {

	_X_Previous = _X_Current;
	_P_Previous = _P_Current;

	_F = Matrix::identity(9, 9, 1);
	_F->set(3, 0, dt);
	_F->set(4, 1, dt);
	_F->set(5, 2, dt);
	_F->set(6, 3, dt);
	_F->set(7, 4, dt);
	_F->set(8, 5, dt);
	_F->set(6, 0, dt * dt / 2);
	_F->set(7, 1, dt * dt / 2);
	_F->set(8, 2, dt * dt / 2);

	std::cout << "F : " << std::endl;
	std::cout << _F->getHeight() << " | " << _F->getWidth() << std::endl;
	_F->print();
	std::cout << "State : " << std::endl;
	std::cout << state->getHeight() << " | " << state->getWidth() << std::endl;
	state->print();

	_X_Current = _F->dot(*state);

	_X_Current->print();
	
	return nullptr;
}

// Destructor
KalmanFilter::~KalmanFilter() {}
