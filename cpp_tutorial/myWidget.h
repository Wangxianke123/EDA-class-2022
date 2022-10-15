/**
 * @file myWidget.cpp
 * @brief example showing how to implememt OOP concept
 * @author Mingzhen Li
 * @date 2022/08/07
 */

#ifndef CPP_TUTORIAL_H
#define CPP_TUTORIAL_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QLabel>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    void keyPressEvent(QKeyEvent* e);

    // constructor
    MyWidget();
    ~MyWidget();

public slots:

private:
    QLabel* label;
    QTextEdit* console;
    QVBoxLayout* layout;
};

#endif