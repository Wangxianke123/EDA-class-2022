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
class QAction;
class QMenu;
class QToolBar;
class QTextEdit;
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

public slots:
    void slotNewFile();
    void slotOpenFile();
    void slotSaveFile();

    /** @brief slot of hello world demo */
    void slotHelloWorld();

    /** @brief slot of regular expression demo */
    // void slotRegExp();

    /** @brief slot of qt plotter demo */
    // void slotCallPlotter();

    /** @brief slot of show C++ tutorial */
    void slotCppTutorial();

    /** @brief slot of matrix operations demo*/
    // void slotMatrixOperations();
    void slotParser();

private:
    QMenu    *fileMenu;
    QMenu    *editMenu;
    QToolBar *fileTool;
    QToolBar *editTool;

    QToolBar *demoTool;

    QAction *fileOpenAction;
    QAction *fileNewAction;
    QAction *fileSaveAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *helloAction;            // hello world demo
    QAction *regexpAction;           // regular expression demo
    QAction *plotterAction;          // qt plotter demo
    QAction *cppTutorialAction;      // C++ tutorial
    QAction *matrixOperationsAction; // matrix operations
    QAction *parserAction;            //parser 

    QTextEdit *text;

    QString fileName = "./";
};

#endif // MAINWINDOW_H
