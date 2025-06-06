#include <stdlib.h>
#include <subthemes.h>
#include <string.h>
#include <stdio.h>

// const char* subthemes_math[] = {
//     "adicao e subtracao de numeros inteiros",
//     "multiplicacao e divisao de numeros inteiros",
//     "ordem das operacoes",
//     "fracoes equivalentes",
//     "adicao e subtracao de fracoes",
//     "multiplicacao e divisao de fracoes",
//     "numeros decimais",
//     "conversao entre fracoes e decimais",
//     "porcentagem e descontos simples",
//     "porcentagem no cotidiano",
//     "proporcoes e regras de tres simples",
//     "multiplos e divisores",
//     "MMC e MDC em problemas com enfase na resposta numerica",
//     "numeros primos conceito e identificacao",
//     "potenciacao com numeros naturais calculo simples",
//     "radiciacao simples raizes exatas",
//     "propriedades da adicao e multiplicacao identificacao",
//     "expressoes numericas simples",
//     "valor absoluto conceito e calculo",
//     "conjuntos e subconjuntos simples conceito e exemplos",
//     "unidades de medida comprimento massa tempo conversoes basicas",
//     "media aritmetica simples calculo",
//     "mediana e moda identificacao em conjuntos pequenos",
//     "probabilidade basica com dados ou moedas resultado numerico",
//     "nocoes de estatistica no cotidiano interpretacao textual",
//     "angulos retos agudos e obtusos identificacao",
//     "operacoes com angulos adicao e subtracao simples",
//     "classificacao de triangulos por lados e angulos textual",
//     "soma dos angulos internos de triangulos conceito",
//     "introducao a algebra o que e uma variavel"
// };

const char* subthemes_videogames[] = {
    "história dos videogames",
    "consoles de videogame clássicos",
    "consoles de videogame atuais",
    "gêneros de jogos mais populares",
    "jogos de aventura famosos",
    "jogos de ação populares",
    "jogos de estratégia marcantes",
    "jogos de RPG clássicos",
    "jogos de simulação conhecidos",
    "jogos de esporte populares",
    "jogos de corrida famosos",
    "jogos de luta icônicos",
    "jogos de plataforma clássicos",
    "jogos indie de sucesso",
    "desenvolvedoras de jogos famosas",
    "personagens de videogames clássicos",
    "personagens de videogames atuais",
    "mundos de jogos memoráveis",
    "mecânicas de jogo inovadoras",
    "a música nos videogames trilhas sonoras",
    "a influência dos videogames na cultura",
    "eSports e jogos competitivos",
    "história dos jogos online",
    "plataformas de streaming de jogos",
    "termos comuns no mundo dos games",
    "curiosidades sobre videogames",
    "a evolução dos gráficos nos games",
    "realidade virtual nos videogames",
    "realidade aumentada nos videogames",
    "jogos mobile de sucesso"
};

const int subthemes_videogames_count = sizeof(subthemes_videogames) / sizeof(subthemes_videogames[0]);

