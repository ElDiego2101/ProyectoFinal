#ifndef FIRSTSCENE_H
#define FIRSTSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

class MainWindow;

class FirstScene : public QGraphicsScene
{
    Q_OBJECT

public:
    FirstScene(MainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void moverFondo();  // Función para manejar el movimiento del fondo

private:
    MainWindow *mainWindow;
    int vidas;
    int puntuacion;
    bool juegoPausado;

    QGraphicsPixmapItem *fondo; // Imagen del fondo
    int velocidadFondo;         // Velocidad del fondo

    QTimer *timerFondo;         // Temporizador para actualizar el fondo
};

#endif // FIRSTSCENE_H
