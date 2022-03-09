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
    // std::vector<double> *x2_history = new std::vector<double>();
    // std::vector<double> *y2_history = new std::vector<double>();

    int simulation_duration = 100;
    for (int i = 0; i < simulation_duration * 10; i++) {
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

                message.getTruePosition().debug();

                double dt = 0.01;

                // convert acceleration to km/h2
                double acceleration = message.getAcceleration() / 1000 * 3600;

                // copy velocity in km/h into new velocity in m/s.
                // update velocity with acceleration.
                message.getVelocity().setX(message.getVelocity().getX() + acceleration * dt);
                message.getVelocity().setY(message.getVelocity().getY() + acceleration * dt);
                message.getVelocity().setZ(message.getVelocity().getZ() + acceleration * dt);

                // Make rotation matrix.
                Matrix rotation_matrix = Matrix(3, 3);
                rotation_matrix.set(0, 0, cos(message.getDirection().getY()) * cos(message.getDirection().getX()) - sin(message.getDirection().getY()) * cos(message.getDirection().getZ()) * sin(message.getDirection().getX()));
                rotation_matrix.set(0, 1, -sin(message.getDirection().getY()) * cos(message.getDirection().getX()) - cos(message.getDirection().getY()) * cos(message.getDirection().getZ()) * sin(message.getDirection().getX()));
                rotation_matrix.set(0, 2, sin(message.getDirection().getZ()) * sin(message.getDirection().getX()));
                rotation_matrix.set(1, 0, cos(message.getDirection().getY()) * sin(message.getDirection().getX()) + sin(message.getDirection().getY()) * cos(message.getDirection().getZ()) * cos(message.getDirection().getX()));
                rotation_matrix.set(1, 1, -sin(message.getDirection().getY()) * sin(message.getDirection().getX()) + cos(message.getDirection().getY()) * cos(message.getDirection().getZ()) * cos(message.getDirection().getX()));
                rotation_matrix.set(1, 2, sin(message.getDirection().getZ()) * cos(message.getDirection().getX()));
                rotation_matrix.set(2, 0, -sin(message.getDirection().getY()) * sin(message.getDirection().getZ()));
                rotation_matrix.set(2, 1, cos(message.getDirection().getY()) * sin(message.getDirection().getZ()));
                rotation_matrix.set(2, 2, cos(message.getDirection().getZ()));

                // apply rotation matrix to velocity.
                Matrix tmpVelocityMatrix = Matrix(1, 3);
                tmpVelocityMatrix.set(0, 0, message.getVelocity().getX());
                tmpVelocityMatrix.set(0, 1, message.getVelocity().getY());
                tmpVelocityMatrix.set(0, 2, message.getVelocity().getZ());

                Matrix *rotatedVelocity = rotation_matrix.dot(tmpVelocityMatrix);
                // rotatedVelocity->print();

                // update position with velocity.
                message.getTruePosition().setX(message.getTruePosition().getX() + rotatedVelocity->get(0, 0) * dt);
                message.getTruePosition().setY(message.getTruePosition().getY() + rotatedVelocity->get(0, 1) * dt);
                message.getTruePosition().setZ(message.getTruePosition().getZ() + rotatedVelocity->get(0, 2) * dt);

                message.getTruePosition().debug();

                x_history->push_back(message.getTruePosition().getX());
                y_history->push_back(message.getTruePosition().getZ());

                std::stringstream ss;
                ss << message.getTruePosition().getX() << " " << message.getTruePosition().getY() << " " << message.getTruePosition().getZ();
                sendto(sockfd, ss.str().c_str(), strlen(ss.str().c_str()), 0, (const sockaddr *)NULL, sizeof(server_address));
            }
        } while (buffer_string.find("MSG_END") != std::string::npos);
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