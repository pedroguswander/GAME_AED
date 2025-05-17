cmake_minimum_required(VERSION 3.10)
project(GAME_AED C)

set(CMAKE_C_STANDARD 99)

# Incluir headers globalmente (pode ser mantido)
include_directories(include)

# Raylib
find_package(raylib REQUIRED)

# curl via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})

# Arquivos fonte
file(GLOB SOURCES "src/*.c")
list(APPEND SOURCES lib/cjson/cJSON.c)

# Executável (DEFINIÇÃO DO ALVO)
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir diretório especificamente para o target (AGORA APÓS A DEFINIÇÃO)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/cjson)

# Linkagem
target_link_libraries(${PROJECT_NAME}
        raylib
        ${CURL_LIBRARIES}
        m
        pthread
        dl
)