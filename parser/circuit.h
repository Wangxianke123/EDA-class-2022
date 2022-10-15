#ifndef CIRCUIT_H

#define CIRCUIT_H
#include <QStringList>
#include <QString>
#include <QSet>
#include <QVector>
#include <QDebug>
struct TwoSideElement
{
    TwoSideElement(char type,QString Name):type(type),Name(Name){};
    char  type;  //r,c,l,v,etc
    QString Name;
    QString Nodes[2];
    double  value;
};

class circuit
{
public:
    QString CircuitTitle;
    QString CircuitAnnotation;
    QSet<QString>   NodesName;
    QSet<QString>   ResistorName;
    QSet<QString>   CapacitorName;
    QSet<QString>   InductanceName;
    QSet<QString>   VSourceName;

    QVector<TwoSideElement> TwoSideElementList;
    int DeviceCounts;

    void getTitle(QString title);
    void getAnnotation(QString annotation);


    bool AddResistor(QString Resistor);
    bool AddCapacitor(QString Capacitor);
    bool AddInductance(QString Inductance);
    bool AddVoltageSource(QString VSource);
    

    QString printTitle();
    void printInfo();
    QString printAnnotation();
    QStringList printElementInfo();
    void printCommand();

    void Update();

    
    circuit();
    ~circuit();
};


void circuit::getTitle(QString title){
    CircuitTitle = title;
    return;
}

void circuit::getAnnotation(QString annotation){
    CircuitAnnotation = annotation;
    return;
}


QString circuit::printTitle(){
    if(CircuitTitle.isNull()){
        return "Circuit has no title";
    }
    else
        return  CircuitTitle;
}


void circuit::printInfo(){
    printTitle();
    printAnnotation();
    printElementInfo();
    printCommand();
}

QString circuit:: printAnnotation(){
    return CircuitAnnotation;
}

QStringList circuit:: printElementInfo(){
        Update();
        QStringList  list;
        list <<"Here's the result of parser:";
        list<<"-----------------------------------------------------";
        list<<"Title:";
        list << printTitle();
        list<<"Nodes:"+ QString("%1").arg(NodesName.size());
//        foreach (const QString &value,NodesName){
//            qDebug()<<value;
//        }

        list<<"Devices:"+ QString("%1").arg(DeviceCounts);
        list <<"Resistor:" + QString("%1").arg(ResistorName.size())+"   Capacitor:"+QString("%1").arg(CapacitorName.size())+"    Inductance:"+QString("%1").arg(InductanceName.size());
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

        list<<"Vsrc:"+QString("%1").arg(VSourceName.size());
//        foreach (const QString &value,VSourceName){
//            qDebug()<<value;
//        }

        foreach (const TwoSideElement &value,TwoSideElementList){
            list<<"Name:"+QString("%1").arg(value.Name)+"    Node1:"+QString("%1").arg(value.Nodes[0])+"     Node2:"+QString("%1").arg(value.Nodes[1]);
        }

        list<<"Parsing finished!";
        list<<"-------------------------------------------------------------------";
        return list;
}

void circuit:: printCommand(){

}


bool circuit::AddResistor(QString Resistor){
    QRegExp node("([^Rr]\\w+)");
    QRegExp res("([Rr]\\w+)");
    int pos = 0;
    if ((pos = res.indexIn(Resistor, pos)) != -1) {
            if(ResistorName.contains(res.cap(1))){
                return false;
            }
            else{
	   		    ResistorName.insert(res.cap(1));
                TwoSideElement temp('r',res.cap(1));
                TwoSideElementList.push_back(temp);
	    	    pos += res.matchedLength();
            }
	}	
	for (int i=0;i<=1;++i){
		if ((pos = node.indexIn(Resistor, pos)) != -1) {
	   		NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
	    	pos += node.matchedLength();
		}
	}
    return true;
}


bool circuit::AddCapacitor(QString Capacitor){
    QRegExp node("([^Cc]\\w+)");
    QRegExp res("([Cc]\\w+)");
    int pos = 0;
    if ((pos = res.indexIn(Capacitor, pos)) != -1) {
            if(CapacitorName.contains(res.cap(1))){
                return false;
            }
            else{
	   		    CapacitorName.insert(res.cap(1));
                TwoSideElement temp('c',res.cap(1));
                TwoSideElementList.push_back(temp);
	    	    pos += res.matchedLength();
            }
	}	
	for (int i=0;i<=1;++i){
		if ((pos = node.indexIn(Capacitor, pos)) != -1) {
	   		NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
	    	pos += node.matchedLength();
		}
	}
    return true;
}

bool circuit::AddInductance(QString Inductance){
    QRegExp node("([^Ll]\\w+)");
    QRegExp res("([Ll]\\w+)");
    int pos = 0;
    if ((pos = res.indexIn(Inductance, pos)) != -1) {
            if(InductanceName.contains(res.cap(1))){
                return false;
            }
            else{
	   		    InductanceName.insert(res.cap(1));
                TwoSideElement temp('l',res.cap(1));
                TwoSideElementList.push_back(temp);
	    	    pos += res.matchedLength();
            }
    }	
	for (int i=0;i<=1;++i){
		if ((pos = node.indexIn(Inductance, pos)) != -1) {
	   		NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
	    	pos += node.matchedLength();
		}
	}
    return true;
}

bool circuit::AddVoltageSource(QString VSource){
    QRegExp node("([^Vv]\\w+)");
    QRegExp res("([Vv]\\w+)");
    int pos = 0;
    if ((pos = res.indexIn(VSource, pos)) != -1) {
            if(VSourceName.contains(res.cap(1))){
                return false;
            }
            else{
	   		    VSourceName.insert(res.cap(1));
                TwoSideElement temp('v',res.cap(1));
                TwoSideElementList.push_back(temp);
	    	    pos += res.matchedLength();
            }
    }	

	for (int i=0;i<=1;++i){
		if ((pos = node.indexIn(VSource, pos)) != -1) {
	   		NodesName.insert(node.cap(1));
            TwoSideElementList.last().Nodes[i] = node.cap(1);
	    	pos += node.matchedLength();
		}
	}
    return true;
}
void circuit::Update(){
    DeviceCounts = ResistorName.size() + CapacitorName.size() + InductanceName.size();
}

circuit::circuit(void){

}
circuit::~circuit(void){

}
#endif