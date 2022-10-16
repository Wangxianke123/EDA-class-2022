#ifndef ANALYZER_H
#define ANALYZER_H
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QString>

//  different return type of analyzer
#define IS_TITLE 1
#define IS_ANNOTATION 2
#define IS_RESISTOR 3
#define IS_CAPACITOR 4
#define IS_COMMAND 5
#define IS_VOLTAGE_SOURCE 6
#define IS_EMPTY 7
#define IS_INDUCTANCE 8

#define FAIL_TO_ANALYZE 0

// main function
int analyze(QString str, int line);

// function to analyze netlist by line
// bool IsTitle(QString str);
bool IsAnnotation(QString str);
bool IsCommand(QString str);
bool IsResistor(QString str);
bool IsCapacitor(QString str);
bool IsVoltageSource(QString str);
bool IsEmpty(QString str);
bool IsInductance(QString str);
QString ReadNetlist(QString Netlist);
QStringList ReadNetlistByLine(QString FineName);

QString parse(QString Netlist)
{
    QStringList list = Netlist.split('\n');
    if (list.size() == 0)
    {
        return "";
    }
    //    qDebug() << strRead;

    circuit newCircuit;
    QStringList ParsedInfo;
    ParsedInfo << "#Start parsing netlist ...";
    for (int i = 0; i < list.size(); i++)
    {
        QString str = list[i];
        switch (analyze(str, i))
        {
        case IS_EMPTY:
        {
            break;
        }
        case IS_TITLE:
        {
            newCircuit.getTitle(str);
            break;
        }
        case IS_ANNOTATION:
        {
            newCircuit.getAnnotation(str);
            ParsedInfo << "#grep an annotation:" << str;
            break;
        }
        case IS_COMMAND:
        {
            ParsedInfo << "#grep a command:" << str;
            break;
        }
        case IS_RESISTOR:
        {
            ParsedInfo << "#grep a resistor:" << str;
            if (!newCircuit.AddResistor(str))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ParsedInfo << temp;
            }
            break;
        }
        case IS_CAPACITOR:
        {
            qDebug() << "#grep a capacitor:" << str;
            if (!newCircuit.AddCapacitor(str))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ParsedInfo << temp;
            }
            break;
        }
        case IS_VOLTAGE_SOURCE:
        {
            ParsedInfo << "#grep a source:" << str;
            if (!newCircuit.AddVoltageSource(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ParsedInfo << temp;
            }
            break;
        }
        case IS_INDUCTANCE:
        {
            ParsedInfo << "#grep a inductance:" << str;
            if (!newCircuit.AddInductance(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ParsedInfo << temp;
            }
            break;
        }
        default:
            QString temp = "error: line " + QString("%1").arg(i + 1) + ":fail to parse\'" + str + "\'";
            ParsedInfo << temp;
        }
    }
    QStringList ElementInfo = newCircuit.printElementInfo();
    QStringList ParserResult = ParsedInfo + ElementInfo;

    QFile file("transcript");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ParserResult.join('\n');
        file.close();
    }
    return ParserResult.join('\n');
}

QString ReadNetlist(QString FileName)
{
    QFile file(FileName);
    QString str;
    // Open netlist
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "error:   Fail to open file:" << file.fileName() << "\n";
        return str;
    }
    else
    {
        qDebug() << "Open file:  " << file.fileName() << "  success!";
    }
    QTextStream in(&file);

    // Read All content of Netlist;
    str = in.readAll();
    file.close();
    return str;
}

QStringList ReadNetlistByLine(QString FileName)
{
    QFile file(FileName);
    QStringList list;
    // Open netlist
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Fail to open file:" << file.fileName() << "!\n";
        return list;
    }
    else
    {
        qDebug() << "Reading netlist:  " << file.fileName() << "   success!";
    }
    QTextStream in(&file);
    // Read Netlist by line
    while (!in.atEnd())
    {
        QString str = in.readLine();
        list << str;
    }
    file.close();
    return list;
}

int analyze(QString str, int line)
{
    if (IsAnnotation(str))
        return IS_ANNOTATION;
    else if (IsCommand(str))
        return IS_COMMAND;
    else if (IsResistor(str))
        return IS_RESISTOR;
    else if (IsCapacitor(str))
        return IS_CAPACITOR;
    else if (IsVoltageSource(str))
        return IS_VOLTAGE_SOURCE;
    else if (IsInductance(str))
        return IS_INDUCTANCE;
    else if (IsEmpty(str))
        return IS_EMPTY;
    else if (line == 0)
        return IS_TITLE;
    else
        return FAIL_TO_ANALYZE;
}

bool IsAnnotation(QString str)
{
    QRegExp Annotation("^[*].*");
    int pos = 0;
    if ((pos = Annotation.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get an annotation:"<<str;
        return true;
    }
    else
        return false;
}

bool IsCommand(QString str)
{
    QRegExp Annotation("^[.].*");
    int pos = 0;
    if ((pos = Annotation.indexIn(str, pos)) != -1)
    {
        //       qDebug()<<"get command:"<<str;
        return true;
    }
    else
        return false;
}

bool IsResistor(QString str)
{
    QRegExp Resistor("^[Rr]\\w*\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*[pPnNuUmMkK]?\\s*$");
    //    qDebug()<<Resistor.isValid();
    int pos = 0;
    if ((pos = Resistor.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}
bool IsCapacitor(QString str)
{
    QRegExp Capacitor("^[Cc]\\w*\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*[pPnNuUmMkK]?\\s*$");
    //    qDebug()<<Resistor.isValid();
    int pos = 0;
    if ((pos = Capacitor.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}

bool IsVoltageSource(QString str)
{
    QRegExp Source("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[AD]?C?\\s*\\d+(.|(e-))?\\d*[pPnNuUmMkK]?\\s*$");
    //    qDebug()<<Resistor.isValid();
    int pos = 0;
    if ((pos = Source.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}

bool IsEmpty(QString str)
{
    QRegExp Empty("^\\s*$");
    //    qDebug()<<Resistor.isValid();
    int pos = 0;
    if ((pos = Empty.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}

bool IsInductance(QString str)
{
    QRegExp Inductance("^[Ll]\\w*\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*[pPnNuUmMkK]?\\s*$");
    //    qDebug()<<Resistor.isValid();
    int pos = 0;
    if ((pos = Inductance.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}

#endif