#include "fscene.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QGraphicsView>
#include <QMainWindow>
#include <QLCDNumber>
#include "mainwindow.h"
#include <QDebug>

FScene::FScene(MainWindow *parent)
    : QGraphicsScene(117, 0, 1920, 1080, parent),
    potSpawnActive(true),
    highSpeedCounter(0),
    pot(nullptr),
    vel_y(0),
    vel_x(0),
    contador_posicion_y(0),
    pos_obst(0),
    mainWindow(parent)
{
    qDebug() << "Iniciando construcción de FScene";

    try {
        // Verificar que el padre no sea nulo
        qDebug() << "MainWindow pointer:" << parent;
        if (!parent) {
            qDebug() << "ERROR: MainWindow parent es nulo";
            throw std::runtime_error("MainWindow parent is null");
        }

        // Configurar fondo
        setBackgroundBrush(QBrush(QPixmap(":/imagenes/fondo_car.jpg")));

        qDebug() << "Creando pixmap de avión";
        QPixmap av(":/imagenes/plane.png");
        if (av.isNull()) {
            qDebug() << "ERROR: No se pudo cargar la imagen del avión";
            throw std::runtime_error("Failed to load avion pixmap");
        }

        Avion = new QGraphicsPixmapItem();
        addItem(Avion);
        Avion->setPixmap(av);
        Avion->setScale(2);
        Avion->setPos(640, -80000);

        qDebug() << "Inicializando timers";
        // Inicializar timers
        highSpeedTimer = new QTimer(this);
        highSpeedTimer->setInterval(1000);
        connect(highSpeedTimer, &QTimer::timeout, this, &FScene::checkHighSpeed);

        aceleracion = new QTimer(this);
        connect(aceleracion, &QTimer::timeout, this, &FScene::acelerar);

        startG = new QTimer(this);
        connect(startG, &QTimer::timeout, this, &FScene::startgame);
        startG->start(20);

        qDebug() << "Creando carro";
        car = new Car(":/imagenes/CarroV.png");
        if (!car) {
            qDebug() << "ERROR: No se pudo crear el carro";
            throw std::runtime_error("Failed to create car");
        }
        addItem(car);
        car->setPos(1138, 1100);

        qDebug() << "Configurando timers de obstáculos y potenciadores";
        obstacleTimer = new QTimer(this);
        connect(obstacleTimer, &QTimer::timeout, this, &FScene::spawnObstacle);

        potTimer = new QTimer(this);
        potTimer->setSingleShot(true);
        connect(potTimer, &QTimer::timeout, this, &FScene::removePotEffect);

        potSpawnTimer = new QTimer(this);
        potSpawnTimer->setInterval(4000);
        connect(potSpawnTimer, &QTimer::timeout, this, &FScene::spawnPot);
        potSpawnTimer->start();

        qDebug() << "Construcción de FScene completada";
    }
    catch (const std::exception& e) {
        qDebug() << "Excepción en constructor de FScene:" << e.what();
        // Puedes agregar manejo de errores adicional aquí
    }
    catch (...) {
        qDebug() << "Excepción desconocida en constructor de FScene";
    }
}

FScene::~FScene() {
    qDebug() << "FScene destructor called";

    // Stop all timers to prevent potential crashes
    if (highSpeedTimer) highSpeedTimer->stop();
    if (aceleracion) aceleracion->stop();
    if (obstacleTimer) obstacleTimer->stop();
    if (potSpawnTimer) potSpawnTimer->stop();
    if (startG) startG->stop();
}

void FScene::keyPressEvent(QKeyEvent *e) {
    keysPressed.insert(e->key());
    QPointF currentCarPos = car->pos();
    if (jugar == true) {
        if ((keysPressed.contains(Qt::Key_W) && keysPressed.contains(Qt::Key_A)) || (keysPressed.contains(Qt::Key_A) && vel_y > 0)) {
            vel_x -= (vel_y / 4);
            car->turnLeft();
            if (contador_posicion_y == 0) {
                car->setPos(currentCarPos.x(), currentCarPos.y() + 30);
                contador_posicion_y = 1;
            }
        }
        if ((keysPressed.contains(Qt::Key_W) && keysPressed.contains(Qt::Key_D)) || (keysPressed.contains(Qt::Key_D) && vel_y > 0)) {
            vel_x += (vel_y / 4);
            car->turnRight();
            if (contador_posicion_y == 0) {
                car->setPos(currentCarPos.x(), currentCarPos.y() + 30);
                contador_posicion_y = 1;
            }
        }
        if (keysPressed.contains(Qt::Key_W)) {
            if (vel_y < 50) {
                vel_y++;
            }
        } else if (keysPressed.contains(Qt::Key_Space)) {
            if (vel_y > 0) {
                vel_y -= 7;
            }
            if (vel_y < 0) {
                vel_y = 0;
            }
        }
    } else {
        keysPressed.remove(e->key());
    }

    if (keysPressed.contains(Qt::Key_P)) {
        progressBar->hide();
        progressLabel->hide();
        highSpeedTimer->stop();

        jugar = false;
        aceleracion->stop();
        obstacleTimer->stop();
        potSpawnTimer->stop();

        // Create pause menu
        PMenu* menu = new PMenu("Pausa", 1, true);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        connect(menu, &PMenu::resume, this, [this]() {
            // Resume game
            jugar = true;
            progressBar->show();
            progressLabel->show();
            aceleracion->start(15);
            obstacleTimer->start(cant_obst);
            if (vel_y >= 40) {
                highSpeedTimer->start();
            }
            potSpawnTimer->start();
        });
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
    }
}

