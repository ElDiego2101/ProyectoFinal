#include "escenario.h"

escenario::escenario(QGraphicsView *viewPtr):view(viewPtr) {
    plataformas.fill(nullptr);
}
void escenario:: dibujarEscenario(QGraphicsScene* scene){
    view->setScene(scene);
    scene->setSceneRect(0, 0, 764 * 3, 382); // Tamaño total de la escena con tres fondos
    QPixmap background1(":/imagenes/fondoJuego.jpg");
    QPixmap background2(":/imagenes/fondoJuego.jpg");
    QPixmap background3(":/imagenes/fondoJuego.jpg");

    // Añade las imágenes en posiciones adyacentes
    QGraphicsPixmapItem *bgItem1 = scene->addPixmap(background1);
    bgItem1->setPos(0, 0);

    QGraphicsPixmapItem *bgItem2 = scene->addPixmap(background2);
    bgItem2->setPos(764, 0);

    QGraphicsPixmapItem *bgItem3 = scene->addPixmap(background3);
    bgItem3->setPos(764 * 2, 0);

}

void escenario::establecerPlataformas(QGraphicsScene* scene){
    int posicion=0;
    for (int i = 0; i < plataformas.size(); ++i) {
        QGraphicsRectItem *plataforma = new QGraphicsRectItem( 0,190+posicion, 2292, 10);
        plataforma->setBrush(Qt::gray);
        scene->addItem(plataforma);
        plataformas[i] = plataforma; // Asigna la plataforma en el índice i
        posicion+=100;
        if(i==1){
            posicion-=20;
        }
    }
}
