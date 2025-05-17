#include <stdlib.h>
#include <subthemes.h>
#include <string.h>
#include <stdio.h>

const char* subthemes_math[] = {
    "adicao e subtracao de numeros inteiros",
    "multiplicacao e divisao de numeros inteiros",
    "ordem das operacoes",
    "fracoes equivalentes",
    "adicao e subtracao de fracoes",
    "multiplicacao e divisao de fracoes",
    "numeros decimais",
    "conversao entre fracoes e decimais",
    "porcentagem e descontos simples",
    "porcentagem no cotidiano",
    "proporcoes e regras de tres simples",
    "multiplos e divisores",
    "MMC e MDC em problemas com enfase na resposta numerica",
    "numeros primos conceito e identificacao",
    "potenciacao com numeros naturais calculo simples",
    "radiciacao simples raizes exatas",
    "propriedades da adicao e multiplicacao identificacao",
    "expressoes numericas simples",
    "valor absoluto conceito e calculo",
    "conjuntos e subconjuntos simples conceito e exemplos",
    "unidades de medida comprimento massa tempo conversoes basicas",
    "media aritmetica simples calculo",
    "mediana e moda identificacao em conjuntos pequenos",
    "probabilidade basica com dados ou moedas resultado numerico",
    "nocoes de estatistica no cotidiano interpretacao textual",
    "angulos retos agudos e obtusos identificacao",
    "operacoes com angulos adicao e subtracao simples",
    "classificacao de triangulos por lados e angulos textual",
    "soma dos angulos internos de triangulos conceito",
    "introducao a algebra o que e uma variavel"
};

const int subthemes_math_count = sizeof(subthemes_math) / sizeof(subthemes_math[0]);

const char* subthemes_musics[] = {
    "notas musicais basicas",
    "escala musical natural",
    "instrumentos de corda",
    "instrumentos de sopro",
    "instrumentos de percussao",
    "instrumentos eletronicos",
    "vozes masculinas e femininas",
    "principais estilos musicais",
    "ritmos populares do Brasil",
    "ritmos populares do mundo",
    "samba e sua origem",
    "forro e suas variacoes",
    "bossa nova e seus representantes",
    "rock nacional anos 80",
    "rock internacional classico",
    "pop internacional atual",
    "artistas da MPB",
    "bandas de rock famosas",
    "cantores pop dos anos 2000",
    "historia do funk brasileiro",
    "caracteristicas do sertanejo",
    "musica classica para iniciantes",
    "compositores classicos famosos",
    "elementos da musica ritmo melodia harmonia",
    "notacao musical basica",
    "figuras ritmicas simples seminima minima colcheia",
    "tempo e compasso musical",
    "leitura de partitura basica",
    "diferenca entre ritmo e melodia",
    "o que e um refrao"
};

const int subthemes_musics_count = sizeof(subthemes_musics) / sizeof(subthemes_musics[0]);

const char* subthemes_movies[] = {
    "a saga Harry Potter",
    "a trilogia Senhor dos Anéis",
    "a saga Star Wars",
    "a saga Crepúsculo",
    "a série de filmes Jogos Vorazes",
    "os filmes do universo Marvel",
    "os filmes do universo DC",
    "os filmes da Pixar",
    "os filmes clássicos da Disney",
    "os filmes da DreamWorks",
    "os filmes da Illumination",
    "os filmes de Quentin Tarantino",
    "os filmes de Steven Spielberg",
    "os filmes de Christopher Nolan",
    "os filmes de Tim Burton",
    "os filmes de Martin Scorsese",
    "os filmes de Alfred Hitchcock",
    "os filmes de James Cameron",
    "os filmes de Ridley Scott",
    "os filmes de Wes Anderson",
    "os filmes dirigidos por mulheres",
    "filmes baseados em livros",
    "filmes inspirados em fatos reais",
    "filmes com viagem no tempo",
    "filmes de distopia futurista",
    "filmes ambientados em escolas",
    "filmes que se passam no espaço",
    "filmes que se passam em um só local",
    "filmes de tribunal",
    "filmes de esporte",
    "filmes com animais protagonistas",
    "filmes com robôs ou inteligência artificial",
    "filmes de zumbis",
    "filmes de monstros gigantes",
    "filmes com vilões memoráveis",
    "filmes com duplas de protagonistas",
    "filmes com crianças como protagonistas",
    "filmes com trilha sonora marcante",
    "filmes de romance adolescente",
    "filmes de comédia romântica",
    "filmes de ação com perseguições",
    "filmes de guerra",
    "filmes de espionagem",
    "filmes de faroeste",
    "filmes de suspense psicológico",
    "filmes de terror sobrenatural",
    "filmes de terror slasher",
    "filmes de máfia ou crime organizado",
    "filmes de investigação policial",
    "filmes de sobrevivência na natureza",
    "filmes com histórias inspiradoras",
    "filmes com finais abertos",
    "filmes com narradores não confiáveis",
    "filmes mudos clássicos",
    "filmes vencedores do Oscar de melhor filme",
    "filmes indicados ao Globo de Ouro",
    "filmes vencedores em Cannes",
    "filmes cult",
    "filmes independentes premiados",
    "filmes dos anos 80",
    "filmes dos anos 90",
    "filmes dos anos 2000",
    "filmes lançados nos anos 2010",
    "filmes brasileiros famosos",
    "filmes de Bollywood",
    "filmes franceses premiados",
    "filmes japoneses de animação",
    "curtas-metragens famosos",
    "atores consagrados de Hollywood",
    "atrizes vencedoras do Oscar",
    "Leonardo DiCaprio e sua filmografia",
    "Tom Hanks e sua carreira",
    "Meryl Streep e seus papéis marcantes",
    "Sandra Bullock em comédias e dramas",
    "Keanu Reeves e seus personagens icônicos",
    "Johnny Depp em filmes excêntricos",
    "Emma Watson e seus papéis pós-Harry Potter",
    "Denzel Washington e seus filmes premiados",
    "Will Smith e suas atuações cômicas e dramáticas",
    "Natalie Portman e sua versatilidade",
    "Robin Williams em papéis emocionantes",
    "atores mirins famosos no cinema",
    "personagens femininas icônicas",
    "casais famosos do cinema",
    "amizades marcantes no cinema",
    "vilões clássicos do cinema",
    "anti-heróis em filmes populares",
    "filmes que viraram séries",
    "filmes que tiveram remakes",
    "filmes que foram rebootados",
    "trilogias famosas",
    "sagas com mais de três filmes",
    "filmes que viraram musicais",
    "filmes que se tornaram livros",
    "bastidores de grandes filmes",
    "filmes com cenas de improviso",
    "erros de gravação engraçados",
    "frases icônicas do cinema",
    "pôsteres de filmes famosos",
    "trailers que enganaram o público",
    "filmes lançados direto em streaming",
    "filmes que causaram polêmica",
    "filmes com grandes bilheterias",
    "filmes que fracassaram no lançamento",
    "estreias mais esperadas do cinema",
    "curiosidades do Oscar",
};

