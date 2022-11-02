#include"solver.h"
#include <QDebug>
#include<vector>
using namespace arma;
mat solveDC(mat stamp, int pos){
    vec x;
    int n = stamp.n_cols;
    int m =stamp.n_rows;
    mat temp(1,n);
    temp.zeros();
    temp(pos)=1;
    stamp.insert_rows(m,temp);
    stamp.print("stamp:");
    if(solve(x,stamp.cols(0,n-2),stamp.col(n-1)))
        return x;
    else{
        qDebug()<<"Error:There's something worng when solving stamp!";
        return x.zeros();
    }
}

bool SaveDCAnswer(mat answer ,struct DC_result* result){
    std::vector<double> temp = conv_to<std::vector<double>>::from(answer);
    result->answer_table.push_back(temp);
    return true;
}
bool SaveACAnswer(double f,cx_mat answer,struct AC_result* result)
{
    result->FrequencyList.push_back(f);
    std::vector<double> temp = conv_to<std::vector<double>>::from(abs(answer));

    result->Magnitude.push_back(temp);

    mat phase = 180/datum::pi*atan(imag(answer)/real(answer));
    result->Phase.push_back(conv_to<std::vector<double>>::from(phase));
    return true;
}


cx_mat solveAC(cx_mat stamp,int pos){
    cx_vec x;
    int n = stamp.n_cols;
    int m =stamp.n_rows;
    cx_mat temp(1,n);
    temp.zeros();
    temp(pos)=1;
    stamp.insert_rows(m,temp);
    if(solve(x,stamp.cols(0,n-2),stamp.col(n-1)))
        return x;
    else{
        qDebug()<<"Error:There's something worng when solving stamp!";
        return x.zeros();
    }
}

bool SaveTranAnswer(double time,mat answer,struct Tran_result* result)
{

    qDebug()<<"Saving answer at time:"<<time;
    result->TimeList.push_back(time);
    std::vector<double> temp = conv_to<std::vector<double>>::from(answer);
    result->ValueList.push_back(temp);
    return true;
}