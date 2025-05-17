//
// Created by antonio on 16/05/25.
//

#ifndef PLAYER_NAMES_H
#define PLAYER_NAMES_H

#include "main_menu.h"

#define MAX_NAME_LENGTH 32
extern char player1Name[MAX_NAME_LENGTH];
extern char player2Name[MAX_NAME_LENGTH];

void showPlayerNameInputScreen(MenuOption option);


#endif //PLAYER_NAMES_H