void FScene::keyReleaseEvent(QKeyEvent *e) {
    QPointF currentCarPos = car->pos();
    keysPressed.remove(e->key());
    if (contador_posicion_y == 1) {
        car->setPos(currentCarPos.x(), currentCarPos.y() - 30);
    }
    car->resetPixmap();
    contador_posicion_y = 0;
    vel_x = 0;
}

void FScene::checkHighSpeed() {
    if (potActive) {
        return;
    }

    if (vel_y >= 40) {
        highSpeedCounter++;
    } else {
        highSpeedCounter = 0;
    }
}

void FScene::acelerar() {

    if (highSpeedCounter >= 3) {
        QPointF currentCarPos = car->pos();
        foreach (QGraphicsItem *item, items()) {
            if (item->type() == QGraphicsPixmapItem::Type && item != car && item != Avion) {
                removeItem(item);
                delete item;
            }
        }
        collisionImage = new QGraphicsPixmapItem(QPixmap(":/imagenes/humo.png"));
        addItem(collisionImage);
        collisionImage->setPos(currentCarPos.x() + 80, currentCarPos.y());
        progressBar->hide();
        progressLabel->hide();
        PMenu* menu = new PMenu("Perdiste", 2);
        connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
        connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
        menu->setParent(mainWindow);
        menu->move(672, 100);
        menu->show();
        aceleracion->stop();
        obstacleTimer->stop();
        jugar = false;
        highSpeedTimer->stop();
        potSpawnTimer->stop();
        return;
    }
    if (vel_y >= 40) {
        if (!highSpeedTimer->isActive() && !potActive) {
            highSpeedTimer->start();
        }
    } else {
        if (highSpeedTimer->isActive()) {
            highSpeedTimer->stop();
            highSpeedCounter = 0;
        }
    }
    QPointF currentCarPos = car->pos();
    QPointF currentPos = sceneRect().topLeft();
    progreso = -(currentCarPos.y() - 800) * 100 / 79028;
    progressBar->setValue(progreso);
    progressLabel->setText(QString::number(progreso) + "%");
    if (!keysPressed.contains(Qt::Key_W)) {
        if (vel_y > 0) {
            vel_y--;
        } else if (vel_y < 0) {
            vel_y++;
        }
    } else if (jugar == false) {
        if (vel_y > 0) {
            vel_y--;
        } else if (vel_y < 0) {
            vel_y++;
        }
    }
    if (win == true) {
        vel_y = 10;
        if (car->collidesWithItem(Avion)) {
            // Add a null check before creating the menu
            if (mainWindow) {
                PMenu* menu = new PMenu("GANASTE", 2);

                // Ensure connections are made before showing
                connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
                connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);

                menu->setParent(mainWindow);
                menu->move(672, 100);

                // Additional safety check
                if (menu) {
                    menu->show();
                } else {
                    qDebug() << "Failed to create win menu";
                }
            } else {
                qDebug() << "MainWindow is null, cannot show win menu";
            }

            aceleracion->stop();
            potSpawnTimer->stop();
        }
    }

    currentPos.setY(currentPos.y() - vel_y);
    setSceneRect(QRectF(currentPos, sceneRect().size()));

    // Controlar los límites del movimiento en el eje x
    if (car->pos().x() < 640 && vel_x < 0) {
        vel_x = 0;
    } else if (car->pos().x() > 1370 && vel_x > 0) {
        vel_x = 0;
    }

    // Actualizar la posición del coche
    car->setPos(currentCarPos.x() + vel_x, currentCarPos.y() - vel_y);

    // Verificar colisión con obstáculos
    if (!humoCreado && win == false) {
        foreach (QGraphicsItem *item, items()) {
            if (item->type() == QGraphicsPixmapItem::Type && item != car && item != pot) {
                if (car->collidesWithItem(item)) {
                    if (potActive) {
                        // Eliminar el carro obstáculo si el carro está potenciado
                        removeItem(item);
                        delete item;
                    } else {
                        if ((item->pos().x() >= 1150) && (item->pos().x() <= 1370)) { //1150 a 1370
                            vel_x = 0;
                            jugar = false;
                            // Si hay colisión, ajusta las velocidades
                            vel_y = (vel_y + 50) / 2;
                            vel_obst = (vel_y + 50) / 2;
                            obstacleTimer->stop(); // ya no se crean más obstáculos

                            // Actualizar la velocidad del obstáculo colisionado
                            carro_choque = dynamic_cast<Obstacle*>(item);
                            if (carro_choque) {
                                carro_choque->updateVelocity(vel_obst);
                            }
                        } else if ((item->pos().x() >= 620) && (item->pos().x() <= 890)) {
                            vel_x = 0;
                            jugar = false;
                            carro_choque = dynamic_cast<Obstacle*>(item);
                            vel_y = (-50) / 2;
                            choque = new QTimer(this);
                            if (carro_choque) {
                                carro_choque->updateVelocity(-25);
                            }
                            connect(obstacleTimer, &QTimer::timeout, this, &FScene::vel_choque);
                            choque->start(20);
                            obstacleTimer->stop();
                        }
                        potSpawnTimer->stop();
                        collisionImage = new QGraphicsPixmapItem(QPixmap(":/imagenes/humo.png"));
                        addItem(collisionImage);
                        humoCreado = true;
                        break;
                    }
                }
            }
        }
    }
    qDebug() << "entra sin problema 1";

    if (pot && car->collidesWithItem(pot)) {
        removeItem(pot);
        handlePotCollision();
    }
    qDebug() << "entra sin problema 2";

    if (humoCreado) {
        collisionImage->setPos(currentCarPos.x() + 80, currentCarPos.y() - 34);
        if (vel_y == 0) {
            progressBar->hide();
            progressLabel->hide();
            PMenu* menu = new PMenu("Perdiste", 2);
            connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
            connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
            menu->setParent(mainWindow);
            menu->move(672, 100);
            menu->show();
            aceleracion->stop();
            obstacleTimer->stop();

        }
    }
    if (jugar == true) {
        distanciaY += vel_y;
        if (distanciaY >= 79028) {
            qDebug() << "Reached end of level";
            progressBar->hide();
            progressLabel->hide();
            jugar = false;
            win = true;
            qDebug() << "Final car position:" << currentCarPos;
            vel_y = 15;
            obstacleTimer->stop();

            if (potActive) {
                qDebug() << "Removing pot effect";
                removePotEffect();
            }

            qDebug() << "Attempting to show win menu";
            PMenu* menu = new PMenu("GANASTE", 1);
            connect(menu, &PMenu::retry, mainWindow, &MainWindow::onLevelSelected);
            connect(menu, &PMenu::goToMenu, mainWindow, &MainWindow::showInitialScene);
            menu->setParent(mainWindow);
            menu->move(672, 100);
            menu->show();
            qDebug() << "Win menu created and shown";

            aceleracion->stop();
            potSpawnTimer->stop();
            qDebug() << "Timers stopped";
        }
    }


    cant_obst = (-((vel_y / 90) - 1) * 1500) + 500;
    pos_obst = currentPos.y();
}

