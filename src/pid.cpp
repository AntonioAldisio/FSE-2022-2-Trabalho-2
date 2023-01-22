#include "../inc/pid.h"

Pid::Pid () {
    this->control_signal_MAX = 100;
    this->control_signal_MIN = -100;
    this->T = 1;
    this->total_error = 0.0;
    this->previous_error = 0.0;
    this->reference_temp = 0.0;
}
void Pid::userSetup(){
    printf("Escolha o modo de configuracao do PID \n");
    printf("1 - Configuracao padrao \n");
    printf("2 - Configuracao personalizada \n");
    int escolha;
    scanf("%d", &escolha);
    if (escolha == 1){
        pid.setup(50.0, 0.2, 400.0);
    }else if (escolha == 2){
        double kp, ki, kd;
        printf("Escolha o valor do kp \n");
        scanf("%lf", &kp);
        printf("Escolha o valor do ki \n");
        scanf("%lf", &ki);
        printf("Escolha o valor do kd \n");
        scanf("%lf", &kd);
        pid.setup(kp, ki, kd);
    }else{
        printf("Escolha invalida \n");
        printf("Foi colocado os seguintes valores Kp = 30.0 - Ki = 0.2 - Kd = 400.0 \n");
        pid.setup(50.0, 0.2, 400.0);
    }
}
void Pid::setup(double Kp_, double Ki_, double Kd_){
    this->Kp = Kp_;
    this->Ki = Ki_;
    this->Kd = Kd_;
}

void Pid::pid_atualiza_referencia(float temp){
    this->reference_temp = (double) temp;
}

double Pid::pid_controle(double measured_output){

    double error = this->reference_temp - measured_output;

    this->total_error += error;

    if (this->total_error >= this->control_signal_MAX)
    {
        this->total_error = this->control_signal_MAX;
    }
    else if (this->total_error <= this->control_signal_MIN)
    {
        this->total_error = this->control_signal_MIN;
    }

    double delta_errorr = error - this->previous_error;

    this->control_signal = this->Kp*error +
        (this->Ki * this->T)*this->total_error +
        (this->Kd/this->T)*delta_errorr;

    if (this->control_signal >= this->control_signal_MAX)
    {
        this->control_signal = this->control_signal_MAX;
    }
    else if (this->control_signal <= this->control_signal_MIN)
    {
        this->control_signal = this->control_signal_MIN;
    }

    this->previous_error = error;

    return this->control_signal;
}