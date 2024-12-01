#ifndef ENEMIGOL1_H
#define ENEMIGOL1_H
#include "personaje.h"

class enemigol1:public personaje
{
     Q_OBJECT
private:
     bool enCooldownDisparo;
     bool cooldownFinalizado;
     QTimer cooldownTimer;

private slots:
     void nextFrame();

public:
    enemigol1(QGraphicsItem *parent = nullptr);
    ~enemigol1() override;
    void dibujarJugador() override;  // Dibuja el personaje en su frame actual
    void moverJugador(Direccion direccion);  // Mueve al jugador
    void detenerJugador() override;
    void cambiarDireccion();
    void startGolpeAnimacion();
    bool puedeDisparar();
    bool finalizoCooldown();
    void iniciarCooldownDisparo();
    bool enCooldown();
};

#endif // ENEMIGOL1_H
