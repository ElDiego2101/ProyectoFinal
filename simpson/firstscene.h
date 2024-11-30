#ifndef FIRSTSCENE_H
#define FIRSTSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <bob.h>
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
    bool sobrePlataforma();
    bool colisionCaja();
    void aplicarGravedad();
    bool puedeBajar();
    void establecerPlataformas();
    void establecerCajas();

private slots:
    void moverFondo();  // Función para manejar el movimiento del fondo

private:
    MainWindow *mainWindow;
    int vidas;
    int puntuacion;
    bool juegoPausado;
    QGraphicsView* vista;
    QRectF rectVista;
    int limiteIzquierda;
    int limiteDerecha;
    bool colisionDerecha;
    bool colisionIzquierda;
    bool veri;
    QGraphicsPixmapItem *fondo; // Imagen del fondo
    int velocidadFondo;         // Velocidad del fondo
    QTimer *timerFondo;
    QTimer *timerAnimacionJugador;
    QTimer *timerGravedad;
    QTimer* simuladorTecla;
    QSet<int> teclasPresionadas;
    std::array<QGraphicsPixmapItem*,27> cajas;       // Temporizador para actualizar el fondo
    std::array<QGraphicsRectItem*, 3> plataformas;
    bob *jugador; //mi puntero a mi personaje bob patiño
    bool enSalto; // Indica si el jugador está actualmente saltando
    bool enBajo;
    short golpeDaño;
    int velocidadSalto;   // Velocidad inicial del salto
    int gravedad;
};

#endif // FIRSTSCENE_H
