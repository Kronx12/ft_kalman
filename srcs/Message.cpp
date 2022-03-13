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

Message::Message() : _acceleration(0), _true_position(Matrix(3, 0)), _velocity(Matrix(3, 0)), _direction(Matrix(3, 0)) {}

double Message::getAcceleration() { return _acceleration; }
Matrix Message::getTruePosition() { return _true_position; }
Matrix Message::getVelocity() { return _velocity; }
Matrix Message::getDirection() { return _direction; }
        
void Message::setAcceleration(double acceleration) { _acceleration = acceleration; }
void Message::setTruePosition(Matrix m) { _true_position = m; }
void Message::setVelocity(Matrix m) { _velocity = m; }
void Message::setDirection(Matrix m) { _direction = m; }

void Message::parseAcceleration(std::string str) {
    // Parse speed
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]SPEED\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _acceleration_timestamp = Timestamp(res[1]);
    _acceleration = std::stod(res[2]);
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

void Message::parseVelocity(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]ACCELERATION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _velocity_timestamp = Timestamp(res[1]);
    double tmp_arr[3][1] = {
        {std::stod(res[2]) * 1000 / 3600},
        {std::stod(res[3]) * 1000 / 3600},
        {std::stod(res[4]) * 1000 / 3600}
        };
    _velocity = Matrix(tmp_arr[0], 3, 1);
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
    std::cout << "Speed = " << _acceleration << std::endl;
    std::cout << "True Position = " << _true_position << std::endl;
    std::cout << "Acceleration = " << _velocity << std::endl;
    std::cout << "Direction = " << _direction << std::endl;
}

void Message::fromState(Matrix state) {
    double _true_position_values[3][1] = { {state(0, 0)}, {state(1, 0)}, {state(2, 0)} };
    _true_position = Matrix(_true_position_values[0], 3, 1);

    double _velocity_values[3][1] = { {state(3, 0)}, {state(4, 0)}, {state(5, 0)} };
    _velocity = Matrix(_velocity_values[0], 3, 1);

    double _direction_values[3][1] = { {state(6, 0)}, {state(7, 0)}, {state(8, 0)} };
    _direction = Matrix(_direction_values[0], 3, 1);
}

Matrix Message::getStateMatrix() {
    // From euler angles to rotation matrix
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
    rotation = rotation.transpose();
    // std::cout << "Rotation Matrix = " << rotation << std::endl;

    // Vector unit
    double acceleration_values[3][1] = { {_acceleration}, {_acceleration}, {_acceleration} };
    Matrix acceleration_vector = Matrix(acceleration_values[0], 3, 1);
    // std::cout << "Acceleration Vector = " << acceleration_vector << std::endl;

    // Update acceleration vector with rotation matrix
    Matrix acceleration_rotated = (rotation * acceleration_vector);
    // std::cout << "Acceleration = " << acceleration_rotated << std::endl;
    
    double tmp_arr[9][1] = {
        {_true_position(0, 0)},
        {_true_position(1, 0)},
        {_true_position(2, 0)},
        {_velocity(0, 0)},
        {_velocity(1, 0)},
        {_velocity(2, 0)},
        {acceleration_rotated(0, 0)},
        {acceleration_rotated(1, 0)},
        {acceleration_rotated(2, 0)}
        };
    return Matrix(tmp_arr[0], 9, 1);
}

Message::~Message() {}
