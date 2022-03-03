//
// Created by Thomas HALLARD  on 03/03/2022.
//

#pragma once

#include "Matrix.hpp"

class KalmanFilter {
	private:
		// Process noise covariance
		double Q;

		// Measurement noise covariance
		double R;

		// Matrix n x n at previous state step (-1)
//		Matrix &A;

		// Matrix n x 1 optional control input u at the state x
//		Matrix &B;

		// Matrix m x n state of measurement y[k]
//		Matrix &H;

		// Matrix control input
//		Matrix &U;

		// Predicted mean and covariance before measurement
		double X;
		double P;

	public:
		KalmanFilter();
		~KalmanFilter();

		void prediction();
};

