#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <cctype>
#include <print>
#include <expected>
#include <spanstream>
#include <stdfloat>
#include <vector>
#include <string>

#include "../include/evaluar.h" 
using namespace std;

unordered_map<string, float64_t> constantes;
unordered_map<string, float64_t> variables;

void cargarConstantes(const string &archivoN) {
    ifstream file(archivoN);
    string linea;
    while (getline(file, linea)) {
        size_t pos = linea.find('=');
        if (pos != string::npos) {
            string nombre = linea.substr(0, pos);
            float64_t valor = stod(linea.substr(pos + 1));
            constantes[nombre] = valor;
        }
    }
}

std::expected<float64_t, string> obtenerValorVariable(const string &var) {
    if (variables.count(var)) return variables[var];
    cout << "Ingrese el valor de " << var << ": ";
    float64_t valor;
    cin >> valor;
    variables[var] = valor;
    return valor;
}


bool esCaracterValido(char c) {
    return isdigit(c) || isalpha(c) || c == '.' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(' || c == ')';
}

int obtenerPrecedencia(char operador) {
    if (operador == '^') return 3;
    if (operador == '*' || operador == '/' || operador == '%') return 2;
    if (operador == '+' || operador == '-') return 1;
    return 0;
}

bool esExpresionInvalida(const string &expresion, string &error) {
    if (expresion.empty()) {
        error = "Error: La expresion no puede estar vacia.";
        return true;
    }
    
    bool anteriorFueNumero = false;
   
    for (size_t i = 0; i < expresion.size(); ++i) {
        char c = expresion[i];

        if (isdigit(c)) {
            anteriorFueNumero = true;
        } else if (isalpha(c)) {
            if (anteriorFueNumero) {
                error = "Error: Un numero no puede ser seguido inmediatamente por una letra.";
                return true;  
            }
        } else if (c == '.') {
            if (i > 0 && isalpha(expresion[i - 1])) {
                error = "Error: Un punto dentro de un identificador no es valido.";
                return true; 
            }

        }   else if (c == '(') {
            if (i > 0 && (isdigit(expresion[i - 1]) || isalpha(expresion[i - 1]) || expresion[i - 1] == ')')) {
                error = "Error: Se requiere un operador antes de '('.";
                return true;
            }
        } else if (c == ')') {
            if (i + 1 < expresion.size() && (isdigit(expresion[i + 1]) || isalpha(expresion[i + 1]) || expresion[i + 1] == '(')) {
                error = "Error: Se requiere un operador después de ')'.";
                return true;
            }
        } else {
            if(!esCaracterValido(c)) {
                error = "Error: Caracter invalido '" + string(1, c) + "' en la expresion.";
                return true;
            }

            anteriorFueNumero = false;
        }
    }
    return false;
}

std::expected<string, string> convertirInfijaAPostfija(const string &infija) {
    stack<char> operadores;
    string postfija;
    bool ultimoFueOperador = true;
    int parentesisAbiertos = 0;
    int contadorPuntos = 0;

    for (size_t i = 0; i < infija.length(); i++) {
        char c = infija[i];
       
        if (isdigit(c) || isalpha(c)) {
            postfija += c;
            ultimoFueOperador = false;
        } else if (c == '.') {
            contadorPuntos++;
            if (contadorPuntos > 1) {
                return std::unexpected("Error: Un numero no puede contener multiples puntos decimales.");
            }
            postfija += c;
            ultimoFueOperador = false;
        } else {
            contadorPuntos = 0;
            postfija += ' ';
            if (c == '(') {
                operadores.push(c);
                parentesisAbiertos++;
                ultimoFueOperador = true;
            } else if (c == ')') {
                parentesisAbiertos--;
                if (parentesisAbiertos < 0) {
                    return std::unexpected("Error: Parentesis de cierre sin apertura correspondiente.");
                }
                while (!operadores.empty() && operadores.top() != '(') {
                    postfija += operadores.top(); postfija += ' ';
                    operadores.pop();
                }
                operadores.pop();
                ultimoFueOperador = false;
            } else {
                if (ultimoFueOperador) {
                    return std::unexpected("Error: Dos operadores no pueden ir seguidos.");
                }
                while (!operadores.empty() && obtenerPrecedencia(operadores.top()) >= obtenerPrecedencia(c)) {
                    postfija += operadores.top(); postfija += ' ';
                    operadores.pop();
                }
                operadores.push(c);
                ultimoFueOperador = true;
            }
        }
    }
    if (ultimoFueOperador) {
        return std::unexpected("Error: La expresion infija no puede terminar con un operador.");
    }
    if (parentesisAbiertos != 0) {
        return std::unexpected("Error: Parentesis incompletos en la expresion.");
    }
    while (!operadores.empty()) {
        postfija += ' '; postfija += operadores.top();
        operadores.pop();
    }
    return postfija;
}

float64_t realizarOperacion(float64_t a, float64_t b, char op) {
    if (op == '+') return a + b;
    else if (op == '-') return a - b;
    else if (op == '*') return a * b;
    else if (op == '/') {
        if (b == 0) {
            cerr << "Error: Division por cero." << endl;
            return NAN;
        }
        return a / b;
    }else if (op == '%') {
        if (b == 0) {
            cerr << "Error: Modulo por cero." << endl;
            return NAN;
        }
        return fmod(a, b);
    } 
    else if (op == '^') return pow(a, b);
    return NAN;
}

vector<string> obtenerTokens(const string &postfija) {
    vector<string> tokens;
    ispanstream ss(postfija);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

float64_t evaluarExpresionPostfija(const vector<string> &tokens) {
    stack<float64_t> valores;
    for (const string &token : tokens) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            valores.push(stod(token));
        } else if (constantes.count(token)) {
            valores.push(constantes[token]);
        } else if (isalpha(token[0])) {
            auto valorVar = obtenerValorVariable(token);
            if (!valorVar) {
                cerr << "Error: Variable no reconocida." << endl;
                return NAN;
            }
            valores.push(valorVar.value());
        } else {
            if (valores.size() < 2) {
                cerr << "Error: Expresion postfija inválida." << endl;
                return NAN;
            }
            float64_t b = valores.top(); valores.pop();
            float64_t a = valores.top(); valores.pop();
            float64_t resultado = realizarOperacion(a, b, token[0]);
            valores.push(resultado);
        }
    }
    return valores.empty() ? NAN : valores.top();
}

