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

    while (true) {
        Message message = Message();
        int receive_length;

        std::cout << ">>> START <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        std::cout << ">>> TRUE POSITION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseTruePosition(buffer);
        std::cout << "[" << buffer << "]" << std::endl;
        
        std::cout << ">>> SPEED <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseSpeed(buffer);
        std::cout << buffer << std::endl;
        
        std::cout << ">>> ACCELERATION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseAcceleration(buffer);
        std::cout << buffer << std::endl;

        std::cout << ">>> DIRECTION <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        message.parseDirection(buffer);
        std::cout << buffer << std::endl;
        
        std::cout << ">>> END <<<" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);

        message.debug();

        std::stringstream ss;
        ss << message.getTruePosition().getX() << " " << message.getTruePosition().getY() << " " << message.getTruePosition().getZ();
        std::cout << sendto(sockfd, ss.str().c_str(), strlen(ss.str().c_str()), 0, (const sockaddr *)NULL, sizeof(server_address)) << std::endl;
    }

    close(sockfd);
    return(0);
}