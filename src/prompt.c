#include "prompt.h"
#include "api.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>

Question addQuestion(void)
{
    char* jsonFormatado = lerJsonDeFormatacao("exemplo_questao.json");
    char prompt[4096];
    snprintf(prompt, sizeof(prompt),
        "Crie uma nova questão sobre sistemas operacionais no formato JSON idêntico ao seguinte exemplo:\n\n%s. Não inclua ```json ```", jsonFormatado);
    char *promptEscapado = escapar_json(prompt);

    char* response = ask_gemini(promptEscapado);

    cJSON *root = cJSON_Parse(response);
    if (!root) {
        printf("Erro ao fazer parse do JSON da resposta\n");
    } else {
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
                        cJSON *question = cJSON_Parse(text->valuestring);
                        if (question) {
                            cJSON *enunciado = cJSON_GetObjectItem(question, "enunciado");
                            cJSON *a = cJSON_GetObjectItem(question, "alternativaA");
                            cJSON *b = cJSON_GetObjectItem(question, "alternativaB");
                            cJSON *c = cJSON_GetObjectItem(question, "alternativaC");
                            cJSON *d = cJSON_GetObjectItem(question, "alternativaD");
                            cJSON *correta = cJSON_GetObjectItem(question, "correta");
                            
                            return createQuestion(enunciado->valuestring, a->valuestring, b->valuestring, c->valuestring, d->valuestring, correta->valuestring);

                            cJSON_Delete(question);
                        } else {
                            printf("Texto da IA não está em formato JSON válido.\n");
                            printf("Texto bruto: \n%s\n", text->valuestring);
                        }
                    }
                }
            }
        }

        cJSON_Delete(root);
    }
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

char* escapar_json(const char* input) {
    size_t len = strlen(input);
    size_t new_len = len * 2 + 1;  // espaço para escapamentos
    char* escaped = malloc(new_len);
    if (!escaped) return NULL;

    char* dst = escaped;
    for (size_t i = 0; i < len; i++) {
        switch (input[i]) {
            case '\"': *dst++ = '\\'; *dst++ = '\"'; break;
            case '\\': *dst++ = '\\'; *dst++ = '\\'; break;
            case '\n': *dst++ = '\\'; *dst++ = 'n';  break;
            case '\r': *dst++ = '\\'; *dst++ = 'r';  break;
            case '\t': *dst++ = '\\'; *dst++ = 't';  break;
            default: *dst++ = input[i]; break;
        }
    }
    *dst = '\0';
    return escaped;
}
