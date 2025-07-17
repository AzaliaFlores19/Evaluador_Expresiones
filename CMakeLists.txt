cmake_minimum_required(VERSION 3.10)

# Definir el nombre del proyecto y el lenguaje
project(Evaluador_Expresiones VERSION 1.0 LANGUAGES CXX)

# Establecer la versión mínima de CMake
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
src/evaluar.cpp 
)


# Incluir todos los archivos fuente en el proyecto
add_executable(Evaluador_Expresiones ${SOURCES} src/mainF.cpp)  # Asegúrate de listar todos tus archivos fuente aquí
target_link_libraries(Evaluador_Expresiones PRIVATE stdc++exp)

target_precompile_headers(Evaluador_Expresiones PRIVATE <iostream> <stack> <sstream> <unordered_map> <fstream> <cmath> <cctype> <string> <vector> <expected>)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Setup Catch2 for testing
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)
message(STATUS "Catch2 version: ${Catch2_VERSION}")
add_executable(tests ${SOURCES} tests/test.cpp )  # Asegúrate de listar todos tus archivos fuente aquí
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain stdc++exp)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Habilitar pruebas en CMake
# enable_testing()
# add_test(NAME Evaluador_Tests COMMAND tests)