# 🧠 MIND RUNNER - Jogo de Perguntas e Respostas Interativo

> Um jogo educacional em C com Raylib, desenvolvido para a disciplina de Algoritmos e Estruturas de Dados. Inspirado em jogos de tabuleiro como *Kahoot!* e *Perguntados*, com perguntas carregadas de forma dinâmica por uma IA.

---

## 🧠 Sobre o Jogo

O **MIND RUNNER** é um jogo de perguntas e respostas em estilo tabuleiro para dois jogadores. Cada jogador rola um dado para se mover por casas temáticas (Matemática, Músicas, Filmes, etc). Ao cair numa casa, responde a uma pergunta com 4 alternativas. O objetivo é chegar até a última casa respondendo corretamente.

---

## 🕹️ Funcionalidades

- 🎨 Interface gráfica com Raylib
- 🎲 Sistema de tabuleiro com animação de jogadores
- 📚 Temas variados com perguntas dinâmicas
- 👤 Inserção dos nomes dos jogadores no início do jogo
- ✅ Verificação de resposta e movimentação conforme acerto/erro
- 🏆 Tela de vitória personalizada com o nome do vencedor
- 🔥 Planejado: Efeitos visuais de celebração (fogos de artifício)


## 🚀 Como Rodar

### Pré-requisitos

- **GCC** (ou Clang)
- **Raylib** (v4.0 ou superior)
- **CMake**
- **libcurl** e **cJSON**

> No Ubuntu, instale com:
```bash
sudo apt update
sudo apt install cmake gcc libcurl4-openssl-dev libraylib-dev libcjson-dev
```

### Compilação

No terminal, dentro da raiz do projeto:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make
./MIND_RUNNER
```

---

## 🛠️ Controles

| Ação                    | Tecla/Mouse           |
|-------------------------|-----------------------|
| Rolar dado              | `Espaço`              |
| Selecionar resposta     | `Clique do mouse`     |
| Avançar após resposta   | `Clique no botão`     |



## 📜 Licença

Este projeto é apenas para fins acadêmicos.

---

## Autores
- Antônio Laprovitera
- Pedro Gusmão
- Fernando Augusto
- Felipe Andrade
