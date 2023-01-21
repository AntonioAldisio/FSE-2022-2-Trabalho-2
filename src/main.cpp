#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>

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

void stop (int signal) {
    signal = signal;
    working = false;
}

void setupPin(){
    if (wiringPiSetup() != -1) {
        pinMode(FORNO, OUTPUT);
        pinMode(VENTOINHA, OUTPUT);

        softPwmCreate(FORNO, 0, 100);
        softPwmCreate(VENTOINHA, 0, 100);

    }
}


void status(double intensidade){
    printf("intensidade: %f \n", intensidade);
    if (intensidade >= 0){
        softPwmWrite(FORNO, intensidade);
        softPwmWrite(VENTOINHA, 0);
    }else{
        if(intensidade > -40){
            intensidade = 40;
            softPwmWrite(FORNO, 40);
            softPwmWrite(VENTOINHA, 0);
        }else{
            intensidade = intensidade * (-1);
            softPwmWrite(VENTOINHA, intensidade);
            softPwmWrite(FORNO, 0);
        }
    }
}


void esquenta(Uart uart, Pid pid, double *intensidade){
    float tempRef = uart.getReferenceTemp();
    float temInter = uart.getInternalTemp();

    *intensidade = 100.0;
    status(*intensidade);

    while(working && temInter <= tempRef){
        
        printf("Estou Esquetando \n");
        printf("temInter %f\n", temInter);
        printf("ambTemp %f\n\n\n", tempRef);

        tempRef = uart.getReferenceTemp();
        temInter = uart.getInternalTemp();
        
        *intensidade = pid.pid_controle(temInter);
        status(*intensidade);
        uart.sendControlSignal((int)*intensidade);
        sleep(10);
    }
}

void esfriando(Uart uart, Sensor Sensor, Pid pid, double *intensidade){
    double ambTemp = Sensor.getSensorTemp(&bme280);
    float temInter = uart.getInternalTemp();
    float tempRef = uart.getReferenceTemp();

    *intensidade = 100.0;
    status(*intensidade);

    while(working && temInter >= ambTemp){
        
        printf("Estou esfriando\n");
        printf("temInter %f\n", temInter);
        printf("ambTemp %f\n\n\n", ambTemp);
        
        ambTemp = Sensor.getSensorTemp(&bme280);
        temInter = uart.getInternalTemp();
        softPwmWrite(VENTOINHA, 100);
        uart.sendControlSignal((int)*intensidade);
        sleep(10);
    }
    uart.setSystemStatus(0);
}

int main(void){
    Uart uart;
    Pid pid;

    double intensidade = 0;
    bool ligado = false;
    bool execucao = false;

    // SetUp
    uart.setup();
    pid.setup(50.0, 0.2, 400.0);
    setupPin();

    Sensor sensor = Sensor("/dev/i2c-1", &bme280, &id);
    
    signal(SIGINT, stop);
    
    while(working){
        int retorno = uart.getUserInput();
        printf("%d \n", retorno);
        if (retorno == 161){
            if (!ligado){
                ligado = true;
                printf("Ligado \n");
                uart.setSystemState(1);
            }
        }
        else if (retorno == 162){
            if (ligado){
                ligado = false;
                printf("Desligado \n");
                uart.setSystemStatus(0);
                uart.setSystemState(0);
            }
        }
        else if (retorno == 163){
            if (!execucao){
                execucao = true;
                printf("Iniciado \n");
                uart.setSystemStatus(1);
                esquenta(uart, pid, &intensidade);
                esfriando(uart, sensor, pid, &intensidade);
            }
        }
        else if (retorno == 164){
            if (execucao){
                execucao = false;
                printf("Parado \n");
                uart.setSystemStatus(0);
            }
        }
        // TODO
        else if (retorno == 165){
            printf("Manual \n");
        }

        else {
            usleep(1000000);
            // printf("%d \n", retorno);
        }
    }
    status(0);
    uart.receive();
    uart.setSystemState(0);
    uart.setSystemStatus(0);
    uart.stop();
    printf("\n\n Desligando... \n\n");
    return 0;
}