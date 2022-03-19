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

int init_connection(int port) {
    int sockfd;
    struct sockaddr_in server_address;
    
    put_info("Port: %d\n", port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        put_error("Socket creation error !");
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        put_error("Network connection error !");
    return (sockfd);
}

std::string handle_message(int sockfd) {
    char buffer[1024];

    bzero(buffer, 1024);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)NULL, NULL);
    return (std::string(buffer));
}

void send_message(int sockfd, std::string message) {
    std::cout << "Sending : " << message << std::endl;
    if (sendto(sockfd, message.c_str(), message.length(), 0, NULL, sizeof(struct sockaddr_in)) < 0)
        put_error("Network send error !");
}

int main(int ac, char **av) {
    int sockfd;
    std::string buffer;
    Message message = Message();
    KalmanFilter *kalman_filter = NULL;

    if (ac != 2)
        put_error("Usage: ./ft_kalman <PORT>");
    if (atoi(av[1]) < 1024 || atoi(av[1]) > 65535)
        put_error("Port must be between 1024 and 65535 !");
    sockfd = init_connection(std::stoi(av[1]));

    send_message(sockfd, "READY"); // Handshake
    handle_message(sockfd); // Trajectory Generation. . .
    handle_message(sockfd); // Trajectory Generated!\nSending Info. . .
    for (int i = 0; i < 1000; i++) {
        buffer = handle_message(sockfd);
        std::cout << "i = " << i << std::endl;
        // std::cout << buffer << std::endl;
        if (buffer.find("TRUE POSITION") != std::string::npos) {
            message.parseTruePosition(buffer);
        } else if (buffer.find("SPEED") != std::string::npos) {
            message.parseSpeed(buffer);
        } else if (buffer.find("ACCELERATION") != std::string::npos) {
            message.parseAcceleration(buffer);
        } else if (buffer.find("DIRECTION") != std::string::npos) {
            message.parseDirection(buffer);
        } else if (buffer.find("MSG_END") != std::string::npos) {
            if (i == 5) {
                message.setup();
                kalman_filter = new KalmanFilter(message.getStateMatrix(), 0.01);
                kalman_filter->predict();
                // std::cout << "====== Initial state ====== " << std::endl;
                // message.debug();
                // std::cout << "====== End state ====== " << std::endl;
            } else { // Kalman filter
                message.update(0.01);
                std::cout << message.getStateMatrix() << std::endl;
                Matrix updated = kalman_filter->update(message.getStateMatrix());
                // message.debug();
            }
            send_message(sockfd, kalman_filter->getState().to_string()); // Send location
            kalman_filter->predict();
        }
    }
    delete kalman_filter;
    close(sockfd);
    return(0);
}
