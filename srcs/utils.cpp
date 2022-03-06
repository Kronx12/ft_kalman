/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 18:02:23 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/05 19:30:26 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

void put_error(std::string error)
{
    std::cerr << "\033[31;1mft_kalman: " << error << "\033[0m" << std::endl;
    exit(1);
}

void put_info(const char *format, ...)
{
    va_list list;

    va_start(list, format);
    std::cout << "\033[34;1m";
    vprintf(format, list);
    std::cout << "\033[0m";
    va_end(list);
}
