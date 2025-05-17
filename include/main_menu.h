#ifndef MENU_H_
#define MENU_H_

typedef enum {
    MAIN_MENU,
    QUIZ_MODE,
    TABULEIRO_MODE,
    HALL_OF_FAME,
    CREDITS,
    FINAL_MENU_SCREEN,
} MenuOption;

extern MenuOption _menuOption;

#endif

