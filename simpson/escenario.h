#ifndef ESCENARIO_H
#define ESCENARIO_H
#include <QPixmap>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <array>
class escenario
{
    QPixmap* background1;
    QPixmap* background2;
    QPixmap* background3;
    QGraphicsView *view;
    std::array<QGraphicsRectItem*, 3> plataformas;
public:
    escenario(QGraphicsView *viewPtr);
    void dibujarEscenario(QGraphicsScene* scene);
    void establecerPlataformas(QGraphicsScene* scene);
    std::array<QGraphicsRectItem *, 3> getPlataformas() const;
};

#endif // ESCENARIO_H
