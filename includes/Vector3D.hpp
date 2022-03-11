/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector3D.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 20:12:46 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/11 13:37:54 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP 

#include <iostream>
#include "Matrix.hpp"

class Vector3D {
    public:
        Vector3D();
        Vector3D(double x, double y, double z);
        
        double getX();
        double getY();
        double getZ();
        
        void setX(double value);
        void setY(double value);
        void setZ(double value);

        Matrix *toMatrix();

        void debug();

        ~Vector3D();

    private:
        double _x;
        double _y;
        double _z;
};

#endif // !VECTOR3D_HPP
