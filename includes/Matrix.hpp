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

#ifndef MATRIX_HPP
#define MATRIX_HPP

class Matrix {
    private:
        size_t size;

    public:
        Matrix();
        ~Matrix();

        Matrix const &add(Matrix &);
        Matrix const &dot(Matrix &);
};

#endif 
