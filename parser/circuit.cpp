#include "parser/circuit.h"

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
    printNodesInfo();
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
    //        foreach (const QString &value,MatrixOrder){
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
    list << "Controled src:" + QString("%1").arg(ControledName.size());
    //        foreach (const QString &value,VSourceName){
    //            qDebug()<<value;
    //        }

    foreach (const TwoSideElement &value, TwoSideElementList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }
    foreach (const FourSideElement &value, FourSideElementList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]) + "\t\tNode3:" + QString("%1").arg(value.Nodes[2]) + "\t\tNode4:" + QString("%1").arg(value.Nodes[3]);
    }
    list << "-----------------------------------------------------";
    list = list + printNodesInfo();
    return list;
}

void circuit::printCommand()
{
}

bool circuit::AddResistor(QString Resistor)
{
    QRegExp node("([^Rr]\\w+)");
    QRegExp res("([Rr]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
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
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
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
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
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
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
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
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
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
            // Modified the nodes
            MatrixOrder.insert("I_" + res.cap(1), MatrixOrder.size());
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Inductance, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
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
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
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
            // Modified the nodes
            MatrixOrder.insert("I_" + res.cap(1), MatrixOrder.size());
            pos += res.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(VSource, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
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

bool circuit::AddCurrentSource(QString ISource)
{
    QRegExp node("([^Ii]\\w+)");
    QRegExp res("([Ii]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    int pos = 0;
    if ((pos = res.indexIn(ISource, pos)) != -1)
    {
        if (ISourceName.contains(res.cap(1)))
        {
            return false;
        }
        else
        { // make a new element
            ISourceName.insert(res.cap(1));
            TwoSideElement temp('i', res.cap(1));
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(ISource, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
            NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(ISource, pos)) != -1)
    { // grep value of element
        TwoSideElementList.last().ValueInString = value.cap(1);
        TwoSideElementList.last().value = StringToNum(TwoSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}

bool circuit::AddVCCS(QString Source) // G
{
    QRegExp node("([^Gg]\\w+)");
    QRegExp name("([Gg]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    int pos = 0;
    if ((pos = name.indexIn(Source, pos)) != -1)
    {
        if (ControledName.contains(name.cap(1)))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1));
            FourSideElement temp('G', name.cap(1));
            FourSideElementList.push_back(temp);
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 3; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
            NodesName.insert(node.cap(1));
            FourSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1);
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}
bool circuit::AddVCVS(QString Source) // E
{
    QRegExp node("([^Ee]\\w+)");
    QRegExp name("([Ee]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    int pos = 0;
    if ((pos = name.indexIn(Source, pos)) != -1)
    {
        if (ControledName.contains(name.cap(1)))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1));
            FourSideElement temp('E', name.cap(1));
            FourSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + name.cap(1), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 3; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
            NodesName.insert(node.cap(1));
            FourSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1);
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }
    return true;
}
bool circuit::AddCCCS(QString Source, QString next) // F
{
    QRegExp node("([^Ff]\\w+)");
    QRegExp name("([Ff]\\w+)");
    QRegExp srcNode("([^Vv]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    int pos = 0;
    if ((pos = name.indexIn(Source, pos)) != -1)
    {
        if (ControledName.contains(name.cap(1)))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1));
            FourSideElement temp('F', name.cap(1));
            FourSideElementList.push_back(temp);
            // Modified the nodes
            // MatrixOrder.insert("I_" + name.cap(1), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
            NodesName.insert(node.cap(1));
            FourSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }
    if ((pos = node.indexIn(Source, pos)) != -1)
    { // add new nodes into nodelist
        VSourceName.insert(node.cap(1));
        FourSideElementList.last().Controler = node.cap(1);
        pos += node.matchedLength();
    }
    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1);
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }

    int pos_next = 0;
    for (int i = 2; i <= 3; ++i)
    {
        if ((pos_next = srcNode.indexIn(next, pos_next)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(srcNode.cap(1)))
                MatrixOrder.insert(srcNode.cap(1), MatrixOrder.size());
            NodesName.insert(srcNode.cap(1));
            FourSideElementList.last().Nodes[i] = srcNode.cap(1);
            pos_next += srcNode.matchedLength();
        }
    }
    return true;
}
bool circuit::AddCCVS(QString Source, QString next) // H
{
    QRegExp node("([^Hh]\\w+)");
    QRegExp name("([Hh]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    QRegExp srcNode("([^Vv]\\w+)");
    int pos = 0;
    if ((pos = name.indexIn(Source, pos)) != -1)
    {
        if (ControledName.contains(name.cap(1)))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1));
            FourSideElement temp('G', name.cap(1));
            FourSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + name.cap(1), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1)))
                MatrixOrder.insert(node.cap(1), MatrixOrder.size());
            NodesName.insert(node.cap(1));
            FourSideElementList.last().Nodes[i] = node.cap(1);
            pos += node.matchedLength();
        }
    }
    if ((pos = node.indexIn(Source, pos)) != -1)
    { // add new nodes into nodelist
        VSourceName.insert(node.cap(1));
        FourSideElementList.last().Controler = node.cap(1);
        pos += node.matchedLength();
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1);
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }

    int pos_next = 0;
    for (int i = 2; i <= 3; ++i)
    {
        if ((pos_next = srcNode.indexIn(next, pos_next)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(srcNode.cap(1)))
                MatrixOrder.insert(srcNode.cap(1), MatrixOrder.size());
            NodesName.insert(srcNode.cap(1));
            FourSideElementList.last().Nodes[i] = srcNode.cap(1);
            pos_next += srcNode.matchedLength();
        }
    }
    return true;
}

