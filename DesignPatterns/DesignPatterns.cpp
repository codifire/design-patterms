// DesignPatterns.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>
#include <assert.h>

#include "Delegate/Delegate.h"
#include "Observer/ObserverPattern.h"

#include "Decorator/Decorator.h"

int main()
{
	ObserverPattern obs;

	obs.entry();

	decorator_pattern::demo();


    std::cout << "\n"; 
	system("pause");
}
