#include "parser/circuit.h"
#include <math.h>
void circuit::getTitle(QString title)
{
    CircuitTitle = title;
    return;
}

void circuit::getAnnotation(QString annotation)
{
    CircuitAnnotation = annotation;
    return;
}

QString circuit::printTitle()
{
    if (CircuitTitle.isNull())
    {
        return "Circuit has no title";
    }
    else
        return CircuitTitle;
}

void circuit::printInfo()
{
    printTitle();
    printAnnotation();
    printElementInfo();
    printCommand();
}

QString circuit::printAnnotation()
{
    return CircuitAnnotation;
}

QStringList circuit::printElementInfo()
{
    Update();
    QStringList list;
    list << "Here's the result of parser:";
    list << "-----------------------------------------------------";
    list << "Title:";
    list << printTitle();
    list << "Nodes:" + QString("%1").arg(NodesName.size());
    //        foreach (const QString &value,NodesName){
    //            qDebug()<<value;
    //        }

    list << "Devices:" + QString("%1").arg(DeviceCounts);
    list << "Resistor:" + QString("%1").arg(ResistorName.size()) + "\tCapacitor:" + QString("%1").arg(CapacitorName.size()) + "\tInductance:" + QString("%1").arg(InductanceName.size());
    //        foreach (const QString &value,ResistorName){
    //            qDebug()<<value;
    //        }

    //        qDebug()<<"Capacitor:"<<CapacitorName.size();
    //        foreach (const QString &value,CapacitorName){
    //            qDebug()<<value;
    //        }

    //        qDebug()<<"Inductance:"<<InductanceName.size();
    //        foreach (const QString &value,InductanceName){
    //            qDebug()<<value;
    //        }

    list << "Vsrc:" + QString("%1").arg(VSourceName.size());
    //        foreach (const QString &value,VSourceName){
    //            qDebug()<<value;
    //        }

    foreach (const TwoSideElement &value, TwoSideElementList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }

    list << "-----------------------------------------------------";
    return list;
}

void circuit::printCommand()
{
}

bool circuit::AddResistor(QString Resistor)
{
    QRegExp node("([^Rr]\\w+)");
    QRegExp res("([Rr]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*[pPnNuUmMkK]?)");
    int pos = 0;
    if ((pos = res.indexIn(Resistor, pos)) != -1)
    {
        if (ResistorName.contains(res.cap(1)))
        {
            return false;
        }
        else
        {
            ResistorName.insert(res.cap(1));
            TwoSideElement temp('r', res.cap(1));
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Resistor, pos)) != -1)
        {
            NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Resistor, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1);
        TwoSideElementList.last().value = StringToNum(TwoSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}

bool circuit::AddCapacitor(QString Capacitor)
{
    QRegExp node("([^Cc]\\w+)");
    QRegExp res("([Cc]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*[pPnNuUmMkK]?)");
    int pos = 0;
    if ((pos = res.indexIn(Capacitor, pos)) != -1)
    {
        if (CapacitorName.contains(res.cap(1)))
        {
            return false;
        }
        else
        {
            CapacitorName.insert(res.cap(1));
            TwoSideElement temp('c', res.cap(1));
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Capacitor, pos)) != -1)
        {
            NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Capacitor, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1);
        TwoSideElementList.last().value = StringToNum(TwoSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}

bool circuit::AddInductance(QString Inductance)
{
    QRegExp node("([^Ll]\\w+)");
    QRegExp res("([Ll]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*[pPnNuUmMkK]?)");
    int pos = 0;
    if ((pos = res.indexIn(Inductance, pos)) != -1)
    {
        if (InductanceName.contains(res.cap(1)))
        {
            return false;
        }
        else
        {
            InductanceName.insert(res.cap(1));
            TwoSideElement temp('l', res.cap(1));
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Inductance, pos)) != -1)
        {
            NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Inductance, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1);
        TwoSideElementList.last().value = StringToNum(TwoSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}

