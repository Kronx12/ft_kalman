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

void Message::parseSpeed(std::string str) {
    // Parse speed
    std::smatch res;
    std::regex const term_pattern { "\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]SPEED\\s(-?[0-9]+\\.[0-9]+)" };
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _speed_timestamp = Timestamp(res[1]);
    _speed = std::stod(res[2]);
}

void Message::parseTruePosition(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex const term_pattern { "\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]TRUE POSITION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)" };
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _true_position_timestamp = Timestamp(res[1]);
    _true_position = Vector3D(std::stod(res[2]), std::stod(res[3]), std::stod(res[4]));
}

void Message::parseAcceleration(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex const term_pattern { "\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]ACCELERATION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)" };
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _acceleration_timestamp = Timestamp(res[1]);
    _acceleration = Vector3D(std::stod(res[2]), std::stod(res[3]), std::stod(res[4]));
}

void Message::parseDirection(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex const term_pattern { "\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]DIRECTION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)" };
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _direction_timestamp = Timestamp(res[1]);
    _direction = Vector3D(std::stod(res[2]), std::stod(res[3]), std::stod(res[4]));
}

void Message::debug() {
    std::cout << "=== Message Debug ===" << std::endl;
    std::cout << "Speed = " << _speed << std::endl;
    std::cout << "True Position = ";
    _true_position.debug();
    std::cout << "Acceleration = ";
    _acceleration.debug();
    std::cout << "Direction = ";
    _direction.debug();
}

Message::~Message() {}