void FScene::spawnObstacle() {
    QPointF currentCarPos = car->pos();
    if (jugar == true) {
        Obstacle *obstacle = new Obstacle(":/imagenes/carro_obst.png", pos_obst, vel_obst, currentCarPos.y());
        addItem(obstacle);
        obstacle->startMoving();
    }
}

void FScene::startgame() {
    QPointF currentCarPos = car->pos();
    car->setPos(currentCarPos.x(), currentCarPos.y() - 20);
    if (currentCarPos.y() <= 800) {
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
        jugar = true;
        aceleracion->start(15);
        obstacleTimer->start(cant_obst); // con una regla de 3 cambio la velocidad de la aparicion de obstaculos para que se vea mas fluido
        startG->stop();
    }
}

void FScene::vel_choque() {
    qDebug() << "entro aca";
    new_vel_choque += 10;
    carro_choque->updateVelocity(new_vel_choque);
    if (new_vel_choque >= 0) {
        carro_choque->updateVelocity(new_vel_choque);
        choque->stop();
    }
}

void FScene::handlePotCollision() {
    car->setPoweredUp(true);
    potActive = true;
    potSpawnActive = false;
    potSpawnTimer->stop();
    highSpeedTimer->stop();
    potTimer->start(5000);
}

void FScene::removePotEffect() {
    car->setPoweredUp(false);
    potActive = false;
    potSpawnActive = true;
    potSpawnTimer->start();
    if (vel_y >= 40) {
        highSpeedTimer->start();
    }
}

void FScene::spawnPot() {
    if (potActive) return;

    // Eliminar pot existente de manera segura
    if (pot) {
        if (items().contains(pot)) {
            removeItem(pot);
        }
        delete pot;
        pot = nullptr;
    }

    // Crear nuevo pot solo si no existe
    if (!pot) {
        pot = new QGraphicsPixmapItem(QPixmap(":/imagenes/pot.png"));
        int potX = (rand() % 2 == 0) ? (640 + rand() % (890 - 640)) : (1150 + rand() % (1370 - 1150));
        int potY = car->pos().y() - 900;
        addItem(pot);
        pot->setPos(potX, potY);
        pot->setScale(0.3);
    }
}
