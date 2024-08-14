//
// Created by Vansh on 8/14/2024.
//

#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include <ws2tcpip.h>

class handshake {
public:
    void create_handshake_packet(SOCKET, const char *, unsigned short, int, int &) const;
private:
    const char * server_address = "cosmicsky.com";
    unsigned short server_port = 25565;
    int protocal = 765;
};



#endif //HANDSHAKE_H
