#ifndef TOPICS_H_
#define TOPICS_H_
#include "string.h"

/*typedef struct 
{
    char text[100];
} Topic;

const Topic AED = {
    .text = "AED",
};*/

extern const char AED_THEMES[2][100];
extern const char INFRA_SO_THEMES[2][100];

const char (*getThemesOfTopic(char *topic))[100];
#endif