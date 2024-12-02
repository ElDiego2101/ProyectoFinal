#include "twoscene.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>
#include <QString>

Bala::Bala(bool especial, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), especial(especial), angulo(0), velocidad(0), gravedad(5.0)
{
    setPixmap(QPixmap(":/imagenes/disparo2.png"));
    setScale(3);
    if (especial) {
        angulo = QRandomGenerator::global()->bounded(360);
        velocidad = 5;
    }
}

void Bala::advance(int phase)
{
    if (!phase) return;

    if (especial) {
        setRotation(rotation() + 10);
        setPos(x() + velocidad * qCos(qDegreesToRadians(angulo)), y() + velocidad * qSin(qDegreesToRadians(angulo)));
    } else {
        setPos(x(), y() + gravedad);
    }


}

bool Bala::esEspecial() const
{
    return especial;
}

Potenciador::Potenciador(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/imagenes/pot.png"));
}

twoscene::twoscene(MainWindow *parent)
    : QGraphicsScene(0, 40, 1920, 1080, parent), mainWindow(parent), vidas(3), puntuacion(0), juegoPausado(false), tiempoTranscurrido(0), potenciadorActivo(false)
{
    setBackgroundBrush(QBrush(QPixmap(":/imagenes/escenario_3.png")));

    carro = new bart();
    carro->setPos(960, 950);
    addItem(carro);

    temporizadorBala = new QTimer(this);
    connect(temporizadorBala, &QTimer::timeout, this, &twoscene::generarBala);
    temporizadorBala->start(QRandomGenerator::global()->bounded(2000, 4000));

    temporizadorBalaEspecial = new QTimer(this);
    connect(temporizadorBalaEspecial, &QTimer::timeout, this, &twoscene::generarBalaEspecial);
    temporizadorBalaEspecial->start(20000);

    temporizadorPotenciador = new QTimer(this);
    connect(temporizadorPotenciador, &QTimer::timeout, this, &twoscene::generarPotenciador);
    temporizadorPotenciador->start(10000);

    temporizadorJuego = new QTimer(this);
    connect(temporizadorJuego, &QTimer::timeout, this, &twoscene::actualizarJuego);
    temporizadorJuego->start(1000 / 60);

    QTimer::singleShot(60000, [this]() {
        if(derrota==false){
            bloquearMovimientoYMostrarBalaFinal();
        }
    });


    temporizadorEliminarPotenciador = new QTimer(this);
    connect(temporizadorEliminarPotenciador, &QTimer::timeout, this, &twoscene::eliminarPotenciador);

    temporizadorTerminarPotenciador = new QTimer(this);
    connect(temporizadorTerminarPotenciador, &QTimer::timeout, this, &twoscene::terminarPotenciador);
}

void twoscene::keyPressEvent(QKeyEvent *event)
{
    if(bloq_mov) return;
    if (juegoPausado) return;
    keysPressed.insert(event->key());
    if (event->key() == Qt::Key_A) {
        carro->establecerDireccion(potenciadorActivo ? "izquierda_p" : "izquierda");
    } else if (event->key() == Qt::Key_D) {
        carro->establecerDireccion(potenciadorActivo ? "derecha_p" : "derecha");
    }
    if (keysPressed.contains(Qt::Key_P)) {
        juegoPausado = true;
        final = true;
        PMenu* menu = new PMenu("Pausa", 3, true);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        connect(menu, &PMenu::resume, this, [this]() {
            juegoPausado = false;
            final = false;
        });
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
    }
}

void twoscene::keyReleaseEvent(QKeyEvent *event)
{
    if(juegoPausado) return;
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        carro->establecerDireccion("");
    }
    if (keysPressed.contains(Qt::Key_P)) {
        keysPressed.remove(event->key());
    }
}


void twoscene::generarBala()
{
    if (juegoPausado) return;

    Bala *bala = new Bala(false);
    bala->setPos(QRandomGenerator::global()->bounded(0, 1850), -20);
    addItem(bala);
    qDebug() << "Bala normal creada en posición" << bala->pos();

    QTimer::singleShot(QRandomGenerator::global()->bounded(2000, 4000), this, &twoscene::generarBala);
}

void twoscene::generarBalaEspecial()
{
    if (juegoPausado) return;

    Bala *bala = new Bala(true);
    bala->setPos(QRandomGenerator::global()->bounded(0, 1850), -20);
    addItem(bala);
    qDebug() << "Bala especial creada en posición" << bala->pos();

    QTimer::singleShot(QRandomGenerator::global()->bounded(2000, 4000), this, &twoscene::generarBalaEspecial);
}

