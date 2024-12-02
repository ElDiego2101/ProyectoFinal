#include "tscene.h"
#include "PMenu.h"
#include <QDebug>
#include <cmath>
#include <QKeyEvent>
#include "mainwindow.h"

TScene::TScene(MainWindow *parent) : mainWindow(parent) {
    initializeScene();
    QImage imagen_fondo(":/imagenes/escenario.png");
    QBrush fondo_escenario(imagen_fondo);
    setBackgroundBrush(fondo_escenario);

    connect(personaje1, &Personaje::moveBackground, this, &TScene::onMoveBackground);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TScene::crear_obs);
    timer->start(1000);

    obs_timer = new QTimer(this);
    connect(obs_timer, &QTimer::timeout, this, &TScene::actualizar_obstaculos);
    obs_timer->start(50);

    coli = new QTimer(this);
    connect(coli, &QTimer::timeout, this, &TScene::verificar_colision);
    coli->start(10);

    Police = new QTimer(this);
    connect(Police, &QTimer::timeout, this, &TScene::runPolice);
    qDebug() << "inicio   " << personaje1->posX;
    QPixmap pol1(":/imagenes/tanque.png");
    policia = new QGraphicsPixmapItem();
    addItem(policia);
    policia->setPixmap(pol1);
    policia->setScale(0.7);
    policia->setPos(-300, 860);
    Police->start(60);

    fire = new QTimer(this);
    connect(fire, &QTimer::timeout, this, &TScene::disparo);
    fire->start(25);

    vidaBar = new QProgressBar();
    vidaBar->setRange(0, 5);
    vidaBar->setValue(5);
    vidaBar->setFormat("");
    vidaBar->setGeometry(QRect(1600, 10, 300, 30));
    vidaBar->setParent(mainWindow);
    vidaBar->setStyleSheet("QProgressBar {"
                           "border: 2px solid grey; "
                           "border-radius: 10px; "
                           "background-color: red; "
                           "}"
                           "QProgressBar::chunk {"
                           "background-color: green; "
                           "border-radius: 8px; "
                           "}");
    vidaBar->show();

    progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setGeometry(QRect(860, 10, 200, 30));
    progressBar->setParent(mainWindow);
    progressBar->setStyleSheet("QProgressBar {"
                               "border: 2px solid grey; "
                               "border-radius: 10px; "
                               "background-color: #E0E0E0; "
                               "}"
                               "QProgressBar::chunk {"
                               "background-color: blue; "
                               "border-radius: 8px; "
                               "}");

    progressBar->setFormat("");
    progressBar->show();

    progressLabel = new QLabel("0%");
    progressLabel->setParent(mainWindow);
    progressLabel->setGeometry(progressBar->geometry().x() + progressBar->width() + 10,
                               progressBar->geometry().y(),
                               50, 30);
    progressLabel->show();
}

void TScene::removeProgressBar() {
    if (progressBar) {
        progressBar->hide();
        delete progressBar;
        progressBar = nullptr;
    }
    if (progressLabel) {
        progressLabel->hide();
        delete progressLabel;
        progressLabel = nullptr;
    }
    if (vidaBar) {
        vidaBar->hide();
        delete vidaBar;
        vidaBar = nullptr;
    }
}

void TScene::disparo() {
    if (personaje1->posX > 850 && !bala) {
        QPixmap b(":/imagenes/disparo.png");
        proyectil = new QGraphicsPixmapItem();
        addItem(proyectil);
        proyectil->setPixmap(b);
        proyectil->setScale(3);
        proyectil->setPos(policia->pos().x() + 200, 870);
        bala = true;
        Distancia_bala = 0;
        t = 0.0;
    } else if (bala) {
        t += 0.025;
        double newX = proyectil->pos().x() + velx;
        double newY = proyectil->pos().y() + 0.5 * g * std::pow(t, 2);
        proyectil->setPos(newX, newY);
        Distancia_bala += velx;
        if (personaje1->collidesWithItem(proyectil)) {
            removeItem(proyectil);
            delete proyectil;
            proyectil = nullptr;
            bala = false;
            vida--;
            vidaBar->setValue(vida);
            if (vida <= 0) {
                jugar = false;
                qDebug() << "antes  " << personaje1->jugar_nivel;
                personaje1->jugar_nivel = false;
                qDebug() << "despues " << personaje1->jugar_nivel;
                removeProgressBar();
                Police->stop();
                fire->stop();
                obs_timer->stop();
                coli->stop();
                timer->stop();
                PMenu* menu = new PMenu("Perdiste", 3);
                connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
                connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
                menu->setParent(mainWindow);
                menu->move(672, 100);
                menu->show();
            }
        } else {
            bool collision_with_obstacle = false;
            QGraphicsPixmapItem *collided_obstacle = nullptr;
            for (auto obstacle : obstaculos) {
                if (proyectil->collidesWithItem(obstacle)) {
                    collision_with_obstacle = true;
                    collided_obstacle = obstacle;
                    break;
                }
            }

            if (collision_with_obstacle) {
                removeItem(proyectil);
                delete proyectil;
                proyectil = nullptr;
                bala = false;

                if (collided_obstacle) {
                    removeItem(collided_obstacle);
                    obstaculos.removeOne(collided_obstacle);
                    delete collided_obstacle;
                }
            } else if (proyectil->pos().y() > 1000) {
                qDebug() << "eliminar bala";
                removeItem(proyectil);
                delete proyectil;
                proyectil = nullptr;
                bala = false;
            }
        }
    }
}

