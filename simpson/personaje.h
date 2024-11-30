#ifndef PERSONAJE_H
#define PERSONAJE_H
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
    golpe
};
class personaje:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
protected:
    int x;
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
  explicit personaje(QGraphicsItem *parent = nullptr);
  virtual ~personaje();
  virtual void moverJugador(Direccion direccion) = 0;  // Método puro virtual, cada personaje define su movimiento
  virtual void detenerJugador()=0;
  virtual void dibujarJugador()=0;
  int getX() const;
  void setX(int newX);
  int getY() const;
  void setY(int newY);
  QGraphicsRectItem *getPies() const;
  Direccion getCurrentDirection() const;
};

#endif // PERSONAJE_H
