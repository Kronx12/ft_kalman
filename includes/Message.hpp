/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:12 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 05:42:29 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <regex>

#include "Vector3D.hpp"

class Message {
    public:
        Message();
        
        double getSpeed();
        Vector3D getTruePosition();
        Vector3D getAcceleration();
        Vector3D getDirection();
        
        void setSpeed(double speed);
        void setTruePosition(Vector3D vector);
        void setAcceleration(Vector3D vector);
        void setDirection(Vector3D vector);

        void parseSpeed(char *raw);
        void parseTruePosition(char *raw);
        void parseAcceleration(char *raw);
        void parseDirection(char *raw);

        void debug();

        ~Message();
    
    private:
        double _speed;
        Vector3D _true_position;
        Vector3D _acceleration;
        Vector3D _direction;
};

#endif // !MESSAGE_HPP
