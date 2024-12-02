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

    proyectilActivo = nullptr; // Puntero al proyectil actual
    timerMovimientoProyectil=new QTimer(this);
    connect(timerMovimientoProyectil, &QTimer::timeout, this, [this]() {
        if (proyectilActivo) {
            proyectilActivo->actualizarMovimiento();

            // Verificar colisión con plataformas
            if (choquePlataforma(proyectilActivo)) {
                proyectilActivo->cambiarDireccion(); // Cambiar dirección si hay colisión
            }

            // Eliminar el proyectil si se sale de la escena
            if (proyectilActivo->y() > 768 || proyectilActivo->x() > 3532 || proyectilActivo->x() < 0 || colisionProyectil1(proyectilActivo)||colisionbalaE(proyectilActivo)) {
                timerMovimientoProyectil->stop(); // Detener el timer
                removeItem(proyectilActivo);     // Quitar el proyectil de la escena
                delete proyectilActivo;         // Liberar memoria del proyectil
                proyectilActivo = nullptr;      // Resetear el puntero
            }
        }
    });

    // Configura el temporizador para el movimiento continuo
    timerFondo = new QTimer(this);
    connect(timerFondo, &QTimer::timeout, this, &FirstScene::moverFondo);
    timerFondo->start(30);  // Intervalo de actualización del movimiento

    timerGravedad = new QTimer(this);
    connect(timerGravedad, &QTimer::timeout, this, &FirstScene::aplicarGravedad);
    timerGravedad->start(30);
    golpeDaño=30;
    cantidadEnemigos=10;
    galletas.resize(3);
    // Cambia según la cantidad necesaria
    establecerCajas();
    establecerPlataformas();
    establecerEnemigos1();
    establecerGalletas();


    //vamos a establecer los parametros del jugador
    jugador = new bob();
    addItem(jugador); // Agregar al jugador a la escena
    jugador->setZValue(0);
    jugador->setPos(0, 600);

    //salto
    enSalto = false;
    enBajo=false;
    velocidadSalto =0;
    gravedad = 2;
    veri=true;

    //colisiones con cajas
    colisionDerecha=false;
    colisionIzquierda=false;

    //inicializar el movimiento de los enemigos1
        movimientoEnemigo1 = new QTimer(this);
        connect(movimientoEnemigo1, &QTimer::timeout, this, &FirstScene::moverEnemigo1);
        movimientoEnemigo1->start(50); // Actualiza cada 50 ms
    //deteccion de bob por parte de los enemigos 1
    deteccionE1=new QTimer(this);
    connect(deteccionE1,&QTimer::timeout,this,&FirstScene::detecionEnemigos1);
    deteccionE1->start(110);

    //disparo del primer enemigo
    dispararE1=new QTimer(this);
    connect(dispararE1,&QTimer::timeout,this,&FirstScene::dispararE1_);
    dispararE1->start(60);

    moverItems=new QTimer(this);
    connect(moverItems,&QTimer::timeout,this,&FirstScene::moverItemsF);
    moverItems->start(50);

    //crear llave a la escena
    llave = new QGraphicsPixmapItem(QPixmap(":/imagenes/llave.png"));

    // Establecer posición inicial del Pixman
    llave->setPos(1730, 500); // Posición X, Y en la escena
    pixmanDirection=1;
    pixmanDirection2=1;
    // Agregar Pixman a la escena
    addItem(llave);
    llaveV=false;

    candado=new QGraphicsPixmapItem(QPixmap(":/imagenes/candado.png"));
    candado->setPos(3280,300);
    addItem(candado);

    //temporarizador de verificacion al siguiente nivel
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
        velocidadSalto = -28;     // Velocidad inicial negativa (hacia arriba
        colisionDerecha=false;
        colisionIzquierda=false;
    }

    if (event->key() == Qt::Key_S && !enSalto && puedeBajar()) {
        jugador->moverJugador(abajo);
        colisionDerecha=false;
        colisionIzquierda=false;
        enBajo = true;           // Activar el salto
        velocidadSalto = +10;
    }

    //vamos a guardar la pos anterior

    // Movimiento del jugador según tecla
    switch (event->key()) {
    case Qt::Key_D:{
        if (!colisionDerecha) { // Si no hay colisión hacia la derecha
            jugador->moverJugador(derecha);
            jugador->setX(jugador->getX() + 6);
            veri = false;
            if (!sobrePlataforma()) { // Detectar colisión
                jugador->setX(posX); // Revertir movimiento
                jugador->detenerJugador();
                velocidadFondo = 0;
            }
            if(!colisionIzquierda){
                if(colisionCaja()){
                    jugador->setX(posX); // Revertir movimiento
                    jugador->detenerJugador();
                    velocidadFondo = 0;
                    colisionDerecha = true; // Activar bandera para bloquear movimiento a la derecha
                }else{
                    if(!colisionCaja()){
                        colisionIzquierda=false;
                    }
                }
        }else{
                if(!colisionCaja()){
                    colisionIzquierda=false;

                }
            }
        }

        break;
    }
    case Qt::Key_A:{
        if (!colisionIzquierda) { // Si no hay colisión hacia la izquierda
            jugador->moverJugador(izquierda);
            jugador->setX(jugador->getX() - 6);
            veri = false;
            if (!sobrePlataforma()) { // Detectar colisión
                jugador->setX(posX); // Revertir movimiento
                jugador->detenerJugador();
                velocidadFondo = 0;
            }
            if(!colisionDerecha){
                if(colisionCaja()){
                    jugador->setX(posX); // Revertir movimiento
                    jugador->detenerJugador();
                    velocidadFondo = 0;
                    colisionIzquierda = true; // Activar bandera para bloquear movimiento a la derecha
                }else{
                    if(!colisionCaja()){
                        colisionDerecha=false;
                    }
                }
            }else{
                if(!colisionCaja()){
                    colisionDerecha=false;

                }
            }
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

    if ((jugador->getX()<=limiteIzquierda) ||(jugador->getX()>=limiteDerecha)||colisionCaja()) {
        jugador->detenerJugador();
        jugador->setX(posX);
        jugador->setY(posY);
    }
    jugador->setPos(jugador->getX(), jugador->getY());

    // Validar si está sobre una plataforma
    if (event->key() == Qt::Key_X) {
        if (event->key() == Qt::Key_X) {
            if (proyectilActivo) {
                qDebug() << "Ya hay un proyectil activo. No se puede disparar otro.";
                return; // Evitar disparar un nuevo proyectil si ya hay uno activo
            }

            proyectilActivo = new proyectil();
            proyectilActivo->dibujarProyectil();
            if(jugador->getLastDirection()==izquierda){
                proyectilActivo->setDireccion(false);
            }
            proyectilActivo->setPos(jugador->getX(), jugador->getY()+30); // Aparece donde está el jugador
            addItem(proyectilActivo);

            timerMovimientoProyectil->start(30); // Iniciar el movimiento
        }
        // Ajusta el tiempo de actualización
    }

}

void FirstScene::keyReleaseEvent(QKeyEvent *event) {

    teclasPresionadas.remove(event->key()); // Eliminar la tecla liberada

    // Verificar qué tecla fue soltada y detener al jugador
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        velocidadFondo = 0; // Detener el fondo
        //jugador->detenerJugador();     // Detener el movimiento del jugador
    }
}
void FirstScene::establecerPlataformas(){
    int posicion=0;
    for (auto i = 0; i < plataformas.size(); ++i) {
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
void FirstScene::establecerEnemigos1(){
    for (auto var = 0; var < cantidadEnemigos; ++var) {
        enemigol1* enemigo1 = new enemigol1;
        //vamos a manejar manualmente las apareciones de las enemigos
        //para asi tener el juego dinamico con la dificultad
        if(var==0){
        }else if(var==1){
            enemigo1->setY(250);
            enemigo1->setX(2000);
        }else if(var==2){
            enemigo1->setY(250);
            enemigo1->setX(2080);

        }else if(var==3){
            enemigo1->setY(440);
            enemigo1->setX(3100);

        }else if(var==4){
            enemigo1->setY(440);
            enemigo1->setX(3200);
        }else if(var==5){
            enemigo1->setY(605);
            enemigo1->setX(3000);

        }else if(var==6){
            enemigo1->setY(605);
            enemigo1->setX(3100);
            addItem(enemigo1);
        }else if(var==7){
            enemigo1->setY(250);
            enemigo1->setX(1300);
            addItem(enemigo1);
        }else if(var==8){
            enemigo1->setY(250);
            enemigo1->setX(1000);
            addItem(enemigo1);
        }else if(var==9){
            enemigo1->setY(250);
            enemigo1->setX(700);
            addItem(enemigo1);
        }else if(var==10){
            enemigo1->setY(250);
            enemigo1->setX(400);
            addItem(enemigo1);
        }
        addItem(enemigo1);
        enemigo1->setPos(enemigo1->getX(), enemigo1->getY());
        enemigos1.push_back(enemigo1);
    }

}
void FirstScene::establecerGalletas(){
    // Aseguramos que haya espacio para 3 galletas en el vector
    int posicionesX[] = {20, 2100, 3300}; // Posiciones predeterminadas en X
    int posicionY[] ={348,544,700}; // Todas las galletas están en la misma altura Y

    for (int var = 0; var < 3; var++) {
        QGraphicsPixmapItem* galleta = new QGraphicsPixmapItem(QPixmap(":/imagenes/galleta.png"));
        addItem(galleta);
        galleta->setZValue(0);
        galleta->setPos(posicionesX[var], posicionY[var]);

        // Usamos el índice para acceder al vector directamente
        galletas[var] = galleta;
    }
}
void FirstScene::establecerCajas(){
    qDebug() << "creando cajas";

    int indiceCaja = 0;
    int posicionX=110;
    int posicionY=340;
    for (int var = 0; var < 3; ++var) {
        for (int var = 0; var < 9; ++var) {
            // Crear un nuevo objeto QGraphicsPixmapItem para la caja
            QGraphicsPixmapItem *caja = new QGraphicsPixmapItem(QPixmap(":/imagenes/caja.jpg"));

            addItem(caja);
            caja->setZValue(0);
            caja->setPos(posicionX, posicionY);
            cajas[indiceCaja] = caja;
            ++indiceCaja;
            if (var% 6==5) {
                posicionX +=600;
                posicionY=698;
            }else if(var % 3 == 2){
                posicionX +=600;
                posicionY=530;
            }
            else{
                posicionY-=50;
            }

        }
        if(var==1){
            posicionX=1000;
            posicionY=340;

        }
        if(var==2){
            posicionX=2000;
            posicionY=340;
        }

    }

    qDebug() << indiceCaja;
}

FirstScene::~FirstScene() {
    delete fondo;
    delete jugador;
    delete timerFondo;
    delete timerGravedad;
    delete vista;
    delete llave;
    for (auto plataforma : plataformas) {
        delete plataforma;
    }
    for (auto caja :cajas) {
        delete caja;
    }
    for (auto& enemigo : enemigos1) {
        delete enemigo; // Liberar la memoria
    }
    enemigos1.clear();
}
bool FirstScene::sobrePlataforma() {
  for (auto *plataforma : plataformas) {
    if (jugador->getPies()->collidesWithItem(plataforma)) {
  return true;
   }
  }
  return false;

}
bool FirstScene::choquePlataforma(proyectil* bala) {
    for (auto *plataforma : plataformas) {
        if (bala->collidesWithItem(plataforma)) {
            return true;
        }
    }
    return false;

}
bool FirstScene::colisionCaja(){
for (auto *caja:cajas) {
    if (jugador->collidesWithItem(caja)) {
        qDebug() << "colision detectada";
        return true; // Si hay colisión con alguna caja
    }
}
//colisionDerecha=false;
//colisionIzquierda=false;
return false; // No hay colisión con ninguna caja
}
//no olvidarme de borrar en la memoria dinamica

void FirstScene::aplicarGravedad() {
    // Si el jugador está en salto, actualiza la posición vertical
    if (enSalto) {
        jugador->setY(jugador->getY() + velocidadSalto);
        velocidadSalto += gravedad;        // Mover según la velocidad
        // Aplicar gravedad
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
void FirstScene::moverEnemigo1(){
    for (auto& enemigo : enemigos1) {
        // Comprobamos si hay colisión con el escenario o algún obstáculo
        if (colisionEnemigos1(enemigo)) {
            // Si hay colisión, cambiamos la dirección del enemigo
            if(enemigo->getCurrentDirection()==derecha){
                //enemigo->setLastDirection(derecha);
                enemigo->setX(enemigo->getX()-6);
                enemigo->setPos(enemigo->getX(),enemigo->getY());

            }else if(enemigo->getCurrentDirection()==izquierda){
               // enemigo->setLastDirection(izquierda);
                enemigo->setX(enemigo->getX()+6);
                enemigo->setPos(enemigo->getX(),enemigo->getY());
            }
            else if(enemigo->getCurrentDirection()==deteccion){
                if(enemigo->getLastDirection()==izquierda){
                    enemigo->setX(enemigo->getX()+10);
                    enemigo->setPos(enemigo->getX(),enemigo->getY());

                }else if(enemigo->getLastDirection()==derecha){
                    enemigo->setX(enemigo->getX()-10);
                    enemigo->setPos(enemigo->getX(),enemigo->getY());

                }
            }
            enemigo->cambiarDireccion();

        } else if(enemigo->getX()<2){
            enemigo->setX(enemigo->getX()+2);
            enemigo->setPos(enemigo->getX(),enemigo->getY());
            enemigo->cambiarDireccion();
        }else if(enemigo->getX()>3530){
            enemigo->setX(enemigo->getX()-2);
            enemigo->setPos(enemigo->getX(),enemigo->getY());
            enemigo->cambiarDireccion();

        }
        else {
            // Si no hay colisión, movemos al enemigo en la dirección actual
            enemigo->moverJugador(enemigo->getCurrentDirection());
        }
        if (enemigo->enCooldown()) {
            if (enemigo->getLastDirection() == derecha) {
                enemigo->setX(enemigo->getX() - 1); // Retrocede a la izquierda
            } else {
                enemigo->setX(enemigo->getX() + 1); // Retrocede a la derecha
            }
            enemigo->setPos(enemigo->getX(), enemigo->getY());
        }else if (enemigo->finalizoCooldown()) {
            // Avanzar en dirección contraria al retroceso
            if (enemigo->getLastDirection() == izquierda) {
                enemigo->setCurrentDirection(derecha);
            } else {
                enemigo->setCurrentDirection(izquierda);                // Ahora avanza a la derecha
            }
        }
    }
}
bool FirstScene::colisionEnemigos1(enemigol1* enemigo1_){
    for (auto *caja:cajas) {
        if (enemigo1_->collidesWithItem(caja)) {
            return true; // Si hay colisión con alguna caja
        }
}
    return false;
}
bool FirstScene::colisionProyectil1(proyectil* proyectil_){
    for (auto *caja:cajas) {
        if (proyectil_->collidesWithItem(caja)) {
            return true; // Si hay colisión con alguna caja
        }
    }
    return false;
}
bool FirstScene::colisionbalaJ(proyectil* proyectil_){
    if(proyectil_){
        if(jugador->collidesWithItem(proyectil_)){
            return true;
        }
    }
    return false;
}
bool FirstScene::colisionbalaE(proyectil *proyectil_){
    for (auto it = enemigos1.begin(); it != enemigos1.end(); ) {
        auto enemigo = *it;

        // Verificar colisión
        if (proyectil_->collidesWithItem(enemigo)) {
            // Reducir vida
            if (enemigo->getVida() > 0) {
                enemigo->setVida(enemigo->getVida() - golpeDaño);
            } else {
                // Eliminar enemigo de la escena y liberar memoria
                removeItem(enemigo);
                delete enemigo;

                // Eliminar enemigo del vector
                it = enemigos1.erase(it);
                continue; // Saltar a la siguiente iteración
            }

            return true; // Hubo colisión
        }

        ++it; // Avanzar al siguiente enemigo
    }

    return false; // No hubo colisión
}
void FirstScene::detecionEnemigos1(){

    for (auto& enemigo : enemigos1) {
        // Calcular distancias entre el jugador y el enemigo
        int distanciaX = std::abs(jugador->getX() - enemigo->getX());
        int distanciaY = std::abs(jugador->getY() - enemigo->getY());

        // Verificar si está dentro del rango de proximidad
        if (distanciaX <= 400 && distanciaY <= 10) {
            if(!enemigo->getCurrentDirection()==deteccion){
                enemigo->setLastDirection(enemigo->getCurrentDirection());
            }
            enemigo->setCurrentDirection(deteccion);
        }
        if (distanciaX <= 200 && distanciaY <= 10 && !colisionEnemigos1(enemigo)) {
            qDebug() << "nivel seleccionado:" << distanciaX;

            // Disparar solo si el enemigo está listo
            if (enemigo->puedeDisparar()) {
                // Determinar dirección del disparo
                if (jugador->getX() > enemigo->getX()) { // Jugador a la derecha
                    enemigo->setLastDirection(izquierda);
                } else { // Jugador a la izquierda
                    enemigo->setLastDirection(derecha);
                }

                // Iniciar disparo y animación
                enemigo->setCurrentDirection(golpe);
                enemigo->startGolpeAnimacion();

                proyectil* nuevoProyectil = new proyectil();
                nuevoProyectil->setTipo(false);
                nuevoProyectil->dibujarProyectil();
                if(jugador->getX() < enemigo->getX()){
                    nuevoProyectil->setDireccion(false);
                }
                nuevoProyectil->setPos(enemigo->getX(), enemigo->getY()+30);
                addItem(nuevoProyectil);
                enemigo->setProyectilActivo(nuevoProyectil); // Asignar proyectil al enemigo

                // Timer para mover el proyectil
                QTimer* timerProyectil = new QTimer(this);
                connect(timerProyectil, &QTimer::timeout, [nuevoProyectil, enemigo, timerProyectil, this]() {
                    nuevoProyectil->actualizarMovimiento();

                    // Verificar colisiones con plataformas
                    if (choquePlataforma(nuevoProyectil)) {
                        nuevoProyectil->cambiarDireccion();
                    }


                    // Verificar si el proyectil sale de los límites
                    if (nuevoProyectil->y() > 768 || nuevoProyectil->x() > 3532 || nuevoProyectil->x() < 0 || colisionProyectil1(nuevoProyectil)) {
                        removeItem(nuevoProyectil);
                        delete nuevoProyectil;
                        enemigo->eliminarProyectil();
                        timerProyectil->stop();
                        timerProyectil->deleteLater(); // Eliminar el timer
                    }
                    else if(colisionbalaJ(nuevoProyectil)){
                        if(jugador->getVida()>0){
                            jugador->setVida(jugador->getVida()-20);

                        }else{
                            qDebug() << "se nos acabo la vida";
                            resetGame();
                        }
                        removeItem(nuevoProyectil);
                        delete nuevoProyectil;
                        enemigo->eliminarProyectil();
                        timerProyectil->stop();
                        timerProyectil->deleteLater();
                    }
                });

                timerProyectil->start(30);

                // Retrasar el próximo disparo
                enemigo->iniciarCooldownDisparo();
            }
        }else{
            if(enemigo->getCurrentDirection()==golpe){
                enemigo->setCurrentDirection(deteccion);
            }
        }
    }

}
void FirstScene::dispararE1_(){

}
void FirstScene::moverItemsF(){
    if(llave){
        QPointF posActual = llave->pos();

        // Oscilar en un rango de 10 píxeles (5 arriba y 5 abajo)
        if (pixmanDirection == 1 && posActual.y() == 510){
            pixmanDirection = -1; // Cambiar dirección hacia abajo
        } else if (pixmanDirection == -1 && posActual.y() == 490) {
            pixmanDirection = 1; // Cambiar dirección hacia arriba
        }

        // Actualizar la posición vertical
        llave->setY(posActual.y() + pixmanDirection);

    }
    QPointF posActual = candado->pos();

    // Oscilar en un rango de 10 píxeles (5 arriba y 5 abajo)
    if (pixmanDirection2 == 1 && posActual.y() ==310){
        pixmanDirection2 = -1; // Cambiar dirección hacia abajo
    } else if (pixmanDirection2 == -1 && posActual.y() == 290) {
        pixmanDirection2 = 1; // Cambiar dirección hacia arriba
    }
    // Actualizar la posición vertical
    candado->setY(posActual.y() + pixmanDirection2);

    colisionItems();

}
void FirstScene::colisionItems(){
            for (auto it = galletas.begin(); it != galletas.end(); ) {
                if (jugador->collidesWithItem(*it)) {
                    qDebug() << "¡Pixman recogido!";
                    removeItem(*it);      // Eliminar Pixman de la escena
                    delete *it;
                    *it=nullptr;                    // Liberar memoria
                    it = galletas.erase(it); // Eliminar del vector y ajustar el iterador
                    golpeDaño += 20;
                    return;                 // Incrementar el daño
                } else {
                    ++it; // Avanzar al siguiente elemento si no hay colisión
                }
            }
            if(llave){
                if (jugador->collidesWithItem(llave)) {
                    removeItem(llave); // Eliminar Pixman de la escena
                    delete llave; // Liberar memoria
                    llave=nullptr;
                    //con llave v activa podemos pasar al siguiente nivel
                    llaveV=true;
                    // pixmanRecogido = true; // Marca como recogido
                }

            }
}
void FirstScene::resetGame(){

}
void FirstScene::pasarNivel(){
    if(jugador->collidesWithItem(candado)&& llaveV){
        qDebug() << "¡pasamos al siguiente nivel!";
        //puedo pasar la siguiente nivel
    }

}
