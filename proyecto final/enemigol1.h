#ifndef ENEMIGOL1_H
#define ENEMIGOL1_H

#include "personajefs.h"
#include "proyectil.h"
class enemigol1:public personajefs
{
    Q_OBJECT
private:
    bool enCooldownDisparo;
    bool cooldownFinalizado;
    QTimer cooldownTimer;
    proyectil* proyectilActivo = nullptr;

private slots:
    void nextFrame();

public:
    enemigol1(QGraphicsItem *parent = nullptr);
    ~enemigol1() override;
    void dibujarJugador() override;
    void moverJugador(Direccion direccion);
    void detenerJugador() override;
    void cambiarDireccion();
    void startGolpeAnimacion();
    bool puedeDisparar();
    bool finalizoCooldown();
    void iniciarCooldownDisparo();
    bool enCooldown();
    proyectil *getProyectilActivo() const;
    bool tieneProyectilActivo() const;
    void setProyectilActivo(proyectil* proyectil);
    void eliminarProyectil();
};

#endif // ENEMIGOL1_H
