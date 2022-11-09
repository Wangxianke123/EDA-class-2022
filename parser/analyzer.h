#ifndef ANALYZER_H
#define ANALYZER_H

#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QChar>
#include <math.h>
#include <armadillo>
#include"circuit/circuit.h"
#include"element/element.h"

using namespace arma;
//  different return type of analyzer
#define IS_TITLE 1
#define IS_ANNOTATION 2
#define IS_RESISTOR 3
#define IS_CAPACITOR 4
#define IS_COMMAND 5
#define IS_VOLTAGE_SOURCE 6
#define IS_EMPTY 7
#define IS_INDUCTANCE 8
#define IS_CURRENT_SOURCE 9
#define IS_VCCS 10
#define IS_VCVS 11
#define IS_CCCS 12
#define IS_CCVS 13
#define IS_DIODE 14


#define FAIL_TO_ANALYZE 0


#define UNIT_Ff 100
#define UNIT_Pp 101
#define UNIT_Nn 102
#define UNIT_Uu 103
#define UNIT_Mm 104
#define UNIT_Kk 105
#define UNIT_MEG 106
#define UNIT_Gg 107
#define UNIT_Tt 108



struct DCInFo
{
    QString Name;
    double start;
    double stop;
    double step;
};
struct ACInFo
{
    //QString SourceName;
    char sweep_type;
    int  sweep_nums;
    double start;
    double stop;
    ACInFo(
    char sweep_type,
    int  sweep_nums,
    double start,
    double stop) : sweep_type(sweep_type),sweep_nums(sweep_nums),start(start),stop(stop){};
};

struct TranInFo{
    double t_step;
    double t_stop;
    TranInFo(){};
};


struct PlotInFo{
    char type;
    QVector<QString> VariableName;
    //PlotInFo(char type, QString VariableName) : type(type),VariableName(VariableName){};
    PlotInFo(){};
};

struct PrintInFo{
    QVector<QString> Nodes;
};
int analyze(QString str, int line);
circuit *parse(QString Netlist);

bool IsAnnotation(QString str);
bool IsCommand(QString str);
bool IsResistor(QString str);
bool IsCapacitor(QString str);
bool IsVoltageSource(QString str);
bool IsCurrentSource(QString str);
bool IsEmpty(QString str);
bool IsInductance(QString str);

bool IsVCCS(QString str);
bool IsVCVS(QString str);
bool IsCCCS(QString str);
bool IsCCVS(QString str);
bool ISDiode(QString str);



bool IsDcCommand(QString str);
bool IsAcCommand(QString str);
bool IsTranCommand(QString str);
bool IsPlot(QString str);
bool IsOpCommand(QString str);
bool IsPrintCommand(QString str);

int Unittype(QChar unitIn);
double StringToNum(QString str);
QString ReadNetlist(QString Netlist);
QStringList ReadNetlistByLine(QString FineName);
QStringList printMatrix(cx_mat A);
QStringList printMatrix(mat A);
QStringList printReal(cx_mat A);

struct DCInFo* ParseDCInFo(QString str);
struct ACInFo* ParseACInFo(QString str);
struct PlotInFo* ParsePlotInFo(QString str);
struct TranInFo* ParseTranInFo(QString str);
struct PrintInFo* ParsePrintInfo(QString str);

struct ISource* ParseISource(QString str);
struct ISource* ParseISource_DC(QString str);
struct ISource* ParseISource_AC(QString str);
struct ISource* ParseISource_pulse(QString str);
struct ISource* ParseISource_sin(QString str);

struct VSource* ParseVSource(QString str); 
struct VSource* ParseVSource_DC(QString str);
struct VSource* ParseVSource_AC(QString str);
struct VSource* ParseVSource_pulse(QString str);
struct VSource* ParseVSource_sin(QString str);

struct DynamicElement* ParseDynamic(QString str);
struct StaticElement* ParseStatic(QString str);
struct Dependent* ParseDependent(QString str);
struct DependentElement* ParseDependent_VC(QString str, char type);
struct DependentElement* ParseDependent_CC(QString str, char type);
Diode* ParseDiode(QString str);
#endif