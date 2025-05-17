#ifndef TOPICS_H_
#define TOPICS_H_
#include <string.h>
#include <subthemes.h>

extern const char AED_THEMES[5][100];
extern const char INFRA_SO_THEMES[2][100];
extern const char POO_THEMES[2][100];
extern const char HIST_BR_THEMES[2][100];

extern const char CONHECIMENTOS_GERAIS_THEMES[2][100];
extern const char FILMES_THEMES[2][100];
extern const char MUSICAS_THEMES[2][100];
extern const char MATEMATICA_THEMES[2][100];

const char (*getThemesOfTopic(const char *topic))[100];
Theme topicToTheme(const char *topic);


#endif
