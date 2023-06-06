// FIXConnector.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "FIXAdapter.h"
#include "Platform.h"
#include <iostream>
#include "TimeObject.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Testing code
	CTimeObject	TimeObj;

	for (int i=0;i<100;i++)
	{
		TimeObj.SetCurrentTime();
		TimeObj.Print();
		Sleep(250);
	}

	cout << "Waiting fo