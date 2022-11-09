#include"element.h"
#include<QDebug>




double getVoltage(const struct VSource* source, double time)
{
    if(!source)
    {
        qDebug()<<"Something wrong in Voltage source!";
        return 0;
    }
    switch (source->type)
    {
    case 'd':
        return source->value;
    case 'a':
        return source->value;
    case 'p':
        return Vpulse(source->v_initial, source->v_peak, source->t_delay, source->t_rise, source->pulse_width, source->t_fall, source->period, time);
    case 's':
        return Vsin(source->v_initial, source->v_peak, source->period, source->t_delay, source->phase, source->dumping_factor, time);
    default:
    {
        qDebug()<<"Error source type!";
        return 0;
    }
    }
}

double Vpulse(double v_initial, double v_peak, double t_delay, double t_rise, double pulse_width, double t_fall, double period, double time)
{
    if(time<0)
        return 0;
    if(time > (t_delay + period))
        return Vpulse(v_initial, v_peak, t_delay, t_rise, pulse_width, t_fall, period, time-period);
    if (time >=0 && time <= t_delay)
    {
        return v_initial;
    }
    if (time > t_delay && time < t_delay + t_rise)
    {
        return v_initial+(v_peak-v_initial)*(time-t_delay)/t_rise;
    }
    if (time >= t_delay + t_rise && time <= t_delay + t_rise + pulse_width)
    {
        return v_peak;
    }
    if (time > t_delay + t_rise + pulse_width && time < t_delay + t_rise + pulse_width + t_fall)
    {
        return v_peak - (v_peak-v_initial)/t_fall * (time - t_delay - t_rise - pulse_width); 
    }
    if ( time >= t_delay + t_rise + pulse_width + t_fall && time <= t_delay + period)
    {
        return v_initial;
    } 
    return 0;
}

double Vsin(double v_initial, double v_peak, double period, double t_delay, double phase, double dumping_factor, double time)
{
    return v_initial + v_peak * exp(-dumping_factor*(time - t_delay))*sin(2*datum::pi/period * (time - t_delay) + (phase/180)*datum::pi);
}


double getCurrent(const struct ISource* source, double time)
{
    if(!source)
    {
        qDebug()<<"Something wrong in Current source!";
        return 0;
    }
    switch (source->type)
    {
    case 'd':
        return source->value;
    case 'a':
        return source->value;
    case 'p':
        return Vpulse(source->v_initial, source->v_peak, source->t_delay, source->t_rise, source->pulse_width, source->t_fall, source->period, time);
    case 's':
        return Vsin(source->v_initial, source->v_peak, source->period, source->t_delay, source->phase, source->dumping_factor, time);
    default:
    {
        qDebug()<<"Error source type!";
        return 0;
    }
    }
}


Diode::Diode(/* args */)
{
}

Diode::~Diode()
{
}

double Diode::I(double N_p, double N_m)
{
    if( Is * (exp((N_p-N_m)/Vt)-1) > If)
        return (N_p-N_m)/Rs;
    // else if(Is * (exp((N_p-N_m)/Vt)-1) < -IBV)
    //     return -IBV + (N_p-N_m-BV)/Ro; 
    else
        return Is * (exp((N_p-N_m)/Vt)-1);
}



double Diode::I_differential(double N_p, double N_m)
{
    if( Is * (exp((N_p-N_m)/Vt)-1) > If){
        return 1/Rs;
    }
    // else if(Is * (exp((N_p-N_m)/Vt)-1) < -IBV){
    //     return 1/Ro; 
    // }
    else{
        return Is/Vt * exp((N_p-N_m)/Vt);
    }
}


