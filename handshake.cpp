//
// Created by Vansh on 8/14/2024.
//

#include "handshake.h"

#include <cstdio>

#include "read_write_varint_varlong.h"

void handshake::create_handshake_packet(SOCKET ConnectSocket, const char *, unsigned short, int, int & iResult) const {
    char * buffer = new char[1024];
    //Packet ID
    int bytes_written = read_write_varint_varlong::write_var_int(0, buffer);
    //Packet Protocal
    bytes_written += read_write_varint_varlong::write_var_int(765, buffer + bytes_written);


    // // write string length as var int
    // bytes_written += read_write_varint_varlong::write_var_int(strlen(server_address), buffer + bytes_written);
    // // write string
    // memcpy(buffer + bytes_written, server_address, strlen(server_address));
    // bytes_written += strlen(server_address);

    memcpy(buffer + bytes_written, server_address, strlen(server_address));
    bytes_written += strlen(server_address);

    // write port as short
    memcpy(buffer + bytes_written, &server_port, 2);
    bytes_written += 2;

    //Next-State - Status
    bytes_written += read_write_varint_varlong::write_var_int(1, buffer + bytes_written);

    //Create PacketID + Data
    char * length_buffer = new char[1024];
    int length_bytes_written = read_write_varint_varlong::write_var_int(bytes_written, length_buffer);

    iResult = send(ConnectSocket, length_buffer, length_bytes_written, 0);
    if (iResult == -1) {
        printf("Error Occurred while sending packets - HandShake length");
    }
    iResult = send(ConnectSocket, buffer, bytes_written, 0);
    if (iResult == -1) {
        printf("Error Occurred while sending packets - Handshake data");
    }
}
