//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : ConcreteFactory2.cpp
//  @ Date : 2014/7/15
//  @ Author : www.newdebug.com
//
//


#include "ConcreteFactory2.h"
#include "ProductA1.h"
#include "ProductB1.h"

ConcreteFactory2::ConcreteFactory2()
{

}

ConcreteFactory2::~ConcreteFactory2()
{

}

AbstractProductA* ConcreteFactory2::createProductA()
{
    return new ProductA1();
}

AbstractProductB* ConcreteFactory2::createProductB()
{
    return new ProductB1();
}

