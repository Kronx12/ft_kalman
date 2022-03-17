/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KalmanFilter.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 13:39:41 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/17 15:44:03 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef KALMANFILTER_HPP
#define KALMANFILTER_HPP

#include <cmath>

#include "Matrix.hpp"
#include "Message.hpp"

class KalmanFilter {
	public:
		KalmanFilter(Matrix initial_state, double dt);
		~KalmanFilter();

		void save();
		Matrix predict();
		Matrix update(Matrix z);
		void forceState(Matrix state);	
		
		Matrix getState();
		
		int uptodate = 0;
		
		std::vector<double> Px;
		std::vector<double> Py;
		std::vector<double> Pz;
		std::vector<double> Vx;
		std::vector<double> Vy;
		std::vector<double> Vz;
		std::vector<double> Ax;
		std::vector<double> Ay;
		std::vector<double> Az;
		std::vector<double> T;
	private:
		Matrix _X_Current;
		Matrix _P_Current;
		Matrix _F;
		Matrix _Q;
		Matrix _R;
		Matrix _H;
		Matrix _Z;
		Matrix _S;
		Matrix _K;
};

#endif // !KALMANFILTER_HPP