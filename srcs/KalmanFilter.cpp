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

Matrix *KalmanFilter::predict(Message msg, double dt) {

	_X_Previous = _X_Current;
	_P_Previous = _P_Current;

	Matrix *B = new Matrix(3, 2);
	B->set(0, 0, msg.getVelocity()->getX());
	B->set(1, 0, msg.getAcceleration());
	B->set(0, 1, msg.getVelocity()->getY());
	B->set(1, 1, msg.getAcceleration());
	B->set(0, 2, msg.getVelocity()->getZ());
	B->set(1, 2, msg.getAcceleration());

	Matrix *u = new Matrix(2, 3);
	u->set(0, 0, sin(msg.getDirection()->getX()));
	u->set(0, 1, dt);
	u->set(1, 0, -(sin(msg.getDirection()->getY()) * cos(msg.getDirection()->getX())));
	u->set(1, 1, dt);
	u->set(2, 0, -(cos(msg.getDirection()->getX()) * cos(msg.getDirection()->getY())));
	u->set(2, 1, dt);

	_X_Current = _A->dot(*_X_Previous)->add(*B->dot(*u))->add(*_w);
	_P_Current = _A->dot(*_P_Previous)->dot(*_A->transpose())->add(*_Q);
	return nullptr;
}

// Destructor
KalmanFilter::~KalmanFilter() {}
