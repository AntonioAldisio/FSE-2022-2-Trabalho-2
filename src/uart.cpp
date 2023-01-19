#include "../inc/uart.h"
#include "../inc/modbus.h"

const int SIZE_MSG = 9;

Uart::Uart() {
    uart0_filestream = -1;
    modbus = Modbus();
    this->read_buffer = new unsigned char[SIZE_MSG];
}

void Uart::setup(){
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void Uart::send(int msgSize, unsigned char *msg){
    if (uart0_filestream != -1) {
        int count = write(uart0_filestream, msg, msgSize);
        if (count <= 0) {
            return;
        }
    }
}

int Uart::receive(){
    usleep(50000);
    // if (uart0_filestream != -1) {
    int rx_length = read(uart0_filestream, (void*)read_buffer, SIZE_MSG);
    if (rx_length <= 0) {
        return 0;
    }
    return rx_length;
    // }
}

void Uart::stop(){
    close(uart0_filestream);
}


float Uart::getInternalTemp(){
    float internalTemp;
    unsigned char *msg = modbus.internalTempMessage();
    this->send(SIZE_MSG, msg);
    this->receive();
    if(!(msg[2] == read_buffer[2])){
        return getInternalTemp();
    }
    memcpy(&internalTemp, &this->read_buffer[3], sizeof(float));
    return internalTemp;
}

float Uart::getReferenceTemp(){
    float referenceTemp;
    unsigned char *msg = modbus.referenceTempMessage();
    this->send(SIZE_MSG, msg);
    this->receive();
    if(!(msg[2] == read_buffer[2])){
        return getReferenceTemp();
    }
    memcpy(&referenceTemp, &this->read_buffer[3], sizeof(float));
    return referenceTemp;
}

int Uart::getUserInput(){
    int userInput;
    send(9, modbus.userInputMessage());
    receive();

    memcpy(&userInput, &this->read_buffer[3], sizeof(int));
    this->read_buffer = new unsigned char[SIZE_MSG];
    return userInput;
}

void Uart::sendControlSignal(int signal){
    send(13, modbus.sendIntSignalMessage(signal));
}

void Uart::setSystemState(unsigned char state){
    send(10, modbus.setSystemStateMessage(state));
    receive();
}

void Uart::setSystemStatus(unsigned char status){
    send(10, modbus.setSystemStateMessage(status));
    receive();
}