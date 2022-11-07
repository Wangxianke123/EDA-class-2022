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

using namespace arma;


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
    QString CircuitTitle;
    QString CircuitAnnotation;
    QMap<QString, int> MatrixOrder;
    QSet<QString> NodesName;

    QVector<StaticElement> StaticList;
    QVector<DynamicElement> DynamicList;
    QVector<DependentElement> DependentList;
    QVector<VSource> VSourceList;
    QVector<ISource> ISourceList;
    QVector<Diode> DiodeList;


    int C_nums,L_nums,R_nums,V_nums,I_nums,Dependent_nums;

    int DeviceCounts;
    int Diode_nums;
    QVector<QString> CommandList;
    QString ReferenceNode;

    struct DC_result* DC_result = nullptr;
    struct AC_result* AC_result = nullptr;
    struct Tran_result* Tran_result = nullptr;

    struct PlotInFo* PlotInFo = nullptr;


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

    mat GenerateDcStamp();
    cx_mat GenerateAcStamp(double f);
    mat GenerateTranStamp(double h);
    // void printTwoSideElement();
    // void printFourSideElement();
    bool UpdateDCStamp_Vsource(double v,QString Name, mat &stamp);

    void UpdateACStamp(double f,cx_mat &stamp);
    void UpdateNonlinearStamp(mat &stamp, mat answer0 , mat answer1);

    void UpdateTranStamp(mat &stamp, double time, double h, struct Tran_result *result);

    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    void printCommand();
    QStringList printNodesInfo();
    void Update();


    void setReferenceNode();
    mat SetInitialState();
    circuit();
    ~circuit();
};

#endif