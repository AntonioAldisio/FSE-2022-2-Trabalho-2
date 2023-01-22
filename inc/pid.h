#ifndef PID_H_
#define PID_H_

class Pid {

    private:
    int control_signal_MAX;
    int control_signal_MIN;
    int T;
    double measured_output, control_signal;
    double reference_temp;
    double Kp;
    double Ki;
    double Kd;
    double total_error, previous_error;

    public:
    Pid();
    void setup(double Kp_, double Ki_, double Kd_);
    void pid_atualiza_referencia(float referencia_);
    double pid_controle(double saida_medida);
    void userSetup();
};

#endif /* PID_H_ */