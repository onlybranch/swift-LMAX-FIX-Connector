
#include "stdafx.h"
#include "FIXOrderCollection.h"
#include "CSGuardian.h"

using namespace std;

CFIXOrderCollection::CFIXOrderCollection(void)
{
}

CFIXOrderCollection::~CFIXOrderCollection(void)
{
}

bool CFIXOrderCollection::Add(const string &OrderID, double Requested, eOrderSide Side)
{
	CCSGuardian csg(m_CS);

	// If we already have an entry with this OrderID, fail
	if (m_mapOrders.find(OrderID) != m_mapOrders.end())
	{
		return false;
	}