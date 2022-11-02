#ifndef SOLVER_H
#define SOLVER_H
#include <armadillo>
#include <QString>
#include <QMap>
using namespace arma;

struct DC_result
{
    QString SourceName;
    double start,stop,step;
    int m,n;
    std::vector<std::vector<double>> answer_table;
    DC_result(QString str,double a,double b,double c,int m,int n) : SourceName(str),start(a), stop(b),step(c),m(m),n(n){};
};

struct AC_result{
    std::vector<double> FrequencyList;
    std::vector<std::vector<double>> Magnitude;
    std::vector<std::vector<double>> Phase;
    int rows;
    int cols;
};

struct Tran_result{
    std::vector<double> TimeList;
    std::vector<std::vector<double>> ValueList;
    int rows;
    int cols;
};

mat solveDC(mat stamp, int pos);
cx_mat solveAC(cx_mat stamp,int pos);

bool SaveDCAnswer(mat answer ,struct DC_result* result);
bool SaveACAnswer(double f,cx_mat answer,struct AC_result* result);
bool SaveTranAnswer(double time,mat answer,struct Tran_result* result);

#endif