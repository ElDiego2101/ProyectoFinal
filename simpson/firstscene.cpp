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

    timerGravedad = new QTimer(this);
    connect(timerGravedad, &QTimer::timeout, this, &FirstScene::aplicarGravedad);
    timerGravedad->start(30);
    golpeDaño=10;

    // Cambia según la cantidad necesaria
    establecerPlataformas();


    //vamos a establecer los parametros del jugador
    jugador = new bob();
    addItem(jugador); // Agregar al jugador a la escena
    jugador->setPos(0, 610);

    //salto
    enSalto = false;
    velocidadSalto =0;
    gravedad = 2;

}
bool FirstScene::puedeBajar() {
    QRectF rectPies = jugador->getPies()->sceneBoundingRect(); // Rectángulo global de los pies
    qreal margenBusqueda = 1; // Rango adicional para buscar una plataforma debajo

    for (auto *plataforma : plataformas) {
        QRectF rectPlataforma = plataforma->sceneBoundingRect();

        // Verifica si la plataforma está en el rango justo debajo de los pies
        if (rectPlataforma.top() > rectPies.bottom() &&                // La parte superior de la plataforma está debajo de los pies
            rectPlataforma.top() >= rectPies.bottom() + margenBusqueda) { // Pero no más allá de un margen razonable
            return true;
        }
    }

    return false; // No
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
    //camara del salto
    if (event->key() == Qt::Key_W && !enSalto) {
        enSalto = true;           // Activar el salto
        velocidadSalto = -28;     // Velocidad inicial negativa (hacia arriba)
    }

    if (event->key() == Qt::Key_S && !enSalto && puedeBajar()) {
        enSalto = false;           // Activar el salto
        velocidadSalto = +10;
        jugador->moverJugador(abajo);
    }

    //vamos a guardar la pos anterior
    int posX=jugador->getX();
    int posY=jugador->getY();

    // Movimiento del jugador según tecla
    switch (event->key()) {
    case Qt::Key_D:{
        jugador->moverJugador(derecha);
        jugador->setX(jugador->getX() + 5);
        if (!sobrePlataforma()) {
            jugador->setX(posX); // Revertir movimiento
            jugador->detenerJugador();
            velocidadFondo=0;}

        break;
    }
    case Qt::Key_A:{
        jugador->moverJugador(izquierda);
        jugador->setX(jugador->getX() - 5);
        if (!sobrePlataforma()) {
            jugador->setX(posX); // Revertir movimiento
            jugador->detenerJugador();
            velocidadFondo=0;}
        break;
    }
    case Qt::Key_W:
        jugador->moverJugador(arriba);
        //jugador->setY(jugador->getY() -10);
        break;
    case Qt::Key_S:{
        if (!enSalto && puedeBajar()) { // Solo puede bajar si no está saltando y hay espacio abajo
            //jugador->setY(jugador->getY() + 5); // Baja una pequeña distancia
            jugador->moverJugador(abajo);
            //jugador->setPos(jugador->getX(), jugador->getY());
        }
        //jugador->moverJugador(abajo);
        //jugador->setY(jugador->getY() + 5);
        break;
    }
    case Qt::Key_Z:{
        jugador->moverJugador(golpe);
    }
    }

    jugador->setPos(jugador->getX(), jugador->getY());

    // Validar si está sobre una plataforma
    if (!sobrePlataforma()) {
        jugador->setPos(posX, posY); // Revertir movimiento
        jugador->detenerJugador();
        return;
    }
}

void FirstScene::keyReleaseEvent(QKeyEvent *event) {
    teclasPresionadas.remove(event->key()); // Eliminar la tecla liberada

    bool teclaActiva = teclasPresionadas.contains(Qt::Key_A) ||
                      teclasPresionadas.contains(Qt::Key_D) ||
                      teclasPresionadas.contains(Qt::Key_W) ||
                     teclasPresionadas.contains(Qt::Key_S);

    if (!teclaActiva) {
       //Detener el jugador y la animación solo si no hay teclas activas
        jugador->detenerJugador();
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
    delete fondo;
    delete jugador;
    delete timerFondo;
    delete timerGravedad;
    for (auto plataforma : plataformas) {
        delete plataforma;
    }
}
bool FirstScene::sobrePlataforma() {
  for (auto *plataforma : plataformas) {
       if (jugador->getPies()->collidesWithItem(plataforma)) {
  return true;
     }
  }
   return false;
}
//no olvidarme de borrar en la memoria dinamica

void FirstScene::aplicarGravedad() {
    // Si el jugador está en salto, actualiza la posición vertical
    if (enSalto) {
        jugador->setY(jugador->getY() + velocidadSalto); // Mover según la velocidad
        velocidadSalto += gravedad;        // Aplicar gravedad
        jugador->setPos(jugador->getX(), jugador->getY());
        // Verificar si el jugador aterrizó en una plataforma
        if (sobrePlataforma()) {
          enSalto = false;              // Termina el salto
            jugador->setY(jugador->getY());  // Ajusta la posición exacta
            velocidadSalto = 0;           // Reinicia la velocidad
        }
    }else if(!enSalto && puedeBajar()){
        jugador->setY(jugador->getY() + velocidadSalto); // Mover según la velocidad
        velocidadSalto += gravedad;        // Aplicar gravedad
        jugador->setPos(jugador->getX(), jugador->getY());
        // Verificar si el jugador aterrizó en una plataforma
        if (sobrePlataforma()) {
            enSalto = false;              // Termina el salto
            jugador->setY(jugador->getY());  // Ajusta la posición exacta
            velocidadSalto = 0;           // Reinicia la velocidad
        }

    }
    else if (!sobrePlataforma()) {
        // Aplicar gravedad si el jugador no está en una plataforma
        jugador->setY(jugador->getY() + gravedad);
    }
}
