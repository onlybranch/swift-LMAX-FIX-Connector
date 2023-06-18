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

void CFIXOrder::SetAmountRequested(double Amount)
{
	m_AmountRequested = Amount;
}

double CFIXOrder::GetAmountRequested()
{
	return m_AmountRequested;
}

void CFIXOrder::SetAmountReceived(double Amount