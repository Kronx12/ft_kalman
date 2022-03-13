/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 17:49:40 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/13 18:12:04 by gbaud            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include "Message.hpp"
#include "Matrix.hpp"
#include "KalmanFilter.hpp"

void put_error(std::string error);
void put_info(const char *format, ...);

#endif