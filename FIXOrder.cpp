#include "stdafx.h"
#include "FIXOrder.h"

using namespace std;

CFIXOrder::CFIXOrder(void)
{
}

CFIXOrder::CFIXOrder(string OrderID)
{
	m_OrderID = OrderID;
}

CFIXOrder::~CFIXOrder(void)
{
}

void CFIXOr