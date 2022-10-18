/**
 * @file mainwindow.cpp
 * @author Deng Qiyu
 * @date 2022/08/03
 * @brief Use Qlabel to print Hello World
 * @ref other free icons: https://www.iconfont.cn/collections/detail?cid=21389
 * @ref https://blog.csdn.net/onlyshi/article/details/47725759
 * @ref https://blog.csdn.net/WQ_1996/article/details/78237151
 * @copyright MSDA Lab, SJTU
 */

#include "mainwindow.h"
#include "cpp_tutorial/myWidget.h"
#include "parser/circuit.h"
#include "parser/analyzer.h"
//#include "matrix/matrix.h"
//#include "plotter/qcustomplot.h"
//#include "regexp/regexp.h"

#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("MainWindow"));

    splitterMainWindow = new QSplitter(Qt::Vertical, this);
    text = new QTextEdit(this);
    transcript = new QTextEdit(QStringLiteral("transcript"), this);
    transcript->setReadOnly(true);
    setCentralWidget(splitterMainWindow);

    splitterMainWindow->addWidget(text);
    splitterMainWindow->addWidget(transcript);

    createActions();
    createMenus();
    createToolBars();

    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

/**
 * @brief Define and connect the actions.
 * @author Deng Qiyu
 * @date 2022/08/04
 */
void MainWindow::createActions()
{

    /** @brief file open action */
    /** @brief QIcon can be replaced. You can find other icons in ref web1. */
    /// The icon path is relative.
    fileOpenAction = new QAction(QIcon(":/images/fileopen.png"), tr("open"), this);
    fileOpenAction->setShortcut(Qt::CTRL + Qt::Key_O);
    fileOpenAction->setStatusTip(tr("Open a file"));
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(slotOpenFile()));

    /** @brief file new action */
    fileNewAction = new QAction(QIcon(":/images/filenew.png"), tr("New"), this);
    fileNewAction->setShortcut(Qt::CTRL + Qt::Key_N);
    fileNewAction->setStatusTip(tr("New file"));
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(slotNewFile()));

    /** @brief save file action */
    fileSaveAction = new QAction(QPixmap(":/images/filesave.png"), tr("Save"), this);
    fileSaveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    fileSaveAction->setStatusTip(tr("Save file"));
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(slotSaveFile()));

    /** @brief cut action */
    cutAction = new QAction(QIcon(":/images/editcut.png"), tr("Cut"), this);
    cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
    cutAction->setStatusTip(tr("Cut to clipboard"));
    connect(cutAction, SIGNAL(triggered()), text, SLOT(cut()));

    /** @brief cut action */
    copyAction = new QAction(QIcon(":/images/editcopy.png"), tr("Copy"), this);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    copyAction->setStatusTip(tr("Copy to clipboard"));
    connect(copyAction, SIGNAL(triggered()), text, SLOT(copy()));

    /** @brief cut action */
    pasteAction = new QAction(QIcon(":/images/editpaste.png"), tr("Paste"), this);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    pasteAction->setStatusTip(tr("Paste clipboard to selection"));
    connect(pasteAction, SIGNAL(triggered()), text, SLOT(paste()));

    /** @brief demo1 to use QLabel to print Hello World*/
    // helloAction = new QAction(tr("hello world"), this);
    // helloAction->setToolTip(tr("use QLabel to print Hello World"));
    // connect(helloAction, SIGNAL(triggered()), this, SLOT(slotHelloWorld()));

    // regexpAction = new QAction(tr("Reg Exp"), this);
    // regexpAction->setToolTip(tr("regular expression demo"));
    // connect(regexpAction, SIGNAL(triggered()), this, SLOT(slotRegExp()));

    // plotterAction = new QAction(tr("Plotter"), this);
    // plotterAction->setToolTip(tr("Qt Plotter demo"));
    // connect(plotterAction, SIGNAL(triggered()), this, SLOT(slotCallPlotter()));

    /*     cppTutorialAction = new QAction(tr("Cpp Tutorial"), this);
        cppTutorialAction->setToolTip(tr("Cpp tutorial"));
        connect(cppTutorialAction, SIGNAL(triggered()), this, SLOT(slotCppTutorial())); */

    // matrixOperationsAction = new QAction(tr("Matrix Operations"), this);
    // matrixOperationsAction->setToolTip(tr("Matrix Operations"));
    // connect(matrixOperationsAction, SIGNAL(triggered()), this, SLOT(slotMatrixOperations()));

    parserAction = new QAction(tr("Parser"), this);
    parserAction->setToolTip(tr("Parser"));
    connect(parserAction, SIGNAL(triggered()), this, SLOT(slotParser()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    editMenu = menuBar()->addMenu(tr("Edit"));

    fileMenu->addAction(fileNewAction);
    fileMenu->addSeparator(); /// Add separator between 2 actions.
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);

    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
}

