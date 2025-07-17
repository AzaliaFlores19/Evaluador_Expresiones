#include "../include/evaluar.h"
#include <iostream>
#include <string>
#include <cmath>
#include <print>

using namespace std;
int main() {
    cargarConstantes("constants.txt");

    string expresion;
    print("Ingrese la expresión infija: ");
    getline(cin, expresion);

    string error;
    if (esExpresionInvalida(expresion, error)) {
        cerr << error << endl;
        return 1;
    }

    auto resultadoPostfijo = convertirInfijaAPostfija(expresion);
    if (!resultadoPostfijo) {
        cerr << resultadoPostfijo.error() << endl;
        return 1;
    }
    string postfija = resultadoPostfijo.value();
    print("Expresión Postfija: {}\n", postfija);

    vector<string> tokens = obtenerTokens(postfija);
    float64_t resultado = evaluarExpresionPostfija(tokens);
    if (!isnan(resultado)) {
        print("Resultado: {}\n", resultado);
    }
    return 0;
}
