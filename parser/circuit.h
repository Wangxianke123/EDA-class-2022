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


using namespace arma;


bool IsDcCommand(QString str);
bool IsAcCommand(QString str);

struct TwoSideElement
{
    TwoSideElement(char type, QString Name) : type(type), Name(Name){};
    char type; // r,c,l,v,etc
    QString Name;
    QString Nodes[2];
    QString ValueInString;
    double value;
};

struct FourSideElement
{
    FourSideElement(char type, QString Name) : type(type), Name(Name){};
    char type; // VCCS:G  VCVS:E  CCVS:H  CCCS:F
    QString Name;
    QString Controler;
    QString Nodes[4];
    QString ValueInString;
    double value;
};

struct PlotInfo{
    char type;
    QString VariableName;
    PlotInfo(char type, QString VariableName) : type(type),VariableName(VariableName){};
};

class circuit
{
public:
    QString CircuitTitle;
    QString CircuitAnnotation;
    QMap<QString, int> MatrixOrder;
    QSet<QString> NodesName;
    QSet<QString> ResistorName;
    QSet<QString> CapacitorName;
    QSet<QString> InductanceName;
    QSet<QString> VSourceName;
    QSet<QString> ISourceName;
    QSet<QString> ControledName;
    QVector<QString> CommandList;

    struct DC_result *DC_result;
    struct PlotInfo  *PlotInfo;
    struct AC_result *AC_result;
    QVector<TwoSideElement> TwoSideElementList;

    QVector<FourSideElement> FourSideElementList;

    QString ReferenceNode;
    
    int DeviceCounts;

    void getTitle(QString title);
    void getAnnotation(QString annotation);

    bool AddResistor(QString Resistor);
    bool AddCapacitor(QString Capacitor);
    bool AddInductance(QString Inductance);
    bool AddVoltageSource(QString VSource);
    bool AddCurrentSource(QString ISource);
    bool AddVCCS(QString Source);
    bool AddVCVS(QString Source);
    bool AddCCCS(QString Source, QString next);
    bool AddCCVS(QString Source, QString next);
    bool ComposeDcCommand(QString);

    void AddCommand(QString Command);

    void CommandParse();
    void DCAanlyze(QString str);
    void ACAanlyze(struct ACInFo* ACInFo);
    void ParsePlotInfo(QString str);
    

    cx_mat GenerateDcStamp(double f);
    cx_mat GenerateAcStamp(double f);

    void printTwoSideElement();
    void printFourSideElement();
    void UpdateDCStamp_Vsource(double v,QString Name, mat &stamp);

    void amendAnswer_DC(mat &answer);
    void amendAnswer_AC(cx_mat &answer);
    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    void printCommand();
    QStringList printNodesInfo();
    void Update();
    void setReferenceNode();
    circuit();
    ~circuit();
};

#endif