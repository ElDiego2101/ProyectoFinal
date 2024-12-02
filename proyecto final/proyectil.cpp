#include "proyectil.h"

bool proyectil::getTipo() const
{
    return tipo;
}

void proyectil::setTipo(bool newTipo)
{
    tipo = newTipo;
}

bool proyectil::getDireccion() const
{
    return direccion;
}

void proyectil::setDireccion(bool newDireccion)
{
    direccion = newDireccion;
}

proyectil::proyectil(QGraphicsItem *parent):QGraphicsPixmapItem(parent) {
    velocidadX = 10; // Velocidad inicial en X
    velocidadY = -10; // Velocidad inicial en Y
    gravedad = 1;
    tipo=true;
    direccion=true;
}
void proyectil::dibujarProyectil(){
    if(tipo){
        setPixmap(QPixmap(":/imagenes/bolaPelo.png"));

    }else{
        setPixmap(QPixmap(":/imagenes/bala.png"));
    }
}
void proyectil::actualizarMovimiento() {
    // Movimiento parabólico
    if(tipo){
        if(direccion){
            setPos(x() + velocidadX, y() + velocidadY);
            velocidadY += gravedad; // Aumenta la velocidad en Y por gravedad
        }else{
            setPos(x() - velocidadX, y() + velocidadY);
            velocidadY += gravedad;

        }

    }else{
        if(direccion){
            setPos(x() + velocidadX, y());
                // Aumenta la velocidad en Y por gravedad
        }else{
            setPos(x() - velocidadX, y());

        }

    }

}

void proyectil::cambiarDireccion() {
    velocidadY = -velocidadY; // Invertir dirección al chocar
}

