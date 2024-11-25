#ifndef FIRSTSCENE_H
#define FIRSTSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <bob.h>
#include <QSet>

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
    void aplicarGravedad();

private slots:
    void moverFondo();  // Función para manejar el movimiento del fondo
    void establecerPlataformas();
private:
    MainWindow *mainWindow;
    int vidas;
    int puntuacion;
    bool juegoPausado;
    QGraphicsPixmapItem *fondo; // Imagen del fondo
    int velocidadFondo;         // Velocidad del fondo
    QTimer *timerFondo;
    QTimer *timerAnimacionJugador;
    QTimer *timerGravedad;
    QSet<int> teclasPresionadas;        // Temporizador para actualizar el fondo
    std::array<QGraphicsRectItem*, 3> plataformas;
    bob *jugador; //mi puntero a mi personaje bob patiño
    bool enSalto;         // Indica si el jugador está actualmente saltando
    int velocidadSalto;   // Velocidad inicial del salto
    int gravedad;
};

#endif // FIRSTSCENE_H
