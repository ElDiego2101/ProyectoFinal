#include "bart.h"
#include <QGraphicsScene>

bart::bart(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), direccionActual(""), velocidad(5), tiempoPotenciador(0)
{
    setPixmap(QPixmap(":/imagenes/bart1.png"));
}

void bart::establecerDireccion(const QString &direccion)
{
    if (direccionActual != direccion) {
        direccionActual = direccion;
        if (direccionActual == "izquierda") {
            setPixmap(QPixmap(":/imagenes/bart2.png"));
        } else if (direccionActual == "derecha") {
            setPixmap(QPixmap(":/imagenes/bart1.png"));
        } else if (direccionActual == "izquierda_p") {
            setPixmap(QPixmap("::/imagenes/bart4.png"));
        } else if (direccionActual == "derecha_p") {
            setPixmap(QPixmap(":/imagenes/bart3.png"));
        }
    }
}

void bart::establecerVelocidad(int nuevaVelocidad)
{
    velocidad = nuevaVelocidad;
}

void bart::establecerTiempoPotenciador(int nuevoTiempoPotenciador)
{
    tiempoPotenciador = nuevoTiempoPotenciador;
}

int bart::obtenerVelocidad() const
{
    return velocidad;
}

int bart::obtenerTiempoPotenciador() const
{
    return tiempoPotenciador;
}

void bart::actualizarPosicion()
{
    if (direccionActual == "izquierda" || direccionActual == "izquierda_p") {
        setPos(qMax(0.0, x() - velocidad), y());
    } else if (direccionActual == "derecha" || direccionActual == "derecha_p") {
        setPos(qMin(1850.0, x() + velocidad), y());
    }
}

void bart::reiniciarImagen()
{
    if (direccionActual == "izquierda_p") {
        setPixmap(QPixmap(":/imagenes/bart4.png"));
    } else if (direccionActual == "derecha_p") {
        setPixmap(QPixmap(":/imagenes/bart3.png"));
    }
}

void bart::advance(int phase)
{
    if (!phase) return;

    if (tiempoPotenciador > 0) {
        tiempoPotenciador--;
        if (tiempoPotenciador == 0) {
            velocidad = 5;
            reiniciarImagen();
        }
    }
}
