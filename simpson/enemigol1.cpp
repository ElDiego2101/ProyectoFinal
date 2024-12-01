#include "enemigol1.h"

enemigol1::enemigol1(QGraphicsItem *parent):personaje(parent) {
    x=600;
    y=600;

    frameHeight = 150;
    frameWidth = 200;
    currentFrame = 0;
    currentDirection = derecha;
    lastDirection = izquierda;
    // Cargar el spritesheet
    spriteSheet = QPixmap(":/imagenes/enemigoS.png");

    // Establecer el primer cuadro
    setPixmap(spriteSheet.copy(0, 0, frameWidth, frameHeight));

    //lo demas
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &enemigol1::nextFrame);

    //pies para colisiones
    pies = new QGraphicsRectItem(this); // Hijo de `bob`
    pies->setRect(0,frameHeight-8,frameWidth, 2); // Rectángulo en la parte inferior
    //pies->setPen(QPen(Qt::NoPen));         // QPen para "sin borde"
   // pies->setBrush(QBrush(Qt::red)); // QBrush para "transpar
}
void enemigol1::dibujarJugador() {
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
    case deteccion:
        frameY = 2 * frameHeight; // Tercera fila
        // Ajustar según la última dirección
        if (lastDirection == derecha) {
            frameY = 2 * frameHeight;
        } else if (lastDirection == izquierda) {
            frameY = 3 * frameHeight;
        }
        break;
    case golpe:
        frameY=4*frameHeight;
        if(lastDirection==derecha){
            frameX=frameWidth;
        }else if(lastDirection==izquierda){
            frameX=2;

        }
        break;
    }

    // Actualizar el cuadro visible
        setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}
void enemigol1::moverJugador(Direccion direccion){
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
    }

    lastDirection = currentDirection; // Actualizar última dirección
}
void enemigol1::nextFrame(){
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
        currentFrame = (currentFrame + 1)%5;
}

enemigol1::~enemigol1(){
    delete timerAnimacion;
    delete pies;
}
