#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)

// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t duty_80  = TPM_MODULE*0.800;       // 50% de duty cycle (meio brilho)
uint16_t duty_98  = TPM_MODULE*0.980; 
uint16_t duty_100 = TPM_MODULE*1;

#define SLEEP_TIME_MS   2000


int main(void)
{
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM1, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);



    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOB_18
    // - modo TPM_PWM_H (nível alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19);
    pwm_tpm_Ch_Init(TPM1, 0, TPM_PWM_H, GPIOE, 20);




    // Define o valor do duty cycle: nesse caso, duty_100 (LED quase desligado)





    // Loop infinito
    for (;;)
    {
    pwm_tpm_CnV(TPM2, 0, duty_80);
    pwm_tpm_CnV(TPM2, 1,duty_98);
    pwm_tpm_CnV(TPM1, 0,0);
    k_msleep(SLEEP_TIME_MS);
    pwm_tpm_CnV(TPM2, 0, duty_100);
    pwm_tpm_CnV(TPM2, 1,duty_100);
    pwm_tpm_CnV(TPM1, 0,1000);
    k_msleep(SLEEP_TIME_MS);// O programa poderia alterar o duty cycle dinamicamente aqui se desejado
    }

    return 0;
}