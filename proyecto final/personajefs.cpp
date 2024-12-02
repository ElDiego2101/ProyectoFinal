#include "personajefs.h"
int personajefs::getX() const
{
    return x;
}

void personajefs::setX(int newX)
{
    x = newX;
}

int personajefs::getY() const
{
    return y;
}

void personajefs::setY(int newY)
{
    y = newY;
}

QGraphicsRectItem *personajefs::getPies() const
{
    return pies;
}

Direccion personajefs::getCurrentDirection() const
{
    return currentDirection;
}
personajefs::~personajefs(){
}

void personajefs::setCurrentDirection(Direccion newCurrentDirection)
{
    currentDirection = newCurrentDirection;
}

Direccion personajefs::getLastDirection() const
{
    return lastDirection;
}

void personajefs::setLastDirection(Direccion newLastDirection)
{
    lastDirection = newLastDirection;
}

short personajefs::getVida() const
{
    return vida;
}

void personajefs::setVida(short newVida)
{
    vida = newVida;
}

personajefs::personajefs(QGraphicsItem *parent):QGraphicsPixmapItem(parent),
    x(0), y(0), dx(0), dy(0),
    frameHeight(0), frameWidth(0), // Ejemplo de tamaños de cuadro
    currentFrame(0),
    currentDirection(ninguna),
    lastDirection(derecha),pies(nullptr){
}