void MainWindow::createToolBars()
{

    /// You can use multiple toolbars. Actions are separated in the interface.
    // fileTool = addToolBar(tr("File"));
    // editTool = addToolBar(tr("Edit"));
    demoTool = addToolBar(tr("demo"));

    // fileTool->addAction(fileNewAction);
    // fileTool->addAction(fileOpenAction);
    // fileTool->addAction(fileSaveAction);

    // editTool->addAction(copyAction);
    // editTool->addAction(cutAction);
    // editTool->addAction(pasteAction);

    // demoTool->addAction(helloAction);
    // demoTool->addAction(regexpAction);
    // demoTool->addAction(plotterAction);
    /*     demoTool->addAction(cppTutorialAction); */
    // demoTool->addAction(matrixOperationsAction);
    demoTool->addAction(parserAction);
}

/**
 * @brief New action will cover and create a new textedit.
 * @author Deng Qiyu
 * @date 2022/08/04
 */
void MainWindow::slotNewFile()
{
    text->clear();          /// Clear the text
    text->setHidden(false); /// Display the text.
}

void MainWindow::slotParser()
{
    QString netList = text->toPlainText();
    MainCircuit = parse(netList);
    // transcript->setPlainText(ParseResult);
    QFile file("transcript");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open transcript!"));
        return;
    }
    else
    {
        if (!file.isReadable())
        {
            QMessageBox::warning(this, tr("Error"), tr("The transcript is unreadable"));
        }
        else
        {
            QTextStream textStream(&file); // Use QTextStream to load text.
            while (!textStream.atEnd())
            {
                transcript->setPlainText(textStream.readAll());
            }
            transcript->show();
            file.close();
        }
    }
}
/**
 * @brief Open action will open the saved files .
 * @author Deng Qiyu
 * @date 2022/08/04
 */
void MainWindow::slotOpenFile()
{

    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), "Text File (*.sp)");
    /// If the dialog is directly closed, the filename will be null.
    if (fileName == "")
    {
        return;
    }
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"));
            return;
        }
        else
        {
            if (!file.isReadable())
            {
                QMessageBox::warning(this, tr("Error"), tr("The file is unreadable"));
            }
            else
            {
                QTextStream textStream(&file); // Use QTextStream to load text.
                while (!textStream.atEnd())
                {
                    text->setPlainText(textStream.readAll());
                }
                text->show();
                file.close();
            }
        }
    }
}

/**
 * @brief Save action will save the text into a new file when it has not been
 * saved. Otherwise it will be saved in the current file.
 * @author Deng Qiyu
 * @date 2022/08/04
 */
void MainWindow::slotSaveFile()
{

    statusBar()->showMessage(tr("Saving file..."));

    if (fileName == "") /// File has not been saved.
    {
        /// Text is empty.
        if (text->toPlainText() == "")
        {
            QMessageBox::warning(this, tr("Warning"), tr("Content cannot be empty!"),
                                 QMessageBox::Ok);
        }
        else
        {
            QFileDialog fileDialog;
            fileName = fileDialog.getSaveFileName(this, tr("Open File"), "./", "Text File(*.txt)");
            if (fileName == "")
            {
                return;
            }
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"),
                                     QMessageBox::Ok);
                return;
            }
            else
            {
                /// Create a text stream and pass text in.
                QTextStream textStream(&file);
                QString str = text->toPlainText();
                textStream << str;
            }
            file.close();
        }
    }
    else
    { /// File has been saved.
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Warning"), tr("Failed to open file!"));
            return;
        }
        else
        {
            QTextStream textStream(&file);
            QString str = text->toPlainText();
            textStream << str;
            file.close();
        }
    }
}

/**
 * @brief demo1 to use QLabel to print Hello World
 * @author Deng Qiyu
 * @date 2022/08/09
 */
void MainWindow::slotHelloWorld()
{
    QLabel *label = new QLabel();
    label->setText("Hello World!");
    /** @brief Detail settings for label.
     * You can find more functions in ref web at the front of this file */
    label->setAlignment(Qt::AlignCenter);
    // label->setStyleSheet("QLabel{font:15px;color:red;background-color:rgb(f9,f9,"
    //  "f9);}");
    label->setStyleSheet("font:30px;color:black;background-color:yellow");
    label->resize(400, 300);
    label->show(); // label should be shown to be seen.
}

/**
 * @brief Demo to show regular expression in Qt
 * @author Limin Hao
 * @date 2022/08/12
 * @note open regexp.cpp and execute it.
 */
