#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "escenario.h"
#include <QGraphicsView>
#include <QGraphicsScene>
class personaje
{  escenario* miEscenario;

protected:
    QGraphicsRectItem *objeto;
    int alto;
    int ancho;
    int dx;
    int dy;
    int indicex;
    int indicey;
    int x;
    int y;
public:
    personaje(int x,int y,escenario* esc);
};

#endif // PERSONAJE_H
