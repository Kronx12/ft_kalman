//
// Created by Thomas HALLARD  on 03/03/2022.
//

#include "../includes/KalmanFilter.hpp"

// Basic constructor
KalmanFilter::KalmanFilter() {
	dt = 3;
	E = new Matrix(1, 10); // TODO Fill E
	A = new Matrix(1, 1);

}

// Destructor
KalmanFilter::~KalmanFilter() {
}

void KalmanFilter::prediction() {
	// Calculate X predicted mean
	// X = this->A.dot(this->X) + this->B.dot(this->U); // TODO

	// Calculate P predicted covariance
	// P = this->A.dot(this->P.dot(this->A.transpose())) + Q;
}

double KalmanFilter::ft_pow(double value, int pow) {
    return (pow > 0 ? (value * ft_pow(value, pow - 1)) : 1);
}