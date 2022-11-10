#ifndef CIRCUIT_H

#define CIRCUIT_H
#include <QStringList>
#include <QString>
#include <QSet>
#include <QVector>
#include <QDebug>
#include <QQueue>
#include <QMap>
#include <QRegExp>
#include <armadillo>
#include <math.h>
#include"solver/solver.h"
#include"element/element.h"
#include"mainwindow/mainwindow.h"
using namespace arma;

class MainWindow;
bool IsDcCommand(QString str);
bool IsAcCommand(QString str);

struct PlotInfo{
    char type;
    QString VariableName;
    PlotInfo(char type, QString VariableName) : type(type),VariableName(VariableName){};
};

class circuit
{
public:

    MainWindow *ui; //pointer to graph user interface
    QString CircuitTitle; //title of netlist
    QString CircuitAnnotation;  // any annotation of netlist will be saved here
    QMap<QString, int> MatrixOrder; //map that record the position of node 
    QSet<QString> NodesName; //name of nodes


    //list that stores different kind of element
    QVector<StaticElement> StaticList;
    QVector<DynamicElement> DynamicList;
    QVector<DependentElement> DependentList;
    QVector<VSource> VSourceList;
    QVector<ISource> ISourceList;
    QVector<Diode> DiodeList;


    //numbers of different element
    int C_nums,L_nums,R_nums,V_nums,I_nums,Dependent_nums;
    int DeviceCounts;
    int Diode_nums;

    //any command will be saved here
    QVector<QString> CommandList;

    //node which Voltage is 0
    QString ReferenceNode;

    char AnalyzeType;


    //result of different simulation 
    struct DC_result* DC_result = nullptr;
    struct AC_result* AC_result = nullptr;
    struct Tran_result* Tran_result = nullptr;
    struct PlotInFo* PlotInFo = nullptr;
    mat Opanswer;

    void getTitle(QString title);
    void getAnnotation(QString annotation);

    //Add element to circuit
    bool addISource(struct ISource* source);
    bool addVSource(struct VSource* source);
    bool addDynamic(struct DynamicElement* element);
    bool addStatic(struct StaticElement* element);
    bool addDependent_VC(struct DependentElement* element);
    bool addDependent_CC(struct DependentElement* element);
    bool addDiode(Diode* element);

    void AddCommand(QString Command);

    void CommandParse();


    void DCAanlyze(struct DCInFo* DCInFo);
    void ACAanlyze(struct ACInFo* ACInFo);
    void TranAanlyze(struct TranInFo* TranInFo);
    void OpAnalyze();
    mat GenerateDcStamp();
    cx_mat GenerateAcStamp(double f);
    mat GenerateTranStamp(double h);
    // void printTwoSideElement();
    // void printFourSideElement();
    bool UpdateDCStamp_Vsource(double v,QString Name, mat &stamp);

    void UpdateACStamp(double f,cx_mat &stamp);
    void UpdateNonlinearStamp(mat &stamp, mat answer0 , mat answer1);
    void UpdateTranStamp(mat &stamp, double time, double h, struct Tran_result *result);

    //print information of circuit
    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    QStringList printNodesInfo();
    QStringList printSourceInfo();
    void PrintResult(struct PrintInFo* info);

    void Update();
    void setReferenceNode();
    mat SetInitialState();
    circuit();
    ~circuit();
};

#endif