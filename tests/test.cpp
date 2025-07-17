#define CATCH_CONFIG_MAIN  
#include <catch2/catch_test_macros.hpp>  
#include "../include/evaluar.h"  

//Pruebas Exitosas
TEST_CASE("Operaciones simples exitosas", "[operation-success]") {
    REQUIRE(realizarOperacion(3, 2, '+') == 6);
    REQUIRE(realizarOperacion(3, 2, '-') == 1);
    REQUIRE(realizarOperacion(3, 2, '*') == 6);
    REQUIRE(realizarOperacion(6, 2, '/') == 3);
    REQUIRE(realizarOperacion(5, 2, '%') == 1);   
    REQUIRE(realizarOperacion(2, 3, '^') == 8);   
    REQUIRE(realizarOperacion(10, 3, '-') == 7); 
    REQUIRE(realizarOperacion(4, 2, '*') == 8);   
    REQUIRE(realizarOperacion(5, 5, '/') == 1);   
    REQUIRE(realizarOperacion(9, 4, '%') == 1);   
}

//Pruebas Fallidas

TEST_CASE("Error en la conversion de infija a postfija1", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("3+5");
    REQUIRE(result);
    REQUIRE(result.value() == "3 + 5");
    
}
TEST_CASE("Error en la conversion de infija a postfija2", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("3-5");
    REQUIRE(result);
    REQUIRE(result.value() == "3 - 5");
    
}

TEST_CASE("Error en la conversion de infija a postfija3", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("3*5");
    REQUIRE(result);
    REQUIRE(result.value() == "3 * 5");
    
}

TEST_CASE("Error en la conversion de infija a postfija4", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("3/5");
    REQUIRE(result);
    REQUIRE(result.value() == "3 / 5");
    
}

TEST_CASE("Error en la conversion de infija a postfija5", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("3%5");
    REQUIRE(result);
    REQUIRE(result.value() == "3 % 5");
    
}

TEST_CASE("Error en la conversion de infija a postfija6", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4+5-1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 - + 5 1");
    
}

TEST_CASE("Error en la conversion de infija a postfija7", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4+5+1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 + + 5 1");
    
}
TEST_CASE("Error en la conversion de infija a postfija8", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4-5-1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 - - 5 1");
    
}
TEST_CASE("Error en la conversion de infija a postfija9", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4*5/1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 * / 5 1");
    
}
TEST_CASE("Error en la conversion de infija a postfija10", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4*5*1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 * * 5 1");
    
}
TEST_CASE("Error en la conversion de infija a postfija11", "[convert-fail]") {
    auto result = convertirInfijaAPostfija("4/5+1");
    REQUIRE(result);
    REQUIRE(result.value() == "4 +/ + 5 1");
    
}

