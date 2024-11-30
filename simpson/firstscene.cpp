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
    establecerCajas();
    establecerPlataformas();


    //vamos a establecer los parametros del jugador
    jugador = new bob();
    addItem(jugador); // Agregar al jugador a la escena
    jugador->setPos(0, 600);

    //salto
    enSalto = false;
    enBajo=false;
    velocidadSalto =0;
    gravedad = 2;
    veri=true;

    //puntero de cajas
    for (int i = 0; i < Ncajas; ++i) {
        cajas[i] = nullptr;
    }

}
bool FirstScene::puedeBajar() {
    QRectF rectPies = jugador->getPies()->sceneBoundingRect(); // Rectángulo global de los pies
    qreal margenBusqueda = 15; // Rango adicional para buscar una plataforma debajo

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
void FirstScene::moverFondo() {
    if (velocidadFondo == 0) return; // No mover si no hay velocidad

    // Almacena las vistas en una variable
    auto vistas = this->views();
    if (vistas.isEmpty()) return; // Verifica que haya al menos una vista

    // Usa la primera vista sin generar objetos temporales
    QGraphicsView *vista = vistas.first();
    if (!vista) return;

    // Obtener el rectángulo de la vista (limites de la cámara)
    rectVista = vista->sceneRect();
    limiteIzquierda = rectVista.left()-10;
    limiteDerecha = rectVista.right()+10;

    // Verificar si el jugador está cerca del borde izquierdo o derecho

    if (jugador->getX() <= limiteIzquierda) {
        // Si el jugador está a la izquierda de la vista, detener el fondo
        velocidadFondo = -10;
    } else if (jugador->getX() >= limiteDerecha) {
        // Si el jugador está a la derecha de la vista, detener el fondo
        velocidadFondo = +10;
    }

    // Desplaza la vista (en lugar del fondo)
    QPointF nuevaPosicion = vista->sceneRect().topLeft() + QPointF(velocidadFondo, 0);

    // Limitar el desplazamiento en X
    if (nuevaPosicion.x() > 2168) {
        nuevaPosicion.setX(2168); // Limitar el movimiento hacia la derecha
    } else if (nuevaPosicion.x() < 0) {
        nuevaPosicion.setX(0); // Limitar el movimiento hacia la izquierda
    }

    // Aplica la nueva posición a la vista
    vista->setSceneRect(QRectF(nuevaPosicion, vista->sceneRect().size()));
}

void FirstScene::keyPressEvent(QKeyEvent *event) {
    teclasPresionadas.insert(event->key()); // Registrar la tecla
    int velocidad = 4; // Velocidad del desplazamiento
    int velocidadp = -4; // Velocidad negativa para mover a la izquierda
    int posX=jugador->getX();
    int posY=jugador->getY();

    if (event->key() == Qt::Key_A) { // Mover a la izquierda
        if(veri){
            velocidadFondo=0;
        }else{
            velocidadFondo = velocidadp;
        }
        // Cambiar la dirección
        // No es necesario verificar fondo->x(), ya que el movimiento se controla con la cámara
    } else if (event->key() == Qt::Key_D) {
        if(veri){
            velocidadFondo=0;
        }else{
            velocidadFondo = velocidad;
        }// Mover a la derecha
    }
    //camara del salto
    if (event->key() == Qt::Key_W && !enSalto) {
        jugador->moverJugador(arriba);
        enSalto = true;           // Activar el salto
        velocidadSalto = -28;     // Velocidad inicial negativa (hacia arriba)
    }

    if (event->key() == Qt::Key_S && !enSalto && puedeBajar()) {
        jugador->moverJugador(abajo);
        enBajo = true;           // Activar el salto
        velocidadSalto = +10;
    }

    //vamos a guardar la pos anterior

    // Movimiento del jugador según tecla
    switch (event->key()) {
    case Qt::Key_D:{
        jugador->moverJugador(derecha);
        jugador->setX(jugador->getX() + 6);
        veri=false;
        if (!sobrePlataforma()) {
            jugador->setX(posX); // Revertir movimiento
            jugador->detenerJugador();
            velocidadFondo=0;}

        break;
    }
    case Qt::Key_A:{
        jugador->moverJugador(izquierda);
        jugador->setX(jugador->getX() - 6);
        veri=false;
        if (!sobrePlataforma()) {
            jugador->setX(posX); // Revertir movimiento
            jugador->detenerJugador();
            //velocidadFondo=0;}
        }
        break;
    }
    case Qt::Key_W:
        jugador->moverJugador(arriba);
        //jugador->setY(jugador->getY() -10);
        break;
    case Qt::Key_S:{
        // Solo puede bajar si no está saltando y hay espacio abajo
        //jugador->setY(jugador->getY() + 5); // Baja una pequeña distancia
        jugador->moverJugador(abajo);
        //jugador->setPos(jugador->getX(), jugador->getY());

        //jugador->moverJugador(abajo);
        //jugador->setY(jugador->getY() + 5);
        break;
    }
    case Qt::Key_Z:{
        jugador->moverJugador(golpe);
    }
    }

    if ((jugador->getX()<=limiteIzquierda) ||(jugador->getX()>=limiteDerecha)) {
        jugador->detenerJugador();
        jugador->setX(posX);
        jugador->setY(posY);
    }
    jugador->setPos(jugador->getX(), jugador->getY());

    // Validar si está sobre una plataforma
}

void FirstScene::keyReleaseEvent(QKeyEvent *event) {

    teclasPresionadas.remove(event->key()); // Eliminar la tecla liberada

    // Verificar qué tecla fue soltada y detener al jugador
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        velocidadFondo = 0; // Detener el fondo
        jugador->detenerJugador(); // Detener el movimiento del jugador
    }
    else if (event->key() == Qt::Key_W) {
        //enSalto = false; // Permitir saltar nuevamente después de liberar la tecla
    }
    else if (event->key() == Qt::Key_S) {
       // enBajo = false; // Detener el movimiento hacia abajo
    }
}

