#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSet>
#include <QPixmap>
#include <QKeyEvent>
#include <QList>
#include "PMenu.h"

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Personaje(QGraphicsItem *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    double posX = -70;
    bool game = false;
    bool colision = false;
    bool inicio = false;
    bool jugar_nivel =  true;
    bool win = false;

signals:
    void moveBackground(int dx);  // Declaración de la señal

private slots:
    void runPlayer();
    void jumpPlayer();
    void updateMovement();


private:
    QTimer *timer;
    QTimer *timer2;
    QTimer *movementTimer;
    QSet<int> keysPressed;
    int lugar_saltoX = 0;
    int lugar_saltoY = 0;
    int cont = 0;
    int distancia = 0;
    bool tocar_arriba = false;
    bool subir = true;
    bool salto = true;
    bool coli = false;



    void initializePlayer();
};

#endif // PERSONAJE_H
