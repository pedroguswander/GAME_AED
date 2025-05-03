#include "raylib.h"
#include "resource_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "api.h"

int main ()
{
	const char* api_key = "";
	char prompt[1024];

	strcpy(prompt,"de oi para os meus amigos rapido e direto");
	char* response = ask_gemini(api_key, prompt);

	cJSON *json = cJSON_Parse(response);
	cJSON *candidates = cJSON_GetObjectItemCaseSensitive(json,"candidates");
	cJSON *first_candidate = cJSON_GetArrayItem(candidates, 0);
	cJSON *content = cJSON_GetObjectItemCaseSensitive(first_candidate, "content");
	cJSON *parts = cJSON_GetObjectItemCaseSensitive(content,"parts");
	cJSON *first_part = cJSON_GetArrayItem(parts, 0);
	cJSON *text = cJSON_GetObjectItemCaseSensitive(first_part, "text");

	char actualText[1000];
	strcpy(actualText, text->valuestring);

	InitWindow(1280, 800, "Hello Raylib");

	while (!WindowShouldClose())
    {

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText("Hello Raylib", 200,200,20,WHITE);

		if(text) {
            DrawText(actualText, 300, 400, 32, WHITE);
        } else {
            printf("Erro ao obter resposta da API.\n");
        }

		EndDrawing();
	}


	CloseWindow();
	return 0;
}
