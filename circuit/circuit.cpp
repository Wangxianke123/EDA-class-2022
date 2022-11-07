#include "circuit/circuit.h"
#include "solver/solver.h"
#include "parser/analyzer.h"
#include <QVector>
#include"element/element.h"

#define ERROR_ABS 1e-10
#define ERROR_RELATIVE 1e-4

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
    list << "Resistor:" + QString("%1").arg(R_nums) + "\tCapacitor:" + QString("%1").arg(C_nums) + "\tInductance:" + QString("%1").arg(L_nums);

    list << "Vsrc:" + QString("%1").arg(V_nums);
    list << "Isrc:" + QString("%1").arg(I_nums);
    list << "Dependent:" + QString("%1").arg(Dependent_nums);
    list << "Diode:" + QString("%1").arg(Diode_nums);
    foreach (const DynamicElement &value, DynamicList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }
    foreach (const StaticElement &value, StaticList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }
    foreach (const VSource &value, VSourceList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }
    foreach (const ISource &value, ISourceList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]) + "type:" + value.type;
    }
    foreach (const DependentElement &value, DependentList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tValue:" + QString("%1").arg(value.value) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]) + "\t\tNode3:" + QString("%1").arg(value.Nodes[2]) + "\t\tNode4:" + QString("%1").arg(value.Nodes[3]);
    }
    foreach (const Diode &value, DiodeList)
    {
        list << "Name:" + QString("%1").arg(value.Name) + "\t\tNode1:" + QString("%1").arg(value.Nodes[0]) + "\t\tNode2:" + QString("%1").arg(value.Nodes[1]);
    }
    list << "-----------------------------------------------------";
    list = list + printNodesInfo();
    return list;
}

void circuit::printCommand()
{
}


