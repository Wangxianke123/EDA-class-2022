#ifndef ELEMENT_H
#define ELEMENT_H
#include<QString>
#include<QChar>

struct VSource  //vDC,vAC,vPulse
{
    QString Name;
    QString Nodes[2];
    char type;

    double value=0;//for DC & AC
    double phase=0;//for AC only
    
    // for vpulse only 
    double v_initial=0;
    double v_peak=0;
    double t_delay=0;
    double t_rise = 0;
    double t_fall = 0;
    double pulse_width = 0;
    double period = 0;

    VSource(QString Name, char type) : Name(Name), type(type){};
    VSource(){};
    bool operator==(const VSource b)const
	{
		return this->Name == b.Name;
	}
};


struct DynamicElement //r,c
{
    DynamicElement(char type, QString Name) : type(type), Name(Name){};
    DynamicElement(){}
    char type; 
    QString Name;
    QString Nodes[2];
    QString ValueInString;
    double value;
    bool operator==(const DynamicElement b)const
	{
		return this->Name == b.Name;
	}
};

struct StaticElement //r 
{
    StaticElement(char type, QString Name) : type(type), Name(Name){};
    StaticElement(){}
    char type; 
    QString Name;
    QString Nodes[2];
    QString ValueInString;
    double value;
    bool operator==(const StaticElement b)const
	{
		return this->Name == b.Name;
	}
};

struct DependentElement
{
    DependentElement(char type, QString Name) : type(type), Name(Name){};
    DependentElement(){}
    char type; // VCCS:G  VCVS:E  CCVS:H  CCCS:F
    QString Name;
    QString Controler;
    QString Nodes[4];
    QString ValueInString;
    double value;
    bool operator==(const DependentElement b)const
	{
		return this->Name == b.Name;
	}
};

double getVoltage(const struct VSource* source, double t);

double Vpulse(double v_initial, double v_peak, double t_delay, double t_rise, double pulse_width, double t_fall, double period, double time);
#endif