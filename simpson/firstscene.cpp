#include "firstscene.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>

FirstScene::FirstScene(MainWindow *parent)
    : QGraphicsScene(0, 0, 1366, 768, parent), mainWindow(parent), vidas(3), puntuacion(0), juegoPausado(false), velocidadFondo(0)
{
    fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/FondoFS.jpg"));
    fondo->setPos(0, 0);  // Posición inicial del fondo
    addItem(fondo);       // Agrega el fondo a la escena

    // Configura el temporizador para el movimiento continuo
    timerFondo = new QTimer(this);
    connect(timerFondo, &QTimer::timeout, this, &FirstScene::moverFondo);
    timerFondo->start(30);  // Intervalo de actualización del movimiento
}

void FirstScene::keyPressEvent(QKeyEvent *event) {
    int velocidad = 5; // Velocidad del desplazamiento

    if (event->key() == Qt::Key_A) {
        velocidadFondo = velocidad;  // Mover a la
        if ((fondo->x()) > 0) {
            velocidadFondo = 0;
        }
    } else if (event->key() == Qt::Key_D) {
        velocidadFondo = -velocidad;  // Mover a la derecha
    }
}

void FirstScene::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        velocidadFondo = 0;  // Detener el movimiento al soltar la tecla
    }
}

void FirstScene::moverFondo() {
    // Actualiza la posición del fondo
    int nuevaPosX = fondo->x() + velocidadFondo;

    // Limitar el desplazamiento dentro de un rango
    int maxDesplazamiento = 500;
    if (nuevaPosX > maxDesplazamiento) {
        nuevaPosX = maxDesplazamiento;
    }

    fondo->setX(nuevaPosX);  // Aplica la nueva posición en X al fondo
}
