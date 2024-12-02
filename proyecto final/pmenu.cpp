#include "PMenu.h"
#include <QFont>
#include <QGraphicsProxyWidget>

PMenu::PMenu(const QString& text, int level, QWidget* parent)
    : PMenu(text, level, false, parent)
{
}

PMenu::PMenu(const QString& text, int level, bool showResumeButton, QWidget* parent)
    : QWidget(parent), m_level(level) {
    setupWindow();

    setFixedSize(577, 600);


    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setFixedSize(577, 600);
    view->setSceneRect(0, 0, 577, 600);
    scene->setBackgroundBrush(QBrush(QPixmap(":/imagenes/mR.jpg")));

    textItem = new QGraphicsTextItem(text);
    QFont font = textItem->font();
    font.setPointSize(24);
    textItem->setFont(font);
    textItem->setPos((288 - textItem->boundingRect().width() / 2)+120, 110);
    scene->addItem(textItem);

    retryButton = new QPushButton("Volver a intentar");
    menuButton = new QPushButton("Ir al menú");
    resumeButton = new QPushButton("Reanudar");

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    if (showResumeButton) {
        buttonLayout->addWidget(resumeButton);
    }
    buttonLayout->addWidget(retryButton);
    buttonLayout->addWidget(menuButton);

    QWidget* buttonWidget = new QWidget;
    buttonWidget->setLayout(buttonLayout);

    QGraphicsProxyWidget* proxyWidget = scene->addWidget(buttonWidget);
    proxyWidget->setPos(288 - buttonWidget->width() / 2, 450);

    connect(retryButton, &QPushButton::clicked, this, [this]() {
        emit retry(m_level);
        closeMenu();
    });
    connect(menuButton, &QPushButton::clicked, this, [this]() {
        emit goToMenu();
        closeMenu();
    });
    if (showResumeButton) {
        connect(resumeButton, &QPushButton::clicked, this, [this]() {
            emit resume();
            closeMenu();
        });
    }


    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    setLayout(mainLayout);
}

void PMenu::setupWindow() {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
}

void PMenu::closeMenu() {
    this->close();
}

void PMenu::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_P) {
        emit resume();
        closeMenu();
    }
    QWidget::keyPressEvent(event);
}