bool circuit::AddVoltageSource(QString VSource)
{
    QRegExp node("([^Vv]\\w+)");
    QRegExp res("([Vv]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*[pPnNuUmMkK]?)");
    int pos = 0;
    if ((pos = res.indexIn(VSource, pos)) != -1)
    {
        if (VSourceName.contains(res.cap(1)))
        {
            return false;
        }
        else
        {
            VSourceName.insert(res.cap(1));
            TwoSideElement temp('v', res.cap(1));
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(VSource, pos)) != -1)
        {
            NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(VSource, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1);
        TwoSideElementList.last().value = StringToNum(TwoSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}
void circuit::Update()
{
    DeviceCounts = ResistorName.size() + CapacitorName.size() + InductanceName.size();
}

void circuit::AddCommand(QString Command)
{
    CommandList.enqueue(Command);
}

int Unittype(QChar unitIn)
{
    if (unitIn == 'F' || unitIn == 'f')
        return UNIT_Ff;
    else if (unitIn == 'P' || unitIn == 'p')
        return UNIT_Pp;
    else if (unitIn == 'N' || unitIn == 'n')
        return UNIT_Nn;
    else if (unitIn == 'U' || unitIn == 'u')
        return UNIT_Uu;
    else if (unitIn == 'M' || unitIn == 'm')
        return UNIT_Mm;
    else if (unitIn == 'K' || unitIn == 'k')
        return UNIT_Kk;
    else if (unitIn == 'G' || unitIn == 'g')
        return UNIT_Gg;
    else if (unitIn == 'T' || unitIn == 't')
        return UNIT_Tt;
    else
        return 777;
}

double StringToNum(QString str)
{
    QRegExp PureNum("(\\d*(.)?\\d+)");
    QRegExp WithUnit("(\\d*(.)?\\d+([FfTtpPnNuUmMkK]|(Meg)))");
    QRegExp Scientific("(\\d+.?\\d*(e|(e-))\\d*)");
    if (PureNum.exactMatch(str))
    {
        bool flag;
        double num = str.toDouble(&flag);
        if (flag)
        {
            return num;
        }
        else
        {
            qDebug() << "Error:Fail To Transform string to num";
            return 0;
        }
    }
    else if (WithUnit.exactMatch(str))
    {
        QChar unit = str[str.size() - 1];
        // qDebug() << str.right(3);
        bool isMeg = (str.right(3) == "Meg");
        bool flag;
        // qDebug() << isMeg;
        //	int Type = Unittype(unit);
        // qDebug() << "Unit is :" << unit;
        if (isMeg)
        {
            return str.left(str.size() - 3).toDouble(&flag) * 1000000;
        }
        int Type = Unittype(unit);
        QString WithoutUnit = str.left(str.size() - 1);
        double value = WithoutUnit.toDouble(&flag);
        // qDebug() << "Num is :" << WithoutUnit;
        switch (Type)
        {
        case UNIT_Ff:
            return value * 0.000000000000001;

        case UNIT_Pp:
            return value * 0.000000000001;

        case UNIT_Nn:
            return value * 0.000000001;

        case UNIT_Uu:
            return value * 0.000001;

        case UNIT_Mm:
            return value * 0.001;

        case UNIT_Kk:
            return value * 1000;
        case UNIT_Gg:
            return value * 1000000000;

        case UNIT_Tt:
            return value * 1000000000000;
        default:
            return 0;
        }

        return 0;
    }
    else if (Scientific.exactMatch(str))
    {
        int pos = str.lastIndexOf('e');
        bool flag;
        // qDebug() << pos;
        // qDebug() << str.left(pos);
        // qDebug() << str.right(str.size() - pos - 1);
        double value = str.left(pos).toDouble(&flag);
        double exponent = str.right(str.size() - pos - 1).toDouble();
        return value * pow(10, exponent);
    }
    else
        return 0;
}

circuit::circuit(void)
{
}
circuit::~circuit(void)
{
}
