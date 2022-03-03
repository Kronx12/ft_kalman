/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:12 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 20:33:33 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <regex>

#include "Vector3D.hpp"
#include "Timestamp.hpp"

class Message {
    public:
        Message();
        
        double getSpeed();
        Timestamp getSpeedTimestamp();
        Vector3D getTruePosition();
        Timestamp getTruePositionTimestamp();
        Vector3D getAcceleration();
        Timestamp getAccelerationTimestamp();
        Vector3D getDirection();
        Timestamp getDirectionTimestamp();
        
        void setSpeedTimestamp(Timestamp timestamp);
        void setSpeed(double speed);
        void setTruePositionTimestamp(Timestamp timestamp);
        void setTruePosition(Vector3D vector);
        void setAccelerationTimestamp(Timestamp timestamp);
        void setAcceleration(Vector3D vector);
        void setDirectionTimestamp(Timestamp timestamp);
        void setDirection(Vector3D vector);

        void parseSpeed(std::string str);
        void parseTruePosition(std::string str);
        void parseAcceleration(std::string str);
        void parseDirection(std::string str);

        void debug();

        ~Message();
    
    private:
        Timestamp _speed_timestamp;
        double _speed;

        Timestamp _true_position_timestamp;
        Vector3D _true_position;

        Timestamp _acceleration_timestamp;
        Vector3D _acceleration;

        Timestamp _direction_timestamp;
        Vector3D _direction;
};

#endif // !MESSAGE_HPP
