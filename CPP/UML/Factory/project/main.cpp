#include "ConcreteFactory.h"
#include "ConcreteProduct1.h"

#include <iostream>

using namespace std;

int main()
{

    Factory* factory = new ConcreteFactory();
    Product* product = factory->createProduct();

    return 0;
}

