#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mscene.h"
#include <QScreen>
#include <QVBoxLayout>
#include <QPalette>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), Menu_Scene(nullptr), First_Scene(nullptr), Two_Scene(nullptr), Third_Scene(nullptr)
{
    ui->setupUi(this);
    graphicsView = new QGraphicsView(this);
    setCentralWidget(graphicsView);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int frameHeight = frameGeometry().height() - geometry().height();

    int viewWidth = screenWidth;
    int viewHeight = screenHeight - frameHeight;

    graphicsView->setFixedSize(viewWidth, viewHeight);

    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    menuButton = new QPushButton(this);
    QString buttonStyleM = "QPushButton {"
                           "background-image: url(:/imagenes/start);"
                           "background-repeat: no-repeat;"
                           "background-position: center;"
                           "border: none;"
                           "width: 905px;"
                           "height: 360px;"
                           "}";

    menuButton->setStyleSheet(buttonStyleM);
    connect(menuButton, &QPushButton::clicked, this, &MainWindow::onMenuButtonClicked);


    menuButton->setGeometry(1166,483,200,197);

    smallImageLabel = new QLabel(this);
    smallImageLabel->setPixmap(QPixmap(":/imagenes/Tstart.png"));
    smallImageLabel->setGeometry(1150, 660, 214, 88);
    smallImageLabel->show();

    smallImage2Label = new QLabel(this);
    smallImage2Label->setPixmap(QPixmap(":/imagenes/TSimpsons.png"));
    smallImage2Label->setGeometry(740, 0, 500, 259);
    smallImage2Label->show();

    creditos = new QLabel(this);
    creditos->setPixmap(QPixmap(":/imagenes/creditos.png"));
    creditos->setGeometry(0, 586, 400, 162);
    creditos->show();

    initialScene = new QGraphicsScene(this);
    initialScene->setSceneRect(2, 2, viewWidth, viewHeight);
    graphicsView->setScene(initialScene);
    initialScene->setBackgroundBrush(QBrush(QPixmap(":/imagenes/portada2.jpg").scaled(viewWidth, viewHeight)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Menu_Scene;
    delete First_Scene;
    delete Two_Scene;
    delete Third_Scene;
}

void MainWindow::onMenuButtonClicked()
{
    if (!Menu_Scene) {
        Menu_Scene = new MScene(this);
        connect(Menu_Scene, &MScene::levelSelected, this, &MainWindow::onLevelSelected);
    }
    graphicsView->setScene(Menu_Scene);
    menuButton->hide();
    smallImageLabel->hide();
    smallImage2Label->hide();
    creditos->hide();
}



void MainWindow::onLevelSelected(int level)
{
    qDebug() << "Nivel seleccionado:" << level;

    try {
        // Limpiar escenas existentes
        if (First_Scene) {
            qDebug() << "Eliminando First_Scene existente";
            delete First_Scene;
            First_Scene = nullptr;
        }
        if (Third_Scene) {
            qDebug() << "Eliminando Third_Scene existente";
            delete Third_Scene;
            Third_Scene = nullptr;
        }
        if (Two_Scene) {
            qDebug() << "Eliminando Two_Scene existente";
            delete Two_Scene;
            Two_Scene = nullptr;
        }

        if (level == 2) {
            qDebug() << "Creando First_Scene";
            First_Scene = new FScene(this);

            if (!First_Scene) {
                qDebug() << "ERROR: No se pudo crear First_Scene";
                return;
            }

            graphicsView->setScene(First_Scene);
        } else if (level == 3) {
            qDebug() << "Crea Two_Scene para level 2";
            Two_Scene = new twoscene(this);
            graphicsView->setScene(Two_Scene);

        } else if (level == 1) {
            qDebug() << "Crea Third_Scene";
            Third_Scene = new FirstScene(this);
            graphicsView->setScene(Third_Scene);
        } else if (level == 4) {
            graphicsView->setScene(initialScene);
            menuButton->show();
            smallImageLabel->show();
            smallImage2Label->show();
            creditos->show();
            delete Menu_Scene;
            Menu_Scene = nullptr;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Excepción en onLevelSelected:" << e.what();
    }
    catch (...) {
        qDebug() << "Excepción desconocida en onLevelSelected";
    }
}


void MainWindow::showInitialScene()
{
    if (First_Scene) {
        qDebug() << "elimino1";
        delete First_Scene;
        First_Scene = nullptr;
    }
    if (Third_Scene) {
        qDebug() << "elimino2";
        delete Third_Scene;
        Third_Scene = nullptr;
    }
    if (Two_Scene) {
        qDebug() << "elimino3";
        delete Two_Scene;
        Two_Scene = nullptr;
    }
    graphicsView->setScene(initialScene);
    menuButton->show();
    smallImageLabel->show();
    smallImage2Label->show();
    creditos->show();
}
