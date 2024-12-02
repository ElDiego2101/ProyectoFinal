#include "personaje.h"
#include <QDebug>
#include "PMenu.h"

Personaje::Personaje(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    initializePlayer();


    timer = new QTimer(this);
    timer->start(80);
    connect(timer, &QTimer::timeout, this, &Personaje::runPlayer);

    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Personaje::jumpPlayer);

    movementTimer = new QTimer(this);
    movementTimer->start(30);
    connect(movementTimer, &QTimer::timeout, this, &Personaje::updateMovement);

}



void Personaje::initializePlayer()
{
    QPixmap pixMap(":/imagenes/jugador1.png");
    setPixmap(pixMap);
    setScale(0.5);
    setPos(0, 850);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Personaje::runPlayer()
{
    cont += 1;
    if (cont > 8) {
        cont = 1;
    }
}

void Personaje::jumpPlayer()
{
    QPixmap jump1(":/imagenes/salto1.png");
    QPixmap jump2(":/imagenes/salto2.png");
    QPixmap jump3(":/imagenes/salto3.png");
    QPixmap base(":/imagenes/jugador1.png");
    if(jugar_nivel == true){
        if (collidesWithItem(parentItem()) && pos().y() < 850) {
            setPixmap(base);
            setPos(lugar_saltoX, lugar_saltoY);
            timer2->stop();
            return;
        }

        if (subir) {
            setPixmap(jump1);
            if (pos().y() <= 850) {
                setPos(pos().x() + 7, pos().y() - 9);
                posX += 7;
                distancia += 5;
                if (distancia == 90) {
                    subir = false;
                }
            }
        } else{
            if(colision == false){
                setPixmap(jump2);
                setPos(pos().x() + 7, pos().y() + 9);
                posX += 7;
                distancia -= 5;
                if (distancia == 0) {
                    setPixmap(jump3);
                    subir = true;
                    salto = false;
                    timer2->stop();
                }
            }
            else{
                qDebug()<<pos().y();
                setPixmap(base);
                timer2-> stop();
                //subir = true;
            }
        }
        if(pos().x()>850 && colision == false && posX <8000){
            emit moveBackground(7);
            inicio = true;
        }
        if(posX >9000){
            jugar_nivel = false;
            win = true;
            qDebug()<<"terminarGanaste";

        }

    }
    else{
        timer2->stop();
    }
}

void Personaje::keyPressEvent(QKeyEvent *event)
{
    keysPressed.insert(event->key());
    game = true;
    if (keysPressed.contains(Qt::Key_W) || (keysPressed.contains(Qt::Key_W) && keysPressed.contains(Qt::Key_D))) {
        if(jugar_nivel == true){
            lugar_saltoX = pos().x();
            salto = true;
            subir = true;
            lugar_saltoY = pos().y();
            if (!timer2->isActive()) {
                timer2->start(18);
            }
        }
        else{
            timer2->stop();
        }
    }

}

void Personaje::keyReleaseEvent(QKeyEvent *event)
{
    keysPressed.remove(event->key());
    game = false;
    if (salto) {
        QPixmap pixMap(":/imagenes/jugador1.png");
        setPixmap(pixMap);
    }
}

void Personaje::updateMovement()
{
    QPixmap play_run1(":/imagenes/jugador2.png");
    QPixmap play_run2(":/imagenes/jugador3.png");
    QPixmap play_run3(":/imagenes/jugador4.png");
    QPixmap play_run4(":/imagenes/jugador5.png");
    QPixmap play_run5(":/imagenes/jugador6.png");
    QPixmap play_run6(":/imagenes/jugador7.png");
    QPixmap play_run7(":/imagenes/jugador8.png");
    QPixmap play_run8(":/imagenes/jugador9.png");
    if(!salto){
        if (keysPressed.contains(Qt::Key_D) && colision == false && jugar_nivel == true) {
            game = true;
            QPixmap pixmaps[8] = {play_run1, play_run2, play_run3, play_run4, play_run5, play_run6, play_run7, play_run8};
            setPixmap(pixmaps[cont - 1]);
            setPos(pos().x() + 10, pos().y());
            posX += 10;
            if(pos().x()>850 && posX<8000){
                inicio = true;
                emit moveBackground(10);
            }

            if(posX>9000){
                jugar_nivel = false;
                win = true;
                qDebug()<<"terminaar"<<" ganaste";
            }
        }
        if(jugar_nivel == false){
            timer->stop();
        }
        if(colision == true){
            setPos(pos().x() - 20, pos().y());
        }
    }
}
