#include "jugador.h"

jugador::jugador(int x,int y,escenario* esc):personaje(x,y,esc) {
    alto=80;
    ancho=20;

    //aqui iran los sprites del personaje, no tengo claro si hacer
    // una clase para bob y otra para bard o los junto
}
void jugador::dibujarJugador(QGraphicsScene *scene){
    if (!objeto) {
        objeto = new QGraphicsRectItem(x, y, ancho, alto);
        objeto->setBrush(QBrush(Qt::blue)); // Color del jugador
        scene->addItem(objeto);
    }
}

