#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

void startTimer(float duration);
void stopTimer();
void updateTimer();
bool isTimeOut();
float getRemainingTime();
void resetTimer();

#endif