#include "prompt.h"
#include "api.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "subthemes.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

Question addQuestion(const char* topic, Theme theme)
{
    // Carrega o JSON de exemplo (mantendo por enquanto, embora não usemos no prompt)
    char* jsonFormatado = lerJsonDeFormatacao("exemplo_questao.json");
    if (!jsonFormatado) {
        fprintf(stderr, "Falha ao carregar o JSON de exemplo\n");
        return createQuestion("", "", "", "", "", "");
    }

    printf("get_random_subtheme chamado com tema: %d\n", theme);
    const char* subtema = get_random_subtheme(theme);
    if (subtema == NULL) {
        fprintf(stderr, "Erro ao obter subtema aleatório.\n");
        return createQuestion("", "", "", "", "", "");
    }
    printf("Subtema selecionado: \"%s\"\n", subtema);

    char prompt[1024];
    snprintf(prompt, sizeof(prompt),
        "Crie uma questão de múltipla escolha sobre %s em formato JSON com enunciado, 4 alternativas (A, B, C, D) e a resposta correta.",
        subtema
    );

    // *** CÓDIGO DE DEBUG (MANTENHA) ***
    printf("Prompt enviado para a API (final):\n%s\n", prompt);

    // Faz a requisição para a API Gemini (o prompt já está formatado como string)
    char* response = ask_gemini(prompt);
    free(jsonFormatado);

    // *** INSERIR O CÓDIGO DE DEBUG ABAIXO DESTA LINHA ***
    printf("Resposta bruta da API (bytes):\n");
    for (size_t i = 0; i < strlen(response); i++) {
        printf("%02x ", (unsigned char)response[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    // *** INSERIR O CÓDIGO DE DEBUG ACIMA DESTA LINHA ***

    if (!response) {
        fprintf(stderr, "Erro na resposta da API Gemini\n");
        return createQuestion("", "", "", "", "", "");
    }

    // Remover espaços em branco
    char *trimmed_response = trim_whitespace(response);

    // Debug da resposta bruta
    if (response != NULL) {
        printf("Tamanho da resposta bruta: %zu\n", strlen(response));
        printf("Primeiro byte da resposta (hex): %02x\n", (unsigned char)response[0]);
        printf("Último byte da resposta (hex): %02x\n", (unsigned char)response[strlen(response) - 1]);
        if (response[0] == '{' && response[strlen(response) - 1] == '}') {
            printf("A resposta parece começar e terminar com chaves.\n");
        } else {
            printf("A resposta NÃO começa ou termina com chaves.\n");
        }
    } else {
        printf("A resposta da API é NULL.\n");
    }
    printf("\nResposta bruta da API (string):\n%s\n", response);

    // Faz o parsing do JSON da resposta
    cJSON *root = cJSON_Parse(response);
    free(response);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Erro ao fazer parse do JSON da resposta na posição: %s\n", error_ptr);
        } else {
            fprintf(stderr, "Erro desconhecido ao fazer parse do JSON da resposta.\n");
        }
        return createQuestion("", "", "", "", "", "");
    }

    cJSON *candidates = cJSON_GetObjectItem(root, "candidates");
    if (cJSON_IsArray(candidates) && cJSON_GetArraySize(candidates) > 0) {
        cJSON *first_candidate = cJSON_GetArrayItem(candidates, 0);
        cJSON *content = cJSON_GetObjectItem(first_candidate, "content");
        if (content) {
            cJSON *parts = cJSON_GetObjectItem(content, "parts");
            if (cJSON_IsArray(parts) && cJSON_GetArraySize(parts) > 0) {
                cJSON *part = cJSON_GetArrayItem(parts, 0);
                cJSON *text = cJSON_GetObjectItem(part, "text");
                if (cJSON_IsString(text)) {
                    printf("String sendo passada para cJSON_Parse(text->valuestring):\n%s\n", text->valuestring);
                    const char *json_start = strstr(text->valuestring, "{");
                    const char *json_end = strrchr(text->valuestring, '}');

                    if (json_start && json_end && json_start < json_end) {
                        size_t json_length = json_end - json_start + 1;
                        char *json_response = malloc(json_length + 1);
                        if (json_response) {
                            strncpy(json_response, json_start, json_length);
                            json_response[json_length] = '\0';
                            printf("JSON extraído da resposta:\n%s\n", json_response);

                            // Tenta fazer parse do JSON extraído
                            cJSON *question_json = cJSON_Parse(json_response);
                            free(json_response);

                            if (!question_json) {
                                const char *error_ptr = cJSON_GetErrorPtr();
                                if (error_ptr != NULL) {
                                    fprintf(stderr, "Erro ao fazer parse do JSON extraído na posição: %s\n", error_ptr);
                                } else {
                                    fprintf(stderr, "Erro desconhecido ao fazer parse do JSON extraído.\n");
                                }
                                printf("Texto bruto recebido:\n%s\n", text->valuestring);
                                // *** TENTATIVA DE PARSE MAIS ROBUSTA ***
                                const char *second_json_start = strchr(json_start + 1, '{');
                                if (second_json_start && second_json_start < json_end) {
                                    size_t first_json_length = second_json_start - json_start;
                                    char *first_json_str = malloc(first_json_length + 1);
                                    if (first_json_str) {
                                        strncpy(first_json_str, json_start, first_json_length);
                                        first_json_str[first_json_length] = '\0';
                                        printf("Tentando parsear o PRIMEIRO JSON extraído:\n%s\n", first_json_str);
                                        cJSON *first_question_json = cJSON_Parse(first_json_str);
                                        free(first_json_str);
                                        if (first_question_json) {
                                            question_json = first_question_json; // Usa o primeiro JSON válido
                                        }
                                    }
                                }
                            }

                            if (question_json) {
                                cJSON *enunciado_item = cJSON_GetObjectItem(question_json, "enunciado");
                                cJSON *alternativas_item = cJSON_GetObjectItem(question_json, "alternativas");
                                cJSON *resposta_correta_item = cJSON_GetObjectItem(question_json, "respostaCorreta");

                                char *enunciado_str = NULL;
                                char *a_str = NULL;
                                char *b_str = NULL;
                                char *c_str = NULL;
                                char *d_str = NULL;
                                char *correta_str = NULL;

                                // Tenta ler com os nomes "enunciado" e "alternativas"
                                if (enunciado_item && cJSON_IsString(enunciado_item) &&
                                    alternativas_item && cJSON_IsObject(alternativas_item) &&
                                    resposta_correta_item && cJSON_IsString(resposta_correta_item)) {
                                    enunciado_str = strdup(enunciado_item->valuestring);
                                    a_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(alternativas_item, "A")));
                                    b_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(alternativas_item, "B")));
                                    c_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(alternativas_item, "C")));
                                    d_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(alternativas_item, "D")));
                                    correta_str = strdup(resposta_correta_item->valuestring);
                                } else {
                                    // Tenta ler com os nomes "question" e "options"
                                    cJSON *question_item = cJSON_GetObjectItem(question_json, "question");
                                    cJSON *options_item = cJSON_GetObjectItem(question_json, "options");
                                    cJSON *answer_item = cJSON_GetObjectItem(question_json, "answer");

                                    if (question_item && cJSON_IsString(question_item) &&
                                        options_item && cJSON_IsObject(options_item) &&
                                        answer_item && cJSON_IsString(answer_item)) {
                                        enunciado_str = strdup(question_item->valuestring);
                                        a_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(options_item, "A")));
                                        b_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(options_item, "B")));
                                        c_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(options_item, "C")));
                                        d_str = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(options_item, "D")));
                                        correta_str = strdup(answer_item->valuestring);
                                    }
                                }

                                Question q = createQuestion(
                                    enunciado_str ? enunciado_str : "",
                                    a_str ? a_str : "",
                                    b_str ? b_str : "",
                                    c_str ? c_str : "",
                                    d_str ? d_str : "",
                                    correta_str ? correta_str : ""
                                );

                                if (enunciado_str) free(enunciado_str);
                                if (a_str) free(a_str);
                                if (b_str) free(b_str);
                                if (c_str) free(c_str);
                                if (d_str) free(d_str);
                                if (correta_str) free(correta_str);
                                cJSON_Delete(question_json);
                                cJSON_Delete(root);
                                return q;
                            } else {
                                printf("JSON extraído não está em formato válido.\n");
                                printf("JSON extraído:\n%s\n", json_response);
                            }
                        } else {
                            fprintf(stderr, "Erro ao alocar memória para json_response.\n");
                        }
                    } else {
                        printf("Estrutura JSON inválida encontrada na resposta de texto.\n");
                        printf("Texto bruto recebido:\n%s\n", text->valuestring);
                    }
                }
            }
        }
    }

    cJSON_Delete(root);
    return createQuestion("", "", "", "", "", "");
}


char* lerJsonDeFormatacao(const char* nome_arquivo)
{
    FILE *fp = fopen(nome_arquivo, "rb");
    if (!fp) {
        perror("Erro ao abrir arquivo JSON de exemplo");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    rewind(fp);

    char *conteudo = malloc(tamanho + 1);
    if (!conteudo) {
        fclose(fp);
        fprintf(stderr, "Erro de alocação\n");
        return NULL;
    }

    fread(conteudo, 1, tamanho, fp);
    conteudo[tamanho] = '\0';
    fclose(fp);

    return conteudo;
}
