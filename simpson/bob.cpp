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
    // Actualiza el sprite según el frame actual
    int frameX = currentFrame * frameWidth;
    int frameY = currentDirection * frameHeight;
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}

void bob::moverJugador(Direccion direccion) {
    if (currentDirection != direccion) {
        currentDirection = direccion;
        lastDirection = direccion;
        currentFrame = 0;           // Reinicia la animación
        timerAnimacion->start(100);
    }
}

void bob::detenerJugador() {
    currentDirection = ninguna;
    timerAnimacion->stop();


    // Mostrar el cuadro estático según la última dirección
    int frameX = 0;
    int frameY = lastDirection * frameHeight;
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));
}

void bob::nextFrame() {
    if (currentDirection == ninguna)
        return;

    // Calcular posición del cuadro en el spritesheet
    int frameX = currentFrame * frameWidth;
    int frameY = currentDirection * frameHeight;

    // Actualizar el cuadro visible
    setPixmap(spriteSheet.copy(frameX, frameY, frameWidth, frameHeight));

    // Avanzar al siguiente cuadro
    currentFrame = (currentFrame + 1) % 4; // Suponiendo 4 cuadros por animación
}
bob::~bob() {
    delete timerAnimacion;  // Limpieza del temporizador
}
