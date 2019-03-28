// DesignPatterns.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <utility>
#include <assert.h>

#include "Delegate/Delegate.h"
#include "Observer/ObserverPattern.h"


int main()
{
	ObserverPattern obs;

	obs.entry();

    std::cout << "\n"; 
	system("pause");
}
