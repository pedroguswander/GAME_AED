// timer.c
#include "timer.h"
#include <raylib.h>

static float timeRemaining = 0;
static bool timerActive = false;
static bool timeOut = false;


bool isTimerRunning() {  // Padronizando o nome (remova qualquer isTimeRunning)
    return timerActive;
}
void startTimer(float duration) {
    timeRemaining = duration;
    timerActive = true;
    timeOut = false;
}

void stopTimer() {
    timerActive = false;
}

void updateTimer() {
    if (timerActive) {
        timeRemaining -= GetFrameTime();
        if (timeRemaining <= 0) {
            timeRemaining = 0;
            timerActive = false;
            timeOut = true;
        }
    }
}

bool isTimeOut() {
    return timeOut;
}

float getRemainingTime() {
    return timeRemaining;
}

void resetTimer() {
    timeRemaining = 0;
    timerActive = false;
    timeOut = false;
}