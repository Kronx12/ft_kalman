/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thallard <thallard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:11:01 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 14:43:115 by thallard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message() : _speed(0), _true_position(Vector3D()), _acceleration(Vector3D()), _direction(Vector3D()) {}

double Message::getSpeed() { return _speed; }
Vector3D Message::getTruePosition() { return _true_position; }
Vector3D Message::getAcceleration() { return _acceleration; }
Vector3D Message::getDirection() { return _direction; }
        
void Message::setSpeed(double speed) { _speed = speed; }
void Message::setTruePosition(Vector3D vector) { _true_position = vector; }
void Message::setAcceleration(Vector3D vector) { _acceleration = vector; }
void Message::setDirection(Vector3D vector) { _direction = vector; }

void Message::parseSpeed(char *raw) {
    // Parse speed
    sscanf(raw, "[00:00:00.000]SPEED\n%lf", &_speed);
}

void Message::parseTruePosition(char *raw) {
    // Parse true position
    // double x, y, z;
//
//    std::string str(raw);
//    // "\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]TRUE POSITION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)"
//    std::cout << "RAW=[" << raw << "]";
//
//    std::regex const term_pattern { "\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]TRUE POSITION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)" };
//    std::smatch res;
//
//    std::string::const_iterator start(str.cbegin());
//    while (std::regex_search(start, str.cend(), res, term_pattern)) {
//        std::cout << res[0] << std::endl;
//        std::cout << res[1] << std::endl;
//        std::cout << res[2] << std::endl;
//    }
    (void)raw;
    // _true_position = Vector3D(x, y, z);
}

void Message::parseAcceleration(char *raw) {
    // Parse true position
    double x, y, z;

    sscanf(raw, "[00:00:00.000]ACCELERATION\n%lf\n%lf\n%lf", &x, &y, &z);
    _true_position = Vector3D(x, y, z);
}

void Message::parseDirection(char *raw) {
    // Parse true position
    double x, y, z;

    sscanf(raw, "[00:00:00.000]DIRECTION\n%lf\n%lf\n%lf", &x, &y, &z);
    _true_position = Vector3D(x, y, z);
}

void Message::debug() {
    std::cout << "=== Message Debug ===" << std::endl;
    std::cout << "Speed = " << _speed << std::endl;
    std::cout << "True Position = " << std::endl;
    _true_position.debug();
    std::cout << "Acceleration = " << std::endl;
    _acceleration.debug();
    std::cout << "DIRECTION = " << std::endl;
    _direction.debug();
}

Message::~Message() {
    
}
