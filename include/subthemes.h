// subthemes.h
#ifndef SUBTHEMES_H
#define SUBTHEMES_H

typedef enum {
    THEME_VIDEO_GAMES,
    THEME_MUSICA,
    THEME_FILMES,
    THEME_CONHECIMENTOS_GERAIS,
    THEME_AED,
    THEME_COUNT
} Theme;

const char* get_random_subtheme(Theme theme);

extern const char* subthemes_videogames[];
extern const int subthemes_videogames_count;

extern const char* subthemes_musics[];
extern const int subthemes_musics_count;

extern const char* subthemes_movies[];
extern const int subthemes_movies_count;

extern const char* subthemes_general_knowledge[];
extern const int subthemes_general_knowledge_count;

extern const char* subthemes_aed[];
extern const int subthemes_aed_count;

#endif
