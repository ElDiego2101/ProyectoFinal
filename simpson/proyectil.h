#ifndef PROYECTIL_H
#define PROYECTIL_H
#include <QGraphicsPixmapItem>

class proyectil:public QGraphicsPixmapItem
{
    bool tipo;
    bool direccion;
    qreal velocidadX, velocidadY, gravedad;
public:
    proyectil(QGraphicsItem *parent = nullptr);
    void actualizarMovimiento();
    void cambiarDireccion();
    void dibujarProyectil();
    bool getTipo() const;
    void setTipo(bool newTipo);
    bool getDireccion() const;
    void setDireccion(bool newDireccion);
};

#endif // PROYECTIL_H
