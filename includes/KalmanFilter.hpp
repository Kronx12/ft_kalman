/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KalmanFilter.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 13:39:41 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/13 08:10:58 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef KALMANFILTER_HPP
#define KALMANFILTER_HPP

#include <cmath>

#include "Matrix.hpp"
#include "Message.hpp"

class KalmanFilter {
	public:
		KalmanFilter();
		~KalmanFilter();

		Matrix predict(Matrix state, double dt);
		void setPosition(Message msg);
		void setVelocity(Message msg);
	
	private:
		Matrix _X_Current;
		Matrix _P_Current;
		Matrix _X_Previous;
		Matrix _P_Previous;
		Matrix _F;
		Matrix _A;
		Matrix _B;
		Matrix _Q;
		Matrix _w;
};

#endif // !KALMANFILTER_HPP