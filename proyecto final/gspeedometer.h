#ifndef GSPEEDOMETER_H
#define GSPEEDOMETER_H

#include <QGraphicsObject>
#include <QPainter>

class GSpeedometer : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Position
    {
        FIRST_POS,
        SECOND_POS,
        THIRD_POS,
        FOURTH_POS,
        FIFTH_POS,
        SIXTH_POS,
        SEVENTH_POS,
        EIGHTH_POS,
        NINTH_POS,
        TENTH_POS
    };

    GSpeedometer(QGraphicsItem *parent = nullptr);

    void incrementPosition(int steps = 1);
    void decrementPosition(int steps = 1);
    void updatePositionBasedOnSpeed(int speed);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    int mPosition;
    double mSize;
    QColor mColor;
    QPointF mFirstPoint;
    QPointF mSecondPoint;
    int mWidth;

    QColor getArrowColor();
    QPointF getSecondPoint() const;
    void fillCircle(QPainter *painter);
    void drawArrow(QPainter *painter);
    void drawNeedle(QPainter *painter);

    static QPointF sFirstPoint;
};

#endif // GSPEEDOMETER_H
