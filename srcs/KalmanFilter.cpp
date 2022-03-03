//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter() {
	X = 0;
	P = 0;
	R = 0;
	Q = 0;
//	U = new Matrix(10, 10);
}

// Destructor
KalmanFilter::~KalmanFilter() {
}

void KalmanFilter::prediction() {
	// Calculate X predicted mean
//	X = this->A.dot(this->X) + this->B.dot(this->U); // TODO

	// Calculate P predicted covariance
	P = 0; // TODO
}