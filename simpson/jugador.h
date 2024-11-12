#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"

class jugador:public personaje
{
    int alto;
    int ancho;
public:
    jugador(int x,int y,escenario* esc);
    void dibujarJugador(QGraphicsScene* scene);
};

#endif // JUGADOR_H
