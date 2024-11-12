#include "personaje.h"

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
        // Verificar límites de la escena (por ejemplo, izquierda y derecha)
        int limiteIzquierdo = 0;
        int limiteDerecho = 2292 - ancho;

        if (nextX >= limiteIzquierdo && nextX <= limiteDerecho) {
            bool sobrePlataforma = false;

            // Verificar si el personaje está sobre alguna plataforma
            for (auto plataforma : miEscenario->getPlataformas()) {
                if (plataforma && plataforma->contains(QPointF(x, y + alto))) {
                    sobrePlataforma = true;
                    break;
                }
            }

            // Si está sobre una plataforma, mover en X e Y
            if (sobrePlataforma) {
                x = nextX;
                y = nextY;
            } else {
                // Si no está sobre una plataforma, mover solo hacia abajo
                dy = 1;  // Incrementa dy para simular caída
                y += dy;
            }

            // Actualizar posición en la escena
            objeto->setRect(x, y, ancho, alto);
        }
    }
}
