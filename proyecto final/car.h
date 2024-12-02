#ifndef CAR_H
#define CAR_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <QPointF>

class Car : public QGraphicsPixmapItem
{
public:
    Car(const QString &filePath, QGraphicsItem *parent = nullptr);
    void turnLeft();
    void turnRight();
    void resetPixmap();
    void updatePosition(qreal vel_x, qreal vel_y);
    void setPoweredUp(bool poweredUp);

private:
    QPixmap carPixmap;
    QPixmap carPixmapLeft;
    QPixmap carPixmapRight;
    QPixmap carPixmapPoweredUp;
    QPixmap carPixmapLeftPoweredUp;
    QPixmap carPixmapRightPoweredUp;
    bool poweredUp;
};

class Obstacle : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Obstacle(const QString &filePath, qreal pos_obst, int vel_obst, qreal car_pos, QGraphicsItem *parent = nullptr);
    void startMoving();
    void updateVelocity(int new_vel_obst);

public slots:
    void moveDown();

private:
    QPixmap obstaclePixmap;
    QTimer *moveTimer;
    int pos_obsta;
    int vel_obsta;
    qreal car_posi;
    bool moveUp;
};

#endif // CAR_H
