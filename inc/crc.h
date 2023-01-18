#ifndef CRC16_H_
#define CRC16_H_

#include <cstring>
#include <stdint.h>

class Crc {
    public:
    Crc();
    uint16_t CRC16(uint16_t crc, char data);
    uint16_t computeCrc(unsigned char *commands, int size);
};

#endif /* CRC16_H_ */