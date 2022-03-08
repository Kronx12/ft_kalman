/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thallard <thallard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 18:36:03 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/05 23:00:554 by thallard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

int main(int ac, char **av) {
    int port;
    int sockfd;
    struct sockaddr_in server_address;
    int buff_len = 512;
    char buffer[buff_len];
    int receive_length;

    // KalmanFilter *filter = new KalmanFilter();
    // delete filter;

    if (ac != 2)
        put_error("Usage: ./ft_kalman <PORT>");
    
    if (!av[1][0])
        put_error("Port must be a number !");
    for (int i = 0; av[1][i]; i++)
        if (!av[1][i])
            put_error("Port must be a number !");
    
    port = atoi(av[1]);
    put_info("Port: %d\n", port);
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        put_error("Socket creation error !");
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        put_error("Network connection error !");
    
    char handshake[] = "READY";
    sendto(sockfd, handshake, strlen(handshake), 0, (const sockaddr *)NULL, sizeof(server_address));
    std::cout << "READY SENT !" << std::endl;

    // Skip Raw Trajectory Generation. . .
    bzero(buffer, buff_len);
    receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
    // Sending Info. . .
    bzero(buffer, buff_len);
    receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);

    std::vector<double> *x_history = new std::vector<double>();
    std::vector<double> *y_history = new std::vector<double>();
    std::vector<double> *x2_history = new std::vector<double>();
    std::vector<double> *y2_history = new std::vector<double>();

    int simulation_duration = 100;
    for (int i = 0; i < simulation_duration * 10; i++) {
        Message message = Message();
        int receive_length;

        // std::cout << ">>> START <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        // std::cout << ">>> TRUE POSITION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseTruePosition(buffer);
        // std::cout << "[" << buffer << "]" << std::endl;
        
        // std::cout << ">>> SPEED <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseSpeed(buffer);
        // std::cout << buffer << std::endl;
        
        // std::cout << ">>> ACCELERATION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseAcceleration(buffer);
        // std::cout << buffer << std::endl;

        // std::cout << ">>> DIRECTION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseDirection(buffer);
        // std::cout << buffer << std::endl;
        
        // std::cout << ">>> END <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);


        message.getAcceleration().setX(message.getAcceleration().getX() / 3.6);
        message.getAcceleration().setY(message.getAcceleration().getY() / 3.6);
        message.getAcceleration().setZ(message.getAcceleration().getZ() / 3.6);
        // message.debug();
        std::cout << "Before : ";
        message.getTruePosition().debug();
        std::cout << "After : ";

        Vector3D vec;
        vec.setX(cos(message.getDirection().getX()) * cos(message.getDirection().getY()) * message.getSpeed());
        vec.setY(sin(message.getDirection().getX()) * cos(message.getDirection().getY()) * message.getSpeed());
        vec.setZ(sin(message.getDirection().getY()) * message.getSpeed());
        // vec.debug();

        message.getAcceleration().setX(message.getAcceleration().getX() + vec.getX() * 0.01);
        message.getAcceleration().setY(message.getAcceleration().getY() + vec.getY() * 0.01);
        message.getAcceleration().setZ(message.getAcceleration().getZ() + vec.getZ() * 0.01);

        Vector3D new_position;
        new_position.setX(message.getTruePosition().getX() + message.getAcceleration().getX());
        new_position.setY(message.getTruePosition().getY() + message.getAcceleration().getY());
        new_position.setZ(message.getTruePosition().getZ() + message.getAcceleration().getZ());
        new_position.debug();

        x_history->push_back(new_position.getX());
        y_history->push_back(new_position.getZ());

        x2_history->push_back(message.getTruePosition().getX());
        y2_history->push_back(message.getTruePosition().getZ());

/*
x(t+1) = x(t) + Vx / 3.6 + cos(Dx) * cos(Dy) * s * 0.01
y(t+1) = y(t) + Vy / 3.6 + sin(Dx) * cos(Dy) * s * 0.01
z(t+1) = z(t) + Vz / 3.6 + sin(Dy) * s * 0.01
*/

        std::stringstream ss;
        ss << new_position.getX() << " " << new_position.getY() << " " << new_position.getZ();
        sendto(sockfd, ss.str().c_str(), strlen(ss.str().c_str()), 0, (const sockaddr *)NULL, sizeof(server_address));
    }

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    std::vector<double> x{-2, -1, 0, 1, 2};
    std::vector<double> y{2, -1, -2, -1, 2};

    StringReference stringRef;
    std::vector<wchar_t> v;
    stringRef.string = &v;

    // x_history->reserve(x_history->size() + x2_history->size());
    // x_history->insert(x2_history->end(), x2_history->begin(), x2_history->end());
    // y_history->reserve(y_history->size() + y2_history->size());
    // y_history->insert(y_history->end(), y2_history->begin(), y2_history->end());

    DrawScatterPlot(imageRef, 2400, 1600, x_history, y_history, &stringRef);


    std::vector<double> *pngData = ConvertToPNG(imageRef->image);
    WriteToFile(pngData, "plot.png");
    // DeleteImage(imageRef->image);

    close(sockfd);
    return(0);
}