void circuit::Update()
{
    DeviceCounts = C_nums + L_nums + R_nums + V_nums;
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

bool circuit::addISource(struct ISource* source)
{
    if(ISourceList.contains(*source))
        return false;
    I_nums++;
    ISourceList.push_back(*source);
    NodesName.insert(source->Nodes[0]);
    NodesName.insert(source->Nodes[1]);
    if(!MatrixOrder.contains(source->Nodes[0]))
        MatrixOrder.insert(source->Nodes[0], MatrixOrder.size());
    if(!MatrixOrder.contains(source->Nodes[1]))
        MatrixOrder.insert(source->Nodes[1], MatrixOrder.size());
    return true;
}


bool circuit::addVSource(struct VSource* source)
{
    if(VSourceList.contains(*source))
        return false;
    V_nums++;
    VSourceList.push_back(*source);
    MatrixOrder.insert("I_" + source->Name, MatrixOrder.size());
    NodesName.insert(source->Nodes[0]);
    NodesName.insert(source->Nodes[1]);
    if(!MatrixOrder.contains(source->Nodes[0]))
        MatrixOrder.insert(source->Nodes[0], MatrixOrder.size());
    if(!MatrixOrder.contains(source->Nodes[1]))
        MatrixOrder.insert(source->Nodes[1], MatrixOrder.size());
    return true;
}


bool circuit::addDynamic(struct DynamicElement* element)
{
    if(DynamicList.contains(*element))
        return false;
    DynamicList.push_back(*element);
    if(element->type == 'l'){
        MatrixOrder.insert("I_" + element->Name, MatrixOrder.size());
        L_nums++;
    }
    else    C_nums++;
    NodesName.insert(element->Nodes[0]);
    NodesName.insert(element->Nodes[1]);
    if(!MatrixOrder.contains(element->Nodes[0]))
        MatrixOrder.insert(element->Nodes[0], MatrixOrder.size());
    if(!MatrixOrder.contains(element->Nodes[1]))
        MatrixOrder.insert(element->Nodes[1], MatrixOrder.size());
    return true;
}


bool circuit::addStatic(struct StaticElement* element)
{
    if(StaticList.contains(*element))
        return false;
    R_nums++;
    StaticList.push_back(*element);
    NodesName.insert(element->Nodes[0]);
    NodesName.insert(element->Nodes[1]);
    if(!MatrixOrder.contains(element->Nodes[0]))
        MatrixOrder.insert(element->Nodes[0], MatrixOrder.size());
    if(!MatrixOrder.contains(element->Nodes[1]))
        MatrixOrder.insert(element->Nodes[1], MatrixOrder.size());
    return true;
}


bool circuit::addDependent_VC(struct DependentElement* element)
{
    if(DependentList.contains(*element))
        return false;
    Dependent_nums++;
    DependentList.push_back(*element);
    for (int i = 0; i < 4; i++)
    {
        NodesName.insert(element->Nodes[i]);
        if(!MatrixOrder.contains(element->Nodes[i]))
            MatrixOrder.insert(element->Nodes[i], MatrixOrder.size());
    }
    if(element->type == 'E')
        MatrixOrder.insert("I_" + element->Name,MatrixOrder.size());
    return true;    
}

bool circuit::addDependent_CC(struct DependentElement* element)
{
    if(DependentList.contains(*element))
        return false;
    Dependent_nums++;
    DependentList.push_back(*element);
    for (int i = 0; i < 1; i++)
    {
        NodesName.insert(element->Nodes[i]);
        if(!MatrixOrder.contains(element->Nodes[i]))
            MatrixOrder.insert(element->Nodes[i], MatrixOrder.size());
    }
    if(element->type == 'H')
        MatrixOrder.insert("I_" + element->Name,MatrixOrder.size());
    return true;
}


bool circuit::addDiode(Diode* element)
{
    if(DiodeList.contains(*element))
        return false;
    Diode_nums++;
    DiodeList.push_back(*element);
    for (int i = 0; i < 1; i++)
    {
        NodesName.insert(element->Nodes[i]);
        if(!MatrixOrder.contains(element->Nodes[i]))
            MatrixOrder.insert(element->Nodes[i], MatrixOrder.size());
    }
    return true;
}

mat circuit::GenerateDcStamp()
{
    int n = MatrixOrder.size();
    qDebug()<<"Matrix size:"<<n;
    mat stamp(n, n);
    mat RHS(n,1);
    stamp.zeros();
    RHS.zeros();
    foreach (const StaticElement &element, StaticList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 1 / element.value;
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 1 / element.value;
    }
    foreach (const DynamicElement &element, DynamicList)
    {
        if(element.type=='l')
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
        }
    }
    foreach (const VSource &element, VSourceList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
        RHS(MatrixOrder["I_" + element.Name]) += element.value;
    }
    foreach (const ISource &element, ISourceList)
    {
        RHS(MatrixOrder[element.Nodes[0]]) += -element.value;
        RHS(MatrixOrder[element.Nodes[1]]) +=  element.value;
    }
    foreach (const DependentElement &element, DependentList)
    {
        switch (element.type)
        {
        case 'G':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[2]]) += element.value;
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[3]]) += -element.value;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[2]]) += -element.value;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }
        case 'E':
        { // VCVS
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[2]]) += -element.value;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }

        default:
            break;
        }
    }
    stamp.print("stamp:");
    qDebug()<<"ABS of stamp:"<<det(stamp);
    RHS.print("RHS:");
 //   qDebug() << "I_Vin" << MatrixOrder["I_" + TwoSideElementList[0].Name];
    return join_horiz(stamp, RHS);
}

mat circuit::GenerateTranStamp(double h)
{

    int n = MatrixOrder.size();
    qDebug()<<"Matrix size:"<<n;
    mat stamp(n, n);   
    mat RHS(n,1);
    stamp.zeros();
    RHS.zeros();
    if (h==0)
    {
        qDebug()<<"Error: tran step can not be 0!";
        return join_horiz(stamp, RHS);
    }
    foreach (const StaticElement &element, StaticList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 1 / element.value;
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 1 / element.value;
    }
    foreach (const VSource &element, VSourceList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
        RHS(MatrixOrder["I_" + element.Name]) = getVoltage(&element,0);
    }
    foreach (const ISource &element, ISourceList)
    {
        RHS(MatrixOrder[element.Nodes[0]]) += -getCurrent(&element,0);
        RHS(MatrixOrder[element.Nodes[1]]) +=  getCurrent(&element,0);
    }
    foreach(const DynamicElement &element, DynamicList)
    {
        switch (element.type)
        {
        case 'c':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += element.value/h;
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -element.value/h;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -element.value/h;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += element.value/h;
            RHS(MatrixOrder[element.Nodes[0]]) += 0;
            RHS(MatrixOrder[element.Nodes[1]]) += 0;
            break;
        }
        case 'l':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Name]) +=  -element.value/h;
            RHS(MatrixOrder["I_" + element.Name]) += 0;
            break;
        }
        default:
            break;
        }
    }
    foreach (const DependentElement &element, DependentList)
    {
        switch (element.type)
        {
        case 'G':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[2]]) += element.value;
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[3]]) += -element.value;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[2]]) += -element.value;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }
        case 'E':
        { // VCVS
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[2]]) += -element.value;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[3]]) += element.value;
            break;
        }

        default:
            break;
        }
    }
    foreach(Diode element, DiodeList)
    {
        int N_p = MatrixOrder[element.Nodes[0]];
        int N_m = MatrixOrder[element.Nodes[1]];  
        stamp(N_p,N_p) += element.I_differential(0, 0);
        stamp(N_p,N_m) +=  -element.I_differential(0, 0);
        stamp(N_m,N_p) += -element.I_differential(0, 0);
        stamp(N_m,N_m) += element.I_differential(0, 0);
    } 
    return join_horiz(stamp, RHS);
}

