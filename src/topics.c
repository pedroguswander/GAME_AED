#include "topics.h"
#include <stdio.h>
#include <string.h>

const char AED_THEMES[5][100] = {
    "PILHA", "FILA", "ARVORE", "BUBBLE SORT", "LISTA ENCADEADA"
};

const char INFRA_SO_THEMES[2][100] = {
    "Gerencia de Memoria", "Escalonamento"
};

const char POO_THEMES[2][100] = {
    "Classes e Objetos", "Herança"
};

const char HIST_BR_THEMES[2][100] = {
    "Independência do Brasil", "Ditadura Militar"
};

const char (*getThemesOfTopic(char *topic))[100] {
    if (strcmp(topic, "Algoritmos e Estruturas de Dados") == 0) {
        return AED_THEMES;
    }
    else if (strcmp(topic, "INFRA_SO") == 0) {
        return INFRA_SO_THEMES;
    }
    else if (strcmp(topic, "POO") == 0) {
        return POO_THEMES;
    }
    else if (strcmp(topic, "Historia do Brasil") == 0) {
        return HIST_BR_THEMES;
    }
    return NULL;
}
