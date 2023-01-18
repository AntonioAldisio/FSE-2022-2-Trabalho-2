#ifndef MODBUS_H_
#define MODBUS_H_

#include "../inc/crc.h"

class Modbus {
    private:
    Crc crcCalculator;

    public:
    Modbus();
    unsigned char * createMessage(unsigned char codigo, unsigned char subcodigo, int tamanhoMsg);
    unsigned char * internalTempMessage();
    unsigned char * referenceTempMessage();
    unsigned char * userInputMessage();
    unsigned char * setSystemStateMessage(unsigned char state);
    unsigned char * setSystemStatusMessage(unsigned char status);
    unsigned char * sendTimerMessage(int timer);
    unsigned char * sendIntSignalMessage(int signal);
    unsigned char * sendFloatSignalMessage(float signal);
};

#endif