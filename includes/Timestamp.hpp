/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timestamp.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 20:20:40 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 20:59:33 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <string>
#include <regex>
#include <iostream>

class Timestamp {
    public:
        Timestamp();
        Timestamp(std::string str);
        
        double getTime();

        void setTime(double time);

        ~Timestamp();

    private:
        double _time;
};

#endif // !TIMESTAMP_HPP