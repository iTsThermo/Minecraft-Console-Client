//
// Created by Vansh on 8/10/2024.
//

#ifndef READ_WRITE_VARINT_VARLONG_H
#define READ_WRITE_VARINT_VARLONG_H

class read_write_varint_varlong {
public:
    int readVarInt() const;

    static int readByte(int socket, unsigned int x, char*buffer);

    static int write_var_int(int x, char*buffer);

    static int read_exact(int socket, unsigned int x, char*buffer);

    static int write_exact(int socket, unsigned int x, char* buffer);

    int read_var_int(int socket, int x);

private:
    int SEGMENT_BITS = 0x7F;
    int CONTINUE_BIT = 0x80;
};



#endif //READ_WRITE_VARINT_VARLONG_H
