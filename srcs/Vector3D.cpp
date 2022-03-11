/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector3D.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:16:53 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/11 13:38:14 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Vector3D.hpp"

Vector3D::Vector3D() : _x(0), _y(0), _z(0) {}
Vector3D::Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
        
double Vector3D::getX() { return (_x); }
double Vector3D::getY() { return (_y); }
double Vector3D::getZ() { return (_z); }
        
void Vector3D::setX(double x) { _x = x; }
void Vector3D::setY(double y) { _y = y; }
void Vector3D::setZ(double z) { _z = z; }

Matrix *Vector3D::toMatrix() {
    Matrix *matrix = new Matrix(4, 1);
    matrix->set(0, 0, _x);
    matrix->set(1, 0, _y);
    matrix->set(2, 0, _z);
    matrix->set(3, 0, 1);
    return (matrix);
}

void Vector3D::debug() {
    std::cout << "Vector3D: [ " << _x << ", " << _y << ", " << _z << " ]" << std::endl;
}

Vector3D::~Vector3D() {}