# Checkers
checkers in c++

falta el que se haga por turnos tipo uno tu uno la pc, el min max y listo, cada ficha tiene un bool   que indica de que jugador es, con eso se podria hacer el min max. Bueno eso y ponerlo bonito tipo con .h y eso

 Tablero copiarTablero() {
        Tablero copia;

        // Copiar el estado del tablero
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (tablero[i][j] != nullptr) {
                    copia.tablero[i][j] = new Ficha(tablero[i][j]->equipo, tablero[i][j]->tipo);
                }
            }
        }

        // Copiar el estado de las piezas
        copia.piezas[0] = piezas[0];
        copia.piezas[1] = piezas[1];

        return copia;
    }
