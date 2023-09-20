/* #include <iostream>
#include <vector>
#include <cmath>
using namespace std;


class Ficha {
public:
    bool equipo;  // '1' para nosotros, '0' para la pc
    bool tipo;   // '1' para dama, '0' para peón

    Ficha(bool equipo_, bool tipo_) {
        equipo = equipo_;
        tipo = tipo_;
    }
};


class Tablero {
public:
    vector<vector<Ficha*>> tablero;
    int piezas[2] = { 12,12 };
    Tablero() {
        tablero = vector<vector<Ficha*>>(8, vector<Ficha*>(8, nullptr));
        //creación de vector de 8 de largo donde en cada espacio hay un vector de 8 fichas en nullptr
    }

    void llenar_tablero() {
        for (int i = 0; i < 8; i++) {

            if (0 <= i && i <= 2) {
                for (int j = 0; j < 8; j++) {
                    if ((j + i) % 2 == 1) tablero[i][j] = new Ficha(0, 0);
                }
            }
            else if (5 <= i && i <= 7) {
                for (int j = 0; j < 8; j++) {
                    if ((j + i) % 2 == 1) tablero[i][j] = new Ficha(1, 0);
                }
            }
        }
    };

    void imprimir_tablero() {
        int i = 0;
        cout << "  0   1   2   3   4   5   6   7" << endl;
        cout << " +---+---+---+---+---+---+---+---+" << endl;
        for (auto it = tablero.begin(); it != tablero.end(); it++, i++) {

            cout << i << "|"; // borde izq
            for (auto jt = it->begin(); jt != it->end(); jt++) {

                cout << " ";
                if (*jt != nullptr) {
                    //imprimimos fichas según equipo :D
                    if ((*jt)->equipo == 0 && (*jt)->tipo == 0) { cout << "Ø"; }
                    else if ((*jt)->equipo == 1 && (*jt)->tipo == 0) { cout << "O"; }
                    else if ((*jt)->equipo == 0 && (*jt)->tipo == 1) { cout << "@"; }
                    else if ((*jt)->equipo == 1 && (*jt)->tipo == 1) { cout << "©"; }
                }

                else {
                    cout << " "; // si celda esta vacía espacio
                }

                cout << " |"; // borde derecho
            }

            cout << endl;
            cout << " +---+---+---+---+---+---+---+---+\n";
        }
    };

    void swap_Ficha(int fila1, int col1, int fila2, int col2) {
        Ficha* temp = tablero[fila1][col1];
        tablero[fila1][col1] = tablero[fila2][col2];
        tablero[fila2][col2] = temp;
    };

    //Estan bien los mensajes pero a la hora de que la computadora calcule la jugada, al haber tantos movimientos salen muchos mensajes


    void comer_Ficha(int fila1, int col1, int fila2, int col2, bool turno) {
        if (tablero[fila1][col1] == nullptr) {
            //cout << "No existe pieza ahi";
            turno = !turno;
            return;
        }

        if (tablero[fila2][col2] != nullptr) {
            //cout << "Ya existe una pieza en el destino" << endl;
            turno = !turno;
            return;
        }

        int Fila_resta = fila2 - fila1; //distancia que se mueve en y
        int Col_resta = col2 - col1; //distancia que se mueve en x. Valor absoluto ya que a veces puede quedar -2

        // verifica si la ficha a comer está en diagonal y a una distancia de 2
        if (abs(Fila_resta) == 2 && abs(Col_resta) == 2) {
            int filaComida = fila1 + Fila_resta / 2;
            int colComida = col1 + Col_resta / 2;

            if (tablero[filaComida][colComida] == nullptr || tablero[filaComida][colComida]->equipo == tablero[fila1][col1]->equipo) {//revisa si el espacio que va a ser no esta vació o si es del mismo equipo
                //cout << "No hay ficha enemiga para comer en esa dirección" << endl;
                turno = !turno;
                return;
            }
            piezas[tablero[filaComida][colComida]->equipo] -= 1; // Restar uno del contador del equipo correspondiente
            delete tablero[filaComida][colComida];
            tablero[filaComida][colComida] = nullptr;
            swap_Ficha(fila1, col1, fila2, col2);
            //cout << "Ficha comida!" << endl;
        }

        else if (tablero[fila1][col1]->tipo == 1 && abs(Fila_resta) == abs(Col_resta)) { // Movimiento de la Reina
            // Verifica si el camino está libre
            int pasoFila = (fila2 > fila1) ? 1 : -1;//ve a que lado ira si arriba (-1) abajo (1)
            int pasoCol = (col2 > col1) ? 1 : -1; //ve a que lado ira si izq (-1) der (1)

            for (int i = 1; i < max(abs(Fila_resta), abs(Col_resta)); i++) {//va hasta la posición indicada y si encuentra una ficha en medio se la come y se queda una posición adelante de la ficha
                if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol] != nullptr) {
                    if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol]->equipo != tablero[fila1][col1]->equipo) {
                        piezas[tablero[fila1 + i * pasoFila][col1 + i * pasoCol]->equipo] -= 1;
                        delete tablero[fila1 + i * pasoFila][col1 + i * pasoCol];
                        tablero[fila1 + i * pasoFila][col1 + i * pasoCol] = nullptr;
                        swap_Ficha(fila1, col1, fila2, col2);
                        // cout << "Ficha comida por la dama!" << endl;
                        return;
                    }
                    else {
                        turno = !turno;
                        // cout << "El camino no está libre" << endl; 
                        return;
                    }
                }
            }
        }
        else {
            turno = !turno;
            //cout << "No puedes mover tu ficha ahi" << endl; 
        }
    };


    void mover_Ficha(int fila1, int col1, int fila2, int col2, bool& turno, bool coronacion) {
        if (tablero[fila1][col1] == nullptr) {
            turno = !turno;
            //cout << "No existe pieza ahi"; 
            return;
        }

        if (tablero[fila2][col2] != nullptr) {
            turno = !turno;
            //cout << "Ya existe una pieza en el destino" << endl;
            return;
        }

        int Fila_resta = fila2 - fila1; //distancia que se mueve en y
        int Col_resta = abs(col2 - col1); //distancia que se mueve en x. Valor absoluto ya que a veces puede quedar -2

        // verifica movimiento para computadora y para nosotros ademas de revisar tmb a que lado van
        if ((tablero[fila1][col1]->equipo == 0 && Fila_resta == 1) || (tablero[fila1][col1]->equipo == 1 && Fila_resta == -1)) {
            if (Col_resta == 1) {//revisa que solo sean diagonales
                swap_Ficha(fila1, col1, fila2, col2);
                //revisa si llego al final y es hora de coronar
                if (((tablero[fila2][col2]->equipo == 0 && fila2 == 7) || (tablero[fila2][col2]->equipo == 1 && fila2 == 0)) && coronacion != 0) {
                    tablero[fila2][col2]->tipo = 1;
                    //cout << "Coronaste a la ficha :D" << endl; 
                }
                return;
            }
        }

        // Movimiento de la Reina
        if (tablero[fila1][col1]->tipo == 1 && (abs(Fila_resta) == Col_resta || Fila_resta == 0 || Col_resta == 0)) {
            // Verifica si el camino está libre
            int pasoFila = (Fila_resta != 0) ? (Fila_resta > 0) ? 1 : -1 : 0; // verifica si esta yendo de arriba ao bajo , ademas de que revisa que en primer lugar se este moviendo
            int pasoCol = (Col_resta != 0) ? (col2 > col1) ? 1 : -1 : 0; //verifica si esta yendo de izq o der , ademas de que revisa que en primer lugar se este moviendo

            for (int i = 1; i < max(abs(Fila_resta), abs(Col_resta)); i++) {
                if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol] != nullptr) {//recorre el camino que pusimos hasta que encuentra un espacio no vació, por lo que debe comer esa ficha o bueno al menos revisar si puede
                    if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol]->equipo != tablero[fila1][col1]->equipo) {
                        comer_Ficha(fila1, col1, fila1 + (i + 1) * pasoFila, col1 + (i + 1) * pasoCol, turno);
                        return;
                    }
                    else {
                        turno = !turno;
                        // cout << "El camino no está libre" << endl; 
                        return;
                    }
                }
            }
            swap_Ficha(fila1, col1, fila2, col2);
            return;
        }

        // Si ninguna de las condiciones anteriores se cumple, intenta comer una ficha
        if (abs(Fila_resta) >= 2 && Col_resta >= 2) {
            comer_Ficha(fila1, col1, fila2, col2, turno);//comer normal de un peon, verifica que no se salga del tablero :D
            if (((tablero[fila2][col2]->equipo == 0 && fila2 == 7) || (tablero[fila2][col2]->equipo == 1 && fila2 == 0)) && coronacion != 0) {//estaba bonito pero no entiendo xq esta condicion se la salta como si nada xd
                tablero[fila2][col2]->tipo = 1;
                // cout << "Coronaste a la ficha :D" << endl; 
            }
            return;
        }
        turno = !turno;
        //cout << "No puedes mover tu ficha ahi" << endl; 
        return;
    };
};


int evaluarTablero(Tablero& tablero) {//lo de la resta pa ver el puntaje
    return tablero.piezas[0] - tablero.piezas[1];
}



vector<Tablero> generarMovimientos(Tablero& tablita, bool esMaximizador, vector<int>& posiciones) { //duda sobre lo de copiar tabla con el &
    vector<Tablero> movimientos;

    // Recorre todas las fichas en el tablero
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Si la ficha pertenece al jugador actual
            if (tablita.tablero[i][j] != nullptr && tablita.tablero[i][j]->equipo == esMaximizador) {
                // Genera todos los posibles movimientos para esta ficha
                for (int di = -2; di <= 2; di += 2) { // Para comer   No uso la funcion mover_ficha porque por alguna razon no me funca aqui es raro 
                    for (int dj = -2; dj <= 2; dj += 2) { // Para comer
                        // Verifica que el movimiento es válido
                        if (i + di >= 0 && i + di < 8 && j + dj >= 0 && j + dj < 8 && tablita.tablero[i + di][j + dj] == nullptr) {
                            // Verifica si hay una ficha del equipo contrario entre el lugar donde estamos y al que queremos ir
                            if (tablita.tablero[i + di / 2][j + dj / 2] != nullptr && tablita.tablero[i + di / 2][j + dj / 2]->equipo != esMaximizador) {
                                // Crea una nueva "tablita" del tablero
                                Tablero nuevoTablero = tablita;//en si es donde se hara el movimiento y asi nada mas
                                // Mueve la ficha en el nuevoTablero y elimina la ficha del equipo contrario
                                nuevoTablero.tablero[i + di][j + dj] = nuevoTablero.tablero[i][j];
                                nuevoTablero.tablero[i][j] = nullptr;
                                nuevoTablero.tablero[i + di / 2][j + dj / 2] = nullptr;
                                // Añade la nueva "tablita" a la lista de movimientos
                                movimientos.push_back(nuevoTablero);
                                posiciones.push_back(i), posiciones.push_back(j), posiciones.push_back(i + di), posiciones.push_back(j + dj);
                            }
                        }
                    }
                }
                for (int di = -1; di <= 1; di++) { // Para mover
                    for (int dj = -1; dj <= 1; dj++) { // Para mover en si si se dan cuenta el movimiento diagonal siempre es con 1 y el de pa comer es en 2
                        // Verifica que el movimiento es válido
                        if (i + di >= 0 && i + di < 8 && j + dj >= 0 && j + dj < 8 && tablita.tablero[i + di][j + dj] == nullptr) {
                            // Crea un nuevo estado del tablero
                            Tablero nuevoTablero = tablita;
                            bool tempTurno = esMaximizador;// este temp esta porque el mover ficha cambia el turno por si te equivocas, para que no pierdas turno, asi que aqui para que no mate el codigo lo aislamos
                            // Mueve la ficha en el nuevoTablero usando mover_Ficha
                            nuevoTablero.mover_Ficha(i, j, i + di, j + dj, tempTurno, 0);
                            // Si el movimiento fue válido, añade el nuevo tablero a la lista de movimientos
                            if (nuevoTablero.tablero[i + di][j + dj] != nullptr) {
                                movimientos.push_back(nuevoTablero);
                                posiciones.push_back(i), posiciones.push_back(j), posiciones.push_back(i + di), posiciones.push_back(j + dj);
                            }
                        }
                    }
                }
            }
        }
    }

    return movimientos;
}



int minimax(Tablero tablero, int profundidad, bool Max, int alfa, int beta) {//"arbol min max" en si solo con recursividad

    if (profundidad == 0) {
        return evaluarTablero(tablero);
    }
    vector<int>posiciones;
    std::vector<Tablero> movimientos = generarMovimientos(tablero, Max, posiciones);//genera siguientes movimientos para ir viendo cual es mejor

    if (Max) {//en este creo q era nuestra jugada 
        int maxEval = -999999; // por no poner infinito negativo
        for (Tablero movimiento : movimientos) {
            int eval = minimax(movimiento, profundidad - 1, false, alfa, beta);//recursividad para que vaya cambiando de max a min y asi
            maxEval = max(maxEval, eval);//este es el espacio ese donde se reemplazaba al infinito con los valores q vamos viendo en el arbol
            alfa = max(alfa, eval);
            if (beta <= alfa) {
                break; // Poda
            }
        }
        return maxEval;
    }

    else {//este es la jugada del enemigo
        int minEval = 999999; // por no poner infinito positivo
        for (Tablero movimiento : movimientos) {
            int eval = minimax(movimiento, profundidad - 1, true, alfa, beta);
            minEval = min(minEval, eval);//en si esto es todo lo mismo pero con el min
            beta = min(beta, eval);
            if (beta <= alfa) {
                break; // Poda 
            }
        }
        return minEval;
    }
    //aqui bueno siempre termina en max en si o eso es lo q devuelve al final de todo la funcion ya que es como q va bajando en el arbol primero max luego va a min luego de eso a max
    // y cuando termina sube de nuevo y ahi como comenzo en max devuelve el max
    //en si esto no es como que predice todas tus jugadas y a base de eso usa todo lo que predijo para repetirlas y ganarte, sino que analiza eventos futuros y asi ver que
    //jugada hacer ahora y que tanto beneficio le puede dar a futuro
}


int main() {
    Tablero tablero;
    tablero.llenar_tablero();
    bool run = 1;
    bool turnoJugador = true; // Asume que el jugador humano comienza
    int x = 0, y = 0, x_ = 0, y_ = 0, profundidad = 0;

    while (run) {
        tablero.imprimir_tablero();
        if (turnoJugador) {
            cout << "ingresa coordenadas de la ficha a mover 'y' y 'x' " << endl;
            cin >> x >> y;
            cout << "ingresa coordenadas a donde se desea mover 'y' y 'x'" << endl;
            cin >> x_ >> y_;
            cout << "ingresar nivel de profundidad de busqueda de la computadora " << endl;
            cin >> profundidad;
            if (tablero.tablero[x][y] != nullptr && tablero.tablero[x][y]->equipo == turnoJugador) {
                tablero.mover_Ficha(x, y, x_, y_, turnoJugador, 0);
                turnoJugador = !turnoJugador; // Cambia el turno
            }
            else {
                cout << "No puedes mover una ficha del oponente." << endl;
            }
        }
        else {
            // La computadora hace su jugada
            int mejorValor = -999999; // pa no poner infinito negativo 
            vector<int>posiciones;
            int fila1 = 0, col1 = 0, fila2 = 0, col2 = 0;
            std::vector<Tablero> movimientos = generarMovimientos(tablero, turnoJugador, posiciones);
            int i = 0;
            for (Tablero movimiento : movimientos) {
                int valor = minimax(movimiento, profundidad, !turnoJugador, -999999, 999999); // genera primero una serie de movimientos los cuales desde cada uno analizara cual saldra mas rentable
                //de ahi bueno el mas rentable lo va guardando aqui abajo 
                if (valor > mejorValor) {
                    mejorValor = valor;
                    fila1 = posiciones[i], col1 = posiciones[i + 1], fila2 = posiciones[i + 2], col2 = posiciones[i + 3];
                }
                i += 4;
            }
            tablero.mover_Ficha(fila1, col1, fila2, col2, turnoJugador, 1); // cambia el tablero por el del mejor movimiento
            turnoJugador = !turnoJugador;
        }
        if (tablero.piezas[0] == 0) { cout << endl << "ganaste :D"; run = false; }
        else if (tablero.piezas[1] == 0) { cout << endl << "perdiste D:"; run = false; }
    }
    return 0;
} */


#include "Board.h"

int main(int argc, char **argv){
    Board wiii(8);
    wiii.run(argc, argv);
    return 0;
}
