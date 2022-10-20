#ifndef CIRCUIT_H

#define CIRCUIT_H
#include <QStringList>
#include <QString>
#include <QSet>
#include <QVector>
#include <QDebug>
#include <QQueue>
#include <QMap>
#include <armadillo>
#include <math.h>

using namespace arma;
#define UNIT_Ff 100
#define UNIT_Pp 101
#define UNIT_Nn 102
#define UNIT_Uu 103
#define UNIT_Mm 104
#define UNIT_Kk 105
#define UNIT_MEG 106
#define UNIT_Gg 107
#define UNIT_Tt 108

double StringToNum(QString str);
int Unittype(QChar unitIn);

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
    QQueue<QString> CommandList;

    QVector<TwoSideElement> TwoSideElementList;

    QVector<FourSideElement> FourSideElementList;

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

    void AddCommand(QString Command);

    cx_mat GenerateDcStamp(double f);
    cx_mat GenerateAcStamp(double f);

    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    void printCommand();
    QStringList printNodesInfo();
    void Update();

    circuit();
    ~circuit();
};

#endif