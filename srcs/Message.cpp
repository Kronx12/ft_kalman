/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 13:24:05 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/17 16:33:12 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message()
    : _true_position(Matrix(3, 0)), _speed(Matrix(3, 0)), _acceleration(Matrix(3, 0)), _direction(Matrix(3, 0)) {}

Matrix Message::getAcceleration() { return _acceleration; }
Matrix Message::getTruePosition() { return _true_position; }
Matrix Message::getSpeed() { return _speed; }
Matrix Message::getDirection() { return _direction; }
        
void Message::setAcceleration(Matrix m) { _acceleration = m; }
void Message::setTruePosition(Matrix m) { _true_position = m; }
void Message::setSpeed(Matrix m) { _speed = m; }
void Message::setDirection(Matrix m) { _direction = m; }

void Message::parseSpeed(std::string str) {
    // Parse speed
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]SPEED\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _speed_timestamp = Timestamp(res[1]);
    double tmp_arr[3][1] = {
        {std::stod(res[2]) / 3.6},
        {std::stod(res[2]) / 3.6},
        {std::stod(res[2]) / 3.6}
        };
    _speed = Matrix(tmp_arr[0], 3, 1);
}

void Message::parseTruePosition(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]TRUE POSITION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _true_position_timestamp = Timestamp(res[1]);
    double tmp_arr[3][1] = {
        {std::stod(res[2])},
        {std::stod(res[3])},
        {std::stod(res[4])}
        };
    _true_position = Matrix(tmp_arr[0], 3, 1);
}

void Message::parseAcceleration(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]ACCELERATION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _acceleration_timestamp = Timestamp(res[1]);
    double tmp_arr[3][1] = {
        { std::stod(res[2]) },
        { std::stod(res[3]) },
        { std::stod(res[4]) }
        };
    _acceleration = Matrix(tmp_arr[0], 3, 1);
}

void Message::parseDirection(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]DIRECTION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _direction_timestamp = Timestamp(res[1]);
    double tmp_arr[3][1] = {
        {std::stod(res[2])},
        {std::stod(res[3])},
        {std::stod(res[4])}
        };
    _direction = Matrix(tmp_arr[0], 3, 1);
}

void Message::debug() {
    std::cout << "=== Message Debug ===" << std::endl;
    std::cout << "True Position = " << _true_position << std::endl;
    std::cout << "Speed = " << _speed << std::endl;
    std::cout << "Acceleration = " << _acceleration << std::endl;
    std::cout << "Direction = " << _direction << std::endl;
    std::cout << "=== End Message Debug ===" << std::endl;
}

Matrix Message::getRotationMatrix() {
    double c1 = cos(_direction(0, 0));
    double c2 = cos(_direction(1, 0));
    double c3 = cos(_direction(2, 0));
    double s1 = sin(_direction(0, 0));
    double s2 = sin(_direction(1, 0));
    double s3 = sin(_direction(2, 0));

    double rotation_values[3][3] = {
        {c2 * c3, -c2 * s3, s2},
        {c1 * s3 + c3 * s1 * s2, c1 * c3 - s1 * s2 * s3, -c2 * s1},
        {s1 * s3 - c1 * c3 * s2, c3 * s1 + c1 * s2 * s3, c1 * c2}
        };

    Matrix rotation = Matrix(rotation_values[0], 3, 3);
    return rotation;
}

void Message::setup() {
    _speed = getRotationMatrix() * _direction * _speed(0, 0);
}

void Message::update(double dt) {
    _speed = (_speed * getRotationMatrix() * _direction * dt); //+ (_acceleration * dt * dt * 0.5);
}

Matrix Message::getStateMatrix() {    
    double tmp_arr[6][1] = {
        {_true_position(0, 0)},
        {_true_position(1, 0)},
        {_true_position(2, 0)},
        {_speed(0, 0)},
        {_speed(1, 0)},
        {_speed(2, 0)},
        };
    return Matrix(tmp_arr[0], 6, 1);
}

Message::~Message() {}
