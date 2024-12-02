#include "gscene.h"
#include <QPainter>

#define XSIZE 248
#define YSIZE 248

GScene::GScene(QObject *parent)
    : QGraphicsScene(0.0, 0.0, XSIZE, YSIZE, parent)
{
    mGauge = addPixmap(QPixmap(":/imagenes/velocimetro.png"));
    mSpeedometer = new GSpeedometer;
    addItem(mSpeedometer);
}
