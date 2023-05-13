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
	std::strin