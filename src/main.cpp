#include "../inc/uart.h"
#include "../inc/bme280.h"
#include "../inc/sensorTemp.h"
#include <time.h>
#include <wiringPi.h>


struct bme280_dev bme;
struct identifier id;


bool working = true;
int count = 0;

int main(void){
    Uart uart;
    // Pid pid;

    uart.setup();
    Sensor sensor = Sensor("/dev/i2c-1", &bme, &id);
    // pid.setup(50.0, 0.2, 400.0);

    while(working){
        printf("%f", sensor.getSensorTemp(&bme));
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
            // printf("%d \n", retorno);
        }
    }
    uart.stop();
    printf("Desligando... \n");
    return 0;
}