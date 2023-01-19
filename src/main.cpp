#include "../inc/uart.h"
#include <time.h>
#include <wiringPi.h>


const int FORNO = 4;
const int FAN = 5;

bool working = true;
int count = 0;

int main(void){
    Uart uart;
    Pid pid;

    uart.setup();
    pid.setup(50.0, 0.2, 400.0);

    while(working){
        int retorno = uart.getUserInput();
        if (retorno == 161){
            printf("Ligado \n");
            uart.setSystemState(1);
        }
        else if (retorno == 162){
            printf("Desligado \n");
            uart.setSystemState(0);
        }
        else if (retorno == 163){
            printf("Iniciado \n");
            uart.setSystemStatus(1);
        }
        else if (retorno == 164){
            printf("Parado \n");
            uart.setSystemStatus(0);

        }
        else if (retorno == 165){
            printf("Manual \n");
        }
        else {
            usleep(1000000);
            printf("%d \n", retorno);
        }
    }
    uart.stop();
    printf("Desligando... \n");
    return 0;
}