#ifndef API_H_
#define API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

#define API_KEY ""

char* ask_gemini(const char* prompt);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif
