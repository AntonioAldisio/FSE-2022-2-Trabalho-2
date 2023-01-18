#include "../inc/modbus.h"
#include "../inc/crc.h"

const unsigned char ENDERECO_DISPOSITIVO = 0x01;
const unsigned char SOLICITA = 0x16;
const unsigned char ENVIA = 0x23;
const unsigned char SUB_CODIGO_C1 = 0xC1;
const unsigned char SUB_CODIGO_C2 = 0xC2;
const unsigned char SUB_CODIGO_C3 = 0xC3;
const unsigned char SUB_CODIGO_D1 = 0xD1;
const unsigned char SUB_CODIGO_D2 = 0xD2;
const unsigned char SUB_CODIGO_D3 = 0xD3;
const unsigned char SUB_CODIGO_D4 = 0xD4;
const unsigned char SUB_CODIGO_D5 = 0xD5;
const unsigned char SUB_CODIGO_D6 = 0xD6;
const unsigned char ID[4] = {0x08, 0x02, 0x01, 0x01};


Modbus::Modbus() {
    this ->crcCalculator = Crc();
}

unsigned char *Modbus::createMessage(unsigned char codigo, unsigned char subcodigo, int tamanhoMsg){
    unsigned char *msg = new unsigned char[tamanhoMsg];
    msg[0] = ENDERECO_DISPOSITIVO;
    msg[1] = codigo;
    msg[2] = subcodigo;
    msg[3] = ID[0];
    msg[4] = ID[1];
    msg[5] = ID[2];
    msg[6] = ID[3];

    return msg;
}

unsigned char *Modbus::internalTempMessage(){
    unsigned char *msg = this->createMessage(ENVIA, SUB_CODIGO_C1, 9);
    uint16_t crc = crcCalculator.computeCrc(msg, 7);

    memcpy(&msg[7], &crc, sizeof(crc));
    // msg[7] = crc >> 8;

    return msg;
}

unsigned char *Modbus::referenceTempMessage(){
    unsigned char *msg = this->createMessage(ENVIA, SUB_CODIGO_C2, 9);
    uint16_t crc = crcCalculator.computeCrc(msg, 7);

    memcpy(&msg[7], &crc, sizeof(crc));
    // msg[7] = crc >> 8;


    return msg;
}

unsigned char *Modbus::userInputMessage(){
    unsigned char *msg = this->createMessage(ENVIA, SUB_CODIGO_C3, 9);
    uint16_t crc = crcCalculator.computeCrc(msg, 7);

    memcpy(&msg[7], &crc, sizeof(crc));
    // msg[7] = crc >> 8;

    return msg;
}

unsigned char *Modbus::sendIntSignalMessage(int signal){
    unsigned char *msg = this->createMessage(SOLICITA, SUB_CODIGO_D1, 12);
    memcpy(&msg[7], &crc, sizeof(crc));
    // msg[7] = signal >> 8;
    msg[8] = signal;
    uint16_t crc = crcCalculator.computeCrc(msg, 10);

    memcpy(&msg[11], &crc, sizeof(crc));
    // msg[10] = crc >> 8;

    return msg;
}

unsigned char *Modbus::setSystemStateMessage(unsigned char state){
    unsigned char *msg = this->createMessage(SOLICITA, SUB_CODIGO_D3, 10);
    msg[7] = state;
    uint16_t crc = crcCalculator.computeCrc(msg, 8);

    memcpy(&msg[8], &crc, sizeof(crc));
    // msg[8] = crc >> 8;

    return msg;
}

unsigned char *Modbus::setSystemStatusMessage(unsigned char status){
    unsigned char *msg = this->createMessage(SOLICITA, SUB_CODIGO_D5, 10);
    msg[7] = status;
    uint16_t crc = crcCalculator.computeCrc(msg, 8);

    msg[8] = crc >> 8;

    return msg;
}

unsigned char *Modbus::sendTimerMessage(int timer){
    unsigned char *msg = this->createMessage(SOLICITA, SUB_CODIGO_D6, 12);
    memcpy(&msg[7], &crc, sizeof(crc));
    // msg[7] = timer >> 8;
    msg[8] = timer;
    uint16_t crc = crcCalculator.computeCrc(msg, 10);
    memcpy(&msg[11], &crc, sizeof(crc));
    // msg[10] = crc >> 8;

    return msg;
}