void circuit::Update()
{
    DeviceCounts = ResistorName.size() + CapacitorName.size() + InductanceName.size();
}

QStringList circuit::printNodesInfo()
{
    QStringList list;
    QMap<QString, int>::iterator iter = MatrixOrder.begin();
    while (iter != MatrixOrder.end())
    {
        list << "name:\t" + QString("%1").arg(iter.key()) + "\t row/col number in matrix:\t" + QString("%1").arg(iter.value());
        qDebug() << "name: " << iter.key() << "\t row/col number in matrix:" << iter.value();
        iter++;
    }
    return list;
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

cx_mat circuit::GenerateDcStamp(double f)
{
    int n = MatrixOrder.size();

    mat real(n, n);
    mat imag(n, n);
    cx_mat stamp(n, n);
    cx_mat RHS(n, 1);
    real.zeros();
    imag.zeros();
    RHS.zeros();
    foreach (const TwoSideElement &element, TwoSideElementList)
    {
        switch (element.type)
        {
        case 'r':
        { // resistor
            real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 1 / element.value;
            real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -1 / element.value;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -1 / element.value;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 1 / element.value;
            break;
        }
        case 'v':
        { // voltage source
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            RHS(MatrixOrder["I_" + element.Name], 0) += element.value;
            break;
        }
        case 'i':
        { // current source
            RHS(MatrixOrder[element.Nodes[0]], 0) += -element.value;
            RHS(MatrixOrder[element.Nodes[0]], 0) += element.value;
            break;
        }
        case 'l':
        { // inductance
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            imag(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Name]) += -2 * datum::pi * f * element.value;
            break;
        }
        case 'c':
        { // capacitor
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += -1 / (2 * datum::pi * f * element.value);
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += 1 / (2 * datum::pi * f * element.value);
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += 1 / (2 * datum::pi * f * element.value);
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += -1 / (2 * datum::pi * f * element.value);
            break;
        }
        default:
            break;
        }
    }
    foreach (const FourSideElement &element, FourSideElementList)
    {
        switch (element.type)
        {
        case 'G':
        { // VCCS
            real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[2]]) += element.value;
            real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[3]]) += -element.value;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[2]]) += -element.value;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }
        case 'E':
        { // VCVS
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[2]]) += -element.value;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }
        case 'F':
        { // CCCS
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Controler]) += element.value;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Controler]) += -element.value;
            real(MatrixOrder[element.Nodes[2]], MatrixOrder["I_" + element.Controler]) += 1;
            real(MatrixOrder[element.Nodes[3]], MatrixOrder["I_" + element.Controler]) += -1;
            break;
        }
        case 'H':
        { // CCVS
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Controler]) += -element.value;
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += -1;
            break;
        }
        default:
            break;
        }
    }

    stamp.set_real(real);
    stamp.set_imag(imag);
    real.print("stamp:");

    RHS.print("RHS:");
    qDebug() << "I_Vin" << MatrixOrder["I_" + TwoSideElementList[0].Name];
    return join_horiz(stamp, RHS);
}

circuit::circuit(void)
{
}
circuit::~circuit(void)
{
}