void twoscene::generarPotenciador()
{
    if (juegoPausado || potenciadorActivo) return;

    Potenciador *potenciador = new Potenciador();
    potenciador->setPos(QRandomGenerator::global()->bounded(0, 1850), 950);
    potenciador->setScale(0.2);
    addItem(potenciador);
    qDebug() << "Potenciador creado en posición" << potenciador->pos();

    temporizadorEliminarPotenciador->start(3000);
}

void twoscene::eliminarPotenciador()
{
    foreach (QGraphicsItem *item, items()) {
        if (Potenciador *potenciador = dynamic_cast<Potenciador *>(item)) {
            removeItem(potenciador);
            delete potenciador;
            qDebug() << "Potenciador eliminado";
        }
    }
    temporizadorEliminarPotenciador->stop();
    temporizadorPotenciador->start(5000);
}

void twoscene::terminarPotenciador()
{
    carro->establecerVelocidad(5);
    carro->reiniciarImagen();
    potenciadorActivo = false;
    temporizadorPotenciador->start(5000);
    temporizadorTerminarPotenciador->stop();
}

void twoscene::actualizarJuego()
{
    if (juegoPausado) return;

    tiempoTranscurrido++;

    comprobarColisiones();
    carro->actualizarPosicion();

    for (QGraphicsItem *item : items()) {
        if (Bala *bala = dynamic_cast<Bala *>(item)) {
            bala->advance(1);
        }
    }

    if (potenciadorActivo && carro->obtenerTiempoPotenciador() <= 0) {
        terminarPotenciador();
    }
}

void twoscene::bloquearMovimientoYMostrarBalaFinal()
{
    if (final) return;

    foreach (QGraphicsItem *item, items()) {
        if (Bala *bala = dynamic_cast<Bala *>(item)) {
            removeItem(bala);
            delete bala;
        }
    }
    bloq_mov=true;
    carro->establecerVelocidad(0);
    carro->reiniciarImagen();
    potenciadorActivo = true;
    temporizadorPotenciador->stop();
    temporizadorEliminarPotenciador->stop();
    temporizadorTerminarPotenciador->stop();
    balaFinal = new Bala(false);
    balaFinal->setPixmap(QPixmap(":/imagenes/disparo2.png"));
    balaFinal->setScale(10);
    balaFinal->setPos(carro->x() + 15, -200);
    addItem(balaFinal);


}


void twoscene::comprobarColisiones()
{
    QList<QGraphicsItem *> elementosColisionados = carro->collidingItems();
    for (QGraphicsItem *item : elementosColisionados) {
        if (item == balaFinal) {
            juegoPausado = true;
            QString puntuacionn = QString::number(puntuacion);
            PMenu* menu = new PMenu("   GANASTE \n      Puntos: \n             " + puntuacionn, 2);
            connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
            connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
            menu->setParent(mainWindow);
            menu->move(672, 100);
            menu->show();
            qDebug() << "Juego pausado después de 60 segundos y golpe de bala";
            removeItem(balaFinal);
            delete balaFinal;
            final = true;
        }
        else if (Bala *bala = dynamic_cast<Bala *>(item)) {
            if (!juegoPausado) {
                if (!potenciadorActivo && vidas > 0) {
                    vidas--;
                    puntuacion-=10;
                    if(puntuacion<0){
                        puntuacion=0;
                    }
                    qDebug() << "Vida disminuida. Vida actual:" << vidas;
                    if (vidas == 0) {
                        juegoPausado = true;
                        QString puntuacionn = QString::number(puntuacion);
                        PMenu* menu = new PMenu("   Perdiste \n      Puntos: \n           " + puntuacionn, 3);
                        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
                        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
                        menu->setParent(mainWindow);
                        menu->move(672, 100);
                        menu->show();
                        derrota=true;
                        qDebug() << "Juego pausado porque la vida llegó a 0";
                        final=true;
                    }
                }
                removeItem(bala);
                delete bala;
            }
        } else if (Potenciador *potenciador = dynamic_cast<Potenciador *>(item)) {
            carro->establecerVelocidad(10);
            carro->establecerTiempoPotenciador(300);
            potenciadorActivo = true;
            puntuacion += 15;
            qDebug() << "Potenciador recogido. Puntuación actual:" << puntuacion;
            removeItem(potenciador);
            delete potenciador;
            carro->setPixmap(QPixmap(":/imagenes/carroDER2p"));
            temporizadorEliminarPotenciador->stop();
            temporizadorPotenciador->stop();
            temporizadorTerminarPotenciador->start(5000);
        }
    }

    foreach (QGraphicsItem *item, items()) {
        if (Bala *bala = dynamic_cast<Bala *>(item)) {
            if (bala->y() > 900) {
                puntuacion += 5;
                qDebug() << "Bala eliminada al tocar el suelo. Puntuación actual:" << puntuacion;
                removeItem(bala);
                delete bala;
            }
        }
    }
}

