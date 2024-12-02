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
    fondo->setPos(0, 0);
    addItem(fondo);
    jugar=true;
    posicionInicialGuardada = false;

    proyectilActivo = nullptr;
    timerMovimientoProyectil=new QTimer(this);
    connect(timerMovimientoProyectil, &QTimer::timeout, this, [this]() {
        if (proyectilActivo) {
            proyectilActivo->actualizarMovimiento();

            if (choquePlataforma(proyectilActivo)) {
                proyectilActivo->cambiarDireccion();
            }

            if (proyectilActivo->y() > 768 || proyectilActivo->x() > 3532 || proyectilActivo->x() < 0 || colisionProyectil1(proyectilActivo)||colisionbalaE(proyectilActivo)) {
                timerMovimientoProyectil->stop();
                removeItem(proyectilActivo);
                delete proyectilActivo;
                proyectilActivo = nullptr;
            }
        }
    });
    timerFondo = new QTimer(this);
    connect(timerFondo, &QTimer::timeout, this, &FirstScene::moverFondo);
    timerFondo->start(30);

    timerGravedad = new QTimer(this);
    connect(timerGravedad, &QTimer::timeout, this, &FirstScene::aplicarGravedad);
    timerGravedad->start(30);
    golpeDaño=30;
    cantidadEnemigos=10;
    galletas.resize(3);
    establecerCajas();
    establecerPlataformas();
    establecerEnemigos1();
    establecerGalletas();

    jugador = new bob();
    addItem(jugador);
    jugador->setZValue(0);
    jugador->setPos(0, 600);

    //salto
    enSalto = false;
    enBajo=false;
    velocidadSalto =0;
    gravedad = 2;
    veri=true;
    colisionDerecha=false;
    colisionIzquierda=false;

    //inicializar el movimiento de los enemigos1
    movimientoEnemigo1 = new QTimer(this);
    connect(movimientoEnemigo1, &QTimer::timeout, this, &FirstScene::moverEnemigo1);
    movimientoEnemigo1->start(50);

    //deteccion de bob por parte de los enemigos 1
    deteccionE1=new QTimer(this);
    connect(deteccionE1,&QTimer::timeout,this,&FirstScene::detecionEnemigos1);
    deteccionE1->start(110);

    //disparo del primer enemigo
    dispararE1=new QTimer(this);
    connect(dispararE1,&QTimer::timeout,this,&FirstScene::dispararE1_);
    dispararE1->start(60);

    //mover items candado llave
    moverItems=new QTimer(this);
    connect(moverItems,&QTimer::timeout,this,&FirstScene::moverItemsF);
    moverItems->start(50);

    //verificar si pasamos el nivel
    passlevel=new QTimer(this);
    connect(passlevel,&QTimer::timeout,this,&FirstScene::pasarNivel);
    passlevel->start(50);

    //crear llave a la escena
    llave = new QGraphicsPixmapItem(QPixmap(":/imagenes/llave.png"));

    llave->setPos(1730, 500);
    pixmanDirection=1;
    pixmanDirection2=1;
    // Agregar Pixman a la escena
    addItem(llave);
    llaveV=false;

    candado=new QGraphicsPixmapItem(QPixmap(":/imagenes/candado.png"));
    candado->setPos(3280,300);
    addItem(candado);
}
bool FirstScene::puedeBajar() {
    QRectF rectPies = jugador->getPies()->sceneBoundingRect();
    qreal margenBusqueda = 15;

    for (auto *plataforma : plataformas) {
        QRectF rectPlataforma = plataforma->sceneBoundingRect();

        if (rectPlataforma.top() > rectPies.bottom() &&
            rectPlataforma.top() >= rectPies.bottom() + margenBusqueda) {
            return true;
        }
    }

    return false;
}
void FirstScene::moverFondo() {
    if(jugar) {
        if (velocidadFondo == 0) return;


        vistas = this->views();
        if (vistas.isEmpty()) return;
        vista = vistas.first();
        if (!vista) return;

        rectVista = vista->sceneRect();
        limiteIzquierda = rectVista.left()-10;
        limiteDerecha = rectVista.right()+10;

        if (!posicionInicialGuardada) {
            posicionInicialVista = vista->sceneRect().topLeft();
            posicionInicialGuardada = true;
        }
        if (jugador->getX() <= limiteIzquierda) {
            velocidadFondo = -10;
        } else if (jugador->getX() >= limiteDerecha) {
            velocidadFondo = +10;
        }

        nuevaPosicion = vista->sceneRect().topLeft() + QPointF(velocidadFondo, 0);

        if (nuevaPosicion.x() > 2168) {
            nuevaPosicion.setX(2168); // Limitar el movimiento hacia la derecha
        } else if (nuevaPosicion.x() < 0) {
            nuevaPosicion.setX(0); // Limitar el movimiento hacia la izquierda
        }
        vista->setSceneRect(QRectF(nuevaPosicion, vista->sceneRect().size()));
    }
}

