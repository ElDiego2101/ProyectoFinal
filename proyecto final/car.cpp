#include "car.h"
#include <QGraphicsScene>

Car::Car(const QString &filePath, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
    carPixmap(filePath),
    carPixmapLeft(":/imagenes/carroV.png"),
    carPixmapRight(":/imagenes/carroV.png"),
    carPixmapPoweredUp(":/imagenes/carroA.png"),
    carPixmapLeftPoweredUp(":/imagenes/carroA.png"),
    carPixmapRightPoweredUp(":/imagenes/carroA.png"),
    poweredUp(false)
{
    setPixmap(carPixmap);
    setScale(0.25);
}

void Car::turnLeft() {
    if (poweredUp) {
        setPixmap(carPixmapLeftPoweredUp);
    } else {
        setPixmap(carPixmapLeft);
    }
}

void Car::turnRight() {
    if (poweredUp) {
        setPixmap(carPixmapRightPoweredUp);
    } else {
        setPixmap(carPixmapRight);
    }
}

void Car::resetPixmap() {
    if (poweredUp) {
        setPixmap(carPixmapPoweredUp);
    } else {
        setPixmap(carPixmap);
    }
}

void Car::updatePosition(qreal vel_x, qreal vel_y) {
    setPos(pos().x() + vel_x, pos().y() - vel_y);
}

void Car::setPoweredUp(bool powered) {
    poweredUp = powered;
    if (poweredUp) {
        setPixmap(carPixmapPoweredUp);
    } else {
        setPixmap(carPixmap);
    }
}

Obstacle::Obstacle(const QString &filePath, qreal pos_obst, int vel_obst, qreal car_pos, QGraphicsItem *parent)
    : QObject(),
    QGraphicsPixmapItem(parent),
    obstaclePixmap(filePath),
    moveTimer(new QTimer(this)),
    pos_obsta(pos_obst),
    vel_obsta(vel_obst),
    car_posi(car_pos),
    moveUp(false)
{
    setScale(0.18);

    int randomX;
    if (rand() % 2 == 0) {
        randomX = 640 + (rand() % (890 - 640));
        int randomImageIndex = 1 + (rand() % 6);
        obstaclePixmap = QPixmap(QString(":/imagenes/img%1.png").arg(randomImageIndex));
    } else {
        randomX = 1150 + (rand() % (1370 - 1150));
        int randomImageIndex = 7 + (rand() % 6);
        obstaclePixmap = QPixmap(QString(":/imagenes/img%1.png").arg(randomImageIndex));
        moveUp = true;
    }

    setPixmap(obstaclePixmap);
    setPos(randomX, pos_obst - 200);
    connect(moveTimer, &QTimer::timeout, this, &Obstacle::moveDown);
}

void Obstacle::startMoving() {
    moveTimer->start(20);
}

void Obstacle::moveDown() {
    if (moveUp) {
        setPos(pos().x(), pos().y() - vel_obsta);
        if (pos().y() > 1078 + pos_obsta) {
            if (scene()) {
                scene()->removeItem(this);
            }
            delete this;
        }
    } else {
        setPos(pos().x(), pos().y() + vel_obsta);
        if (pos().y() > car_posi) {
            if (scene()) {
                scene()->removeItem(this);
            }
            delete this;
        }
    }
}

void Obstacle::updateVelocity(int new_vel_obst) {
    vel_obsta = new_vel_obst;
}
