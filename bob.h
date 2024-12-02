#ifndef BOB_H
#define BOB_H
#include "personajefs.h"

class bob:public personajefs
{
    Q_OBJECT
private slots:
    void nextFrame();
public:
    bob(QGraphicsItem *parent = nullptr);
    ~bob();
    void dibujarJugador();
    void moverJugador(Direccion direccion);
    void detenerJugador();

};
#endif // BOB_H
