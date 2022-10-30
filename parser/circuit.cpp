#include "parser/circuit.h"
#include "solver/solver.h"
#include "parser/analyzer.h"
#include <QVector>

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

void circuit::printTwoSideElement(){
    qDebug()<<"Twoside Element:";
    foreach(const TwoSideElement& value,TwoSideElementList){
        qDebug()<<"Type:"<<value.type;
        qDebug()<<"Name:"<<value.Name;

    }
}

void circuit::printFourSideElement(){
    qDebug()<<"Fourside Element:";
    foreach(const FourSideElement& value,FourSideElementList){
        qDebug()<<"Type:"<<value.type;
        qDebug()<<"Name:"<<value.Name;

    }
}

bool circuit::AddResistor(QString Resistor)
{
    QRegExp node("([^Rr]\\w+)");
    QRegExp res("([Rr]\\w+)");
    QRegExp value("(\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?)");
    int pos = 0;
    if ((pos = res.indexIn(Resistor, pos)) != -1)
    {
        if (ResistorName.contains(res.cap(1).simplified()))
        {
            return false;
        }
        else
        {
            ResistorName.insert(res.cap(1).simplified());
            TwoSideElement temp('r', res.cap(1).simplified());
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Resistor, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            TwoSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Resistor, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (CapacitorName.contains(res.cap(1).simplified()))
        {
            return false;
        }
        else
        {
            CapacitorName.insert(res.cap(1).simplified());
            TwoSideElement temp('c', res.cap(1).simplified());
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Capacitor, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            TwoSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Capacitor, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (InductanceName.contains(res.cap(1).simplified()))
        {
            return false;
        }
        else
        {
            InductanceName.insert(res.cap(1).simplified());
            TwoSideElement temp('l', res.cap(1).simplified());
            TwoSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + res.cap(1).simplified(), MatrixOrder.size());
            pos += res.matchedLength();
        }
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Inductance, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            TwoSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(Inductance, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (VSourceName.contains(res.cap(1).simplified()))
        {
            return false;
        }
        else
        {
            VSourceName.insert(res.cap(1).simplified());
            TwoSideElement temp('v', res.cap(1).simplified());
            TwoSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + res.cap(1).simplified(), MatrixOrder.size());
            qDebug()<<"Insert:"<<"I_" + res.cap(1).simplified()<<","<<MatrixOrder.size();
            pos += res.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(VSource, pos)) != -1)
        {
            if (!MatrixOrder.contains(node.cap(1).simplified().simplified()))
                MatrixOrder.insert(node.cap(1).simplified().simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified().simplified());
            TwoSideElementList.last().Nodes[i] = node.cap(1).simplified().simplified();
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(VSource, pos)) != -1)
    {
        TwoSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (ISourceName.contains(res.cap(1).simplified()))
        {
            return false;
        }
        else
        { // make a new element
            ISourceName.insert(res.cap(1).simplified());
            TwoSideElement temp('i', res.cap(1).simplified());
            TwoSideElementList.push_back(temp);
            pos += res.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(ISource, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            TwoSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(ISource, pos)) != -1)
    { // grep value of element
        TwoSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (ControledName.contains(name.cap(1).simplified()))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1).simplified());
            FourSideElement temp('G', name.cap(1).simplified());
            FourSideElementList.push_back(temp);
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 3; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (ControledName.contains(name.cap(1).simplified()))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1).simplified());
            FourSideElement temp('E', name.cap(1).simplified());
            FourSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + name.cap(1).simplified(), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 3; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1).simplified();
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
        if (ControledName.contains(name.cap(1).simplified()))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1).simplified());
            FourSideElement temp('F', name.cap(1).simplified());
            FourSideElementList.push_back(temp);
            // Modified the nodes
            // MatrixOrder.insert("I_" + name.cap(1).simplified(), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = node.indexIn(Source, pos)) != -1)
    { // add new nodes into nodelist
        VSourceName.insert(node.cap(1).simplified());
        FourSideElementList.last().Controler = node.cap(1).simplified();
        pos += node.matchedLength();
    }
    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1).simplified();
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }

    int pos_next = 0;
    for (int i = 2; i <= 3; ++i)
    {
        if ((pos_next = srcNode.indexIn(next, pos_next)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(srcNode.cap(1).simplified()))
                MatrixOrder.insert(srcNode.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(srcNode.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = srcNode.cap(1).simplified();
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
        if (ControledName.contains(name.cap(1).simplified()))
        {
            return false;
        }
        else
        { // make a new element
            ControledName.insert(name.cap(1).simplified());
            FourSideElement temp('G', name.cap(1).simplified());
            FourSideElementList.push_back(temp);
            // Modified the nodes
            MatrixOrder.insert("I_" + name.cap(1).simplified(), MatrixOrder.size());
            pos += name.matchedLength();
        }
    }

    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(Source, pos)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(node.cap(1).simplified()))
                MatrixOrder.insert(node.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(node.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = node.indexIn(Source, pos)) != -1)
    { // add new nodes into nodelist
        VSourceName.insert(node.cap(1).simplified());
        FourSideElementList.last().Controler = node.cap(1).simplified();
        pos += node.matchedLength();
    }

    if ((pos = value.indexIn(Source, pos)) != -1)
    { // grep value of element
        FourSideElementList.last().ValueInString = value.cap(1).simplified();
        FourSideElementList.last().value = StringToNum(FourSideElementList.last().ValueInString);
        pos += value.matchedLength();
    }

    int pos_next = 0;
    for (int i = 2; i <= 3; ++i)
    {
        if ((pos_next = srcNode.indexIn(next, pos_next)) != -1)
        { // add new nodes into nodelist
            if (!MatrixOrder.contains(srcNode.cap(1).simplified()))
                MatrixOrder.insert(srcNode.cap(1).simplified(), MatrixOrder.size());
            NodesName.insert(srcNode.cap(1).simplified());
            FourSideElementList.last().Nodes[i] = srcNode.cap(1).simplified();
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
    CommandList.push_back(Command);
}




cx_mat circuit::GenerateDcStamp(double f)
{
    int n = MatrixOrder.size();
    // printNodesInfo();
    // printTwoSideElement();
    // printFourSideElement();
    qDebug()<<"Matrix size:"<<n;
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
    qDebug()<<"ABS of stamp:"<<det(real);

    RHS.print("RHS:");
 //   qDebug() << "I_Vin" << MatrixOrder["I_" + TwoSideElementList[0].Name];
    return join_horiz(stamp, RHS);
}

cx_mat circuit::GenerateAcStamp(double f)
{
    int n = MatrixOrder.size();
    // printNodesInfo();
    // printTwoSideElement();
    // printFourSideElement();
    qDebug()<<"Matrix size:"<<n;
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
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 2 * datum::pi * f * element.value;
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
   // real.print("stamp:");
    //qDebug()<<"ABS of stamp:"<<det(real);

    //RHS.print("RHS:");
 //   qDebug() << "I_Vin" << MatrixOrder["I_" + TwoSideElementList[0].Name];
    return join_horiz(stamp, RHS);
}

bool IsDcCommand(QString str){
    QRegExp DC("([.][Dd][Cc])");
    int pos = 0;
    if ((pos = DC.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else    
        return false;
}

bool IsAcCommand(QString str){
    QRegExp AC("([.][Aa][Cc])");
    int pos = 0;
    if ((pos = AC.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else    
        return false;
}

bool IsPlot(QString str){
    QRegExp plot("([.][Pp][Ll][Oo][Tt])");
    int pos = 0;
    if ((pos = plot.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else    
        return false;
}

void circuit::CommandParse(){
    foreach(const QString &value,CommandList){
        if(IsDcCommand(value)){
            DCAanlyze(value);
        }
        if(IsAcCommand(value)){
            qDebug()<<"grep an ac command";
            struct ACInFo *ACInFo = ParseACInFo(value);
            ACAanlyze(ACInFo);
        }
        if(IsPlot(value)){
            ParsePlotInfo(value);
        }
    }
}

void circuit::ACAanlyze(struct ACInFo* ACInFo){
    setReferenceNode();
    int index = MatrixOrder[ReferenceNode];
    switch (ACInFo->sweep_type)
    {
    case 'd':
        {
            qDebug()<<"AC DEC analysis";
            struct AC_result *result = new(struct AC_result);          
            for (double f = ACInFo->start; f < ACInFo->stop; f*=10)
            {
                double l_start = f;
                double l_stop = f*10;
                double step;
                if(ACInFo->sweep_nums==1)
                    step = l_stop-l_start;
                else
                    step = (l_stop-l_start)/(ACInFo->sweep_nums-1);
                for (double i = l_start; i <l_stop ; i+=step)
                {
                    qDebug()<<"f:"<<i;
                    cx_mat stamp = GenerateAcStamp(i);
                    stamp.print("stamp:");
                    cx_mat answer = solveAC(stamp,index);
                    //amendAnswer_AC(answer);
                    answer.print("answer:");
                    SaveACAnswer(i,answer,result);
                }
            }    
            result->cols = result->FrequencyList.size();
            result->rows = MatrixOrder.size();
            AC_result = result;   
            break;
        }
    default:
        break;
    }
}


void circuit::DCAanlyze(QString str)
{
    setReferenceNode();
    int index = MatrixOrder[ReferenceNode];
    QRegExp Sourcename("([^.DdCc\\s]\\w+)");
    QRegExp Value("(\\d*[.]?\\d+)");
    int pos = 0;
    double start,stop,step;
    QString Name;
    mat temp = real(GenerateDcStamp(0));
    if ((pos = Sourcename.indexIn(str, pos)) != -1)
	{
		Name = Sourcename.cap(1).simplified();
        pos += Sourcename.matchedLength();
        
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		start = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		stop = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		step = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return;
    }
    int n = (start==stop)? 1:int((stop-start)/step);
    struct DC_result  *result=new struct DC_result(start,stop,step,MatrixOrder.size()+1,n);  
    for(double v=start;v<stop;v+=step){
        qDebug()<<"Name:"<<Name;
        UpdateDCStamp_Vsource(v,Name,temp);
        mat answer = solveDC(temp,index);
        temp.print("temp:");
        //amendAnswer_DC(answer);
        answer.print("Answer:");
        SaveDCAnswer(answer,result);
    }
    DC_result = result;
    return;
}


void circuit::UpdateDCStamp_Vsource(double v,QString Name, mat &stamp){
    printNodesInfo();
    int m = MatrixOrder["I_"+Name];
    int n = stamp.n_cols;
    stamp(m,n-1) = v;
}


void circuit::ParsePlotInfo(QString str){
    QRegExp DC("([Dd][Cc])");
    QRegExp AC("([Aa][Cc])");
    QRegExp tran("([Tt][Rr][Aa][Nn])");
    QRegExp Name("(\\(\\w+\\))");
    QString VariableName;

    int pos_DC = 0;
    int pos_AC = 0;
    int pos_TRAN = 0;
    if ((pos_DC = DC.indexIn(str, pos_DC)) != -1)
	{
        pos_DC += DC.matchedLength();

        if ((pos_DC = Name.indexIn(str, pos_DC)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            struct PlotInfo *plotinfo = new struct PlotInfo('d',VariableName);
            PlotInfo = plotinfo;
            return;
        }
        else{
             qDebug()<<"Fail to parse Plot Command!";
        }
	}

    else if ((pos_AC = AC.indexIn(str, pos_AC)) != -1)
	{
        pos_AC += AC.matchedLength();

        if ((pos_AC = Name.indexIn(str, pos_AC)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            struct PlotInfo *plotinfo = new struct PlotInfo('a',VariableName);
            PlotInfo = plotinfo;
            return;
        }
        else{
             qDebug()<<"Fail to parse Plot Command!";
        }      
	}

    else if ((pos_TRAN = tran.indexIn(str, pos_TRAN)) != -1)
	{
        pos_TRAN += tran.matchedLength();

        if ((pos_TRAN = Name.indexIn(str, pos_TRAN)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            struct PlotInfo *plotinfo = new struct PlotInfo('t',VariableName);
            PlotInfo = plotinfo;
            return;
        }
        else{
             qDebug()<<"Fail to parse Plot Command!";
        }      
	}

    else {
        qDebug()<<"Fail to parse Plot Command!";
    }
    return;
}


void circuit::amendAnswer_DC(mat &answer){
    int pos = MatrixOrder[ReferenceNode];
    double delta = answer(pos);
    QMap<QString, int>::iterator iter = MatrixOrder.begin();
    while (iter != MatrixOrder.end())
    {
        if(iter.key()[0]!="I")
            answer(iter.value())-=delta;
        iter++;
    }
}

void circuit::amendAnswer_AC(cx_mat &answer){
    int pos = MatrixOrder[ReferenceNode];
    cx_double delta = answer(5);
    QMap<QString, int>::iterator iter = MatrixOrder.begin();
    while (iter != MatrixOrder.end())
    {
        if(iter.key()[0]!="I"){
            answer(iter.value())-=answer(pos);
            answer(iter.value())/=delta;
        }
        iter++;
    }
}

void circuit::setReferenceNode(){
    if(NodesName.contains("gnd"))
        ReferenceNode = "gnd";
    else if(NodesName.contains("0"))
        ReferenceNode = "0";
    else 
        return;
}
circuit::circuit(void)
{
}
circuit::~circuit(void)
{
}
