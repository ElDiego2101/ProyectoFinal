QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bob.cpp \
    enemigol1.cpp \
    firstscene.cpp \
    main.cpp \
    mainwindow.cpp \
    mscene.cpp \
    personaje.cpp

HEADERS += \
    bob.h \
    enemigol1.h \
    firstscene.h \
    mainwindow.h \
    mscene.h \
    personaje.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    imagenes/FondoFS.jpg \
    imagenes/FondoFs.png \
    imagenes/TSimpsons.png \
    imagenes/Tback.png \
    imagenes/Tstart.png \
    imagenes/back.png \
    imagenes/creditos.png \
    imagenes/portada.png \
    imagenes/portada2.jpg \
    imagenes/start.png