void FirstScene::keyPressEvent(QKeyEvent *event) {
    if(jugar){
        teclasPresionadas.insert(event->key());
        int velocidad = 4; // Velocidad del desplazamiento
        int velocidadp = -4; // Velocidad negativa para mover a la izquierda
        int posX=jugador->getX();
        int posY=jugador->getY();

        if (event->key() == Qt::Key_A) {
            if(veri){
                velocidadFondo=0;
            }else{
                velocidadFondo = velocidadp;
            }
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
            enSalto = true;
            velocidadSalto = -28;
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

        switch (event->key()) {
        case Qt::Key_D:{
            if (!colisionDerecha) {
                jugador->moverJugador(derecha);
                jugador->setX(jugador->getX() + 6);
                veri = false;
                if (!sobrePlataforma()) {
                    jugador->setX(posX);
                    jugador->detenerJugador();
                    velocidadFondo = 0;
                }
                if(!colisionIzquierda){
                    if(colisionCaja()){
                        jugador->setX(posX);
                        jugador->detenerJugador();
                        velocidadFondo = 0;
                        colisionDerecha = true;
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
            if (!colisionIzquierda) {
                jugador->moverJugador(izquierda);
                jugador->setX(jugador->getX() - 6);
                veri = false;
                if (!sobrePlataforma()) {
                    jugador->setX(posX);
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

            break;
        case Qt::Key_S:{
            jugador->moverJugador(abajo);

            break;
        }
        case Qt::Key_Z:{
            jugador->moverJugador(golpe);
        }
        }

        if ((jugador->getX()<=limiteIzquierda) ||(jugador->getX()>=limiteDerecha)||colisionCaja()||jugador->getX()>3400) {
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

                timerMovimientoProyectil->start(30);
            }
            // Ajusta el tiempo de actualización
        }

    }

}
void FirstScene::keyReleaseEvent(QKeyEvent *event) {

    teclasPresionadas.remove(event->key());

    // Verificar qué tecla fue soltada y detener al jugador
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        velocidadFondo = 0; // Detener el fondo
    }

}
void FirstScene::establecerPlataformas(){
    int posicion=0;
    for (auto i = 0; i < plataformas.size(); ++i) {
        QGraphicsRectItem *plataforma = new QGraphicsRectItem(0,390+posicion, 3532, 10);
        plataforma->setBrush(Qt::darkGray);
        addItem(plataforma);
        plataformas[i] = plataforma;
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
    int posicionesX[] = {20, 2100, 3300};
    int posicionY[] ={348,544,700};

    for (int var = 0; var < 3; var++) {
        QGraphicsPixmapItem* galleta = new QGraphicsPixmapItem(QPixmap(":/imagenes/galleta.png"));
        addItem(galleta);
        galleta->setZValue(0);
        galleta->setPos(posicionesX[var], posicionY[var]);

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
    delete llave;
    for (auto plataforma : plataformas) {
        delete plataforma;
    }
    for (auto caja :cajas) {
        delete caja;
    }
    for (auto& enemigo : enemigos1) {
        delete enemigo;
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
            return true;
        }
    }
    return false;
}

void FirstScene::aplicarGravedad() {

    if (enSalto) {
        jugador->setY(jugador->getY() + velocidadSalto);
        velocidadSalto += gravedad;

        jugador->setPos(jugador->getX(), jugador->getY());

        if (sobrePlataforma()) {
            enSalto = false;
            jugador->setY(jugador->getY());
            jugador->setPos(jugador->getX(), jugador->getY());// Ajusta la posición exacta
            velocidadSalto = 0;
        }
    }else if(enBajo && puedeBajar()){
        jugador->setY(jugador->getY() + velocidadSalto); // Mover según la velocidad
        velocidadSalto += gravedad;
        jugador->setPos(jugador->getX(), jugador->getY());
        if (sobrePlataforma()) {
            enBajo = false;
            jugador->setY(jugador->getY());
            jugador->setPos(jugador->getX(), jugador->getY());
            velocidadSalto = 0;
        }

    }
    else if (!sobrePlataforma()) {
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
        if (colisionEnemigos1(enemigo)) {
            if(enemigo->getCurrentDirection()==derecha){
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
            enemigo->moverJugador(enemigo->getCurrentDirection());
        }
        if (enemigo->enCooldown()) {
            if (enemigo->getLastDirection() == derecha) {
                enemigo->setX(enemigo->getX() - 1);
            } else {
                enemigo->setX(enemigo->getX() + 1);
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

        if (proyectil_->collidesWithItem(enemigo)) {
            if (enemigo->getVida() > 0) {
                enemigo->setVida(enemigo->getVida() - golpeDaño);
            } else {
                removeItem(enemigo);
                delete enemigo;

                // Eliminar enemigo del vector
                it = enemigos1.erase(it);
                continue;
            }

            return true;
        }

        ++it;
    }

    return false; // No hubo colisión
}
void FirstScene::detecionEnemigos1(){

    if(jugar){    for (auto& enemigo : enemigos1) {

            int distanciaX = std::abs(jugador->getX() - enemigo->getX());
            int distanciaY = std::abs(jugador->getY() - enemigo->getY());

            if (distanciaX <= 400 && distanciaY <= 10) {
                if(!enemigo->getCurrentDirection()==deteccion){
                    enemigo->setLastDirection(enemigo->getCurrentDirection());
                }
                enemigo->setCurrentDirection(deteccion);
            }
            if (distanciaX <= 200 && distanciaY <= 10 && !colisionEnemigos1(enemigo)) {
                qDebug() << "nivel seleccionado:" << distanciaX;


                if (enemigo->puedeDisparar()) {
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

                    if(jugar){
                        timerProyectil->start(30);

                    }else if(!jugar && timerProyectil->isActive()){
                        timerProyectil->stop();
                    }

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


}
void FirstScene::dispararE1_(){

}
void FirstScene::moverItemsF(){
    if(llave){
        QPointF posActual = llave->pos();


        if (pixmanDirection == 1 && posActual.y() == 510){
            pixmanDirection = -1;
        } else if (pixmanDirection == -1 && posActual.y() == 490) {
            pixmanDirection = 1;
        }

        // Actualizar la posición vertical
        llave->setY(posActual.y() + pixmanDirection);

    }
    QPointF posActual = candado->pos();


    if (pixmanDirection2 == 1 && posActual.y() ==310){
        pixmanDirection2 = -1;
    } else if (pixmanDirection2 == -1 && posActual.y() == 290) {
        pixmanDirection2 = 1;
    }

    candado->setY(posActual.y() + pixmanDirection2);

    colisionItems();

}
void FirstScene::colisionItems(){
    for (auto it = galletas.begin(); it != galletas.end(); ) {
        if (jugador->collidesWithItem(*it)) {
            qDebug() << "¡Pixman recogido!";
            removeItem(*it);
            delete *it;
            *it=nullptr;
            it = galletas.erase(it); // Eliminar del vector y ajustar el iterador
            golpeDaño += 20;
            return;
        } else {
            ++it;
        }
    }
    if(llave){
        if (jugador->collidesWithItem(llave)) {
            removeItem(llave);
            delete llave;
            llave=nullptr;
            llaveV=true;
        }

    }
}
void FirstScene::resetGame(){
        if (timerMovimientoProyectil && timerMovimientoProyectil->isActive()) {
            timerMovimientoProyectil->stop();
        }
        if (timerFondo && timerFondo->isActive()) {
            timerFondo->stop();
        }
        if (timerGravedad && timerGravedad->isActive()) {
            timerGravedad->stop();
        }
        if (movimientoEnemigo1 && movimientoEnemigo1->isActive()) {
            movimientoEnemigo1->stop();
        }
        if (deteccionE1 && deteccionE1->isActive()) {
            deteccionE1->stop();
        }
        if (dispararE1 && dispararE1->isActive()) {
            dispararE1->stop();
        }
        if (moverItems && moverItems->isActive()) {
            moverItems->stop();
        }
        if (passlevel && passlevel->isActive()) {
            passlevel->stop();
        }
        //vistas.clear();
        qDebug() << "perdimo";
        juegoPausado = true;
        jugar=false;
        vista->setSceneRect(QRectF(posicionInicialVista, vista->sceneRect().size()));
        PMenu* menu = new PMenu(" perdiste \n", 1);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
        qDebug() << "Juego pausado porque la vida llegó a 0";

}
void FirstScene::pasarNivel(){
    if(jugador->collidesWithItem(candado)&& llaveV){
        if (timerMovimientoProyectil && timerMovimientoProyectil->isActive()) {
            timerMovimientoProyectil->stop();
        }
        if (timerFondo && timerFondo->isActive()) {
            timerFondo->stop();
        }
        if (timerGravedad && timerGravedad->isActive()) {
            timerGravedad->stop();
        }
        if (movimientoEnemigo1 && movimientoEnemigo1->isActive()) {
            movimientoEnemigo1->stop();
        }
        if (deteccionE1 && deteccionE1->isActive()) {
            deteccionE1->stop();
        }
        if (dispararE1 && dispararE1->isActive()) {
            dispararE1->stop();
        }
        if (moverItems && moverItems->isActive()) {
            moverItems->stop();
        }
        if (passlevel && passlevel->isActive()) {
            passlevel->stop();
        }
        //vistas.clear();
        qDebug() << "¡pasamos al siguiente nivel!";
        juegoPausado = true;
        jugar=false;
        vista->setSceneRect(QRectF(posicionInicialVista, vista->sceneRect().size()));
        //QString puntuacionn = QString::number(puntuacion);
        PMenu* menu = new PMenu(" ganaste \n", 1);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
        //derrota=true;
        qDebug() << "Juego pausado porque la vida llegó a 0";
        //final=true;
        //puedo pasar la siguiente nivel
    }

}