void FirstScene::establecerPlataformas(){
    int posicion=0;
    for (int i = 0; i < plataformas.size(); ++i) {
        QGraphicsRectItem *plataforma = new QGraphicsRectItem(0,390+posicion, 3532, 10);
        plataforma->setBrush(Qt::darkGray);
        addItem(plataforma);
        plataformas[i] = plataforma; // Asigna la plataforma en el índice i
        posicion+=190;
        if(i==1){
            posicion-=30;
        }
    }
}

void FirstScene::establecerCajas(){
    qDebug() << "creando cajas";

    int indiceCaja = 0;
    int posicionX=110;
    int posicionY=348;
    for (int var = 0; var < 3; ++var) {
        for (int var = 0; var < 9; ++var) {
            // Crear un nuevo objeto QGraphicsPixmapItem para la caja
            QGraphicsPixmapItem *caja = new QGraphicsPixmapItem(QPixmap(":/imagenes/caja.jpg"));

            addItem(caja);
            caja->setPos(posicionX, posicionY);
            cajas[indiceCaja] = caja;
            ++indiceCaja;
            if (var% 6==5) {
                posicionX +=600;
                posicionY=698;
            }else if(var % 3 == 2){
                posicionX +=600;
                posicionY=538;
            }
            else{
                posicionY-=50;
            }

        }
        if(var==1){
            posicionX=1000;
            posicionY=348;

        }
        if(var==2){
            posicionX=2000;
            posicionY=348;
        }

    }

}

FirstScene::~FirstScene() {
    delete fondo;
    delete jugador;
    delete timerFondo;
    delete timerGravedad;
    delete vista;
    for (auto plataforma : plataformas) {
        delete plataforma;
    }
    for (int i = 0; i < Ncajas; ++i) {
        delete cajas[i];
        cajas[i] = nullptr;
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
          jugador->setY(jugador->getY());
          jugador->setPos(jugador->getX(), jugador->getY());// Ajusta la posición exacta
            velocidadSalto = 0;           // Reinicia la velocidad
        }
    }else if(enBajo && puedeBajar()){
        jugador->setY(jugador->getY() + velocidadSalto); // Mover según la velocidad
        velocidadSalto += gravedad;        // Aplicar gravedad
        jugador->setPos(jugador->getX(), jugador->getY());
        // Verificar si el jugador aterrizó en una plataforma
        if (sobrePlataforma()) {
            enBajo = false;              // Termina el salto
            jugador->setY(jugador->getY());
            jugador->setPos(jugador->getX(), jugador->getY());    // Ajusta la posición exacta
            velocidadSalto = 0;           // Reinicia la velocidad
        }

    }
    else if (!sobrePlataforma()) {
        // Aplicar gravedad si el jugador no está en una plataforma
        jugador->setY(jugador->getY() + gravedad);
        jugador->setPos(jugador->getX(), jugador->getY());
        if (jugador->getY() > 600) {
            jugador->setY(600 - jugador->boundingRect().height());
            enSalto = false;
            velocidadSalto = 0;
        }
    }
}
