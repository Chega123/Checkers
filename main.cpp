#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


class Ficha {
public:
    bool equipo;  // '1' para nosotros, '0' para la pc
    bool tipo;   // '1' para dama, '0' para peón

    Ficha(bool equipo_, bool tipo_) {
        equipo=equipo_;
        tipo=tipo_;
    }
};


class Tablero {
private:
    vector<vector<Ficha*>> tablero;

public:
    Tablero() {
        tablero = vector<vector<Ficha*>>(8, vector<Ficha*>(8, nullptr)); //creación de vector de 8 de largo donde en cada espacio hay un vector de 8 fichas en nullptr
    }

    void llenar_tablero(){
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

    void imprimir_tablero(){
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

    void swap_Ficha(int fila1, int col1, int fila2, int col2){
        Ficha* temp = tablero[fila1][col1];
        tablero[fila1][col1] = tablero[fila2][col2];
        tablero[fila2][col2] = temp;
    };


    //no lo negare Bing me ayudo a acortarlo que había hecho bastante redundancia en cuanto a lo de revisar diagonales xd

    void comer_Ficha(int fila1, int col1, int fila2, int col2) {
        if (tablero[fila1][col1] == nullptr) {
            cout << "No existe pieza ahi";
            return;
        }

        if (tablero[fila2][col2] != nullptr) {
            cout << "Ya existe una pieza en el destino" << endl;
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
                return;
            }

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
                        cout << "El camino no está libre" << endl;
                        return;
                    }
                }
            }
        }
        else {
            cout << "No puedes mover tu ficha ahi" << endl;
        }
    };



    void mover_Ficha(int fila1, int col1, int fila2, int col2) {
        if (tablero[fila1][col1] == nullptr) {
            cout << "No existe pieza ahi";
            return;
        }

        if (tablero[fila2][col2] != nullptr) {
            cout << "Ya existe una pieza en el destino" << endl;
            return;
        }

        int Fila_resta = fila2 - fila1; //distancia que se mueve en y
        int Col_resta = abs(col2 - col1); //distancia que se mueve en x. Valor absoluto ya que a veces puede quedar -2

        // verifica movimiento para computadora y para nosotros ademas de revisar tmb a que lado van
        if ((tablero[fila1][col1]->equipo == 0 && Fila_resta == 1) || (tablero[fila1][col1]->equipo == 1 && Fila_resta == -1)) {
            if (Col_resta == 1) {//revisa que solo sean diagonales
                swap_Ficha(fila1, col1, fila2, col2);
                //revisa si llego al final y es hora de coronar
                if ((tablero[fila2][col2]->equipo == 0 && fila2 == 7) || (tablero[fila2][col2]->equipo == 1 && fila2 == 0)) {
                    tablero[fila2][col2]->tipo = 1;
                    cout << "Coronaste a la ficha :D" << endl;
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
                if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol] != nullptr) {//recorre el camino que pusimos hasta que encuentra un espacio no vacio, por lo que debe comer esa ficha o bueno al menos revisar si puede
                    if (tablero[fila1 + i * pasoFila][col1 + i * pasoCol]->equipo != tablero[fila1][col1]->equipo) {
                        comer_Ficha(fila1, col1, fila1 + (i+1) * pasoFila, col1 + (i+ 1) * pasoCol);
                        return;
                    }
                    else {
                        cout << "El camino no está libre" << endl;
                        return;
                    }
                }
            }
            swap_Ficha(fila1, col1, fila2, col2);
            return;
        }


        // Si ninguna de las condiciones anteriores se cumple, intenta comer una ficha
        if (abs(Fila_resta) >= 2 && Col_resta >= 2) {
            comer_Ficha(fila1, col1, fila2, col2);//comer normal de un peon, verifica que no se salga del tablero :D
            if ((tablero[fila2][col2]->equipo == 0 && fila2 == 7) || (tablero[fila2][col2]->equipo == 1 && fila2 == 0)) {
                tablero[fila2][col2]->tipo = 1;
                cout << "Coronaste a la ficha :D" << endl;
            }
            return;
        }

        cout << "No puedes mover tu ficha ahi" << endl;
        return;
    };
};

int main() {
    Tablero tablero;
    tablero.llenar_tablero();   
    bool run=1;
    int x=0,y=0,x_=0,y_=0;
    while(run){
        tablero.imprimir_tablero();
        cout<<"ingresa coordenadas de la ficha a mover 'y' y 'x' "<<endl;
        cin>>x>>y;
        cout<<"ingresa coordenadas a donde se desea mover 'y' y 'x'"<<endl;
        cin>>x_>>y_;
        tablero.mover_Ficha(x,y,x_,y_);
    }
    return 0;
};
