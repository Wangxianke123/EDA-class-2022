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
#include "circuit/circuit.h"
#include "parser/analyzer.h"
#include "solver/solver.h"
#include "plotter/qcustomplot.h"
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

   
    parserAction = new QAction(tr("Parser"), this);
    parserAction->setToolTip(tr("Parser"));
    connect(parserAction, SIGNAL(triggered()), this, SLOT(slotParser()));

    stampAction = new QAction(tr("Stamp"), this);
    stampAction->setToolTip(tr("Stamp"));
    connect(stampAction, SIGNAL(triggered()), this, SLOT(slotStamp()));

    plotAction = new QAction(tr("Plot"), this);
    plotAction->setToolTip(tr("Plot"));
    connect(plotAction, SIGNAL(triggered()), this, SLOT(slotPlot()));
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
    demoTool = addToolBar(tr("demo"));


    demoTool->addAction(parserAction);
    demoTool->addAction(stampAction);
    demoTool->addAction(plotAction);
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
/*  
*/

void MainWindow::slotStamp()
{
    QLabel *label = new QLabel();
    qDebug()<<MainCircuit->CommandList;
    MainCircuit->CommandParse();
    QStringList content;
    mat A = MainCircuit->GenerateDcStamp();
    content << "Here is the stamp of circuit:";
    content = content + printMatrix(A.cols(0,A.n_cols-2));
    content << "RHS:";
    content = content + printMatrix(A.col(A.n_cols-1));
    label->setText(content.join('\n'));

    label->setAlignment(Qt::AlignCenter);
    label->resize(400, 300);
    label->show();
}


void MainWindow::slotPlot()
{
    if (MainCircuit->PlotInFo == nullptr){
        return;
    }
    switch (MainCircuit->PlotInFo->type)
    {
    case 'd':{
        QVector<double> x,y;
        double start,stop,step;
        start = MainCircuit->DC_result->start;
        stop = MainCircuit->DC_result->stop;
        step = MainCircuit->DC_result->step;
        int n = MainCircuit->DC_result->answer_table.size();
        int pos = MainCircuit->MatrixOrder[MainCircuit->PlotInFo->VariableName];
        qDebug()<<"pos:"<<pos;
        for (int i = 0; i < n; i++)
        {
            qDebug()<<"x:"<<start+i*step<<"y:"<<MainCircuit->DC_result->answer_table[i][pos];
            x.push_back(start+i*step);
            y.push_back(MainCircuit->DC_result->answer_table[i][pos]);
        }
        QCustomPlot *customPlot = new QCustomPlot();
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);
        // give the axes some labels:
        customPlot->xAxis->setLabel("Vsrc");
        customPlot->yAxis->setLabel("V"+ MainCircuit->PlotInFo->VariableName);
        qDebug()<<"begin:"<<*y.begin()<<"end:"<<*y.end();
        // set axes ranges, so we see all data:
        customPlot->xAxis->setRange(start, stop);
        customPlot->yAxis->setRange(*y.begin(), *(y.end()-1));
        customPlot->replot();
        customPlot->setMinimumSize(450, 300);
        customPlot->show();
        return;
        break;
    }
    case 'a':{
        QVector<double> freqData,dBMagData,phaseData;
        int pos = MainCircuit->MatrixOrder[MainCircuit->PlotInFo->VariableName];
        int n = MainCircuit->AC_result->cols;
         for (int i = 0; i < n; i++)
        {
            qDebug()<<"x:"<<MainCircuit->AC_result->FrequencyList[i]<<"M:"<<MainCircuit->AC_result->Magnitude[i][pos];
            freqData.push_back(MainCircuit->AC_result->FrequencyList[i]);
            dBMagData.push_back(MainCircuit->AC_result->Magnitude[i][pos]);
            phaseData.push_back(MainCircuit->AC_result->Phase[i][pos]);
        }
        QCustomPlot *custPlot = new QCustomPlot();
        custPlot->addGraph(custPlot->xAxis, custPlot->yAxis);
        custPlot->graph(0)->setPen(QPen(QPen(Qt::blue)));
        custPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
        custPlot->graph(0)->setData(freqData, dBMagData);
        custPlot->graph(0)->rescaleAxes();

        custPlot->xAxis->setLabel("Freq");
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        custPlot->xAxis->setTicker(logTicker);
        custPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        custPlot->yAxis->setLabel("M");

        // custPlot->addGraph(custPlot->xAxis, custPlot->yAxis2);
        // custPlot->graph(1)->setPen(QPen(Qt::red));
        // custPlot->graph(1)->setData(freqData, phaseData);
        // custPlot->graph(1)->rescaleAxes(true);
        // custPlot->yAxis2->setLabel("Phase");
        // custPlot->yAxis2->setVisible(true);

        custPlot->replot();
        custPlot->setMinimumSize(450, 300);
        custPlot->show();
    }
    case 't':{
        QVector<double> x,y;
        int n = MainCircuit->Tran_result->TimeList.size();
        int pos = MainCircuit->MatrixOrder[MainCircuit->PlotInFo->VariableName];
        qDebug()<<"pos:"<<pos;
        for (int i = 0; i < n; i++)
        {
            x.push_back(MainCircuit->Tran_result->TimeList[i]);
            y.push_back(MainCircuit->Tran_result->ValueList[i][pos]);
        }
        QCustomPlot *customPlot = new QCustomPlot();
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);
        // give the axes some labels:
        customPlot->xAxis->setLabel("Vsrc");
        customPlot->yAxis->setLabel("V"+ MainCircuit->PlotInFo->VariableName);
        qDebug()<<"begin:"<<*y.begin()<<"end:"<<*y.end();
        // set axes ranges, so we see all data:
        double x_max = *std::max_element(x.begin(),x.end());
	    double x_min = *std::min_element(x.begin(),x.end());
        double y_max = *std::max_element(y.begin(),y.end());
	    double y_min = *std::min_element(y.begin(),y.end());

        customPlot->xAxis->setRange(x_min, x_max);
        customPlot->yAxis->setRange(y_min, y_max);
        customPlot->replot();
        customPlot->setMinimumSize(450, 300);
        customPlot->show();
        return;
        break;
    }
    default:
        break;
    }
}