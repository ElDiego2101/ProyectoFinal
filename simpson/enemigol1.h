#ifndef ENEMIGOL1_H
#define ENEMIGOL1_H
#include <QGraphicsPixmapItem>
#include <Qpixmap>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QRectF>

class enemigol1:public QObject,public QGraphicsPixmapItem
{
     Q_OBJECT
private:

public:
    enemigol1();
};

#endif // ENEMIGOL1_H