cx_mat circuit::GenerateAcStamp(double f)
{
    int n = MatrixOrder.size();
    qDebug()<<"Matrix size:"<<n;
    mat real(n, n);
    mat imag(n, n);
    cx_mat stamp(n, n);
    cx_mat RHS(n, 1);
    mat RHS_real(n, 1);
    mat RHS_imag(n, 1);
    real.zeros();
    imag.zeros();
    RHS_real.zeros();
    RHS_imag.zeros();
    foreach (const StaticElement &element, StaticList)
    {
        real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 1 / element.value;
        real(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -1 / element.value;
        real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -1 / element.value;
        real(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 1 / element.value;
    }
    foreach(const VSource &element, VSourceList)
    {
        switch (element.type)
        {
        case 'a':
        { // voltage source
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            RHS_real(MatrixOrder["I_" + element.Name], 0) += element.value * cos(element.phase/180*datum::pi);
            RHS_imag(MatrixOrder["I_" + element.Name], 0) += element.value * sin(element.phase/180*datum::pi);
            break;
        }
        default:
            break;
        }
    }
    foreach (const ISource &element, ISourceList)
    {   if(element.type == 'a')
        {
            RHS_real(MatrixOrder[element.Nodes[0]]) += -element.value * cos(element.phase/180*datum::pi);
            RHS_real(MatrixOrder[element.Nodes[1]]) +=  element.value * cos(element.phase/180*datum::pi);
            RHS_imag(MatrixOrder[element.Nodes[0]]) += -element.value * sin(element.phase/180*datum::pi);
            RHS_imag(MatrixOrder[element.Nodes[1]]) +=  element.value * sin(element.phase/180*datum::pi);
        }
    }
    foreach (const DynamicElement &element, DynamicList)
    {
    switch(element.type)
    {    case 'l':
        {
            real(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            real(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            real(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            imag(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Name]) += -2 * datum::pi * f * element.value;
            break;
        }
        case 'c':
        {
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 2 * datum::pi * f * element.value;
            break;
        }
        default:
            break;
    }
        foreach (const DependentElement &element, DependentList)
    {
        switch (element.type)
        {
        case 'G':
        {
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

        default:
            break;
        }
    }
    }
    stamp.set_real(real);
    stamp.set_imag(imag);
    RHS.set_real(RHS_real);
    RHS.set_imag(RHS_imag);
    return join_horiz(stamp, RHS);
}




void circuit::CommandParse(){
    foreach(const QString &value,CommandList){
        if(IsDcCommand(value)){
            qDebug()<<"grep an dc command";
            struct DCInFo *DCInFo = ParseDCInFo(value);
            qDebug()<<"start dc analysis";
            DCAanlyze(DCInFo);
        }
        if(IsAcCommand(value)){
            qDebug()<<"grep an ac command";
            struct ACInFo *ACInFo = ParseACInFo(value);
            ACAanlyze(ACInFo);
        }
        if(IsTranCommand(value)){
            qDebug()<<"grep an tran command";
            struct TranInFo *TranInFo = ParseTranInFo(value);
            qDebug()<<"step:"<<TranInFo->t_step;
	        qDebug()<<"stop:"<<TranInFo->t_stop;
            TranAanlyze(TranInFo);
        }
        if(IsPlot(value)){
            PlotInFo = ParsePlotInFo(value);
        }
    }
}

void circuit::DCAanlyze(struct DCInFo* DCInFo)
{
    setReferenceNode();
    int index = MatrixOrder[ReferenceNode];
    double start = DCInFo->start;
    double stop = DCInFo->stop;
    double step = DCInFo->step;
    int n = (start==stop)? 1:int((stop-start)/step);
    struct DC_result  *result=new struct DC_result(DCInFo->Name,start,stop,step,MatrixOrder.size()+1,n); 
    mat temp = GenerateDcStamp(); 
    for(double v=start;v<stop;v+=step){
        qDebug()<<"V="<<v;
        if(!UpdateDCStamp_Vsource(v,DCInFo->Name,temp))
            {break;}
        mat answer = solveDC(temp,index);
        temp.print("temp:");
        //amendAnswer_DC(answer);
        answer.print("Answer:");
        SaveDCAnswer(answer,result);
    }
    if(DC_result!=nullptr){
        delete DC_result;
    }
    DC_result = result;
    qDebug()<<"end dc analysis";
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
            //cx_mat stamp = GenerateAcStamp();          
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
                    //UpdateACStamp(i,stamp);
                    cx_mat stamp = GenerateAcStamp(i);
                    stamp.print("stamp:");
                    cx_mat answer = solveAC(stamp,index);
                    answer.print("answer:");
                    SaveACAnswer(i,answer,result);
                }
            }    
            result->cols = result->FrequencyList.size();
            result->rows = MatrixOrder.size();
            if(AC_result!=nullptr){
                delete AC_result;
            }
            AC_result = result;   
            break;
        }
    default:
        break;
    }
}

void circuit::TranAanlyze(struct TranInFo* TranInFo)
{
    qDebug()<<"Startint transient simulation....";
    setReferenceNode();
    int index = MatrixOrder[ReferenceNode];
    std::vector<double> TimeList;
    for (double i = TranInFo->t_step; i <= TranInFo->t_stop; i+=TranInFo->t_step)
    {
        TimeList.push_back(i);
    }
    struct Tran_result *result = new (struct Tran_result);
    result->rows = MatrixOrder.size();
    result->cols = 0;
    mat stamp = GenerateTranStamp(TranInFo->t_step);
    stamp.print("stamp:");
    mat answer = SetInitialState();
    answer.print("answer:");
    SaveTranAnswer(0,answer,result);
    foreach(const double &time, TimeList)
    {
        qDebug()<<"time:"<<time;
        UpdateTranStamp(stamp, time, TranInFo->t_step, result);
        stamp.print("stamp:");
        mat answer = solveDC(stamp,index);
        mat answer_temp;
        do
        {
            answer_temp = answer;
            UpdateNonlinearStamp(stamp,answer_temp,answer);
            stamp.print("NR stamp:");
            answer = solveDC(stamp, index);
        } while (!convergent(answer, answer_temp, ERROR_ABS, ERROR_RELATIVE));
        
        answer.print("answer:");
        if(!SaveTranAnswer(time,answer,result))
        {
            break;
        }
    }
    if(Tran_result!=nullptr){
        delete Tran_result;
    }
    for (int i = 0; i < result->ValueList.size(); i++)
    {
        for (int j = 0; j <result->ValueList[i].size(); j++)
        {
            std::cout<<result->ValueList[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }
    
    Tran_result = result;
    return;
}

void circuit::setReferenceNode()
{
    if(NodesName.contains("gnd"))
        ReferenceNode = "gnd";
    else if(NodesName.contains("0"))
        ReferenceNode = "0";
    else 
        return;
}

mat circuit::SetInitialState()
{
    int n = MatrixOrder.size();
    mat RHS(n,1);
    RHS.zeros();
    foreach(const VSource &element, VSourceList)
    {
        RHS(MatrixOrder["I_"+ element.Name]) = getVoltage(&element,0);
    }
    return RHS;
}



bool circuit::UpdateDCStamp_Vsource(double v,QString Name, mat &stamp)
{
    if(!MatrixOrder.contains(Name)){
        qDebug()<<"Error: Circuit has no element named:"<<Name<<"! Please check your netlist!";
        return false;
    }
    printNodesInfo();
    int m = MatrixOrder["I_"+Name];
    int n = stamp.n_cols;

    stamp(m,n-1) = v;
    return true;
}

void circuit::UpdateACStamp(double f,cx_mat &stamp)
{
    int m = stamp.n_rows;
    int n = stamp.n_cols;
    mat imag(m,n);
    foreach(const DynamicElement &element, DynamicList)
    {
        switch (element.type)
        {
        case 'c':
        {
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -2 * datum::pi * f * element.value;
            imag(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 2 * datum::pi * f * element.value;
            break;
        }
        case 'l':
        {
            imag(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Name]) += -2 * datum::pi * f * element.value;
            break;
        }
        default:
            break;
        }
    }
    stamp.set_imag(imag);
}



void circuit::UpdateTranStamp(mat &stamp, double time, double h, struct Tran_result *result)
{
    int col = result->ValueList.size()-1;
    int col_pre = (col==0)? 0:col-1;
    int n = stamp.n_cols;
    printNodesInfo();
    
    
    if (h==0)
    {
        qDebug()<<"Error: tran step can not be 0!";
        return;
    }
    
    foreach (const VSource &element, VSourceList)
    {
        //stamp(MatrixOrder["I_" + element.Name],n-1) += -getVoltage(&element,time-h);
        stamp(MatrixOrder["I_" + element.Name],n-1) = getVoltage(&element,time);
    }
    foreach (const ISource &element, ISourceList)
    {
        stamp(MatrixOrder[element.Nodes[0]],n-1) += getCurrent(&element,time-h);
        stamp(MatrixOrder[element.Nodes[0]],n-1) += -getCurrent(&element,time);

        stamp(MatrixOrder[element.Nodes[1]],n-1) +=  -getCurrent(&element,time-h);
        stamp(MatrixOrder[element.Nodes[1]],n-1) +=  getCurrent(&element,time);
    }
    foreach(const DynamicElement &element, DynamicList)
    {
        switch (element.type)
        {
        case 'c':
        {
            double V_t_2h = result->ValueList[col_pre][MatrixOrder[element.Nodes[0]]] - result->ValueList[col_pre][MatrixOrder[element.Nodes[1]]];
            double V_t_h = result->ValueList[col][MatrixOrder[element.Nodes[0]]] - result->ValueList[col][MatrixOrder[element.Nodes[1]]];
            stamp(MatrixOrder[element.Nodes[0]], n-1) += element.value / h * V_t_h;
            stamp(MatrixOrder[element.Nodes[0]], n-1) += -element.value / h * V_t_2h;
            stamp(MatrixOrder[element.Nodes[1]], n-1) += -element.value / h * V_t_h;
            stamp(MatrixOrder[element.Nodes[1]], n-1) += element.value / h * V_t_2h;
            break;
        }
        case 'l':
        {
            double i_t_2h = result->ValueList[col_pre][MatrixOrder["I_" + element.Name]];
            double i_t_h = result->ValueList[col][MatrixOrder["I_" + element.Name]];
            stamp(MatrixOrder["I_" + element.Name], n-1) += -element.value/h * i_t_h;
            stamp(MatrixOrder["I_" + element.Name], n-1) += element.value/h * i_t_2h;
            break;
        }
        default:
            break;
        }
    }
    return ;
}




/* mat circuit::UpdateTranStamp(double time, double h, struct Tran_result *result)
{
    int col = (int)((time-h)/h);
    int n = MatrixOrder.size();
    qDebug()<<"Matrix size:"<<n;
    printNodesInfo();
    mat stamp(n, n);   
    mat RHS(n,1);
    stamp.zeros();
    RHS.zeros();
    if (h==0)
    {
        qDebug()<<"Error: tran step can not be 0!";
        return join_horiz(stamp, RHS);
    }
    foreach (const StaticElement &element, StaticList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += 1 / element.value;
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -1 / element.value;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += 1 / element.value;
    }
    foreach (const VSource &element, VSourceList)
    {
        stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
        stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
        stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
        RHS(MatrixOrder["I_" + element.Name]) = getVoltage(&element,time);
    }
    foreach (const ISource &element, ISourceList)
    {
        RHS(MatrixOrder[element.Nodes[0]]) += -getCurrent(&element,time);
        RHS(MatrixOrder[element.Nodes[1]]) +=  getCurrent(&element,time);
    }
    foreach(const DynamicElement &element, DynamicList)
    {
        switch (element.type)
        {
        case 'c':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[0]]) += element.value/h;
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder[element.Nodes[1]]) += -element.value/h;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[0]]) += -element.value/h;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder[element.Nodes[1]]) += element.value/h;
            double V_t_h = result->ValueList[col][MatrixOrder[element.Nodes[0]]] - result->ValueList[col][MatrixOrder[element.Nodes[1]]];
            RHS(MatrixOrder[element.Nodes[0]]) += element.value / h * V_t_h;
            RHS(MatrixOrder[element.Nodes[1]]) += -element.value / h * V_t_h;
            break;
        }
        case 'l':
        {
            stamp(MatrixOrder[element.Nodes[0]], MatrixOrder["I_" + element.Name]) += 1;
            stamp(MatrixOrder[element.Nodes[1]], MatrixOrder["I_" + element.Name]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[0]]) += 1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder[element.Nodes[1]]) += -1;
            stamp(MatrixOrder["I_" + element.Name], MatrixOrder["I_" + element.Name]) += -element.value/h;
            double i_t_h = result->ValueList[col][MatrixOrder["I_" + element.Name]];
            RHS(MatrixOrder["I_" + element.Name]) += -element.value/h * i_t_h;
            break;
        }
        default:
            break;
        }
    }
    return join_horiz(stamp, RHS);
}


*/


void circuit::UpdateNonlinearStamp(mat &stamp, mat answer0 , mat answer1)
{
    foreach(Diode element, DiodeList)
    {
        int m = stamp.n_rows;
        int n = stamp.n_cols;
        int N_p = MatrixOrder[element.Nodes[0]];
        int N_m = MatrixOrder[element.Nodes[1]];
        double VN_p0 = answer0(N_p,0);
        double VN_m0 = answer0(N_m,0);
        double VN_p1 = answer1(N_p,0);
        double VN_m1 = answer1(N_m,0);
        

        qDebug()<<"N_p = "<< N_p;
        qDebug()<<"N_m = "<< N_m;

        qDebug()<<"VN_p0 = "<< VN_p0;
        qDebug()<<"VN_m0 = "<< VN_m0;

        qDebug()<<"VN_p1 = "<< VN_p1;
        qDebug()<<"VN_m1 = "<< VN_m1;

        qDebug()<<"I'(V0) = "<< element.I_differential(VN_p0, VN_m0);
        qDebug()<<"I'(V1) = "<< element.I_differential(VN_p1, VN_m1);

        qDebug()<<"I(V0) = "<< element.I(VN_p0, VN_m0);
        qDebug()<<"I(V1) = "<< element.I(VN_p1, VN_m1);

        double Gn = element.I_differential(VN_p0, VN_m0);
        double Gn1 = element.I_differential(VN_p1, VN_m1);

        double Jn = element.I(VN_p0, VN_m0) - Gn*(VN_p0 - VN_m0);
        double Jn1 = element.I(VN_p1, VN_m1) - Gn1*(VN_p1 - VN_m1);

        qDebug()<<"Gn:\t"<<Gn<<"\tGn+1:\t"<<Gn1;
        qDebug()<<"Jn:\t"<<Jn<<"\tJn+1:\t"<<Jn1;
        stamp(N_p,N_p) += -Gn + Gn1;
        stamp(N_p, N_m) += Gn - Gn1;
        stamp(N_m,N_p) += Gn - Gn1;
        stamp(N_m,N_m) += -Gn + Gn1;

        stamp(N_p, n-1) += Jn - Jn1;
        stamp(N_m, n-1) += -Jn + Jn1;
    }
}
circuit::circuit(void)
{
    C_nums = 0;
    L_nums = 0;
    R_nums = 0;
    V_nums = 0;
    I_nums = 0;
    Dependent_nums = 0;
    Diode_nums = 0;
    DC_result = nullptr;
    AC_result = nullptr;
    Tran_result = nullptr;
    PlotInFo = nullptr;
}
circuit::~circuit(void)
{
    if(DC_result!=nullptr)
    {
        delete DC_result;
        DC_result = nullptr;
    }
    if(AC_result!=nullptr)
    {
        delete AC_result;
        AC_result = nullptr;
    }
    if(Tran_result!=nullptr)
    {
        delete Tran_result;
        Tran_result = nullptr;
    }
    if(PlotInFo!=nullptr)
    {
        delete PlotInFo;
        PlotInFo = nullptr;
    }

}
