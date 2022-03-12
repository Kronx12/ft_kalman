/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:12 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/12 01:10:16 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <regex>
#include <cmath>

#include "Vector3D.hpp"
#include "Timestamp.hpp"

class Message {
    public:
        Message();
        
        double      getAcceleration();
        Timestamp   getAccelerationTimestamp();
        Vector3D    *getTruePosition();
        Timestamp   getTruePositionTimestamp();
        Vector3D    *getVelocity();
        Timestamp   getVelocityTimestamp();
        Vector3D    *getDirection();
        Timestamp   getDirectionTimestamp();
        
        void setAccelerationTimestamp(Timestamp timestamp);
        void setAcceleration(double acceleration);
        void setTruePositionTimestamp(Timestamp timestamp);
        void setTruePosition(Vector3D vector);
        void setVelocityTimestamp(Timestamp timestamp);
        void setVelocity(Vector3D vector);
        void setDirectionTimestamp(Timestamp timestamp);
        void setDirection(Vector3D vector);

        void parseAcceleration(std::string str);
        void parseTruePosition(std::string str);
        void parseVelocity(std::string str);
        void parseDirection(std::string str);
        
        Matrix *getStateMatrix();

        void debug();

        ~Message();
        
    private:
        Timestamp _acceleration_timestamp;
        double _acceleration;

        Timestamp _true_position_timestamp;
        Vector3D _true_position;

        Timestamp _velocity_timestamp;
        Vector3D _velocity;

        Timestamp _direction_timestamp;
        Vector3D _direction;
};

#endif // !MESSAGE_HPP
