#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"
#include "QKeyEvent"

class jugador:public personaje
{
    int alto;
    int ancho;
protected:
    void keyPressEvent(QKeyEvent *event) override;
public:
    jugador(int x,int y,escenario* esc);
    void dibujarJugador(QGraphicsScene* scene);
};

#endif // JUGADOR_H
