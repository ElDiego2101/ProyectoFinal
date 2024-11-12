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
    bobPatiño=new jugador(0, 0,miEscenario);
    bobPatiño->dibujarJugador(scene);

    //temporarizador posicion bob
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarMovimiento);
    timer->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    delete bobPatiño;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        bobPatiño->setDy(-5); // Mover hacia arriba
        break;
    case Qt::Key_S:
        bobPatiño->setDy(5);  // Mover hacia abajo
        break;
    case Qt::Key_A:
        bobPatiño->setDx(-5); // Mover hacia la izquierda
        break;
    case Qt::Key_D:
        bobPatiño->setDx(5);  // Mover hacia la derecha
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
   case Qt::Key_W:
    case Qt::Key_S:
        bobPatiño->setDy(0);  // Detener el movimiento vertical
        break;
    case Qt::Key_A:
    case Qt::Key_D:
        bobPatiño->setDx(0);  // Detener el movimiento horizontal
        break;
    default:
        QMainWindow::keyReleaseEvent(event);
    }
}
void MainWindow::actualizarMovimiento()
{
   bobPatiño->moverObjeto(); // Actualiza la posición del personaje
}
