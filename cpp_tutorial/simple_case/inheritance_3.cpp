/**
 * @file inheritance_3.cpp
 * @brief simple example showing the concept inheritance
 * @author Mingzhen Li
 * @date 2022/08/06
 */

#include<iostream>
using namespace std;

class ClassA
{
  public:
    int a;
};
 
class ClassB : virtual public ClassA
{
  public:
    int b;
};
 
class ClassC : virtual public ClassA
{
  public:
    int c;
};
 
class ClassD : public ClassB, public ClassC
{
  public:
    int d;
};
 
int main()
{
    ClassD obj;
 
    // only one copy of ClassA
    obj.a = 10;
    obj.a = 100;
 
    obj.b = 20;
    obj.c = 30;
    obj.d = 40;
 
    cout << "a: " << obj.a << endl;
    cout << "b: " << obj.b << endl;
    cout << "c: " << obj.c << endl;
    cout << "d: " << obj.d << endl;
}