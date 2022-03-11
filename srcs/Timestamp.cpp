/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timestamp.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 20:20:36 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/11 02:44:42 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Timestamp.hpp"

Timestamp::Timestamp() : _time(0) {}

Timestamp::Timestamp(std::string str) {
    std::regex term_pattern("([0-9]{2}):([0-9]{2}):([0-9]{2}\\.[0-9]{3})");
    std::smatch res;
    std::string::const_iterator start(str.cbegin());
    std::regex_search(start, str.cend(), res, term_pattern);
    _time = (std::stod(res[1]) * 60 + std::stod(res[2])) * 60 + std::stod(res[3]);
}

double Timestamp::getTime() { return (_time); }

void Timestamp::setTime(double time) { _time = time; }

Timestamp::~Timestamp() {}
