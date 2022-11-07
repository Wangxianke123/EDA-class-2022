#ifndef ELEMENT_H
#define ELEMENT_H
#include<QString>
#include<QChar>
#include<armadillo>

using namespace arma;

class Diode
{
private:
    double Is = 0.468; //satruation current
    double Vt = 2.585e-2;// Vt = kq/T ,which depends on temperature,here we assume T=300k
    double IBV = 1.0e-3;// current at broken voltage
    double BV = 32.236;//broken voltage
    double If = 3;// Maximium current
    double Rs = Vt/If * log(If/Is);  //inner resistor 
public:
    QString Name;
    QString Nodes[2];
    double I(double N_p, double N_m);
    double I_differential(double N_p, double N_m);
    bool operator==(const Diode b)const
	{
		return this->Name == b.Name;
	}
    Diode(/* args */);
    ~Diode();
};





struct ISource
{
    QString Name;
    QString Nodes[2];//'d' for DC,'a' for AC
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

    ISource(QString Name, char type) : Name(Name), type(type){};
    ISource(){};
    bool operator==(const ISource b)const
	{
		return this->Name == b.Name;
	}
};


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
    QString Controler; //for H and F
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

double getCurrent(const struct ISource* source, double t);

#endif