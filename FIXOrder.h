#pragma once
#include <string>
#include <time.h>

enum eOrderState
{
	NEW = 1,
	PARTIALLY_FILLED,
	FILLED,
	REJECTED,
	CANCELED
};

enum eO