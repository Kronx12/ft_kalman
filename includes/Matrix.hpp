/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thallard <thallard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 06:13:32 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 15:14:111 by thallard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <iostream>
#include <array>

class Matrix {
    private:
        unsigned char x_size;
        unsigned char y_size;
        double* data;

    public:
        Matrix(int x, int y);
        ~Matrix();

        // Utilities
        Matrix const &clone(Matrix const &) const;

        bool is_square() const;
        bool is_identity() const;

        double get(int x, int y) const;
        void set(int x, int y, double value);

        unsigned char getX() const;
        unsigned char getY() const;

        void print() const;

        // Operators and functions
        Matrix &add(Matrix &);
        Matrix &dot(Matrix &);
		Matrix &transpose();

		Matrix operator+(Matrix &left, const Matrix &right);
};

