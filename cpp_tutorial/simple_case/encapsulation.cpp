/**
 * @file encapsulation.cpp
 * @brief simple example showing the concept encapsulation
 * @author Mingzhen Li
 * @date 2022/08/06
 */

#include<iostream>
using namespace std;

class Person
{
private:
    // no direct access from outside
    int age;
    int id;

public:
    void increaseAge() 
    {
        age += 1;
    }
    int getAge() 
    {
        return age;
    }
    int getid() 
    {
        return id;
    }

    // constrcutor & de-constrcutor
    Person(int a, int i) 
    {
        age = a;
        id = i;
    }
    ~Person();
};

int main() {
    // create object
    Person* john_p = new Person(22, 12345);
    // get variables
    cout << "***** john's profile of 2022 *****" << endl;
    cout << "age: " << john_p->getAge() << endl;
    cout << "id: " << john_p->getid() << endl;
    // can only increase age
    john_p->increaseAge();
    // get variables
    cout << "***** john's profile of 2023 *****" << endl;
    cout << "age: " << john_p->getAge() << endl;
    cout << "id: " << john_p->getid() << endl;

    return 0;
}
