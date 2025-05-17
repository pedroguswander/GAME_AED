#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"

// Função de callback para libcurl armazenar a resposta
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        // Aqui não dá pra logar arquivo, porque nem tem memória, então só stderr
        fprintf(stderr, "Erro: não foi possível alocar memória\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para enviar prompt para a API do Gemini e obter resposta
char* ask_gemini(const char* prompt) {
    FILE *logfile = fopen("api_log.txt", "a"); // Abre o arquivo para acrescentar conteúdo
    if (!logfile) {
        fprintf(stderr, "Não foi possível abrir o arquivo de log\n");
        // Continua, só sem log em arquivo
    }

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    // Inicializa a estrutura de memória
    chunk.memory = malloc(1);
    if (!chunk.memory) {
        if (logfile) fclose(logfile);
        fprintf(stderr, "Erro de alocação inicial\n");
        return NULL;
    }
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        const char* url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent";

        char full_url[512];
        snprintf(full_url, sizeof(full_url), "%s?key=%s", url, API_KEY);

        // Aloca dinamicamente json_body para evitar truncamento
        size_t json_body_size = strlen(prompt) + 1000;
        char *json_body = malloc(json_body_size);
        if (!json_body) {
            if (logfile) fclose(logfile);
            fprintf(stderr, "Erro de alocação para json_body\n");
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return NULL;
        }

        snprintf(json_body, json_body_size, "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}", prompt);
        
        if (logfile) {
            fprintf(logfile, "=== JSON Enviado para a API ===\n%s\n\n", json_body);
            fflush(logfile); // garante que grava no arquivo imediatamente
        }

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            if (logfile) {
                fprintf(logfile, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                fflush(logfile);
            }
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            chunk.memory = NULL;
        } else {
            if (logfile) {
                fprintf(logfile, "=== Resposta bruta da API ===\n%s\n\n", chunk.memory);
                fflush(logfile);
            }
        }

        free(json_body);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        if (logfile) {
            fprintf(logfile, "Erro ao inicializar curl\n");
            fflush(logfile);
        }
        fprintf(stderr, "Erro ao inicializar curl\n");
        free(chunk.memory);
        chunk.memory = NULL;
    }

    curl_global_cleanup();

    if (logfile) fclose(logfile);

    return chunk.memory;
}
