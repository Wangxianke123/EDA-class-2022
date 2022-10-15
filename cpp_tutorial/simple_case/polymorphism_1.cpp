/**
 * @file polymorphism_0.cpp
 * @brief simple example showing the concept polymorphism
 * @author Mingzhen Li
 * @date 2022/08/06
 */

#include<iostream>
#include<string>
using namespace std;

class Car
{
public:
    void horn() 
    {
        cout << "Di !!!" << endl;
    }

    // function overload
    void horn(int times) 
    {
        for (int i = 0; i < times; i++) {
            cout << "Di ";
        }
        cout << "!!!" << endl;
    }

    // no virtual here, overwrite would happen
    void applyBarke()
    {
        cout << "Braking distance is 30M." << endl;
    }
    
    void virtual showLabel() {};
};

class Benz : public Car
{
public:
    void applyBarke()
    {
        cout << "Braking distance is 10M." << endl;
    }

    void showLabel()
    {
        cout << "Mercedes-Benz" << endl;
    }
};

class Porsche : public Car
{
public:
    // void applyBarke() override // ERROR
    void applyBarke()
    {
        cout << "Braking distance is 15M." << endl;
    }

    // void showLabel() override // ERROR
    void showLabel()
    {
        cout << "Ferdinand-Porsche" << endl;
    }

    // overwrite because of different signiture
    void showLabel(int x)
    {
        cout << "ERROR" << endl;
    }
};

int main()
{
    Car* car_0 = new Car();
    Car* car_1 = new Benz();
    Car* car_2 = new Porsche();
    Benz* car_3 = new Benz();
    Porsche* car_4 = new Porsche();

    cout << "***** info of base vehicle *****" << endl;
    car_0->applyBarke();
    car_0->showLabel();
    car_0->horn();
    car_0->horn(3);
    cout << "***** info of vehicle #1 *****" << endl;
    car_1->applyBarke();
    car_1->showLabel();
    cout << "***** info of vehicle #2 *****" << endl;
    car_2->applyBarke();
    car_2->showLabel();
    // car_2->showLabel(0); // ERROR
    cout << "***** info of vehicle #3 *****" << endl;
    car_3->applyBarke();
    car_3->showLabel();
    cout << "***** info of vehicle #4 *****" << endl;
    car_4->applyBarke();
    car_4->showLabel();
    car_4->showLabel(0);

    return 0;
}