const char* subthemes_musics[] = {
    "notas musicais básicas",
    "escala musical natural",
    "instrumentos de corda",
    "instrumentos de sopro",
    "instrumentos de percussão",
    "instrumentos eletrônicos",
    "vozes masculinas e femininas",
    "principais estilos musicais",
    "ritmos populares do Brasil",
    "ritmos populares do mundo",
    "samba e sua origem",
    "forró e suas variações",
    "bossa nova e seus representantes",
    "rock nacional anos 80",
    "rock internacional clássico",
    "pop internacional atual",
    "artistas da MPB",
    "bandas de rock famosas",
    "cantores pop dos anos 2000",
    "história do funk brasileiro",
    "características do sertanejo",
    "música clássica para iniciantes",
    "compositores clássicos famosos",
    "elementos da música ritmo melodia harmonia",
    "notação musical básica",
    "figuras rítmicas simples seminima mínima colcheia",
    "tempo e compasso musical",
    "leitura de partitura básica",
    "diferença entre ritmo e melodia",
    "o que é um refrão"
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
    "capitais dos países",
    "bandeiras nacionais",
    "idiomas oficiais ao redor do mundo",
    "principais rios do mundo",
    "maiores desertos da Terra",
    "cordilheiras e montanhas famosas",
    "mares e oceanos",
    "continentes e suas características",
    "geografia do Brasil",
    "estados e capitais brasileiras",
    "biomas brasileiros",
    "animais ameaçados de extinção",
    "invenções que mudaram o mundo",
    "descobrimentos históricos",
    "personagens históricos importantes",
    "guerras mundiais",
    "mitologias gregas e romanas",
    "religião e símbolos religiosos",
    "festas e tradições culturais",
    "alimentos típicos de diversos países",
    "costumes de diferentes culturas",
    "curiosidades sobre o corpo humano",
    "órgãos e funções do corpo humano",
    "sistemas do corpo humano",
    "descobertas científicas famosas",
    "cientistas renomados",
    "teorias da física clássica",
    "química do dia a dia",
    "símbolos da tabela periódica",
    "energia renovável e sustentável"
};


const int subthemes_general_knowledge_count = sizeof(subthemes_general_knowledge) / sizeof(subthemes_general_knowledge[0]);

// Subtópicos para "Algoritmos e Estruturas de Dados" (já existente)
const char* subthemes_aed[] = {
    "o que é um algoritmo",
    "passos para criar um algoritmo",
    "algoritmos sequenciais",
    "algoritmos com decisões (if/else)",
    "uso de repetições (loops)",
    "diferença entre for e while",
    "variáveis em algoritmos",
    "entrada e saída de dados",
    "conceito de função em programação",
    "funções simples e chamadas em programação"
};
const int subthemes_aed_count = sizeof(subthemes_aed) / sizeof(subthemes_aed[0]);

// Subtópicos para "INFRA_SO" (Infraestrutura de Software/Sistemas Operacionais)
const char* subthemes_infra_so[] = {
    "o que é um sistema operacional",
    "gerenciamento de processos",
    "gerenciamento de memória",
    "sistemas de arquivos",
    "virtualização e containers",
    "redes e protocolos básicos",
    "segurança em sistemas operacionais",
    "shell scripting",
    "ferramentas de monitoramento"
};
const int subthemes_infra_so_count = sizeof(subthemes_infra_so) / sizeof(subthemes_infra_so[0]);

// Subtópicos para "POO" (Programação Orientada a Objetos)
const char* subthemes_poo[] = {
    "conceitos de classes e objetos",
    "encapsulamento",
    "herança",
    "polimorfismo",
    "interfaces e classes abstratas",
    "métodos construtores e destrutores",
    "sobrecarga de métodos",
    "princípios SOLID",
    "design patterns básicos"
};
const int subthemes_poo_count = sizeof(subthemes_poo) / sizeof(subthemes_poo[0]);

