#ifndef JFK_H
#define JFK_H

#include <QGraphicsPixmapItem>

class jfk : public QGraphicsPixmapItem
{
public:
    jfk(QGraphicsItem *parent = nullptr);

    void establecerDireccion(const QString &direccion);
    void establecerVelocidad(int nuevaVelocidad);
    void establecerTiempoPotenciador(int nuevoTiempoPotenciador);
    int obtenerVelocidad() const;
    int obtenerTiempoPotenciador() const;
    void actualizarPosicion();
    void reiniciarImagen();

protected:
    void advance(int phase) override;

private:
    QString direccionActual;
    int velocidad;
    int tiempoPotenciador;
};

#endif // JFK_H
