######################################################################
# Automatically generated by qmake (3.1) Tue Oct 11 19:30:22 2022
######################################################################

TEMPLATE = app
TARGET = EDA_new
INCLUDEPATH += .

LIBS += -larmadillo
QT += widgets printsupport
# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


MOC_DIR     = build
OBJECTS_DIR = build
RCC_DIR     = build
UI_DIR      = build

# Input
HEADERS +=  circuit/circuit.h    \
            parser/analyzer.h  \
            mainwindow/mainwindow.h \
            cpp_tutorial/myWidget.h \
            solver/solver.h \
            plotter/qcustomplot.h \
            element/element.h

SOURCES +=  main.cpp            \
            circuit/circuit.cpp   \
            parser/analyzer.cpp  \
            mainwindow/mainwindow.cpp   \
            cpp_tutorial/myWidget.cpp   \
            solver/solver.cpp   \
            plotter/qcustomplot.cpp \
            element/element.cpp

RESOURCES += mainwindow/mainwindow.qrc