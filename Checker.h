#ifndef CHECKER_H
#define CHECKER_H

#include "Ficha.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class Checker {
private:
    vector<vector<Ficha*>> tablero;

public:
    Checker();
    ~Checker();
    int piezas[2] = { 12,12 };
    Checker& operator=(const Checker& other);
    Checker copiarTablero();
    void swap_Ficha(int fila1, int col1, int fila2, int col2);
    void comer_Ficha(int fila1, int col1, int fila2, int col2, bool turno);
    void mover_Ficha(int fila1, int col1, int fila2, int col2, bool& turno, bool coronacion);
    void llenar_tablero();
    void imprimir_tablero();
    vector<vector<Ficha *>> getTablero();
    int evaluarTablero(Checker& tablero);
    vector<Checker> generarMovimientos(Checker& tablita, bool esMaximizador, vector<int>& posiciones);
    int minimax(Checker tablero, int profundidad, bool Max, int alfa, int beta);
};

#endif // CHECKER_H
