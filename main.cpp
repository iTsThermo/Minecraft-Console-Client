#undef UNICODE

#include <ws2tcpip.h>
#include <stdio.h>

#include "read_write_varint_varlong.h"
#include "client.h"

#pragma comment (lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <winsock2.h>

#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 25565
#define DEFAULT_PORT "25565"

int __cdecl main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Declare pointers to a structure that will hold the server's address information
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to one of the addresses returned by getaddrinfo
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to the server using the address information from getaddrinfo
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Attempt to connect the socket to the server
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        // Check if the connection failed
        if (iResult == SOCKET_ERROR) {
            // If it failed, close the socket and try the next address
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }

        // If the connection was successful, break out of the loop
        break;
    }

    // Free the memory allocated by getaddrinfo for the result
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    const char * server_address = "cosmicsky.com";
    short server_port = 25565;

    char * buffer = new char[1024];
    int bytes_written = read_write_varint_varlong::write_var_int(0, buffer);
    bytes_written += read_write_varint_varlong::write_var_int(765, buffer + bytes_written);
    // write string length as var int
    bytes_written += read_write_varint_varlong::write_var_int(strlen(server_address), buffer + bytes_written);
    // write string
    memcpy(buffer + bytes_written, server_address, strlen(server_address));
    bytes_written += strlen(server_address);

    // write port as short
    memcpy(buffer + bytes_written, &server_port, 2);
    bytes_written += 2;

    buffer[bytes_written++] = 1;

    char * length_buffer = new char[1024];
    int length_bytes_written = read_write_varint_varlong::write_var_int(bytes_written, &*length_buffer);

    iResult = send(ConnectSocket, buffer, length_bytes_written, 0);

    if (iResult == -1) {
        printf("Error Occurred while sending packets.");
        return -1;
    }

    // if (obj->write_exact(SD_SEND, length_bytes_written, *length_buffer) == -1)
    //     return -1;
    // if (obj->write_exact(SD_SEND, bytes_written, buffer) == -1)
    //     return -1;

    iResult = shutdown(ConnectSocket, SD_SEND);

    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    do {
        // Receive data into the receive buffer
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

        // Check if data was received
        if (iResult > 0)
            // If so, print the number of bytes received
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            // If no data was received, the connection was closed
            printf("Connection closed\n");
        else
            // If recv failed, print an error message with the error code
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0); // Continue the loop as long as data is being received

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
