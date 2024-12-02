#ifndef TWOSCENE_H
#define TWOSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include "jfk.h"

class MainWindow;

class Bala : public QGraphicsPixmapItem
{
public:
    Bala(bool especial, QGraphicsItem *parent = nullptr);

    void advance(int phase) override;
    bool esEspecial() const;

private:
    bool especial;
    double angulo;
    double velocidad;
    double gravedad;
};

class Potenciador : public QGraphicsPixmapItem
{
public:
    Potenciador(QGraphicsItem *parent = nullptr);
};

class twoscene : public QGraphicsScene
{
    Q_OBJECT

public:
    twoscene(MainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void generarBala();
    void generarBalaEspecial();
    void generarPotenciador();
    void actualizarJuego();
    void eliminarPotenciador();
    void terminarPotenciador();

private:
    void comprobarColisiones();
    void bloquearMovimientoYMostrarBalaFinal();
    MainWindow *mainWindow;
    jfk *carro;
    QTimer temporizadorBalaFinal;
    QTimer *temporizadorBala;
    QTimer *temporizadorBalaEspecial;
    QTimer *temporizadorPotenciador;
    QTimer *temporizadorJuego;
    QTimer *temporizadorEliminarPotenciador;
    QTimer *temporizadorTerminarPotenciador;
    QTimer singleShot;
    Bala *balaFinal;
    int vidas;
    int puntuacion;
    bool juegoPausado;
    int tiempoTranscurrido;
    bool potenciadorActivo;
    bool final=false;
    bool bloq_mov=false;
    bool derrota=false;

    QSet<int> keysPressed;
};

#endif // TWOSCENE_H
