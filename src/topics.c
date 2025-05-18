#include "topics.h"
#include <stdio.h>
#include <string.h>
#include <subthemes.h>


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

const char CONHECIMENTOS_GERAIS_THEMES[2][100] = {
    "", ""
};

const char FILMES_THEMES[2][100] = {
    "", ""
};

const char MUSICAS_THEMES[2][100] = {
    "", ""
};

const char MATEMATICA_THEMES[2][100] = {
    "Álgebra", "Geometria"
};

const char (*getThemesOfTopic(const char *topic))[100] {
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
    else if (strcmp(topic, "Conhecimentos Gerais") == 0) {
        return CONHECIMENTOS_GERAIS_THEMES;
    }
    else if (strcmp(topic, "Filmes") == 0) {
        return FILMES_THEMES;
    }
    else if (strcmp(topic, "Músicas") == 0) {
        return MUSICAS_THEMES;
    }
    else if (strcmp(topic, "Matemática") == 0) {
        return THEME_VIDEO_GAMES;
    }
    return NULL;
}

Theme topicToTheme(const char *topic) {
    if (strcmp(topic, "Video-Games") == 0) return THEME_VIDEO_GAMES;
    if (strcmp(topic, "Músicas") == 0) return THEME_MUSICA;
    if (strcmp(topic, "Filmes") == 0) return THEME_FILMES;
    if (strcmp(topic, "Conhecimentos Gerais") == 0) return THEME_CONHECIMENTOS_GERAIS;
    if (strcmp(topic, "Boss Node AED") == 0 || strcmp(topic, "Algoritmos e Estruturas de Dados") == 0) return THEME_AED;
    if (strcmp(topic, "Harry Potter") == 0) return THEME_HARRY_POTTER;
    if (strcmp(topic, "INFRA_SO") == 0) return THEME_SO;
    if (strcmp(topic, "POO") == 0) return THEME_POO;
    if (strcmp(topic, "Final") == 0) return THEME_FILMES;
    // temas do quiz

    //if (strcmp(topic, "POO") == 0) return POO_THEMES;

    return THEME_COUNT; // valor padrão para erro
}

