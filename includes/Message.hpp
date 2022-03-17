/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:12 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/17 08:48:12 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <regex>
#include <cmath>

#include "Timestamp.hpp"
#include "Matrix.hpp"

class Message {
    public:
        Message();
        
        double      getAcceleration();
        Timestamp   getAccelerationTimestamp();
        Matrix      getTruePosition();
        Timestamp   getTruePositionTimestamp();
        Matrix      getVelocity();
        Timestamp   getVelocityTimestamp();
        Matrix      getDirection();
        Timestamp   getDirectionTimestamp();
        
        void setAccelerationTimestamp(Timestamp timestamp);
        void setAcceleration(double acceleration);
        void setTruePositionTimestamp(Timestamp timestamp);
        void setTruePosition(Matrix m);
        void setVelocityTimestamp(Timestamp timestamp);
        void setVelocity(Matrix m);
        void setDirectionTimestamp(Timestamp timestamp);
        void setDirection(Matrix m);

        void parseAcceleration(std::string str);
        void parseTruePosition(std::string str);
        void parseVelocity(std::string str);
        void parseDirection(std::string str);
        
        void fromState(Matrix state);
        
        Matrix getStateMatrix();
        std::string exportLocation();

        void debug();

        ~Message();
        
    private:
        Timestamp _acceleration_timestamp;
        double _acceleration;

        Timestamp _true_position_timestamp;
        Matrix _true_position;

        Timestamp _velocity_timestamp;
        Matrix _velocity;

        Timestamp _direction_timestamp;
        Matrix _direction;
};

#endif // !MESSAGE_HPP
