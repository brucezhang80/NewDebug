//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : ConcreteFactory1.h
//  @ Date : 2014/7/15
//  @ Author : www.newdebug.com
//
//


#if !defined(_CONCRETEFACTORY1_H)
#define _CONCRETEFACTORY1_H

#include "AbstractFactory.h"

class ConcreteFactory1 : public AbstractFactory
{
public:
	ConcreteFactory1();
	~ConcreteFactory1();
	AbstractProductA* createProductA();
	AbstractProductB* createProductB();
};

#endif  //_CONCRETEFACTORY1_H
