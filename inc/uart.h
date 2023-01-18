#ifndef UART_H_
#define UART_H_

#include "../inc/modbus.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class Uart {
    private:
    int uart0_filestream;
    struct termios options;
    unsigned char *read_buffer;
    Modbus modbus;
    Crc crc;


    public:
    Uart();
    void setup();
    void send(int msgSize, unsigned char *message);
    int receive();
    void stop();
    float getInternalTemp();
    float getReferenceTemp();
    int getUserInput();
    void sendControlSignal(int signal);
    void sendReferenceSignal(float signal);
    void sendReferenceSignal(unsigned char *signal);
    void setSystemState(unsigned char state);
    void setSystemStatus(unsigned char status);
    void sendTimerSignal(int timer);
    bool isCorrectTemperature(unsigned char sCode);
};

#endif