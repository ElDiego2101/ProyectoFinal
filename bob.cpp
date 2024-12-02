#include "bob.h"

#include <QBrush>
#include <QPen>

bob::bob(QGraphicsItem *parent)
    : personajefs(parent) {
    //inicializo
    vida=100;
    x = 0;
    y = 600;
    dx = 5;
    dy = 5;
    frameHeight = 150;
    frameWidth = 200;
    currentFrame = 0;
    currentDirection = ninguna;
    lastDirection = derecha;
    // Cargar el spritesheet
    spriteSheet = QPixmap(":/imagenes/spriteBob.png");

    // Establecer el primer cuadro
    setPixmap(spriteSheet.copy(0, 0, frameWidth, frameHeight));

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &bob::nextFrame);

    //pies para colisiones
    pies = new QGraphicsRectItem(this); // Hijo de `bob`
    pies->setRect(0,frameHeight-8,frameWidth, 2); // Rectángulo en la parte inferior
    pies->setPen(QPen(Qt::NoPen));         // QPen para "sin borde"
    pies->setBrush(QBrush(Qt::red)); // QBrush para "transparente"

}


void bob::dibujarJugador() {
    int frameX = currentFrame * frameWidth; // Columna actual
    // Fila depende de la dirección actual
    int frameY = 0;
    switch (currentDirection) {
    case derecha:
        frameY = 0; // Primera fila
        break;
    case izquierda:
        frameY = 1 * frameHeight; // Segunda fila
        break;
    case ninguna:
        frameY = 2 * frameHeight; // Tercera fila
        // Ajustar según la última dirección
        if (lastDirection == izquierda) {
            frameX = frameWidth; // Segundo cuadro
        } else if (lastDirection == derecha) {
            frameX = 0; // Primer cuadro
        }
        break;
    case golpe:
        frameY=4*frameHeight;
        if(lastDirection==derecha){
            frameX=frameWidth;
        }else if(lastDirection==izquierda){
            frameX=0;

        }
        break;
    case arriba:
    case abajo:
        frameY = 3 * frameHeight; // Cuarta fila
        break;
    }

    // Actualizar el cuadro visible
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}

void bob::moverJugador(Direccion direccion) {
    if(currentDirection != direccion) {
        currentDirection = direccion;
        lastDirection = (direccion == derecha || direccion == izquierda) ? direccion : lastDirection;
        currentFrame = 0;           // Reinicia la animación
            // Inicia temporizador (100 ms por cuadro)
        timerAnimacion->start(100);
    }
    if (!timerAnimacion->isActive()) {
        timerAnimacion->start(100); // Inicia la animación si no está activa
    }
}

void bob::detenerJugador() {
    currentDirection = ninguna; // Sin movimiento
    timerAnimacion->stop();

    // Mostrar el cuadro estático según la última dirección
    int frameX = (lastDirection == izquierda) ? frameWidth : 0; // Izquierda o derecha
    int frameY = 2 * frameHeight; // Fila estática (ninguna)
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}

void bob::nextFrame() {
    if (currentDirection == ninguna)
        return;
    if (currentDirection==golpe){
        currentFrame=(currentFrame+1)%2;
        dibujarJugador();
        return;
    }
    dibujarJugador();
    if (currentFrame >= 4) {
        currentFrame = 0;
    }
    currentFrame = (currentFrame + 1)%4;
}

bob::~bob() {
    delete timerAnimacion;
}

