//
// Created by Vansh on 8/12/2024.
//

#include "client.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT "25565"

int client::start() {
    int sockfd, connfd;
    struct sockaddr_in servaddr;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    WSAData wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    iResult = getaddrinfo("cosmicsky.com", DEFAULT_PORT, &hints, &result);
    // Attempt to connect to one of the addresses returned by getaddrinfo
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to the server using the address information from getaddrinfo
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        // Check if the socket creation failed
        if (sockfd == INVALID_SOCKET) {
            // If it failed, print an error message with the error code, clean up, and exit
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Attempt to connect the socket to the server
        iResult = connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);

        // Check if the connection failed
        if (iResult == SOCKET_ERROR) {
            // If it failed, close the socket and try the next address
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }

        // If the connection was successful, break out of the loop
        break;
    }

    // Free the memory allocated by getaddrinfo for the result
    freeaddrinfo(result);
    // socket create and verification
    if (sockfd == -1)
        return -1;
    ZeroMemory(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(25565);

    // connect the client socket to server socket
    if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        return -1;
    else
        std::cout << "Connected to the server\n";
}
