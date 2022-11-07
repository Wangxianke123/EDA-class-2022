/**
 * @file mainwindow.h
 * @author Deng Qiyu
 * @date 2022/08/03
 * @brief Defination for class and functions
 * @ref https://doc.qt.io/qt-5/qmainwindow.html
 * @copyright MSDA Lab, SJTU
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QTextDocument>
#include <QSplitter>
#include "circuit/circuit.h"

class QAction;
class QMenu;
class QToolBar;
class QTextEdit;
class QTextDocument;
class QSplitter;
class QWidget;
class QTextStream;

/**
 * @class MainWindow
 * @brief QMainWindow defination
 * @author Deng Qiyu
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief constructor */
    MainWindow(QWidget *parent = 0);
    /** @brief destructor */
    ~MainWindow();
    /** @brief All of the menus should be defined in this function*/
    void createMenus();
    /** @brief All of the actions should be defined in this function*/
    void createActions();
    /** @brief All of the toolbars should be defined in this function*/
    void createToolBars();
    void setInsertTextColor(const QColor &color);


public slots:
    void slotNewFile();
    void slotOpenFile();
    void slotSaveFile();
    void slotParser();

    void slotStamp();
    void slotPlot();
    void slotDiode();
private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *simulateMenu;
    QMenu *ViewMenu;
    QToolBar *fileTool;
    QToolBar *editTool;

    QToolBar *demoTool;

    QAction *fileOpenAction;
    QAction *fileNewAction;
    QAction *fileSaveAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *parserAction; // parser

    QAction *stampAction;

    QAction *plotAction;

    QAction *DiodeAction;
    QTextEdit *text;

    QString fileName = "./";

    QTextEdit *transcript;

    QSplitter *splitterMainWindow;

    circuit *MainCircuit = nullptr;
};

#endif // MAINWINDOW_H
