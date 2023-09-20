# Checkers
checkers in c++

falta el que se haga por turnos tipo uno tu uno la pc, el min max y listo, cada ficha tiene un bool   que indica de que jugador es, con eso se podria hacer el min max. Bueno eso y ponerlo bonito tipo con .h y eso

Tablero& operator=(const Tablero& other) {
    if (this != &other) { // protect against invalid self-assignment
        // 1: deallocate old memory
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                delete tablero[i][j];
            }
        }

        // 2: allocate new memory and copy the elements
        tablero = vector<vector<Ficha*>>(8, vector<Ficha*>(8, nullptr));
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (other.tablero[i][j] != nullptr) {
                    tablero[i][j] = new Ficha(*(other.tablero[i][j])); // assuming Ficha has a copy constructor
                }
            }
        }

        // 3: copy piezas
        piezas[0] = other.piezas[0];
        piezas[1] = other.piezas[1];
    }
    return *this;
}
