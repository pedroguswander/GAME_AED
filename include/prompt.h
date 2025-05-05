#ifndef PROMPT_H_
#define PROMPT_H_

#include <string.h>
#include "question.h"
#include <stdio.h>
#include <stdlib.h>

Question addQuestion(void);
char* lerJsonDeFormatacao(const char* nome_arquivo);
char* escapar_json(const char* input);

#endif