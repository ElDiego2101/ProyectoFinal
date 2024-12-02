#ifndef FIRSTSCENE_H
#define FIRSTSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <bob.h>
#include "enemigol1.h"
#include "proyectil.h"
#include <QSet>
#define  Ncajas 27

class MainWindow;

class FirstScene : public QGraphicsScene
{
    Q_OBJECT

public:
    FirstScene(MainWindow *parent = nullptr);
    ~FirstScene();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resetGame();
    void pasarNivel();
    bool sobrePlataforma();
    bool colisionCaja();
    bool colisionEnemigos1(enemigol1* enemigo1_);
    bool colisionbalaJ(proyectil* proyectil_);
    bool colisionbalaE(proyectil* proyectil_);
    bool choquePlataforma(proyectil* bala);
    bool puedeBajar();
    bool colisionProyectil1(proyectil* proyectil_);
    void detecionEnemigos1();
    void aplicarGravedad();
    void establecerPlataformas();
    void establecerCajas();
    void establecerEnemigos1();
    void moverEnemigo1();
    void dispararE1_();
    void moverItemsF();
    void establecerGalletas();
    void colisionItems();
private slots:
    void moverFondo();  // Función para manejar el movimiento del fondo

private:
    MainWindow *mainWindow;
    int cantidadEnemigos;
    int vidas;
    int puntuacion;
    int pixmanDirection;
    bool juegoPausado;
    QGraphicsView* vista;
    QRectF rectVista;
    int limiteIzquierda;
    int limiteDerecha;
    bool colisionDerecha;
    bool colisionIzquierda;
    bool veri;
    bool llaveV;
    QGraphicsPixmapItem *fondo;    // Imagen del fondo
    QGraphicsPixmapItem *llave;
    int velocidadFondo;         // Velocidad del fondo
    QTimer *timerFondo;
    QTimer *timerAnimacionJugador;
    QTimer *timerGravedad;
    QTimer* simuladorTecla;
    QTimer* movimientoEnemigo1;
    QTimer* deteccionE1;
    QTimer* dispararE1;
    QTimer* moverItems;
    proyectil* proyectilActivo; // Puntero al proyectil actual
    QTimer* timerMovimientoProyectil;
    QSet<int> teclasPresionadas;

    std::array<QGraphicsPixmapItem*,27> cajas;
    std::vector<QGraphicsPixmapItem*> galletas;
    std::vector<enemigol1*> enemigos1;        // Temporizador para actualizar el fondo
    std::array<QGraphicsRectItem*, 3> plataformas;
    bob *jugador; //mi puntero a mi personaje bob patiño
    bool enSalto; // Indica si el jugador está actualmente saltando
    bool enBajo;
    short golpeDaño;
    int velocidadSalto;   // Velocidad inicial del salto
    int gravedad;
};

#endif // FIRSTSCENE_H
