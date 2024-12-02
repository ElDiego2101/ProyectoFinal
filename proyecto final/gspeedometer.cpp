#include "gspeedometer.h"
#include <QGraphicsScene>
#include <QtMath>

// Definición de las constantes de posición
enum Position {
    FIRST_POS = 0,
    SECOND_POS,
    THIRD_POS,
    FOURTH_POS,
    FIFTH_POS,
    SIXTH_POS,
    SEVENTH_POS,
    EIGHTH_POS,
    NINTH_POS,
    TENTH_POS,
    NUM_POSITIONS // Esta constante ayuda a calcular las proporciones
};

static QPointF getFirstPoint()
{
    const double sWidth = 248.0;
    const double sHeight = 248.0;
    const double x = sWidth / 2.0;
    const double y = sHeight / 2.0;
    return QPointF(x, y);
}

QPointF GSpeedometer::sFirstPoint = getFirstPoint();

GSpeedometer::GSpeedometer(QGraphicsItem *parent)
    : QGraphicsObject(parent), mPosition(FIRST_POS), mSize(20.0), mWidth(6)
{
}

void GSpeedometer::incrementPosition(int steps)
{
    mPosition += steps;
    if (mPosition > TENTH_POS)
    {
        mPosition = TENTH_POS;
    }
}

void GSpeedometer::decrementPosition(int steps)
{
    mPosition -= steps;
    if (mPosition < FIRST_POS)
    {
        mPosition = FIRST_POS;
    }
}

void GSpeedometer::updatePositionBasedOnSpeed(int speed)
{
    // Mapear speed de 0 a 50 a posiciones de FIRST_POS a TENTH_POS
    mPosition = (speed * (NUM_POSITIONS - 1)) / 50;
    if (mPosition > TENTH_POS)
    {
        mPosition = TENTH_POS;
    }
    if (mPosition < FIRST_POS)
    {
        mPosition = FIRST_POS;
    }
    update();
}

QColor GSpeedometer::getArrowColor()
{
    switch (mPosition)
    {
    case FIRST_POS:
    case SECOND_POS:
    case THIRD_POS:
    case FOURTH_POS:
    case FIFTH_POS:
    case SIXTH_POS:
        return Qt::green;
    case SEVENTH_POS:
        return Qt::yellow;
    case EIGHTH_POS:
        return QColor("#FFA500");
    case NINTH_POS:
    case TENTH_POS:
        return Qt::red;
    }
    return QColor();
}

void GSpeedometer::fillCircle(QPainter *painter)
{
    painter->setBrush(Qt::white);
    painter->drawEllipse(sFirstPoint, 20.0, 20.0);
}

void GSpeedometer::drawArrow(QPainter *painter)
{
    const double x1 = mFirstPoint.x();
    const double y1 = mFirstPoint.y();
    const double x2 = mSecondPoint.x();
    const double y2 = mSecondPoint.y();
    const double alpha = qAtan2(y2 - y1, x2 - x1);
    const double distance = qSqrt(qPow(x2 - x1, 2.0) + qPow(y2 - y1, 2.0));
    const double dx = distance * qCos(alpha);
    const double dy = distance * qSin(alpha);
    double theta = qAcos(dx / distance);
    if (dy >= 0.0)
    {
        theta = (M_PI * 2.0) - theta;
    }
    QPointF arrowP1 = mSecondPoint - QPointF(qSin(theta + M_PI / 3.0) * mSize, qCos(theta + M_PI / 3.0) * mSize);
    QPointF arrowP2 = mSecondPoint - QPointF(qSin(theta + M_PI - M_PI / 3.0) * mSize, qCos(theta + M_PI - M_PI / 3.0) * mSize);
    QPolygonF arrow;
    arrow << mSecondPoint << arrowP1 << arrowP2;
    painter->setPen(mColor);
    painter->setBrush(mColor);
    painter->drawPolygon(arrow);
}

void GSpeedometer::drawNeedle(QPainter *painter)
{
    QPen pen(Qt::white);
    pen.setWidth(mWidth);
    painter->setPen(pen);
    painter->drawLine(mFirstPoint, mSecondPoint);
}

void GSpeedometer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    fillCircle(painter);

    mColor = getArrowColor();
    mFirstPoint = sFirstPoint;
    mSecondPoint = getSecondPoint();

    drawNeedle(painter);
    drawArrow(painter);
}

QRectF GSpeedometer::boundingRect() const
{
    return QRectF(0, 0, 248, 248);
}

QPointF GSpeedometer::getSecondPoint() const
{
    switch (mPosition)
    {
    case FIRST_POS: return QPointF(63, 185);
    case SECOND_POS: return QPointF(41, 146);
    case THIRD_POS: return QPointF(41, 102);
    case FOURTH_POS: return QPointF(63, 63);
    case FIFTH_POS: return QPointF(102, 41);
    case SIXTH_POS: return QPointF(147, 41);
    case SEVENTH_POS: return QPointF(184, 64);
    case EIGHTH_POS: return QPointF(207, 102);
    case NINTH_POS: return QPointF(207, 146);
    case TENTH_POS: return QPointF(184, 184);
    }
    return QPointF();
}
