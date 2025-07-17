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

using namespace std;

// ====================== VALORES VÁLIDOS ======================
unordered_map<string, double> constantes;
unordered_map<string, double> variables;

void cargarConstantes(const string &archivoN) {
    ifstream file(archivoN);
    string linea;
    while (getline(file, linea)) {
        size_t pos = linea.find('=');
        if (pos != string::npos) {
            string nombre = linea.substr(0, pos);
            double valor = stod(linea.substr(pos + 1));
            constantes[nombre] = valor;
        }
    }
}

std::expected<double, string> obtenerValorVariable(const string &var) {
    if (variables.count(var)) return variables[var];
    cout << "Ingrese el valor de " << var << ": ";
    double valor;
    cin >> valor;
    variables[var] = valor;
    return valor;
}

// ====================== VALIDACIONES EXPRESIÓN INFIXA ======================
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
    bool anteriorFueNumero = false;
   
    for (size_t i = 0; i < expresion.size(); ++i) {
        char c = expresion[i];

        if (isdigit(c)) {
            anteriorFueNumero = true;
        } else if (isalpha(c)) {
            if (anteriorFueNumero) {
                error = "Error: Un número no puede ser seguido inmediatamente por una letra.";
                return true;  // Número seguido de una letra => ERROR
            }
        } else if (c == '.') {
            // Verificar que el punto no esté en una variable
            if (i > 0 && isalpha(expresion[i - 1])) {
                error = "Error: Un punto dentro de un identificador no es válido.";
                return true; // Error: punto dentro de un identificador
            }

        } else {
            if(!esCaracterValido(c)) {
                error = "Error: Caracter inválido '" + string(1, c) + "' en la expresión.";
                return true;
            }

            anteriorFueNumero = false;
        }
    }
    return false;
}

// ====================== CONVERSIÓN INFIX A POSTFIX ======================
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
                return std::unexpected("Error: Un número no puede contener múltiples puntos decimales.");
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
                    return std::unexpected("Error: Paréntesis de cierre sin apertura correspondiente.");
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
        return std::unexpected("Error: La expresión infija no puede terminar con un operador.");
    }
    if (parentesisAbiertos != 0) {
        return std::unexpected("Error: Paréntesis incompletos en la expresión.");
    }
    while (!operadores.empty()) {
        postfija += ' '; postfija += operadores.top();
        operadores.pop();
    }
    return postfija;
}

// ====================== OPERACIONES MATEMÁTICAS ======================
double realizarOperacion(double a, double b, char op) {
    if (op == '+') return a + b;
    else if (op == '-') return a - b;
    else if (op == '*') return a * b;
    else if (op == '/') {
        if (b == 0) {
            cerr << "Error: División por cero." << endl;
            return NAN;
        }
        return a / b;
    }else if (op == '%') {
        if (b == 0) {
            cerr << "Error: Módulo por cero." << endl;
            return NAN;
        }
        return fmod(a, b);
    } 
    else if (op == '^') return pow(a, b);
    return NAN;
}

// ====================== PROCESAMIENTO DE TOKENS ======================
vector<string> obtenerTokens(const string &postfija) {
    vector<string> tokens;
    ispanstream ss(postfija);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// ====================== EVALUACIÓN DE POSTFIJA ======================
double evaluarExpresionPostfija(const vector<string> &tokens) {
    stack<double> valores;
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
                cerr << "Error: Expresión postfija inválida." << endl;
                return NAN;
            }
            double b = valores.top(); valores.pop();
            double a = valores.top(); valores.pop();
            double resultado = realizarOperacion(a, b, token[0]);
            valores.push(resultado);
        }
    }
    return valores.empty() ? NAN : valores.top();
}


// ====================== PROGRAMA PRINCIPAL ======================
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
