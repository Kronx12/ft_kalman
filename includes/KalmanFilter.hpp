/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KalmanFilter.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 13:39:41 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/13 17:57:54 by gbaud            ###   ########lyon.fr   */
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

		Matrix predict();
		Matrix update(Matrix z);
		void forceState(Matrix state);	
		
		int uptodate = 0;
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