#include"analyzer.h"

circuit *parse(QString Netlist)
{
    QStringList list = Netlist.split('\n');
    if (list.size() == 0)
    {
        return nullptr;
    }
    //    qDebug() << strRead;

    circuit *newCircuit = new circuit;
    QStringList ParsedInfo;
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
            if (!newCircuit->AddResistor(str))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CAPACITOR:
        {
            qDebug() << "#grep a capacitor:" << str;
            if (!newCircuit->AddCapacitor(str))
            {

                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VOLTAGE_SOURCE:
        {
            ParsedInfo << "#grep a source:" << str;
            if (!newCircuit->AddVoltageSource(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CURRENT_SOURCE:
        {
            ParsedInfo << "#grep a source:" << str;
            if (!newCircuit->AddCurrentSource(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_INDUCTANCE:
        {
            ParsedInfo << "#grep an inductance:" << str;
            if (!newCircuit->AddInductance(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VCCS:
        {
            ParsedInfo << "#grep a VCCS:" << str;
            if (!newCircuit->AddVCCS(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_VCVS:
        {
            ParsedInfo << "#grep a VCVS:" << str;
            if (!newCircuit->AddVCVS(str))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CCCS:
        {
            ParsedInfo << "#grep a CCCS:" << str;
            QString next = list[i + 1];
            if (!newCircuit->AddCCCS(str, next))
            {
                QString temp = "error: line " + QString("%1").arg(i + 1) + ":Element already exsists! \'" + str + "\'";
                ErrorInfo << temp;
            }
            break;
        }
        case IS_CCVS:
        {
            ParsedInfo << "#grep a CCVS:" << str;
            QString next = list[i + 1];
            if (!newCircuit->AddCCVS(str, next))
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
    QRegExp SweepType("([^.]\\w+)");
    QRegExp value("(\\d*[.]?\\d+((MEG)|(Meg)|[FfTtpPnNuUkKMm])?)");
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
    QRegExp Resistor("^[Rr]\\w*\\s+\\w+\\s+\\w+\\s+\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
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
    QRegExp Source("^[Vv]\\w*\\s+\\w+\\s+\\w+\\s+[AaDd]?[Cc]?\\s*\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
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

bool IsCurrentSource(QString str)
{
    QRegExp Source("^[Ii]\\w*\\s+\\w+\\s+\\w+\\s+[AD]?C?\\s*\\d+(.|(e-))?\\d*([FfTtpPnNuUmMkK]|(Meg))?\\s*$");
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


