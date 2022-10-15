/**
 * @file inheritance_1.cpp
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
class Vehicle
{
// allow subclass to access it
protected:
    int fuelAmount;
    int capacity;

public:
    void applyBarkes() 
    {
        cout << "STOP !!!" << endl;
    }

    void showSpec() {
        cout << "fuel amount is: " << fuelAmount << endl;
        cout << "capacity is: " << capacity << endl;
    }

    // constrcutor
    Vehicle()
    {
        fuelAmount = 0;
        capacity = 0;
    }
};

/**
 * @brief Superclass
 */
class PassengerCarrier
{
// allow subclass to access it
protected:
    int passenger;

public:
    void showPassenegr() {
        cout << "passenger count is: " << passenger << endl;
    }

    // constrcutor
    PassengerCarrier()
    {
        passenger = 0;
    }
};

/**
 * @brief level 1 subclass inherit "Vehicle"
 */
class Car : public Vehicle
{
public:
    // constrcutor
    Car()
    {
        fuelAmount = 100;
        capacity = 2;
    }
};

/**
 * @brief level 1 subclass inherit "Vehicle" & "PassengerCarrier"
 */
class Bus : public Vehicle, public PassengerCarrier
{
public:
    // constrcutor
    Bus()
    {
        fuelAmount = 2000;
        capacity = 50;
        passenger += 10;
    }
};

/**
 * @brief level 2 subclass inherit "Car"
 */
class Benz : public Car
{
public:
    // constrcutor
    Benz() 
    {
        fuelAmount += 100;
        capacity += 3;
    }
};

/**
 * @brief level 2 subclass inherit "Car"
 */
class Porsche : public Car
{
public:
    // constrcutor
    Porsche()
    {
        fuelAmount += 200;
        capacity += 1;
    }
};

int main()
{
    Benz* car_1 = new Benz();
    Porsche* car_2 = new Porsche();
    Bus* transport = new Bus();

    cout << "***** info of vehicle #1 *****" << endl;
    car_1->showSpec();
    // car_1->showPassenegr(); // ERROR
    car_1->applyBarkes();
    cout << "***** info of vehicle #2 *****" << endl;
    car_2->showSpec();
    // car_2->showPassenegr(); // ERROR
    car_2->applyBarkes();
    cout << "***** info of vehicle #3 *****" << endl;
    transport->showSpec(); 
    transport->showPassenegr();
    transport->applyBarkes();

    return 0;
}