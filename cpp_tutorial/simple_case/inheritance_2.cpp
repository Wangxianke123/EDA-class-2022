/**
 * @file inheritance_2.cpp
 * @brief simple example showing the concept inheritance
 * @author Mingzhen Li
 * @date 2022/08/06
 */

#include<iostream>
using namespace std;

class ClassA {
public:
    int a;
};
 
class ClassB : public ClassA {
public:
    int b;
};
 
class ClassC : public ClassA {
public:
    int c;
};
 
class ClassD : public ClassB, public ClassC {
public:
    int d;
};
 
int main()
{
    ClassD obj;
    
    // obj.a = 10; // ERROR
    // obj.a = 100; // ERROR
    
    // still 2 copies of ClassA
    obj.ClassB::a = 10;
    obj.ClassC::a = 100;
 
    obj.b = 20;
    obj.c = 30;
    obj.d = 40;
 
    cout << "a from ClassB: " << obj.ClassB::a << endl;
    cout << "a from ClassC: " << obj.ClassC::a << endl;
 
    cout << "b: " << obj.b << endl;
    cout << "c: " << obj.c << endl;
    cout << "d: " << obj.d << endl;
}