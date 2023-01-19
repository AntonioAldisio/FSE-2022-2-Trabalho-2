#ifndef PID_H_
#define PID_H_

void setup(double Kp_, double Ki_, double Kd_);
void pid_atualiza_referencia(float referencia_);
double pid_controle(double saida_medida);

#endif /* PID_H_ */