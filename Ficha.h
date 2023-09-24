#ifndef FICHA_H
#define FICHA_H

class Ficha {
public:
    bool equipo;  // '1' para nosotros, '0' para la pc
    bool tipo;   // '1' para dama, '0' para peón

    Ficha(bool equipo_, bool tipo_) {
        equipo=equipo_;
        tipo=tipo_;
    }
};

#endif
