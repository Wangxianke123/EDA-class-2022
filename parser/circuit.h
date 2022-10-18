#ifndef CIRCUIT_H

#define CIRCUIT_H
#include <QStringList>
#include <QString>
#include <QSet>
#include <QVector>
#include <QDebug>
#include <QQueue>
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

class circuit
{
public:
    QString CircuitTitle;
    QString CircuitAnnotation;
    QSet<QString> NodesName;
    QSet<QString> ResistorName;
    QSet<QString> CapacitorName;
    QSet<QString> InductanceName;
    QSet<QString> VSourceName;
    QQueue<QString> CommandList;
    QVector<TwoSideElement> TwoSideElementList;
    int DeviceCounts;

    void getTitle(QString title);
    void getAnnotation(QString annotation);

    bool AddResistor(QString Resistor);
    bool AddCapacitor(QString Capacitor);
    bool AddInductance(QString Inductance);
    bool AddVoltageSource(QString VSource);
    void AddCommand(QString Command);

    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    void printCommand();
    void Update();

    circuit();
    ~circuit();
};

#endif