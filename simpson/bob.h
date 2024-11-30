#ifndef BOB_H
#define BOB_H
#include "personaje.h"

//enum  Direccion{
 //   ninguna,
  //  arriba,
  //  abajo,
   // izquierda,
    //derecha,
    //golpe
//};
class bob:public personaje
{
    Q_OBJECT
private slots:
    void nextFrame();    // Detiene la animación
public:
    bob(QGraphicsItem *parent = nullptr);
    ~bob();
    void dibujarJugador();  // Dibuja el personaje en su frame actual
    void moverJugador(Direccion direccion);  // Mueve al jugador
    void detenerJugador();
   // int getX() const;
   // void setX(int newX);
    //int getY() const;
   // void setY(int newY);
    //QGraphicsRectItem *getPies() const;
    //Direccion getCurrentDirection() const;
};

#endif // BOB_H
