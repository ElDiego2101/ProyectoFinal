#include "firstscene.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>

FirstScene::FirstScene(MainWindow *parent)
    : QGraphicsScene(0, 0, 1366, 768, parent), mainWindow(parent), vidas(3), puntuacion(0), juegoPausado(false), velocidadFondo(0)
{
    fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/FondoFs.png"));
    fondo->setPos(0, 0);  // Posición inicial del fondo
    addItem(fondo);       // Agrega el fondo a la escena

    // Configura el temporizador para el movimiento continuo
    timerFondo = new QTimer(this);
    connect(timerFondo, &QTimer::timeout, this, &FirstScene::moverFondo);
    timerFondo->start(30);  // Intervalo de actualización del movimiento
    establecerPlataformas();


    //vamos a establecer los parametros del jugador
    jugador = new bob();
    addItem(jugador); // Agregar al jugador a la escena
    jugador->setPos(0, 520);

    //teAnimation

}

void FirstScene::keyPressEvent(QKeyEvent *event) {
    teclasPresionadas.insert(event->key()); // Registrar la tecla

    int velocidad = 5; // Velocidad del desplazamiento

    if (event->key() == Qt::Key_A) {
        velocidadFondo = velocidad;
        if ((fondo->x()) > 0) {
            velocidadFondo = 0;
        }
    } else if (event->key() == Qt::Key_D) {
        velocidadFondo = -velocidad;
    }

    // Movimiento del jugador según tecla
    switch (event->key()) {
    case Qt::Key_D:
        jugador->moverJugador(derecha);
        jugador->setX(jugador->getX() + 5);
        break;
    case Qt::Key_A:
        jugador->moverJugador(izquierda);
        jugador->setX(jugador->getX() - 5);
        break;
    case Qt::Key_W:
        jugador->moverJugador(arriba);
        jugador->setY(jugador->getY() - 5);
        break;
    case Qt::Key_S:
        jugador->moverJugador(abajo);
        jugador->setY(jugador->getY() + 5);
        break;
    }

    jugador->setPos(jugador->getX(), jugador->getY());
}

void FirstScene::keyReleaseEvent(QKeyEvent *event) {
    teclasPresionadas.remove(event->key()); // Eliminar la tecla liberada

    bool teclaActiva = teclasPresionadas.contains(Qt::Key_A) ||
                       teclasPresionadas.contains(Qt::Key_D) ||
                       teclasPresionadas.contains(Qt::Key_W) ||
                       teclasPresionadas.contains(Qt::Key_S);

    if (!teclaActiva) {
        // Detener el jugador y la animación solo si no hay teclas activas
        //jugador->detenerJugador();
        velocidadFondo = 0; // Detén el movimiento del fondo
    }
}

void FirstScene::moverFondo() {
    // Actualiza la posición del fondo
    if (velocidadFondo == 0) return; // No mover si no hay velocidad

    int nuevaPosX = fondo->x() + velocidadFondo;

    // Limitar el desplazamiento dentro de un rango
    int maxDesplazamiento = 500;
    if (nuevaPosX > maxDesplazamiento) {
        nuevaPosX = maxDesplazamiento;
    }

    fondo->setX(nuevaPosX); // Aplica la n // Aplica la nueva posición en X al fondo
}
void FirstScene::establecerPlataformas(){
    int posicion=0;
    for (int i = 0; i < plataformas.size(); ++i) {
        QGraphicsRectItem *plataforma = new QGraphicsRectItem( 0,390+posicion, 2292, 10);
        plataforma->setBrush(Qt::darkGray);
        addItem(plataforma);
        plataformas[i] = plataforma; // Asigna la plataforma en el índice i
        posicion+=190;
        if(i==1){
            posicion-=30;
        }
    }
}

FirstScene::~FirstScene() {
}

//no olvidarme de borrar en la memoria dinamica
