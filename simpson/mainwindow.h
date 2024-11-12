#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include "escenario.h"
#include "jugador.h"
#include <QKeyEvent>
#include <QTimer>

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
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void actualizarMovimiento();

private:
    Ui::MainWindow *ui;
    QGraphicsView*  view;
    QGraphicsScene* scene;
    QVBoxLayout *layout;
    escenario* miEscenario;
    jugador* bobPatiño;
};
#endif // MAINWINDOW_H
