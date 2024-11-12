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
