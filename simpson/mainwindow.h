#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mscene.h"
#include "firstscene.h"
#include "qlcdnumber.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMenuButtonClicked();
    void onLevelSelected(int level);
    void showInitialScene();

private:
    Ui::MainWindow *ui;
    MScene *Menu_Scene;
    QGraphicsScene *initialScene;
    QGraphicsView *graphicsView;
    QPushButton *menuButton;
    QLabel *smallImageLabel;
    QLabel *smallImage2Label;
    QLabel *creditos;
    FirstScene *First_Scene;


};

#endif // MAINWINDOW_H
