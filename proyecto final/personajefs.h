#ifndef PERSONAJEFS_H
#define PERSONAJEFS_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QRectF>

enum  Direccion{
    ninguna,
    arriba,
    abajo,
    izquierda,
    derecha,
    golpe,
    deteccion
};
class personajefs:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
protected:
    int x;
    short vida;
    int y;
    int dx;
    int dy;
    int frameHeight;
    int frameWidth;
    int currentFrame;  // Cuadro actual de la animación
    Direccion currentDirection;  // Dirección actual
    Direccion lastDirection;  // Última dirección activa
    QGraphicsRectItem *pies;
    QPixmap spriteSheet;  // Imagen completa de los cuadros
    QTimer *timerAnimacion;
protected slots:
    virtual void nextFrame()=0;
public:
    explicit personajefs(QGraphicsItem *parent = nullptr);
    virtual ~personajefs();
    virtual void moverJugador(Direccion direccion) = 0;  // Método puro virtual, cada personaje define su movimiento
    virtual void detenerJugador()=0;
    virtual void dibujarJugador()=0;
    int getX() const;
    void setX(int newX);
    int getY() const;
    void setY(int newY);
    QGraphicsRectItem *getPies() const;
    Direccion getCurrentDirection() const;
    void setCurrentDirection(Direccion newCurrentDirection);
    Direccion getLastDirection() const;
    void setLastDirection(Direccion newLastDirection);
    short getVida() const;
    void setVida(short newVida);
};
#endif // PERSONAJEFS_H
