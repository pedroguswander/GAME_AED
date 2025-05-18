#include "prompt.h"
#include "api.h"
#include <curl/curl.h>
//#include <cjson/cJSON.h>
#include "subthemes.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include "cJSON.h"

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
    // Carrega o JSON de exemplo (mantendo por enquanto)
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
        "Crie uma questão de múltipla escolha sobre %s em formato JSON com enunciado (máximo 90 caracteres), 4 alternativas (A, B, C, D, máximo 60 caracteres cada) e a resposta correta.",
        subtema
    );

    
    printf("Prompt enviado para a API (final):\n%s\n", prompt);

    char* response = ask_gemini(prompt);
    free(jsonFormatado);

    printf("Resposta bruta da API (bytes):\n");
    for (size_t i = 0; i < strlen(response); i++) {
        printf("%02x ", (unsigned char)response[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }

    if (!response) {
        fprintf(stderr, "Erro na resposta da API Gemini\n");
        return createQuestion("", "", "", "", "", "");
    }

    char *trimmed_response = trim_whitespace(response);

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
                                // Tenta parsear o primeiro JSON encontrado (se houver múltiplos)
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
                                char *rendered_json = cJSON_Print(question_json);
                                printf("JSON Parseado (Preformatado):\n%s\n", rendered_json);
                                free(rendered_json);

                                cJSON *enunciado_item = cJSON_GetObjectItem(question_json, "question");
                                if (!enunciado_item) {
                                    enunciado_item = cJSON_GetObjectItem(question_json, "enunciado");
                                    if (!enunciado_item) {
                                        enunciado_item = cJSON_GetObjectItem(question_json, "pergunta");
                                        if (!enunciado_item) {
                                        printf("Erro: Campo 'question' ou 'enunciado' não encontrado no JSON parseado.\n");
                                        }
                                    }
                                }

                                cJSON *alternativas_item = cJSON_GetObjectItem(question_json, "options");
                                if (!alternativas_item) {
                                    alternativas_item = cJSON_GetObjectItem(question_json, "alternatives");

                                    if (!alternativas_item) {
                                        alternativas_item = cJSON_GetObjectItem(question_json, "alternativas");
                                        if (!alternativas_item) {
                                            printf("Erro: Campo 'options' ou 'alternatives' ou 'alternativas' não encontrado no JSON parseado.\n");
                                        }
                                    } else {
                                        printf("Campo 'alternatives' ENCONTRADO.\n");
                                    }
                                } else {
                                    printf("Campo 'options' ENCONTRADO.\n");
                                }

                                // DEBUG DAS ALTERNATIVAS COMO OBJETO
                                if (alternativas_item && cJSON_IsObject(alternativas_item)) {
                                    printf("Tentando extrair alternativas como objeto.\n");
                                    cJSON *a_item = cJSON_GetObjectItem(alternativas_item, "A");
                                    printf("Item A: %p\n", a_item);
                                    cJSON *b_item = cJSON_GetObjectItem(alternativas_item, "B");
                                    printf("Item B: %p\n", b_item);
                                    cJSON *c_item = cJSON_GetObjectItem(alternativas_item, "C");
                                    printf("Item C: %p\n", c_item);
                                    cJSON *d_item = cJSON_GetObjectItem(alternativas_item, "D");
                                    printf("Item D: %p\n", d_item);
                                }

                                // DEBUG DAS ALTERNATIVAS COMO ARRAY
                                if (alternativas_item && cJSON_IsArray(alternativas_item)) {
                                    printf("Tentando extrair alternativas como array.\n");
                                    int num_alternativas = cJSON_GetArraySize(alternativas_item);
                                    printf("Número de alternativas no array: %d\n", num_alternativas);
                                    for (int i = 0; i < num_alternativas; i++) {
                                        cJSON *alt_obj = cJSON_GetArrayItem(alternativas_item, i);
                                        printf("Objeto de alternativa %d: %p\n", i, alt_obj);
                                        if (alt_obj) {
                                            cJSON *letter_item = cJSON_GetObjectItem(alt_obj, "letter");
                                            printf("  Letra: %p\n", letter_item);
                                            cJSON *text_item = cJSON_GetObjectItem(alt_obj, "text");
                                            printf("  Texto: %p\n", text_item);
                                        }
                                    }
                                }

                                cJSON *resposta_correta_item = cJSON_GetObjectItem(question_json, "resposta_correta");
                                if (!resposta_correta_item) {
                                    resposta_correta_item = cJSON_GetObjectItem(question_json, "respostaCorreta");
                                    if (!resposta_correta_item) {
                                        cJSON *answer_item = cJSON_GetObjectItem(question_json, "answer");
                                        if (!answer_item) {
                                            cJSON *resposta_item = cJSON_GetObjectItem(question_json, "resposta");
                                            if (!resposta_item) {
                                                printf("Erro: Campo 'resposta_correta', 'respostaCorreta', 'answer' ou 'resposta' não encontrado no JSON parseado.\n");
                                            } else {
                                                resposta_correta_item = resposta_item; // Se "resposta" foi encontrado, use-o
                                            }
                                        } else {
                                            resposta_correta_item = answer_item;
                                        }
                                    }
                                }

                                char *enunciado_str = NULL;
                                char *a_str = NULL;
                                char *b_str = NULL;
                                char *c_str = NULL;
                                char *d_str = NULL;
                                char *correta_str = NULL;

                                if (enunciado_item && cJSON_IsString(enunciado_item)) enunciado_str = strdup(enunciado_item->valuestring);

                                if (alternativas_item) {
                                    if (cJSON_IsArray(alternativas_item)) {
                                        int num_alternativas = cJSON_GetArraySize(alternativas_item);
                                        for (int i = 0; i < num_alternativas; i++) {
                                            cJSON *alt_obj = cJSON_GetArrayItem(alternativas_item, i);
                                            cJSON *letter_item = cJSON_GetObjectItem(alt_obj, "letter");
                                            cJSON *text_item = cJSON_GetObjectItem(alt_obj, "text");
                                            if (letter_item && cJSON_IsString(letter_item) && text_item && cJSON_IsString(text_item)) {
                                                if (strcmp(letter_item->valuestring, "A") == 0) a_str = strdup(text_item->valuestring);
                                                else if (strcmp(letter_item->valuestring, "B") == 0) b_str = strdup(text_item->valuestring);
                                                else if (strcmp(letter_item->valuestring, "C") == 0) c_str = strdup(text_item->valuestring);
                                                else if (strcmp(letter_item->valuestring, "D") == 0) d_str = strdup(text_item->valuestring);
                                            } else {
                                                printf("Erro: Formato inválido de alternativa no array.\n");
                                            }
                                        }
                                    } else if (cJSON_IsObject(alternativas_item)) {
                                        cJSON *a_item = cJSON_GetObjectItem(alternativas_item, "A");
                                        if (a_item && cJSON_IsString(a_item)) a_str = strdup(a_item->valuestring); else printf("Erro: Campo 'A' em 'alternativas'/'options' ausente ou não é string.\n");
                                        cJSON *b_item = cJSON_GetObjectItem(alternativas_item, "B");
                                        if (b_item && cJSON_IsString(b_item)) b_str = strdup(b_item->valuestring); else printf("Erro: Campo 'B' em 'alternativas'/'options' ausente ou não é string.\n");
                                        cJSON *c_item = cJSON_GetObjectItem(alternativas_item, "C");
                                        if (c_item && cJSON_IsString(c_item)) c_str = strdup(c_item->valuestring); else printf("Erro: Campo 'C' em 'alternativas'/'options' ausente ou não é string.\n");
                                        cJSON *d_item = cJSON_GetObjectItem(alternativas_item, "D");
                                        if (d_item && cJSON_IsString(d_item)) d_str = strdup(d_item->valuestring); else printf("Erro: Campo 'D' em 'alternativas'/'options' ausente ou não é string.\n");
                                    } else {
                                        printf("Erro: Campo 'alternativas'/'options' não é um objeto nem um array.\n");
                                    }
                                }

                                if (resposta_correta_item && cJSON_IsString(resposta_correta_item)) correta_str = strdup(resposta_correta_item->valuestring);

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