void TScene::runPolice() {
    if (personaje1->posX > 850) {
        if (Distancia_tanque < 500) {
            policia->setPos(policia->pos().x() + 50, policia->pos().y());
            Distancia_tanque += 50;
        } else {
            policia->setPos(policia->pos().x() + 20, policia->pos().y());
        }
    }
    if (personaje1->jugar_nivel == false) {
        Police->stop();
        fire->stop();
    }
    if (jugar == true) {
        if (personaje1->collidesWithItem(policia)) {
            tanque_actual = policia->pos().x();
            if (proyectil) {
                removeItem(proyectil);
                delete proyectil;
            }
            proyectil = nullptr;
            bala = false;
            vida = 0;
            vidaBar->setValue(vida);
            jugar = false;
            fire->stop();
            personaje1->setPixmap(QPixmap(":/imagenes/salto4.png"));
            personaje1->setPos(personaje1->posX, 900);
        }
    }
    if ((jugar == false) && (policia->pos().x() > tanque_actual + 850) && (animacion_final == false)) {
        Police->stop();
        obs_timer->stop();
        coli->stop();
        timer->stop();
        animacion_final = true;
        personaje1->jugar_nivel = false;
        progressBar->hide();
        progressLabel->hide();
        vidaBar->hide();
        removeProgressBar();
        PMenu* menu = new PMenu("Perdiste", 3);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
    }
}

void TScene::initializeScene() {
    setSceneRect(0, 30, 1920, 1080);
    personaje1 = new Personaje();
    addItem(personaje1);
    personaje1->setFocus();
}

void TScene::onMoveBackground(int dx) {
    backgroundOffsetX += dx;
    setSceneRect(backgroundOffsetX, 30, 1920, 1080);
}

void TScene::crear_obs() {
    if ((personaje1->posX > 850) && (personaje1->posX < 8000)) {
        if (personaje1->game) {
            QPixmap cj(":/imagenes/caja.png");
            caja = new QGraphicsPixmapItem();
            addItem(caja);
            caja->setPixmap(cj);
            caja->setScale(0.2);
            caja->setPos(personaje1->posX + 800, 930);
            obstaculos.append(caja);
        }
    }
}

void TScene::actualizar_obstaculos() {
    for (int i = 0; i < obstaculos.size(); i++) {
        QGraphicsPixmapItem *obstaculo = obstaculos[i];
        if (obstaculo->pos().x() < backgroundOffsetX - 100) {
            removeItem(obstaculo);
            obstaculos.removeAt(i);
        }
    }
}

void TScene::verificar_colision() {
    progreso = personaje1->posX * 100 / 9000;
    progressBar->setValue(progreso);
    progressLabel->setText(QString::number(progreso) + "%");
    if (personaje1->win) {
        QBrush fondo_escenario_rejas(QImage(":/imagenes/escenario_rejas.png"));
        setBackgroundBrush(fondo_escenario_rejas);
        Police->stop();
        obs_timer->stop();
        coli->stop();
        fire->stop();
        progressBar->hide();
        progressLabel->hide();
        vidaBar->hide();
        removeProgressBar();

        PMenu* menu = new PMenu("GANASTE", 3);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
    }

    bool colision_detectada = false;
    for (int i = 0; i < obstaculos.size(); i++) {
        if (personaje1->collidesWithItem(obstaculos[i])) {
            colision_detectada = true;
            break;
        }
    }

    if (colision_detectada) {
        personaje1->colision = true;
    } else {
        personaje1->colision = false;
    }

    for (int i = 0; i < obstaculos.size(); i++) {
        if (policia->collidesWithItem(obstaculos[i])) {
            removeItem(obstaculos[i]);
            obstaculos.removeAt(i);
        }
    }
}

void TScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_P) {
        showPauseMenu();
    } else {
        QGraphicsScene::keyPressEvent(event);
    }
}

void TScene::showPauseMenu() {
    PMenu* menu = new PMenu("Pausa", 3, true);
    jugar = false;
    Police->stop();
    fire->stop();
    obs_timer->stop();
    coli->stop();
    timer->stop();
    progressBar->hide();
    progressLabel->hide();
    vidaBar->hide();
    connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
    connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
    connect(menu, &PMenu::resume, this, [this]() {
        jugar = true;
        obs_timer->start();
        coli->start();
        timer->start();
        Police->start();
        fire->start();
        progressBar->show();
        progressLabel->show();
        vidaBar->show();
    });
    menu->show();
}
