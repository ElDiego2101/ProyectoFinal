#include "enemigol1.h"

enemigol1::enemigol1(QGraphicsItem *parent):personajefs(parent) {
    x=600;
    y=605;
    vida=100;
    frameHeight = 150;
    frameWidth = 200;
    currentFrame = 0;
    currentDirection = derecha;
    lastDirection = izquierda;
    spriteSheet = QPixmap(":/imagenes/enemigoS.png");


    setPixmap(spriteSheet.copy(0, 0, frameWidth, frameHeight));


    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &enemigol1::nextFrame);


    pies = new QGraphicsRectItem(this);
    pies->setRect(0,frameHeight-8,frameWidth, 2);


    enCooldownDisparo = false;
    cooldownFinalizado = false;
}
void enemigol1::dibujarJugador() {
    int frameX = currentFrame * frameWidth;

    int frameY = 0;
    switch (currentDirection) {
    case derecha:
        frameY = 0;
        break;
    case izquierda:
        frameY = 1 * frameHeight;
        break;
    case deteccion:
        frameY = 2 * frameHeight;
        // Ajustar según la última dirección
        if (lastDirection == derecha) {
            frameY = 2 * frameHeight;
        } else if (lastDirection == izquierda) {
            frameY = 3 * frameHeight;
        }
        break;
    case golpe:
        frameY=4*frameHeight;
        if(lastDirection==izquierda){
            frameX=frameWidth;
        }else if(lastDirection==derecha){
            frameX=2*frameWidth;

        }
        break;
    }

    // Actualizar el cuadro visible
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}
void enemigol1::moverJugador(Direccion direccion){

    if(currentDirection != direccion && currentDirection !=golpe) {
        currentDirection = direccion;
        lastDirection = (direccion == derecha || direccion == izquierda) ? direccion : lastDirection;
        currentFrame = 0;

        timerAnimacion->start(150);
    }
    if (!timerAnimacion->isActive()) {
        timerAnimacion->start(150); // Inicia la animación si no está activa
    }
    int desplazamiento = 5; // Velocidad del enemigo
    // Mover en la dirección actual
    switch (currentDirection) {
    case derecha:
        x += desplazamiento;
        break;
    case izquierda:
        x -= desplazamiento;
        break;
    case arriba:
        y -= desplazamiento;
        break;
    case abajo:
        y += desplazamiento;
        break;
    case golpe:

        break;
    case deteccion:
        if(lastDirection==derecha){
            x+=desplazamiento+4;

        }else if(lastDirection==izquierda){
            x-=desplazamiento+4;
        }
    default:
        break;
    }
    setPos(x, y);
}

void enemigol1::detenerJugador(){
    currentDirection = ninguna; // Sin movimiento
    timerAnimacion->stop();

    // Mostrar el cuadro estático según la última dirección
    int frameX = (lastDirection == izquierda) ? frameWidth : 0; // Izquierda o derecha
    int frameY = 2 * frameHeight; // Fila estática (ninguna)
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}
void enemigol1::cambiarDireccion(){
    if (currentDirection == derecha) {
        currentDirection = izquierda;
    } else if (currentDirection == izquierda) {
        currentDirection = derecha;
    } else if (currentDirection == arriba) {
        currentDirection = abajo;
    } else if (currentDirection == abajo) {
        currentDirection = arriba;
    }else if(currentDirection==deteccion){
        if(lastDirection==izquierda){
            lastDirection=derecha;
        }else if(lastDirection==derecha){
            lastDirection=izquierda;
        }
        return;
    }

    lastDirection = currentDirection; // Actualizar última dirección
}
proyectil *enemigol1::getProyectilActivo() const
{
    return proyectilActivo;
}
bool enemigol1::tieneProyectilActivo() const{
    return proyectilActivo != nullptr;
}
void enemigol1::setProyectilActivo(proyectil *proyectil){
    proyectilActivo = proyectil;
}
void enemigol1::eliminarProyectil(){
    proyectilActivo = nullptr;
}
void enemigol1::nextFrame(){
    if (currentDirection == ninguna)
        return;
    if (currentDirection == golpe) {
        dibujarJugador();
            // La animación de golpe solo usa dos cuadros
        currentFrame = (currentFrame + 1) % 2;
        return;
    }
    dibujarJugador();
    if (currentFrame >= 4) {
        currentFrame = 0;
    }
    currentFrame = (currentFrame + 1)%5;
}
void enemigol1::startGolpeAnimacion() {
    if (currentDirection == golpe && !timerAnimacion->isActive()) {

    }
}
bool enemigol1::puedeDisparar(){
    return !enCooldownDisparo;

}
void enemigol1::iniciarCooldownDisparo(){
    enCooldownDisparo = true;
    cooldownTimer.singleShot(500, [this]() {
        enCooldownDisparo = false;
    });
}
bool enemigol1::enCooldown(){
    return enCooldownDisparo;
}
bool enemigol1::finalizoCooldown(){
    if (cooldownFinalizado) {
        cooldownFinalizado = false;
        return true;
    }
    return false;
}
enemigol1::~enemigol1(){
    delete timerAnimacion;
    delete pies;
}

