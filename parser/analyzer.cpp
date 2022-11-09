#include"analyzer.h"
#include"element/element.h"
#include<QDate>

circuit *parse(QString Netlist)
{
    if (Netlist.isEmpty())
    {
        qDebug()<<"Error: Empty Netlist!";
        return nullptr;
    }
    QStringList list = Netlist.split('\n');

    circuit *newCircuit = new circuit;

    QDateTime dt;
	QDate date;
	dt.setDate(date.currentDate());
	QString currentDate = dt.toString("yyyy-MM-dd\t");
	QTime time;
	dt.setTime(time.currentTime());
	QString currentTime = dt.toString("hh:mm:ss\t");


    QStringList ParsedInfo;

    ParsedInfo << "Date:\t"+ currentDate + "Time:\t" + currentTime;
    QStringList ErrorInfo;
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
            newCircuit->getTitle(str);
            break;
        }
        case IS_ANNOTATION:
        {
            newCircuit->getAnnotation(str);
            ParsedInfo << "#grep an annotation:" << str;
            break;
        }
        case IS_COMMAND:
        {
            ParsedInfo << "#grep a command:" << str;
            newCircuit->AddCommand(str);
            break;
        }
        case IS_RESISTOR:
        {
            ParsedInfo << "#grep a resistor:" << str;
            struct StaticElement* element = ParseStatic(str);
            if (!newCircuit->addStatic(element))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CAPACITOR:
        {
            qDebug() << "#grep a capacitor:" << str;
            struct DynamicElement* element = ParseDynamic(str);
            if (!newCircuit->addDynamic(element))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VOLTAGE_SOURCE:
        {
            ParsedInfo << "#grep a source:" << str;
            struct VSource* element = ParseVSource(str);
            if (!newCircuit->addVSource(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CURRENT_SOURCE:
        {
            ParsedInfo << "#grep a source:" << str;
            struct ISource* element = ParseISource(str);
            if (!newCircuit->addISource(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_INDUCTANCE:
        {
            ParsedInfo << "#grep an inductance:" << str;
            struct DynamicElement* element = ParseDynamic(str);
            if (!newCircuit->addDynamic(element))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VCCS://G
        {
            ParsedInfo << "#grep a dependent source:" << str;
            struct DependentElement* element = ParseDependent_VC(str,'G');
            if (!newCircuit->addDependent_VC(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VCVS://E
        {
            ParsedInfo << "#grep a dependent source:" << str;
            struct DependentElement* element = ParseDependent_VC(str, 'E');
            if (!newCircuit->addDependent_VC(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CCCS://F
        {
            ParsedInfo << "#grep a dependent source:" << str;
            struct DependentElement* element = ParseDependent_CC(str,'F');
            if (!newCircuit->addDependent_CC(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CCVS://H
        {
            ParsedInfo << "#grep a dependent source:" << str;
            struct DependentElement* element = ParseDependent_CC(str, 'H');
            if (!newCircuit->addDependent_CC(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_DIODE:
        {
            ParsedInfo << "#grep a diode:" << str;
            Diode *element = ParseDiode(str);
            if (!newCircuit->addDiode(element))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        default:
            QString temp = "error: line " + QString("%1").arg(i + 1) + ":fail to parse\'" + str + "\'";
            ErrorInfo << temp;
            break;
        }
    }
    QStringList ElementInfo = newCircuit->printElementInfo();
    QStringList ParserResult = ParsedInfo + ElementInfo + ErrorInfo;

    QFile file("transcript");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ParserResult.join('\n');
        out <<"\n";
        file.close();
    }
    return newCircuit;
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

struct ACInFo* ParseACInFo(QString str)
{
    //.AC DEC 10 1 100MEG
    qDebug()<<str;
    QRegExp SweepType("([^\\.]\\w+)");
    QRegExp value("(\\d*[\\.]?\\d+((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    char  sweep_type;
    int   sweep_nums;
    double start,stop;
    int pos = 0;
    if ((pos = SweepType.indexIn(str, pos)) != -1)
	{
		QString temp= SweepType.cap(1).simplified();
        pos += SweepType.matchedLength();
        if(temp=="DEC")
            sweep_type = 'd';
        if(temp=="OCT")
            sweep_type = 'o';
        if(temp=="LEN")
            sweep_type = 'l';   
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }

    if ((pos = value.indexIn(str, pos)) != -1)
	{
        sweep_nums = (int) StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }
    if ((pos = value.indexIn(str, pos)) != -1)
	{
        start =  StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }
    if ((pos = value.indexIn(str, pos)) != -1)
	{
        stop =  StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }
    qDebug()<<"type:"<<sweep_type;
	qDebug()<<"nums:"<<sweep_nums;
	qDebug()<<"start:"<<start;
	qDebug()<<"stop:"<<stop;
    struct ACInFo *result = new struct ACInFo(sweep_type,sweep_nums,start,stop);
    return result;
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
    else if (IsCurrentSource(str))
        return IS_CURRENT_SOURCE;
    else if (IsInductance(str))
        return IS_INDUCTANCE;
    else if (IsVCVS(str))
        return IS_VCVS;
    else if (IsCCCS(str))
        return IS_CCCS;
    else if (IsVCCS(str))
        return IS_VCCS;
    else if (IsCCVS(str))
        return IS_CCVS;
    else if (ISDiode(str))
        return IS_DIODE;
    else if (IsEmpty(str))
        return IS_EMPTY;
    else if (line == 0)
        return IS_TITLE;
    else
        return FAIL_TO_ANALYZE;
}

bool IsAnnotation(QString str)
{
    QRegExp Annotation("^(\\*).*");
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
    QRegExp Annotation("^(\\.).*");
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
    QRegExp Resistor("^[Rr]\\w*\\s+\\w+\\s+\\w+\\s+\\d+(\\.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
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
    QRegExp Source1("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
	QRegExp Source2("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+((pulse)|(PULSE))?\\s*[\\(](\\s*\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)+[\\)]\\s*$");
    QRegExp Source3("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?(\\d*\\s*)?(tran)?\\s*(const\\(\\w+\\))?\\s*$");
    QRegExp Source4("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?((tran)|(TRAN))?\\s*(sin)?\\s*\\((\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*)+\\)\\s*$");
    return (Source1.exactMatch(str) || Source2.exactMatch(str) || Source3.exactMatch(str)  || Source4.exactMatch(str));
}

bool IsCurrentSource(QString str)
{
    QRegExp Source1("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
	QRegExp Source2("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+((pulse)|(PULSE))?\\s*[\\(](\\s*\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)+[\\)]\\s*$");
    QRegExp Source3("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?(\\d*\\s*)?(tran)?\\s*(const\\(\\w+\\))?\\s*$");
    QRegExp Source4("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?((tran)|(TRAN))?\\s*(sin)?\\s*\\((\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*)+\\)\\s*$");

	return (Source1.exactMatch(str) || Source2.exactMatch(str) || Source3.exactMatch(str) || Source4.exactMatch(str));
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

bool IsVCCS(QString str) // G
{
    QRegExp Source("^[Gg]\\w*\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
    int pos = 0;
    if ((pos = Source.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}
bool IsVCVS(QString str) // E
{
    QRegExp Source("^[Ee]\\w*\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
    int pos = 0;
    if ((pos = Source.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}
bool IsCCCS(QString str) // F
{
    QRegExp Source("^[Ff]\\w*\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
    int pos = 0;
    if ((pos = Source.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}
bool IsCCVS(QString str) // H
{
    QRegExp Source("^[Hh]\\w*\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
    int pos = 0;
    if ((pos = Source.indexIn(str, pos)) != -1)
    {
        //        qDebug()<<"get a resistor:"<<str;
        return true;
    }
    else
        return false;
}

bool ISDiode(QString str)
{
    QRegExp Diode("^[Dd]\\w*\\s+\\w+\\s+\\w+\\s+diode\\s*$");
    int pos = 0;
    if ((pos = Diode.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else
        return false;
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

bool IsTranCommand(QString str){
    QRegExp Tran("([.][Tt][Rr][Aa][Nn])");
    int pos = 0;
    if ((pos = Tran.indexIn(str, pos)) != -1)
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

bool IsOpCommand(QString str)
{
    QRegExp plot("([.][Oo][Pp])");
    int pos = 0;
    if ((pos = plot.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else    
        return false;
}

bool IsPrintCommand(QString str)
{
    QRegExp print("([.][Pp][Rr][Ii][Nn][Tt])");
    int pos = 0;
    if ((pos = print.indexIn(str, pos)) != -1)
    {
        return true;
    }
    else    
        return false;
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
    QRegExp PureNum("(\\d*[.]?\\d+)");
    QRegExp WithUnit("(\\d*[.]?\\d+((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    QRegExp Scientific("(\\d+[.]?\\d*(e|(e-))\\d*)");
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
        bool isMeg = (str.right(3) == "Meg" || str.right(3) == "MEG");
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

QStringList printMatrix(cx_mat A)
{
    QStringList content;
    for (long long unsigned int i = 0; i < A.n_rows; i++)
    {
        QString temp = "[";
        for (long long unsigned int j = 0; j < A.n_cols; j++)
        {
            temp += "\t\t\'" + QString("%1 +").arg(A(i, j).real()) + QString("%1").arg(A(i, j).imag()) + "j\'";
            if (j != A.n_cols - 1)
                temp += ",\t\t";
        }
        temp += "]";
        content << temp;
    }
    return content;
}

QStringList printReal(cx_mat A)
{
    QStringList content;
    for (long long unsigned int i = 0; i < A.n_rows; i++)
    {
        QString temp = "[";
        for (long long unsigned int j = 0; j < A.n_cols; j++)
        {
            temp += "\t\t\'" + QString("%1 ").arg(A(i, j).real()) + "\'";
            if (j != A.n_cols - 1)
                temp += ",\t\t";
        }
        temp += "]";
        content << temp;
    }
    return content;
}

QStringList printMatrix(mat A)
{
    QStringList content;
    for (long long unsigned int i = 0; i < A.n_rows; i++)
    {
        QString temp = "[";
        for (long long unsigned int j = 0; j < A.n_cols; j++)
        {
            temp += "\t\t\'" + QString("%1 ").arg(A(i, j)) + "\'";
            if (j != A.n_cols - 1)
                temp += ",\t\t";
        }
        temp += "]";
        content << temp;
    }
    return content;
}


struct VSource* ParseVSource(QString str)
{
    QRegExp Source1("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[Dd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
	QRegExp Source2("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+((pulse)|(PULSE))?\\s*[\\(](\\s*\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)+[\\)]\\s*$");
    QRegExp Source3("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[Aa]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
    QRegExp Source4("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?(\\d*\\s*)?((tran)|(TRAN))?\\s*(const\\(\\w+\\))?\\s*$");
    QRegExp Source5("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+((tran)|(TRAN))?\\s*(sin)?\\s*\\((\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*)+\\)\\s*$");

    if(Source1.exactMatch(str)||Source4.exactMatch(str)){
        return ParseVSource_DC(str);
    }
    else if(Source2.exactMatch(str)){
        return ParseVSource_pulse(str);
    }
    else if(Source3.exactMatch(str)){
        return ParseVSource_AC(str);
    }
    else if(Source5.exactMatch(str)){
        return ParseVSource_sin(str);
    }
    else return nullptr;
}


struct VSource* ParseVSource_DC(QString str)
{
    QRegExp node("([^Vv]\\w+)");
    QRegExp name("([Vv]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct VSource* element = new struct VSource('d');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct VSource* ParseVSource_AC(QString str)
{
    QRegExp node("([^Vv]\\w+)");
    QRegExp name("([Vv]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct VSource* element = new struct VSource('a');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->phase = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct VSource* ParseVSource_pulse(QString str)
{
    QRegExp node("([^Vv]\\w+)");
    QRegExp name("([Vv]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct VSource* element = new struct VSource('p');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    double valuelist[7];
	for (int i = 0; i < 7; ++i)
    {
        if ((pos = value.indexIn(str, pos)) != -1)
        {
    
            valuelist[i]= StringToNum(value.cap(1).simplified());
            pos += value.matchedLength();
        }
    }
	element->v_initial=valuelist[0];
    element->v_peak=valuelist[1];
    element->t_delay=valuelist[2];
    element->t_rise = valuelist[3];
    element->t_fall = valuelist[4];
    element->pulse_width = valuelist[5];
    element->period = valuelist[6];
    return element;
}

struct VSource* ParseVSource_sin(QString str)
{
    QRegExp node("([^Vv]\\w+)");
    QRegExp name("([Vv]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct VSource* element = new struct VSource('s');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }

    QVector<double> valuelist;
    while ((pos = value.indexIn(str, pos)) != -1)
    {
        valuelist.push_back(StringToNum(value.cap(1).simplified()));
        pos += value.matchedLength();
    }
    for (int i = 0; i < valuelist.size(); i++)
    {
        switch (i)
        {
        case 0:
            element->v_initial = valuelist[i];
            break;
        case 1:
            element->v_peak = valuelist[i];
            break;
        case 2:
            element->period = 1/valuelist[i];
            break;
        case 3:
            element->t_delay = valuelist[i];
            break;
        case 4:
            element->phase = valuelist[i];
            break;
        case 5:
            element->dumping_factor = valuelist[i];
            break;
        default:
            break;
        }
    }
    return element;
}


struct ISource* ParseISource(QString str)
{
    QRegExp Source1("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[Dd]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
	QRegExp Source2("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+((pulse)|(PULSE))?\\s*[\\(](\\s*\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)+[\\)]\\s*$");
    QRegExp Source3("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[Aa]?[Cc]?\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*$");
    QRegExp Source4("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+\\s*\\d+(.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?(\\d*\\s*)?((tran)|(TRAN))?\\s*(const\\(\\w+\\))?\\s*$");
    QRegExp Source5("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+((tran)|(TRAN))?\\s*(sin)?\\s*\\((\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?\\s*)+\\)\\s*$");

    if(Source1.exactMatch(str)||Source4.exactMatch(str)){
        return ParseISource_DC(str);
    }
    else if(Source2.exactMatch(str)){
        return ParseISource_pulse(str);
    }
    else if(Source3.exactMatch(str)){
        return ParseISource_AC(str);
    }
    else if(Source5.exactMatch(str)){
        return ParseISource_sin(str);
    }
    else return nullptr;
}


struct ISource* ParseISource_DC(QString str)
{
    QRegExp node("([^Ii]\\w+)");
    QRegExp name("([Ii]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct ISource* element = new struct ISource('d');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct ISource* ParseISource_AC(QString str)
{
    QRegExp node("([^Ii]\\w+)");
    QRegExp name("([Ii]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct ISource* element = new struct ISource('a');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->phase = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct ISource* ParseISource_pulse(QString str)
{
    QRegExp node("([^Ii]\\w+)");
    QRegExp name("([Ii]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct ISource* element = new struct ISource('p');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    double valuelist[7];
	for (int i = 0; i < 7; ++i)
    {
        if ((pos = value.indexIn(str, pos)) != -1)
        {
    
            valuelist[i]= StringToNum(value.cap(1).simplified());
            pos += value.matchedLength();
        }
    }
	element->v_initial=valuelist[0];
    element->v_peak=valuelist[1];
    element->t_delay=valuelist[2];
    element->t_rise = valuelist[3];
    element->t_fall = valuelist[4];
    element->pulse_width = valuelist[5];
    element->period = valuelist[6];
    return element;
}

struct ISource* ParseISource_sin(QString str)
{
    QRegExp node("([^Ii]\\w+)");
    QRegExp name("([Ii]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct ISource* element = new struct ISource('s');
	int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
	for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }

    QVector<double> valuelist;
    while ((pos = value.indexIn(str, pos)) != -1)
    {
        valuelist.push_back(StringToNum(value.cap(1).simplified()));
        pos += value.matchedLength();
    }
    for (int i = 0; i < valuelist.size(); i++)
    {
        switch (i)
        {
        case 0:
            element->v_initial = valuelist[i];
            break;
        case 1:
            element->v_peak = valuelist[i];
            break;
        case 2:
            element->period = 1/valuelist[i];
            break;
        case 3:
            element->t_delay = valuelist[i];
            break;
        case 4:
            element->phase = valuelist[i];
            break;
        case 5:
            element->dumping_factor = valuelist[i];
            break;
        default:
            break;
        }
    }
    return element;
}


struct DynamicElement* ParseDynamic(QString str)
{
    QRegExp node("([^CcLl]\\w+)");
    QRegExp name("([CcLl]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct DynamicElement* element = new (struct DynamicElement);
    int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
    if (element->Name[0]=='C'||element->Name[0]=='c')
    {
        element->type = 'c';
    }
    else{
        element->type = 'l';
    }
    
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->ValueInString = value.cap(1).simplified();
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct StaticElement* ParseStatic(QString str)
{
    QRegExp node("([^Rr]\\w+)");
    QRegExp res("([Rr]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct StaticElement* element = new (struct StaticElement);
    int pos = 0;
    if ((pos = res.indexIn(str, pos)) != -1)
    {
        element->Name = res.cap(1).simplified();
        pos += res.matchedLength();

    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->ValueInString = value.cap(1).simplified();
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}



struct DependentElement* ParseDependent_VC(QString str,char type)
{
    QRegExp name("([EeGg]\\w+)");
    QRegExp node("([^EeGg]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct DependentElement* element = new (struct DependentElement);
    element->type = type;
    int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();
    }
    for (int i = 0; i <= 3; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->ValueInString = value.cap(1).simplified();
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct DependentElement* ParseDependent_CC(QString str,char type)
{
    QRegExp name("([FfHh]\\w+)");
    QRegExp node("([^FfHh]\\w+)");
    QRegExp value("(\\d+(\\.|(e-))?\\d*((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    struct DependentElement* element = new (struct DependentElement);
    element->type = type;
    int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();
    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    if ((pos = node.indexIn(str, pos)) != -1)
    {
            element->Controler = node.cap(1).simplified();
            pos += node.matchedLength();
    }
    if ((pos = value.indexIn(str, pos)) != -1)
    {
        element->ValueInString = value.cap(1).simplified();
        element->value = StringToNum(value.cap(1).simplified());
        pos += value.matchedLength();
    }
    return element;
}

struct DCInFo* ParseDCInFo(QString str)
{
    struct DCInFo* dcinfo = new(struct DCInFo);
    QRegExp Sourcename("([^.DdCc\\s]\\w+)");
    QRegExp Value("(\\d*[.]?\\d+)");
    int pos = 0;
    if ((pos = Sourcename.indexIn(str, pos)) != -1)
	{
		dcinfo->Name = Sourcename.cap(1).simplified();
        pos += Sourcename.matchedLength();
        
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		dcinfo->start = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		dcinfo->stop = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		dcinfo->step = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }
    return dcinfo;
}

struct PlotInFo* ParsePlotInFo(QString str){
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
        struct PlotInFo *plotinfo = new (struct PlotInFo);
        plotinfo->type = 'a';
        while ((pos_DC = Name.indexIn(str, pos_DC)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            //struct PlotInFo *plotinfo = new struct PlotInFo('d',VariableName);
            plotinfo->VariableName.push_back(VariableName);
            pos_DC += Name.matchedLength();
        }
        return plotinfo;
	}

    else if ((pos_AC = AC.indexIn(str, pos_AC)) != -1)
	{
        pos_AC += AC.matchedLength();
        struct PlotInFo *plotinfo = new (struct PlotInFo);
        plotinfo->type = 'a';
        while ((pos_AC = Name.indexIn(str, pos_AC)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            plotinfo->VariableName.push_back(VariableName);
            pos_AC += Name.matchedLength();
        }
        return plotinfo;   
	}

    else if ((pos_TRAN = tran.indexIn(str, pos_TRAN)) != -1)
	{
        struct PlotInFo *plotinfo = new (struct PlotInFo);
        pos_TRAN += tran.matchedLength();
        while ((pos_TRAN = Name.indexIn(str, pos_TRAN)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            plotinfo->type = 't';
            plotinfo->VariableName.push_back(VariableName);
            pos_TRAN += Name.matchedLength();        
        }
        return plotinfo;
    }    
      /*   if ((pos_TRAN = Name.indexIn(str, pos_TRAN)) != -1)
        {
            QString temp = Name.cap(1).simplified();
            VariableName = temp.right(temp.size()-1);
            VariableName = VariableName.left(VariableName.size()-1);
            struct PlotInFo *plotinfo = new struct PlotInFo('t',VariableName);
            return plotinfo;
        }
        else{
             qDebug()<<"Fail to parse Plot Command!";
        }       */
	

    else {
        qDebug()<<"Fail to parse Plot Command!";
    }
    return nullptr;
}


struct TranInFo* ParseTranInFo(QString str)
{
    struct TranInFo *traninfo = new(struct TranInFo);
    QRegExp Value("(\\d*[.]?\\d+((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
    int pos = 0;
    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		traninfo->t_step = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }

    if ((pos = Value.indexIn(str, pos)) != -1)
	{
		traninfo->t_stop = StringToNum(Value.cap(1).simplified());
        pos += Value.matchedLength();
	}
    else{
        qDebug()<<"Fail to parse DC Command!";
        return nullptr;
    }
    return traninfo;
}

struct PrintInFo* ParsePrintInfo(QString str)
{
    QRegExp Name("(\\(\\w+\\))");
    QString VariableName;
    int pos = 0;
    struct PrintInFo* info = new (struct PrintInFo);
    while ((pos = Name.indexIn(str, pos)) != -1)
    {
        QString temp = Name.cap(1).simplified();
        VariableName = temp.right(temp.size()-1);
        VariableName = VariableName.left(VariableName.size()-1);
        info->Nodes.push_back(VariableName);
        pos += Name.matchedLength();
    }
    return info;
}

Diode* ParseDiode(QString str)
{
    QRegExp name("([Dd]\\w+)");
    QRegExp node("([^Dd]\\w+)");
    Diode* element = new Diode;
    int pos = 0;
    if ((pos = name.indexIn(str, pos)) != -1)
    {
        element->Name = name.cap(1).simplified();
        pos += name.matchedLength();

    }
    for (int i = 0; i <= 1; ++i)
    {
        if ((pos = node.indexIn(str, pos)) != -1)
        {
    
            element->Nodes[i] = node.cap(1).simplified();
            pos += node.matchedLength();
        }
    }
    return element;
}