/* void MainWindow::slotRegExp()
{
    QString currPath = QDir::currentPath();
    qDebug() << "Current path is " << currPath << endl;

    QString filename = currPath + "/src/regexp/regexp.cpp";
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"));
        return;
    }
    if (!file.isReadable()) {
        QMessageBox::warning(this, tr("Error"), tr("The file is unreadable"));
        return;
    }

    text->clear();
    text->setStyleSheet("font:15px; color: blue");
    QTextStream textStream(&file); // Use QTextStream to load text.
    while (!textStream.atEnd()) {
        text->setPlainText(textStream.readAll());
    }
    text->show();
    file.close();

    //
    // Call regular expression function
    //
    testRegExp();
} */

/**
 * @brief Demo to show Qt Plotter
 * @author Smallxie
 * @brief This is a demo for simple bode plot
 * @date 2022/08/12
 */
/* void MainWindow::slotCallPlotter()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Load the content in txt file failed."),
                             QMessageBox::Ok);
        return;
    }
    QTextStream textStream(&file);

    QString         xlabel, y1label, y2label;
    QVector<double> freqData, dBMagData, phaseData;

    int lineCount = 0;
    while (!textStream.atEnd()) {
        lineCount += 1;
        QString     line = textStream.readLine();
        QStringList elements = line.split(",");

        // qDebug() << line << elements << textStream.realNumberPrecision();
        if (elements.length() != 3) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("The data format in our demo demands <x1, y1, y2>."));
            return;
        } else {
            // The first line is used for creating label
            if (lineCount == 1) {
                xlabel = elements[0];
                y1label = elements[1];
                y2label = elements[2];
            } else {
                freqData << elements[0].toDouble();
                dBMagData << elements[1].toDouble();
                phaseData << elements[2].toDouble();
            }
        }
    }

    file.close();

    QCustomPlot *custPlot = new QCustomPlot();
    custPlot->addGraph(custPlot->xAxis, custPlot->yAxis);
    custPlot->graph(0)->setPen(QPen(QPen(Qt::blue)));
    custPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    custPlot->graph(0)->setData(freqData, dBMagData);
    custPlot->graph(0)->rescaleAxes();

    custPlot->xAxis->setLabel(xlabel);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    custPlot->xAxis->setTicker(logTicker);
    custPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    custPlot->yAxis->setLabel(y1label);

    custPlot->addGraph(custPlot->xAxis, custPlot->yAxis2);
    custPlot->graph(1)->setPen(QPen(Qt::red));
    custPlot->graph(1)->setData(freqData, phaseData);
    custPlot->graph(1)->rescaleAxes(true);
    custPlot->yAxis2->setLabel(y2label);
    custPlot->yAxis2->setVisible(true);

    custPlot->replot();
    custPlot->setMinimumSize(450, 300);
    custPlot->show();
} */

/**
 * @brief Simple C++ tutorial
 * @author Mingzhen Li
 * @brief This is a demo for OOP feature of C++
 * @date 2022/08/12
 */
/* void MainWindow::slotCppTutorial()
{
    QString currPath = QDir::currentPath();
    qDebug() << "Current path is " << currPath << Qt::endl;

    QString filename = currPath + "/cpp_tutorial/myWidget.cpp";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"));
        return;
    }
    if (!file.isReadable())
    {
        QMessageBox::warning(this, tr("Error"), tr("The file is unreadable"));
        return;
    }

    text->clear();
    text->setStyleSheet("font:15px; color: blue");
    QTextStream textStream(&file); // Use QTextStream to load text.
    while (!textStream.atEnd())
    {
        text->setPlainText(textStream.readAll());
    }
    text->show();
    file.close();

    MyWidget *myWidget = new MyWidget();

    // MyWidget can call any function of QWidget
    myWidget->setFixedSize(300, 200);
    myWidget->setWindowTitle("New Window");
    myWidget->show();
} */

/**
 * @brief Matrix Operations
 * @author Limin Hao
 * @date 2022/09/22
 */
/* void MainWindow::slotMatrixOperations()
{
    QString currPath = QDir::currentPath();
    qDebug() << "Current path is " << currPath << endl;

    QString filename = currPath + "/src/matrix/matrix.cpp";
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file!"));
        return;
    }
    if (!file.isReadable()) {
        QMessageBox::warning(this, tr("Error"), tr("The file is unreadable"));
        return;
    }

    text->clear();
    text->setStyleSheet("font:15px; color: blue");
    QTextStream textStream(&file); // Use QTextStream to load text.
    while (!textStream.atEnd()) {
        text->setPlainText(textStream.readAll());
    }
    text->show();
    file.close();

    // dense and sparse matrix operations
    testDenseSparseMatrix();
} */