// Subtópicos para "Harry Potter" (como exemplo de tema não técnico)
const char* subthemes_harry_potter[] = {
    "Hogwarts: fundação e história do castelo",
    "Grifinória: valores e membros notáveis (Harry, Hermione, Rony)",
    "Sonserina: características e membros famosos (Draco, Snape, Voldemort)",
    "Corvinal: símbolos e alunos brilhantes (Luna Lovegood)",
    "Lufa-Lufa: lealdade e figuras importantes (Cedrico Diggory)",
    "O Chapéu Seletor: como funciona e suas decisões polêmicas",
    "O Salão Comunal da Grifinória: localização e segredos",
    "A Câmara Secreta: lenda e revelação no segundo livro",
    "Harry Potter: cicatriz em forma de raio e conexão com Voldemort",
    "Hermione Granger: habilidades mágicas e origem trouxa",
    "Rony Weasley: família Weasley e seu papel no trio",
    "Alvo Dumbledore: varinha das varinhas e plano contra Voldemort",
    "Severo Snape: lealdade dupla e memórias no Pensador",
    "Lord Voldemort: horcruxes e busca pela imortalidade",
    "Draco Malfoy: redenção e conflitos morais",
    "Minerva McGonagall: habilidades como animago e liderança",
    "Rúbeo Hagrid: guarda-caça e criaturas mágicas",
    "Bellatrix Lestrange: lealdade a Voldemort e crueldade",
    "Dolores Umbridge: políticas opressoras em Hogwarts",
    "Comensais da Morte: seguidores de Voldemort e suas ações",
    "Pedro Pettigrew: traição aos Potter e forma de rato",
    "Basilisco: monstro da Câmara Secreta e como foi derrotado",
    "Dementadores: guardas de Azkaban e efeitos nas pessoas",
    "Expecto Patronum: como conjurar e significado",
    "Avada Kedavra: maldição da morte e consequências",
    "Wingardium Leviosa: primeiro feitiço aprendido por Hermione",
    "Feitiço de Desarmamento (Expelliarmus): marca registrada de Harry",
    "Poções Polissuco: usos e casos famosos (Barty Crouch Jr.)",
    "Mapa do Maroto: criação e como revela pessoas",
    "Varinhas: materiais e núcleos (Fênix, Dragão, Unicórnio)",
    "Pedra Filosofal: criação e propósito de Nicolas Flamel",
    "Relíquias da Morte: três objetos e lenda dos irmãos Peverell",
    "Horcruxes: o que são e como foram destruídos",
    "A Taca de Fogo: torneio tribruxo e seu papel no quarto livro",
    "O Nevoeiro: jornal sensacionalista e suas mentiras",
    "Torneio Tribruxo: tarefas e manipulação por Barty Crouch Jr.",
    "Batalha de Hogwarts: estratégias e mortes importantes",
    "Fuga de Gringotes: roubo da taça de Helga Hufflepuff",
    "A Ordem da Fênix: membros e missões contra Voldemort",
    "A Profecia de Harry e Voldemort: significado e impacto",
    "Hipogrifo: caso de Bicuço e julgamento",
    "Testrálio: como são vistos e ligação com a morte",
    "Elfos Domésticos: escravidão e caso do Dobby",
    "Dragões: tipos e desafio no Torneio Tribruxo",
    "Centauros: cultura e papel na Floresta Proibida",
    "Amizade: relação entre Harry, Rony e Hermione",
    "Coragem: atitudes de Neville Longbottom",
    "Preconceito: tratamento a nascidos-trouxas e meio-gigantes",
    "Sacrifício: proteção de Lily Potter a Harry",
    "Dualidade: luz e sombra em personagens como Snape e Dumbledore"
};

const int subthemes_harry_potter_count = sizeof(subthemes_harry_potter) / sizeof(subthemes_harry_potter[0]);

const char* get_random_subtheme(Theme theme) {
    printf("get_random_subtheme chamado com tema: %d\n", theme);
    int index = 0;
    switch (theme) {
        case THEME_VIDEO_GAMES:
            index = rand() % subthemes_videogames_count;
            printf("Tema MATEMATICA, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_videogames[index]);
            return subthemes_videogames[index];
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
        case THEME_SO:
            index = rand() % subthemes_infra_so_count;
            printf("Tema SO, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_infra_so[index]);
            return subthemes_infra_so[index];
        case THEME_POO:
            index = rand() % subthemes_poo_count;
            printf("Tema POO, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_poo[index]);
            return subthemes_poo[index];
        case THEME_HARRY_POTTER:
            index = rand() % subthemes_harry_potter_count;
            printf("Tema HARRY_POTTER, índice selecionado: %d, subtema: \"%s\"\n", index, subthemes_harry_potter[index]);
            return subthemes_harry_potter[index];
        default:
            printf("AVISO: Tema inválido (%d) passado para get_random_subtheme. Retornando NULL.\n", theme);
            return NULL;
    }
}
