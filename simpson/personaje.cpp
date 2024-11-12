#include "personaje.h"

void personaje::setDx(int newDx)
{
    dx = newDx;
}

void personaje::setIndicey(int newIndicey)
{
    indicey = newIndicey;
}

void personaje::setDy(int newDy)
{
    dy = newDy;
}

personaje::personaje(int x,int y,escenario* esc) {
    this ->x=x;
    this ->y=y;
    //cuantos espacio me voy a mover
    dx=0;
    dy=0;

    indicex=0;// cero sera de espaldas
    indicey=0;
    objeto=(nullptr);
    miEscenario=(esc);
}
void personaje::moverObjeto(){
    int nextX = x + dx;
    int nextY = y + dy;

    if (objeto) {
        // Verificar límites de la escena
        int limiteIzquierdo = 0;
        int limiteDerecho = 2292 - ancho;

        if (nextX >= limiteIzquierdo && nextX <= limiteDerecho) {
            bool sobrePlataforma = false;

            // Verificar si el personaje está sobre alguna plataforma
            for (auto plataforma : miEscenario->getPlataformas()) {
                if (plataforma && plataforma->contains(QPointF(nextX, nextY + alto))) {
                    sobrePlataforma = true;
                    break;
                }
            }

            // Si está sobre una plataforma, mover en X e Y
            if (sobrePlataforma) {
                dy = 0;  // Detener la caída
                x = nextX;
                y = nextY;
            } else {
                // Si no está sobre una plataforma, mover solo hacia abajo
                dy = 1;  // Incrementar para simular caída
                y += dy;
            }

            // Actualizar posición en la escena
            objeto->setRect(x, y, ancho, alto);
        }
    }
}
