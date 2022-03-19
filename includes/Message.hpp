/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thallard <thallard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:12 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/18 16:58:07 by thallard         ###   ########lyon.fr   */
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
        Message(Matrix inital_position, Matrix inital_speed, Matrix inital_acceleration, Matrix inital_direction);
        
        Matrix      getTruePosition();
        Timestamp   getTruePositionTimestamp();
        Matrix      getSpeed();
        Timestamp   getSpeedTimestamp();
        Matrix      getAcceleration();
        Timestamp   getAccelerationTimestamp();
        Matrix      getDirection();
        Timestamp   getDirectionTimestamp();
        
        void setTruePositionTimestamp(Timestamp timestamp);
        void setTruePosition(Matrix m);
        void setSpeedTimestamp(Timestamp timestamp);
        void setSpeed(Matrix m);
        void setAccelerationTimestamp(Timestamp timestamp);
        void setAcceleration(Matrix acceleration);
        void setDirectionTimestamp(Timestamp timestamp);
        void setDirection(Matrix m);

        void parseTruePosition(std::string str);
        void parseSpeed(std::string str);
        void parseAcceleration(std::string str);
        void parseDirection(std::string str);
        
        Matrix getRotationMatrix();
        Matrix getStateMatrix();
        std::string exportLocation();

        void setup();
        void update(double dt);

        void debug();

        ~Message();
        
    private:
        Timestamp _true_position_timestamp;
        Matrix _true_position;

        Timestamp _speed_timestamp;
        Matrix _speed;
        
        Timestamp _acceleration_timestamp;
        Matrix _acceleration;

        Timestamp _direction_timestamp;
        Matrix _direction;

        Matrix _position;
        Matrix _D;
};

#endif // !MESSAGE_HPP
