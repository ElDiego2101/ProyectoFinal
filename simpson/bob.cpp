#include "bob.h"

bob::bob(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
    x(0), y(0), dx(5), dy(5),
    frameHeight(150), frameWidth(200), // Ejemplo de tamaños de cuadro
    currentFrame(0),
    currentDirection(ninguna),
    lastDirection(derecha) {
    // Cargar el spritesheet
    spriteSheet = QPixmap(":/imagenes/spriteBob.png");

    // Establecer el primer cuadro
    setPixmap(spriteSheet.copy(0, 0, frameWidth, frameHeight));

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &bob::nextFrame);
}

int bob::getX() const {
    return x;
}

void bob::setX(int newX) {
    x = newX;
}

int bob::getY() const {
    return y;
}

void bob::setY(int newY) {
    y = newY;
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
        timerAnimacion->start(200); // Inicia temporizador (100 ms por cuadro)
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

    // Calcular posición del cuadro en el spritesheet
   // int frameX = currentFrame * frameWidth;
    //int frameY = currentDirection * frameHeight;

    // Actualizar el cuadro visible
    dibujarJugador();

    // Avanzar al siguiente cuadro
    currentFrame = (currentFrame + 1) % 4; // Suponiendo 4 cuadros por animación
}
bob::~bob() {
    delete timerAnimacion;  // Limpieza del temporizador
}
