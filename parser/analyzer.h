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
#include"circuit.h"

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

struct ACInFo{
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

int Unittype(QChar unitIn);
double StringToNum(QString str);
QString ReadNetlist(QString Netlist);
QStringList ReadNetlistByLine(QString FineName);
QStringList printMatrix(cx_mat A);
QStringList printReal(cx_mat A);
struct ACInFo* ParseACInFo(QString str);


#endif