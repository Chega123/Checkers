#ifndef BOARD_H
#define BOARD_H

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "Ficha.h"
#include "Checker.h"

class Board {
private:
    int SIZE;
    int profundidad;
    Checker juego;
    double lightred, lightgreen, lightblue;
    double darkred, darkgreen, darkblue;
    double p1red, p1green, p1blue;
    double p2red, p2green, p2blue;
    std::vector<std::vector<Ficha*>> box;

    void cir(int xc, int yc, int r);
    void cir1(int xc, int yc, int r);
    void Reshape(int width, int height);
    void begin();
    void Draw();
    void computadoraJuega();

public:
    Board(int size = 8);
    ~Board();
    bool turnojugador = true;
    void run(int argc, char** argv);
    void setBoardData(const std::vector<std::vector<Ficha*>>& inputData);
    std::pair<int, int> selectedPiece = std::make_pair(-1, -1);
    static void StaticReshape(int width, int height);
    static void StaticDraw();
    static void MouseClick(int button, int state, int x, int y);
    void handleMouseClick(int x, int y);
};

#endif // BOARD_H
