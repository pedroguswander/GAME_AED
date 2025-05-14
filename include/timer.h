#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

// Variável global para controle do tempo
volatile sig_atomic_t timeout_flag = false;

/**
 * @brief Handler para o sinal de alarme
 * 
 * @param sig Número do sinal (não utilizado)
 */
void timeout_handler(int sig) {
    (void)sig;
    timeout_flag = true;
}

/**
 * @brief Configura um timer com um limite de tempo
 * 
 * @param seconds Tempo em segundos para o limite
 */
void setup_timer(int seconds) {
    timeout_flag = false;
    signal(SIGALRM, timeout_handler);
    alarm(seconds);
}

/**
 * @brief Desativa o timer
 */
void disable_timer() {
    alarm(0);
    signal(SIGALRM, SIG_IGN);
}

/**
 * @brief Verifica se o tempo acabou
 * 
 * @return true se o tempo acabou
 * @return false se ainda há tempo
 */
bool is_timed_out() {
    return timeout_flag;
}

#endif // TIMER_H