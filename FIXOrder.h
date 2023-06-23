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

enum eOrderSide
{
	BUY = 1,
	SELL
};

class CFIXOrder
{
	std::string		m_OrderID;
	double			m_AmountRequested;
	double			m_AmountReceived;
	eOrderState		m_OrderState;
	eOrderSide		m_Side;
	tm				m_CreationTime;
	tm				m_LastUpdated