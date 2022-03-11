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
    Message message = Message();
    std::string buffer_string;

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

    int simulation_duration = 1000;
    for (int i = 0; i < simulation_duration * 10; i++) {
        std::cout << "I = " << i << std::endl;
        do {
            bzero(buffer, buff_len);
            receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
            buffer_string = buffer;

            int debug = 0;
            if (buffer_string.find("MSG_START") != std::string::npos) {
                std::cout << ">> START <<" << std::endl;
            } else if (buffer_string.find("TRUE POSITION") != std::string::npos) {
                std::cout << ">> Update True Position <<" << std::endl;
                if (debug)
                    std::cout << "True Position : " << buffer_string << std::endl;
                message.parseTruePosition(buffer_string);
            } else if (buffer_string.find("SPEED") != std::string::npos) {
                std::cout << ">> Update Acceleration <<" << std::endl;
                if (debug)
                    std::cout << "Acceleration : " << buffer_string << std::endl;
                message.parseAcceleration(buffer_string);
            } else if (buffer_string.find("ACCELERATION") != std::string::npos) {
                std::cout << ">> Update Velociy <<" << std::endl;
                if (debug)
                    std::cout << "Velociy : " << buffer_string << std::endl;
                message.parseVelocity(buffer_string);
            } else if (buffer_string.find("DIRECTION") != std::string::npos) {
                std::cout << ">> Update Direction <<" << std::endl;
                if (debug)
                    std::cout << "Direction : " << buffer_string << std::endl;
                message.parseDirection(buffer_string);
            } else if (buffer_string.find("MSG_END") != std::string::npos) {
                std::cout << ">> END <<" << std::endl;

                message.debug();
                // message.getTruePosition().debug();

                double dt = 0.01;

                // Add acceleration to velocity.
                // message.getVelocity()->debug();
                message.getVelocity()->setX(message.getVelocity()->getX() + message.getAcceleration() * dt);
                message.getVelocity()->setY(message.getVelocity()->getY() + message.getAcceleration() * dt);
                message.getVelocity()->setZ(message.getVelocity()->getZ() + message.getAcceleration() * dt);


                // Rotate velocity with euler angles.
                // message.getVelocity()->debug();
                message.getVelocity()->setX(message.getVelocity()->getX() * sin(message.getDirection()->getX()));
                message.getVelocity()->setY(message.getVelocity()->getY() * -(sin(message.getDirection()->getY() * cos(message.getDirection()->getX()))));
                message.getVelocity()->setZ(message.getVelocity()->getZ() * -(cos(message.getDirection()->getY()) * cos(message.getDirection()->getX())));

                // Add velocity to position.
                // message.getVelocity()->debug();
                // message.getTruePosition()->debug();
                message.getTruePosition()->setX(message.getTruePosition()->getX() + message.getVelocity()->getX() * dt);
                message.getTruePosition()->setY(message.getTruePosition()->getY() + message.getVelocity()->getY() * dt);
                message.getTruePosition()->setZ(message.getTruePosition()->getZ() + message.getVelocity()->getZ() * dt);

                message.debug();
                // message.getTruePosition()->debug();
                x_history->push_back(message.getTruePosition()->getX());
                y_history->push_back(message.getTruePosition()->getZ());

                std::stringstream ss;
                ss << message.getTruePosition()->getX() << " " << message.getTruePosition()->getY() << " " << message.getTruePosition()->getZ();
                sendto(sockfd, ss.str().c_str(), strlen(ss.str().c_str()), 0, (const sockaddr *)NULL, sizeof(server_address));
            }
        } while (buffer_string.find("MSG_END") != std::string::npos);
    }

    close(sockfd);
    return(0);
}