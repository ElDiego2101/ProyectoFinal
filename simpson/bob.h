#ifndef BOB_H
#define BOB_H
#include <QGraphicsPixmapItem>
#include <Qpixmap>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QRectF>

enum  Direccion{
    ninguna,
    arriba,
    abajo,
    izquierda,
    derecha
};
class bob:public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT  // Esta línea es importante
private:
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
private slots:
    void nextFrame();    // Detiene la animación
public:
    bob(QGraphicsItem *parent = nullptr);
    ~bob();
    void dibujarJugador();  // Dibuja el personaje en su frame actual
    void moverJugador(Direccion direccion);  // Mueve al jugador
    void detenerJugador();
    int getX() const;
    void setX(int newX);
    int getY() const;
    void setY(int newY);
    QGraphicsRectItem *getPies() const;
};

#endif // BOB_H
