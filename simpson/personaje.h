#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "escenario.h"
#include <QGraphicsView>
#include <QGraphicsScene>
class personaje
{

protected:
    QGraphicsRectItem *objeto;
    escenario* miEscenario;
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
    void moverObjeto();
    void setDx(int newDx);
    void setIndicey(int newIndicey);
    void setDy(int newDy);
};

#endif // PERSONAJE_H
