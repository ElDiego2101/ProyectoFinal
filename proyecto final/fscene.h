#ifndef FSCENE_H
#define FSCENE_H
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTimer>
#include <QSet>
#include "pmenu.h"
#include "car.h"
#include "qlabel.h"
#include "qprogressbar.h"

class MainWindow;
class FScene : public QGraphicsScene
{
    Q_OBJECT
public:
    FScene(MainWindow *parent = nullptr);
    ~FScene();
protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
private slots:
    void acelerar();
    void spawnObstacle();
    void vel_choque();
    void checkHighSpeed();
    void handlePotCollision();
    void removePotEffect();
    void spawnPot();
private:
    QTimer *highSpeedTimer;
    QTimer *potTimer;
    QTimer *potSpawnTimer;
    bool potSpawnActive;
    int highSpeedCounter;
    QGraphicsPixmapItem *mCarretera;
    QGraphicsPixmapItem *collisionImage;
    QGraphicsPixmapItem *Avion;
    QGraphicsPixmapItem *pot;
    Car *car;
    QTimer *aceleracion;
    QTimer *obstacleTimer;
    QTimer *choque;
    QTimer *startG;
    Obstacle *carro_choque;
    QProgressBar *progressBar;
    QLabel *progressLabel;
    int vel_y;
    int vel_x;
    int divisor;
    int new_vel_choque = 0;
    int contador_posicion_y;
    int pos_obst;
    int cant_obst = 2000;
    int vel_obst = 50;
    int distanciaY = 0;
    int progreso = 0;
    bool win = false;
    bool jugar = false;
    bool humoCreado = false;
    bool potActive = false;
    void startgame();
    QSet<int> keysPressed;
    MainWindow *mainWindow;
};
#endif // FSCENE_H
