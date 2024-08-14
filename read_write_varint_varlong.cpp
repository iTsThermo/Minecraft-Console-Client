//
// Created by Vansh on 8/10/2024.
//

#include "read_write_varint_varlong.h"
#include <WS2tcpip.h>
#include <unistd.h>

int read_write_varint_varlong::readVarInt() const {
    int value = 0;
    int position = 0;
    byte currentByte;

    char * buffer[25565];

    while (true) {
        currentByte = readByte(1, 1000, * buffer);
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;

        if (position >= 32){};
    }

    return value;
}

int read_write_varint_varlong::write_var_int(int x, char * buffer)
{
    int bytes_written = 0;
    while (true)
    {
        unsigned char b = x & 0b01111111;
        x >>= 7;
        if (x != 0)
        {
            b |= 0b10000000;
        }
        buffer[bytes_written++] = b;
        if (x == 0)
        {
            return bytes_written;
        }
    }
}

int read_write_varint_varlong::read_exact(int socket, unsigned int x, char * buffer)
{
    int bytes_read = 0;
    int result;
    while (bytes_read < x)
    {
        result = read(socket, buffer + bytes_read, x - bytes_read);
        if (result < 1)
        {
            return -1;
        }
        bytes_read += result;
    }
    return 0;
}

int read_write_varint_varlong::write_exact(int socket, unsigned int x, char * buffer)
{
    int bytes_written = 0;
    int result;
    while (bytes_written < x)
    {
        result = write(socket, buffer + bytes_written, x - bytes_written);
        if (result < 1)
        {
            return -1;
        }
        bytes_written += result;
    }
    return bytes_written;
}

int read_write_varint_varlong::read_var_int(int socket, int x)
{
    char byte = 0;
    int shift = 0;
    while (true)
    {
        if (read_exact(socket, 1, &byte) == -1)
            return -1;

        x |= (byte & 0b01111111) << shift;
        if (byte & 0b10000000 == 0)
            return 0;

        shift += 7;
    }
}

//socket 1 = read from terminal
int read_write_varint_varlong::readByte(int socket, unsigned int x, char* buffer)
{
    int bytesRead = 0;
    int result = 0;
    while (bytesRead < x)
    {
        result = read(socket, buffer + bytesRead, x - bytesRead);
        if (result < 1 )
        {
            // Throw your error.
        }
        bytesRead += result;
    }
    return result;
}
