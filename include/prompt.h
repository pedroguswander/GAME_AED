#ifndef PROMPT_H_
#define PROMPT_H_

#include <string.h>
#include <question.h>
#include <stdio.h>
#include <stdlib.h>
#include <topics.h>
#include <subthemes.h>

Question addQuestion(const char* topic, Theme theme);
char* lerJsonDeFormatacao(const char* nome_arquivo);
char* escapar_json(const char* input);
char* trim_whitespace(char *str);

#endif