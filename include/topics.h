#ifndef TOPICS_H_
#define TOPICS_H_
#include "string.h"

extern const char AED_THEMES[5][100];
extern const char INFRA_SO_THEMES[2][100];
extern const char POO_THEMES[2][100];
extern const char HIST_BR_THEMES[2][100];

const char (*getThemesOfTopic(char *topic))[100];

#endif