#pragma once
#include <string>
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/SocketInitiator.h"
#include "FIXOrderCollection.h"
#include "CurrencyCollection.h"

using namespace FIX;

class CFIXAdapter : public FIX::Application,
					public FIX::MessageCracker
{
private:
	FIX::SocketInitiator		*m_pInitiator;

	unsigned int				m_OrderCount;
	FIX::SessionID				m_TradeSessionID;
	FIX::SessionID				m_PriceSessionID;

	std::string					m_UserName;
	std::string					m_Password;

	std::string					m_TradeCompID;
	std::string					m_PriceCompID;

	bool						m_bTradeLoggedIn;
	bool						m_bPriceLoggedIn;

	std::map<int, std::string>	m_mapMarketData;

	// Stores all the or