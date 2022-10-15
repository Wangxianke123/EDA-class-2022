/**
 * @file inheritance_0.cpp
 * @brief simple example showing the concept inheritance
 * @author Mingzhen Li
 * @date 2022/08/06
 */

#include<iostream>
#include<string>
using namespace std;

/**
 * @brief Superclass
 */
class A {
public:
    int x;
 
protected:
    int y;
 
private:
    int z;
};

/**
 * @brief inheritance mode: public
 */
class B : public A {
    // x is public
    // y is protected
    // z is not accessible from B
};

/**
 * @brief inheritance mode: protect
 */ 
class C : protected A {
    // x is protected
    // y is protected
    // z is not accessible from C
};
 
/**
 * @brief inheritance mode: private
 * @note private is default
 */ 
class D : private A
{
    // x is private
    // y is private
    // z is not accessible from D
};