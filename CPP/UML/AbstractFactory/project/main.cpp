#include "AbstractFactory.h"
#include "ConcreteFactory1.h"
#include "ConcreteFactory2.h"

#include <iostream>

using namespace std;

int main()
{
    AbstractFactory* factory1 = new ConcreteFactory1();
    factory1->createProductA();
    factory1->createProductB();

    AbstractFactory* factory2 = new ConcreteFactory2();
    factory2->createProductA();
    factory2->createProductB();

    return 0;
}

