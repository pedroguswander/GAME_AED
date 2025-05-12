cmake_minimum_required(VERSION 3.10)
project(GAME_AED C)

set(CMAKE_C_STANDARD 99)

# Incluir headers
include_directories(include)

# Raylib
find_package(raylib REQUIRED)

# curl via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})

# cjson manual (pkg-config não encontra .pc)
include_directories(/usr/include)
link_directories(/usr/lib)

# Arquivos fonte
file(GLOB SOURCES "src/*.c")

# Executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Linkagem
target_link_libraries(${PROJECT_NAME}
        raylib
        ${CURL_LIBRARIES}
        cjson
        m
        pthread
        dl
)
