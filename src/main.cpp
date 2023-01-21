#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "../inc/uart.h"
#include "../inc/bme280.h"
#include "../inc/pid.h"
#include "../inc/sensorTemp.h"


struct bme280_dev bme280;
struct identifier id;


bool working = true;
int count = 0;
const int FORNO = 4;
const int VENTOINHA = 5;

void setupPin(){
    if (wiringPiSetup() != -1) {
        pinMode(FORNO, OUTPUT);
        pinMode(VENTOINHA, OUTPUT);

        softPwmCreate(FORNO, 0, 100);
        softPwmCreate(VENTOINHA, 0, 100);

    }
}


void status(double intensidade){
    if (intensidade >= 0){
        softPwmWrite(FORNO, intensidade);
        softPwmWrite(VENTOINHA, 0);
    }else{
        if(intensidade > -40){
            softPwmWrite(FORNO, 40);
            softPwmWrite(VENTOINHA, 0);
        }else{
            double aux = intensidade * (-1);
            softPwmWrite(FORNO, aux);
            softPwmWrite(VENTOINHA, 0);
        }
    }
}


void esquenta(Uart uart, Pid pid, double *intensidade){
    float tempRef = uart.getReferenceTemp();
    float temInter = uart.getInternalTemp();

    *intensidade = 100.0;
    status(*intensidade);

    while(working && temInter <= tempRef){
        tempRef = uart.getReferenceTemp();
        temInter = uart.getInternalTemp();
        *intensidade = pid.pid_controle(temInter);
        uart.sendControlSignal((int)*intensidade);
        sleep(1);
    }
}

int main(void){
    Uart uart;
    Pid pid;

    double intensidade = 0;

    // SetUp
    uart.setup();
    pid.setup(50.0, 0.2, 400.0);
    setupPin();

    Sensor sensor = Sensor("/dev/i2c-1", &bme280, &id);

    while(working){
        int retorno = uart.getUserInput();
        printf("%d \n", retorno);
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
            esquenta(uart, pid, &intensidade);

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