const int subthemes_movies_count = sizeof(subthemes_movies) / sizeof(subthemes_movies[0]);

const char* subthemes_general_knowledge[] = {
    "capitais dos paises",
    "bandeiras nacionais",
    "idiomas oficiais ao redor do mundo",
    "principais rios do mundo",
    "maiores desertos da Terra",
    "cordilheiras e montanhas famosas",
    "mares e oceanos",
    "continentes e suas caracteristicas",
    "geografia do Brasil",
    "estados e capitais brasileiras",
    "biomas brasileiros",
    "animais ameacados de extincao",
    "invencoes que mudaram o mundo",
    "descobrimentos historicos",
    "personagens historicos importantes",
    "guerras mundiais",
    "mitologias gregas e romanas",
    "religiao e simbolos religiosos",
    "festas e tradicoes culturais",
    "alimentos tipicos de diversos paises",
    "costumes de diferentes culturas",
    "curiosidades sobre o corpo humano",
    "orgaos e funcoes do corpo humano",
    "sistemas do corpo humano",
    "descobertas cientificas famosas",
    "cientistas renomados",
    "teorias da fisica classica",
    "quimica do dia a dia",
    "simbolos da tabela periodica",
    "energia renovavel e sustentavel"
};


const int subthemes_general_knowledge_count = sizeof(subthemes_general_knowledge) / sizeof(subthemes_general_knowledge[0]);

const char* subthemes_aed[] = {
    "o que é um algoritmo",
    "passos para criar um algoritmo",
    "algoritmos sequenciais",
    "algoritmos com decisões (if/else)",
    "uso de repetições (loops)",
    "diferença entre for e while",
    "variáveis em algoritmos",
    "entrada e saída de dados",
    "conceito de função",
    "funções simples e chamadas"
};


const int subthemes_aed_count = sizeof(subthemes_aed) / sizeof(subthemes_aed[0]);

const char* get_random_subtheme(Theme theme) {
    printf("get_random_subtheme chamado com tema: %d\n", theme);
    int index = 0;
    switch (theme) {
        case THEME_MATEMATICA:
            index = rand() % subthemes_math_count;
            printf("Tema MATEMATICA, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_math[index]);
            return subthemes_math[index];
        case THEME_MUSICA:
            index = rand() % subthemes_musics_count;
            printf("Tema MUSICA, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_musics[index]);
            return subthemes_musics[index];
        case THEME_FILMES:
            index = rand() % subthemes_movies_count;
            printf("Tema FILMES, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_movies[index]);
            return subthemes_movies[index];
        case THEME_CONHECIMENTOS_GERAIS:
            index = rand() % subthemes_general_knowledge_count;
            printf("Tema CONHECIMENTOS_GERAIS, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_general_knowledge[index]);
            return subthemes_general_knowledge[index];
        case THEME_AED:
            index = rand() % subthemes_aed_count;
            printf("Tema AED, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_aed[index]);
            return subthemes_aed[index];
        default:
            printf("AVISO: Tema inválido (%d) passado para get_random_subtheme. Retornando NULL.\n", theme);
            return NULL;
    }
}
