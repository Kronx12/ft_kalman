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

Message::Message() : _acceleration(0), _true_position(Vector3D()), _velocity(Vector3D()), _direction(Vector3D()) {}

double Message::getAcceleration() { return _acceleration; }
Vector3D *Message::getTruePosition() { return &_true_position; }
Vector3D *Message::getVelocity() { return &_velocity; }
Vector3D *Message::getDirection() { return &_direction; }
        
void Message::setAcceleration(double acceleration) { _acceleration = acceleration; }
void Message::setTruePosition(Vector3D vector) { _true_position = vector; }
void Message::setVelocity(Vector3D vector) { _velocity = vector; }
void Message::setDirection(Vector3D vector) { _direction = vector; }

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
    _true_position = Vector3D(std::stod(res[2]), std::stod(res[3]), std::stod(res[4]));
}

void Message::parseVelocity(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]ACCELERATION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _velocity_timestamp = Timestamp(res[1]);
    _velocity = Vector3D(std::stod(res[2]) * 1000 / 3600, std::stod(res[3]) * 1000 / 3600, std::stod(res[4]) * 1000 / 3600);
}

void Message::parseDirection(std::string str) {
    // Parse true position
    std::smatch res;
    std::regex term_pattern("\\[([0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3})\\]DIRECTION\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)\\s(-?[0-9]+\\.[0-9]+)");
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _direction_timestamp = Timestamp(res[1]);
    _direction = Vector3D(std::stod(res[2]), std::stod(res[3]), std::stod(res[4]));
}

void Message::debug() {
    std::cout << "=== Message Debug ===" << std::endl;
    std::cout << "Speed = " << _acceleration << std::endl;
    std::cout << "True Position = ";
    _true_position.debug();
    std::cout << "Acceleration = ";
    _velocity.debug();
    std::cout << "Direction = ";
    _direction.debug();
}

Matrix *Message::getStateMatrix() {
    Matrix *matrix = new Matrix(9, 1);
    matrix->set(0, 0, _true_position.getX());
    matrix->set(1, 0, _true_position.getY());
    matrix->set(2, 0, _true_position.getZ());
    matrix->set(3, 0, _velocity.getX());
    matrix->set(4, 0, _velocity.getY());
    matrix->set(5, 0, _velocity.getZ());
    
    double x = _direction.getX();
    double y = _direction.getY();
    double z = _direction.getZ();

    // From euler angles to rotation matrix
    Matrix *rotation = new Matrix(3, 3);
    
    double c1 = cos(x);
    double c2 = cos(y);
    double c3 = cos(z);
    double s1 = sin(x);
    double s2 = sin(y);
    double s3 = sin(z);
    rotation->set(0, 0, c2 * c3);
    rotation->set(0, 1, -c2 * s3);
    rotation->set(0, 2, s2);
    rotation->set(1, 0, c1 * s3 + c3 * s1 * s2);
    rotation->set(1, 1, c1 * c3 - s1 * s2 * s3);
    rotation->set(1, 2, -c2 * s1);
    rotation->set(2, 0, s1 * s3 - c1 * c3 * s2);
    rotation->set(2, 1, c3 * s1 + c1 * s2 * s3);
    rotation->set(2, 2, c1 * c2);
    
    // Si ca marche pas faut transpose la matrice

    // Make acceleration vector
    Matrix *acceleration = new Matrix(3, 1);
    acceleration->set(0, 0, _acceleration);
    acceleration->set(1, 0, _acceleration);
    acceleration->set(2, 0, _acceleration);

    // Update acceleration vector with rotation matrix
    Matrix *acceleration_rotated = rotation->dot(*acceleration);
    matrix->set(6, 0, acceleration_rotated->get(0, 0));
    matrix->set(7, 0, acceleration_rotated->get(1, 0));
    matrix->set(8, 0, acceleration_rotated->get(2, 0));
    
    return matrix;
}

Message::~Message() {}
