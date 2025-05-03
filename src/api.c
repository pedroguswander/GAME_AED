#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> 
#include "api.h"

// Estrutura para armazenar a resposta da API


// Função de callback para libcurl armazenar a resposta
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Erro: não foi possível alocar memória\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para enviar prompt para a API do Gemini e obter resposta
char* ask_gemini(const char* api_key, const char* prompt) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    // Inicializa a estrutura de memória
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        // URL da API do Gemini
        const char* url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent";
        
        // Monta a URL com a chave da API
        char full_url[512];
        snprintf(full_url, sizeof(full_url), "%s?key=%s", url, api_key);
        
        // Monta o corpo da requisição JSON
        char json_body[2048];
        snprintf(json_body, sizeof(json_body), 
                "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}", 
                prompt);
        
        // Configura os headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        // Configura as opções do curl
        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        
        // Faz a requisição
        res = curl_easy_perform(curl);
        
        // Verifica erros
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }
        
        // Limpeza
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    
    curl_global_cleanup();
    
    return chunk.memory;
}
