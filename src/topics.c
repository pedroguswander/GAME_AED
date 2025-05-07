#include "topics.h"
#include <stdio.h>
#include <string.h>

const char AED_THEMES[2][100] = {
    "PILHA",
    "FILA",
    "ARVORE",
    "BUBBLE SORT",
    "LISTA ENCADEADA",
};

const char INFRA_SO_THEMES[2][100] = {
    "Gerencia de Memoria",
    "Escalanamento"
};

const char (*getThemesOfTopic(char *topic))[100] {
    if (strcmp(topic, "Algoritmos e Estruturas de Dados") == 0) {
        return AED_THEMES;
    }
    else if (strcmp(topic, "INFRA_SO") == 0) {
        return INFRA_SO_THEMES;
    }
    return NULL;
}