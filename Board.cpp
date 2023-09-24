#include "Board.h"
#include "Checker.h"

Board* currentInstance = nullptr;

Board::Board(int size) : SIZE(size),
    lightred(0.9608), lightgreen(0.8706), lightblue(0.702),
    darkred(0.4824), darkgreen(0.2078), darkblue(0.1373),
    p1red(0.098), p1green(0.4588), p1blue(0.8235),
    p2red(1), p2green(0.4980), p2blue(0.3137),
    box(size, std::vector<Ficha*>(size, 0)) {
    currentInstance = this;
    profundidad = 3;
    juego.llenar_tablero();
    setBoardData(juego.getTablero());
}

Board::~Board() {}


void drawCircle(int xc, int yc, int r) {
    double alfa;
    glBegin(GL_POLYGON);
    for (alfa = 0; alfa < 360; alfa += 0.5) {
        int x = xc + r * cos(alfa * M_PI / 180.0);
        int y = yc + r * sin(alfa * M_PI / 180.0);
        glVertex2i(x, y);
    }
    glEnd();
}

void Board::Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SIZE * 50, SIZE * 50, 0);
    glMatrixMode(GL_MODELVIEW);
}


void Board::begin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((i + j) % 2 == 1) {
                glColor3f(lightred, lightgreen, lightblue);
            } else {
                glColor3f(darkred, darkgreen, darkblue);
            }
            glRectf(j * 50, i * 50, j * 50 + 50, i * 50 + 50);

            Ficha* ficha = box[i][j];
            if (ficha != nullptr) {
                if (i == selectedPiece.first && j == selectedPiece.second) {
                    // If this is the sele int main() {cted piece, use a different color, for example a yellow highlight
                    glColor3f(1.0, 1.0, 0.0);
                    drawCircle(j * 50 + 25, i * 50 + 25, 20);
                } else if (ficha->equipo == 0 && ficha->tipo == 0) {
                    glColor3f(p1red, p1green, p1blue);
                    drawCircle(j * 50 + 25, i * 50 + 25, 20);
                } else if (ficha->equipo == 1 && ficha->tipo == 0) {
                    glColor3f(p2red, p2green, p2blue);
                    drawCircle(j * 50 + 25, i * 50 + 25, 20);
                }
            }
        }
    }
}

void Board::Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    begin();
    glFlush();
}

void Board::StaticReshape(int width, int height) {
    if (currentInstance) {
        currentInstance->Reshape(width, height);
    }
}

void Board::StaticDraw() {
    if (currentInstance) {
        currentInstance->Draw();
    }
}

void Board::run(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SIZE * 50, SIZE * 50);
    glutCreateWindow("Board");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutDisplayFunc(StaticDraw);
    glutReshapeFunc(StaticReshape);
    glutMouseFunc(MouseClick);
    glutMainLoop();
}

void Board::MouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        currentInstance->handleMouseClick(x, y);
    }
}

void Board::computadoraJuega() {
    if (!turnojugador) {
        int mejorValor = -999999; // pa no poner infinito negativo 
        vector<int>posiciones;
        Checker Copia = juego;
        int fila1 = 0, col1 = 0, fila2 = 0, col2 = 0;
        std::vector<Checker> movimientos = juego.generarMovimientos(Copia, turnojugador, posiciones);
        int i = 0;
        for (Checker movimiento : movimientos) {
            int valor = juego.minimax(movimiento, profundidad, !turnojugador, -999999, 999999); // genera primero una serie de movimientos los cuales desde cada uno analizara cual saldra mas rentable
            //de ahi bueno el mas rentable lo va guardando aqui abajo 
            if (valor >= mejorValor) {
                mejorValor = valor;
                fila1 = posiciones[i], col1 = posiciones[i + 1], fila2 = posiciones[i + 2], col2 = posiciones[i + 3];
            }
            i += 4;
        }
        juego.mover_Ficha(fila1, col1, fila2, col2, turnojugador, 1); // cambia el tablero por el del mejor movimiento
    }
    if (juego.piezas[0] == 0) {
        cout << endl << "ganaste :D\n";
    }
    else if (juego.piezas[1] == 0) {
        cout << endl << "perdiste D:\n";
    }
}

void Board::handleMouseClick(int x, int y) {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    int cellWidth = windowWidth / currentInstance->SIZE;
    int cellHeight = windowHeight / currentInstance->SIZE;

    int i = y / cellHeight;  // fila
    int j = x / cellWidth;   // columna

    std::cout << i << "," << j << std::endl;
    if (turnojugador) {
        if (selectedPiece == std::pair<int, int>(-1, -1)) {
            selectedPiece = std::make_pair(i, j);
        } 
        else if (juego.getTablero()[selectedPiece.first][selectedPiece.second]->equipo == turnojugador) {
            juego.mover_Ficha(selectedPiece.first, selectedPiece.second, i , j, turnojugador, 0);
            selectedPiece = std::pair<int, int>(-1, -1);
            turnojugador = !turnojugador; //turno de la maquina
        } else {
            std::cout << "No puedes mover esa ficha" << std::endl;
            selectedPiece = std::pair<int, int>(-1, -1);
        }
        setBoardData(juego.getTablero());
        glutPostRedisplay();

    }

    if(!turnojugador) {
        computadoraJuega();
        turnojugador = 1;
        setBoardData(juego.getTablero());
        glutPostRedisplay();
    }
}

void Board::setBoardData(const std::vector<std::vector<Ficha*>>& inputData) {
    box = inputData;
}
