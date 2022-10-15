/* #include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QTextStream>
#include <QFile>
#include "./circuit.h"
#include "./analyzer.h"

#define IS_TITLE            1
#define IS_ANNOTATION       2
#define IS_RESISTOR         3
#define IS_CAPACITOR        4
#define IS_COMMAND          5
#define IS_VOLTAGE_SOURCE   6
#define IS_EMPTY            7
#define IS_INDUCTANCE       8

#define FAIL_TO_ANALYZE     0 */

/* void parse(QString FileName){
    //    QString strRead = ReadNetlist("test.sp");
    QStringList list = ReadNetlistByLine(FileName);
    if(list.size()==0){
        return;
    }
//    qDebug() << strRead;

    circuit newCircuit;
    QStringList ErrorInfo;
    qDebug()<<"Start parsing netlist ...";
    for (int i=0;i<list.size();i++){
        QString str = list[i];
        switch(analyze(str,i)){
            case    IS_EMPTY:{
                break;
            }
            case    IS_TITLE:{
                newCircuit.getTitle(str);
                break;
            }
            case    IS_ANNOTATION:{
                newCircuit.getAnnotation(str);
                qDebug()<<"grep an annotation:"<<str;
                break;
            }
            case    IS_COMMAND:{
                qDebug()<<"grep a command:"<<str;
                break;
            }
            case    IS_RESISTOR:{
                qDebug()<<"grep a resistor:"<<str;
                if(!newCircuit.AddResistor(str)){
                    qDebug()<<"error: line "<<i+1<<":Element already exsists! \'"<<str<<"\'";
                    QString temp = "error: line " + QString("%1").arg(i+1) + ":Element already exsists! \'" + str + "\'";
                    ErrorInfo<<temp;
                }
                break;
            }
            case    IS_CAPACITOR:{
                qDebug()<<"grep a capacitor:"<<str;
                if(!newCircuit.AddCapacitor(str)){
                    qDebug()<<"error: line "<<i+1<<":Element already exsists! \'"<<str<<"\'";
                    QString temp = "error: line " + QString("%1").arg(i+1) + ":Element already exsists! \'" + str + "\'";
                    ErrorInfo<<temp;
                }
                break;
            }
            case    IS_VOLTAGE_SOURCE:{
                qDebug()<<"grep a source:"<<str;
                 if(!newCircuit.AddVoltageSource(str)){
                    qDebug()<<"error: line "<<i+1<<":Element already exsists! \'"<<str<<"\'";
                    QString temp = "error: line " + QString("%1").arg(i+1) + ":Element already exsists! \'" + str + "\'";
                    ErrorInfo<<temp;
                }
                break;
            }
            case    IS_INDUCTANCE:{
                qDebug()<<"grep a inductance:"<<str;
                 if(!newCircuit.AddInductance(str)){
                    qDebug()<<"error: line "<<i+1<<":Element already exsists! \'"<<str<<"\'";
                    QString temp = "error: line " + QString("%1").arg(i+1) + ":Element already exsists! \'" + str + "\'";
                    ErrorInfo<<temp;
                }
                break;
            }
            default:
                qDebug()<<"error: line "<<i+1<<":fail to parse\'"<<str<<"\'";
                QString temp = "error: line " + QString("%1").arg(i+1) + ":fail to parse\'" + str + "\'";
                ErrorInfo<<temp;
        }

    }
    newCircuit.printElementInfo();
    foreach (const QString &value,ErrorInfo){
            qDebug()<<value;
    }
} */

/* int main(int argc, char *argv[])
{
    (void) argc;
	(void) argv;
    parse("tb1.sp");
    parse("tb2.sp");
    parse("tb3.sp");
    parse("tb4.sp");
    return 0;
} */


