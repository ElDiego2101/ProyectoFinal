#include "personaje.h"

int personaje::getX() const
{
    return x;
}

void personaje::setX(int newX)
{
    x = newX;
}

int personaje::getY() const
{
    return y;
}

void personaje::setY(int newY)
{
    y = newY;
}

QGraphicsRectItem *personaje::getPies() const
{
    return pies;
}

Direccion personaje::getCurrentDirection() const
{
    return currentDirection;
}
personaje::~personaje(){
}

void personaje::setCurrentDirection(Direccion newCurrentDirection)
{
    currentDirection = newCurrentDirection;
}

Direccion personaje::getLastDirection() const
{
    return lastDirection;
}

void personaje::setLastDirection(Direccion newLastDirection)
{
    lastDirection = newLastDirection;
}

short personaje::getVida() const
{
    return vida;
}

void personaje::setVida(short newVida)
{
    vida = newVida;
}

personaje::personaje(QGraphicsItem *parent):QGraphicsPixmapItem(parent),
    x(0), y(0), dx(0), dy(0),
    frameHeight(0), frameWidth(0), // Ejemplo de tamaños de cuadro
    currentFrame(0),
    currentDirection(ninguna),
    lastDirection(derecha),pies(nullptr){
}


