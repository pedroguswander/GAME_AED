#include "raylib.h"
#include "resource_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "api.h"
#include "prompt.h"

int main ()
{
	float screenWidth = 1280;
	float screenHeigth = 800;
	bool goToAnwser = false;

	Question _newQuestion = addQuestion();

	Rectangle _statmentRec = {0, 0, strlen(_newQuestion.statement)*16, 16};
	Rectangle _optionARec = {0, 50, strlen(_newQuestion.statement)*16, 16};
	Rectangle _optionBRec = {0, 100, strlen(_newQuestion.statement)*16, 16};
	Rectangle _optionCRec = {0, 150, strlen(_newQuestion.statement)*16, 16};
	Rectangle _optionDRec = {0, 200, strlen(_newQuestion.statement)*16, 16};

	Rectangle _anwserRec = {0, 0, strlen(_newQuestion.statement)*16, 16};

	InitWindow(screenWidth, screenHeigth, "Hello Raylib");

	while (!WindowShouldClose())
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			goToAnwser = true;
		}

		BeginDrawing();

		ClearBackground(BLACK);
		if (!goToAnwser)
		{
			DrawRectangleRec(_statmentRec, RED);
			DrawText(_newQuestion.statement, _statmentRec.x, _statmentRec.y, 16, WHITE);

			DrawRectangleRec(_optionARec, BLUE);
			DrawText(_newQuestion.optionA, _optionARec.x, _optionARec.y, 16, WHITE);

			DrawRectangleRec(_optionBRec, GREEN);
			DrawText(_newQuestion.optionB, _optionBRec.x, _optionBRec.y, 16, WHITE);

			DrawRectangleRec(_optionCRec, YELLOW);
			DrawText(_newQuestion.optionC, _optionCRec.x , _optionCRec.y, 16, WHITE);

			DrawRectangleRec(_optionDRec, PURPLE);
			DrawText(_newQuestion.optionD, _optionDRec.x, _optionDRec.y, 16, WHITE);
		}

		else
		{
			DrawRectangleRec(_anwserRec, RED);
			DrawText(_newQuestion.anwser, _anwserRec.x, _anwserRec.y, 16, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
