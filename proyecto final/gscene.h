#ifndef GSCENE_H
#define GSCENE_H

#include "gspeedometer.h"
#include <QGraphicsScene>

class GScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GScene(QObject *parent = nullptr);

    GSpeedometer *getGSpeedometer() const { return mSpeedometer; }

private:
    QGraphicsPixmapItem *mGauge;
    GSpeedometer *mSpeedometer;
};

#endif // GSCENE_H
