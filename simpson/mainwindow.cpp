#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    // Crea la vista
    view = new QGraphicsView(this);
    view->setFixedSize(1080, 395);  // Tamaño de la vista visible
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // Permite el scroll horizontal
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Deshabilita el scroll vertical
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);           // Alinea la vista arriba a la izquierda
    view->setRenderHint(QPainter::Antialiasing);
    // Coloca la vista en el widget central
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(view, 0, Qt::AlignCenter);
    setCentralWidget(centralWidget);

    //ojito
    miEscenario = new escenario(view);
    miEscenario->dibujarEscenario(scene);
    miEscenario->establecerPlataformas(scene);
    bobPatiño=new jugador(755*3, 350,miEscenario);
    bobPatiño->dibujarJugador(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    delete bobPatiño;
}
