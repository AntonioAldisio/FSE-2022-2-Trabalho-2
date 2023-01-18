#include "../inc/uart.h"


const int FORNO = 4;
const int FAN = 5;

bool working = true;

int main(void){
    Uart uart;
    uart.setup();

    // while(working){
        int retorno = uart.getUserInput();
        printf("%d", retorno);
    // }
}