//
// Created by Thomas HALLARD  on 03/03/2022.
//

#pragma once

#include "Matrix.hpp"

class KalmanFilter {
	private:
		// Delta time
		double dt;

		Matrix *E;
		Matrix *A;
		// Matrix *Q;
		// Matrix *P;

	public:
		KalmanFilter();
		~KalmanFilter();

		void prediction();
		
		static double ft_pow(double value, int pow);
    
};

