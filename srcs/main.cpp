/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbaud <gbaud@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 18:36:03 by gbaud             #+#    #+#             */
/*   Updated: 2022/03/02 06:18:57 by gbaud            ###   ########lyon.fr   */
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


    Matrix *matrix = new Matrix(3, 3);
	Matrix *matrix1 = new Matrix(3, 3);
    std::cout << matrix->get(1, 1) << std::endl;
    matrix->set(2, 2, 17);
	matrix1->set(2, 2, 17);

	matrix1->print();
	matrix->add(*matrix1);
	matrix->print();

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
    
    std::cout << "Try send" << std::endl;
    char handshake[] = "READY";
    sendto(sockfd, handshake, strlen(handshake), 0, (const sockaddr *)NULL, sizeof(server_address));
    std::cout << "End send" << std::endl;

    // Skip Raw Trajectory Generation. . .
    bzero(buffer, buff_len);
    receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
    // Sending Info. . .
    bzero(buffer, buff_len);
    receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);

    while (1) {
        Message message = Message();
        int receive_length;

        std::cout << "MSG_START:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        std::cout << "Raw " << buffer << std::endl;
        
        std::cout << "MSG_SPEED:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        std::cout << "Raw " << buffer << std::endl;
        // message.parseSpeed(buffer);
        
        std::cout << "MSG_TRUEPOS:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        std::cout << "Raw " << buffer << std::endl;
        message.parseTruePosition(buffer);
        
        std::cout << "MSG_ACCEL:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        // std::cout << "Raw " << buffer << std::endl;
        // message.parseAcceleration(buffer);


        std::cout << "MSG_DIREC:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);
        
        // std::cout << "Raw " << buffer << std::endl;
        // message.parseDirection(buffer);
        
        std::cout << "MSG_END:" << std::endl;
        bzero(buffer, buff_len);
        receive_length = recvfrom(sockfd, buffer, buff_len, 0, (struct sockaddr *)NULL, NULL);

        message.debug();
    }
    
    close(sockfd);
    return(0);
}