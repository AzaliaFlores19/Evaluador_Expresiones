#ifndef EVALUAR_H
#define EVALUAR_H

#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <cctype>
#include <string>
#include <vector>
#include <expected>
#include <spanstream>
#include <stdfloat>

using namespace std;

// ====================== VALORES VÁLIDOS ======================
extern unordered_map<string, float64_t> constantes;
extern unordered_map<string, float64_t> variables;

void cargarConstantes(const string &archivoN);
std::expected<float64_t, string> obtenerValorVariable(const string &var);

// ====================== VALIDACIONES EXPRESIÓN INFIXA ======================
bool esCaracterValido(char c);
int obtenerPrecedencia(char operador);
bool esExpresionInvalida(const string &expresion, string &error);

// ====================== CONVERSIÓN INFIX A POSTFIX ======================
std::expected<string, string> convertirInfijaAPostfija(const string &infija);

// ====================== OPERACIONES MATEMÁTICAS ======================
float64_t realizarOperacion(float64_t a, float64_t b, char op);

// ====================== PROCESAMIENTO DE TOKENS ======================
vector<string> obtenerTokens(const string &postfija);

// ====================== EVALUACIÓN DE POSTFIJA ======================
float64_t evaluarExpresionPostfija(const vector<string> &tokens);

#endif 
