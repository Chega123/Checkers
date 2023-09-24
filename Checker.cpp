#include "Checker.h"

Checker::Checker() {
    tablero = vector<vector<Ficha*>>(8, vector<Ficha*>(8, nullptr)); //creación de vector de 8 de largo donde en cada espacio hay un vector de 8 fichas en nullptr
    llenar_tablero();
}

vector<vector<Ficha *>> Checker::getTablero() {
    return tablero;
}

void Checker::llenar_tablero(){
    for(int i = 0; i < 8; i++) {

        if(0<=i && i<=2){
            for(int j = 0; j < 8; j++) {
                if((j+i) % 2 == 1) tablero[i][j]=new Ficha(0,0);    
            }  
        }      
        else if(5<=i && i<=7){
            for(int j = 0; j < 8; j++) {
                if((j+i) % 2 == 1) tablero[i][j]=new Ficha(1,0);    
            }  
        }
    }   
};

void Checker::imprimir_tablero(){
    int i=0;
    cout <<"  0   1   2   3   4   5   6   7"<<endl;
    cout << " +---+---+---+---+---+---+---+---+"<<endl;
    for(auto it = tablero.begin(); it != tablero.end(); it++,i++) {

        cout <<i<< "|"; // borde izq
        for(auto jt = it->begin(); jt != it->end(); jt++) {

            cout << " ";
            if(*jt != nullptr) {
                //imprimimos fichas según equipo :D
                if((*jt)->equipo == 0 && (*jt)->tipo==0) {cout << "Ø";}
                else if((*jt)->equipo == 1 && (*jt)->tipo==0) {cout << "O";}
                else if((*jt)->equipo == 0 && (*jt)->tipo==1) {cout << "@";}
                else if((*jt)->equipo == 1 && (*jt)->tipo==1) {cout << "©";}
            } 

            else {
                cout << " "; // si celda esta vacía espacio
            }

            cout << " |"; // borde derecho
        }

        cout <<endl;
        cout << " +---+---+---+---+---+---+---+---+\n";
    }
};

void Checker::swap_Ficha(int fila1, int col1, int fila2, int col2){
    Ficha* temp = tablero[fila1][col1];
    tablero[fila1][col1] = tablero[fila2][col2];
    tablero[fila2][col2] = temp;
};


    //no lo negare Bing me ayudo a acortarlo que había hecho bastante redundancia en cuanto a lo de revisar diagonales xd

void Checker::comer_Ficha(int fila1, int col1, int fila2, int col2, bool turno) {
    if (tablero[fila1][col1] == nullptr) {
        cout << "No existe pieza ahi";
        turno = !turno;
        return;
    }

    if (tablero[fila2][col2] != nullptr) {
        cout << "Ya existe una pieza en el destino" << endl;
        turno = !turno;
        return;
    }

    int Fila_resta = fila2 - fila1; //distancia que se mueve en y
    int Col_resta = col2 - col1; //distancia que se mueve en x. Valor absoluto ya que a veces puede quedar -2

    // verifica si la ficha a comer está en diagonal y a una distancia de 2
    if (abs(Fila_resta) == 2 && abs(Col_resta) == 2) {
        int filaComida = fila1 + Fila_resta / 2;
        int colComida = col1 + Col_resta / 2;

        if (tablero[filaComida][colComida] == nullptr || tablero[filaComida][colComida]->equipo == tablero[fila1][col1]->equipo) {//revisa si el espacio que va a ser no esta vacio o si es del mismo equipo
            cout << "No hay ficha enemiga para comer en esa dirección" << endl;
            turno = !turno;
            return;
        }
        piezas[tablero[filaComida][colComida]->equipo] -= 1; // Restar uno del contador del equipo correspondiente

        delete tablero[filaComida][colComida];
        tablero[filaComida][colComida] = nullptr;
        swap_Ficha(fila1, col1, fila2, col2);
        cout << "Ficha comida!" << endl;
    }

    else if (tablero[fila1][col1]->tipo == 1 && abs(Fila_resta) == abs(Col_resta)) { // Movimiento de la Reina
        // Verifica si el camino está libre
        int pasoFila = (fila2 > fila1) ? 1 : -1;//ve a que lado ira si arriba (-1) abajo (1)
        int pasoCol = (col2 > col1) ? 1 : -1; //ve a que lado ira si izq (-1) der (1)

        for (int i = 1; i < max(abs(Fila_resta), abs(Col_resta)); i++) {//va hasta la posicion indicada y si encuentra una ficha en medio se la come y se queda una posicion adelante de la ficha
            if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol] != nullptr) {
                if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol]->equipo != tablero[fila1][col1]->equipo) {
                    delete tablero[fila1 + i * pasoFila][col1 + i * pasoCol];
                    tablero[fila1 + i * pasoFila][col1 + i * pasoCol] = nullptr;
                    swap_Ficha(fila1, col1, fila2, col2);
                    cout << "Ficha comida por la dama!" << endl;
                    return;
                }
                else {
                    turno = !turno;
                    cout << "El camino no está libre" << endl;
                    return;
                }
            }
        }
    }
    else {
        turno = !turno;
        cout << "No puedes mover tu ficha ahi" << endl;
    }
};


void Checker::mover_Ficha(int fila1, int col1, int fila2, int col2, bool& turno, bool coronacion) {
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


int Checker::evaluarTablero(Checker& tablero) {//lo de la resta pa ver el puntaje
    return tablero.piezas[0] - tablero.piezas[1];
}

vector<Checker> Checker::generarMovimientos(Checker& tablita, bool esMaximizador, vector<int>& posiciones) { //duda sobre lo de copiar tabla con el &
    vector<Checker> movimientos;

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
                                Checker nuevoTablero = tablita;//en si es donde se hara el movimiento y asi nada mas
                                // Mueve la ficha en el nuevoTablero y elimina la ficha del equipo contrario
                                nuevoTablero.tablero[i + di][j + dj] = nuevoTablero.tablero[i][j];
                                nuevoTablero.tablero[i][j] = nullptr;
                                nuevoTablero.tablero[i + di / 2][j + dj / 2] = nullptr;
                                // Añade la nueva "tablita" a la lista de movimientos

                                nuevoTablero.piezas[tablita.tablero[i + di / 2][j + dj / 2]->equipo] -= 1;
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
                            Checker nuevoTablero = tablita;
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

int Checker::minimax(Checker tablero, int profundidad, bool Max, int alfa, int beta) {//"arbol min max" en si solo con recursividad

    if (profundidad == 0) {
        return evaluarTablero(tablero);
    }
    vector<int>posiciones;
    std::vector<Checker> movimientos = generarMovimientos(tablero, Max, posiciones);//genera siguientes movimientos para ir viendo cual es mejor

    if (Max) {//en este creo q era nuestra jugada 
        int maxEval = -999999; // por no poner infinito negativo
        for (Checker movimiento : movimientos) {
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
        for (Checker movimiento : movimientos) {
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
