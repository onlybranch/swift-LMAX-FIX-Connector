#include "stdafx.h"
#include "Platform.h"
#include "TimeObject.h"
#include "FIXAdapter.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/Session.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/fix44/Logon.h"
#include "quickfix/fix44/Logout.h"
#include "quickfix/fix44/TestRequest.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/OrderCancelRequest.h"
#include "quickfix/fix44/SequenceReset.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/MarketDataRequest.h"
#include "quickfix/fix44/MarketDataSnapshotFullRefresh.h"

using namespace std;
using namespace FIX;

CFIXAdapter::CFIXAdapter(void):
			m_pInitiator(nullptr),
			m_OrderCount(0),
			m_bTradeLoggedIn(false),
			m_bPriceLoggedIn(false)
{
}

CFIXAdapter::~CFIXAdapter(void)
{
}

void CFIXAdapter::SetLoginDetails(const string &UserName,
								  const string &Password)
{
	m_UserName = UserName;
	m_Password = Password;
}

void CFIXAdapter::SetCompIDs(const std::string &TradeCompID,
							 const std::string &PriceCompID)
{
	m_TradeCompID = TradeCompID;
	m_PriceCompID = PriceCompID;
}

bool CFIXAdapter::Initialise()
{
	bool bRet = true;
	try
	{
		// Assume that the config file is in the same directory as the executable
		FIX::SessionSettings	settings("fixsettings.cfg");
		FIX::FileStoreFactory	storeFactory(settings);
		FIX::FileLogFactory		logFactory(settings);
		m_pInitiator = new SocketInitiator(*this, storeFactory, settings, logFactory);

		// Start the connection and login process
		m_pInitiator->start();

		// Setup all the currency details
		if (!m_Currencies.Initialise())
		{
			cout << "Failed to initialise the currency collection" << endl;
			return false;
		}
	}
	catch(const FIX::ConfigError &error)
	{
		cout << error.detail << endl;
	}
	catch(...)
	{
		bRet = false;
	}
	return bRet;
}

bool CFIXAdapter::Shutdown()
{
	bool bRet = true;
	try
	{
		if (m_pInitiator)
		{
			m_pInitiator->stop();

			while (!m_pInitiator->isStopped())
			{
				Sleep(100);
			}
//			delete m_pInitiator;
		}
	}
	catch(...)
	{
		bRet = false;
	}
	return bRet;
}

bool CFIXAdapter::IsTradeLoggedIn()
{
	return m_bTradeLoggedIn;
}

bool CFIXAdapter::IsPriceLoggedIn()
{
	return m_bPriceLoggedIn;
}

void CFIXAdapter::onCreate( const SessionID &sessionID )
{
//	cout << "Session to LMAX has been created successfully" << endl;
}

void CFIXAdapter::onLogon( const SessionID &sessionID )
{
	cout << "We are logged on to the ";

	string CompID = sessionID.getTargetCompID();
	if (CompID.compare(m_TradeCompID)==0)
	{
		cout << "trade server";
		m_bTradeLoggedIn = true;
	}
	else if (CompID.compare(m_PriceCompID)==0)
	{
		cout << "market data server";
		m_bPriceLoggedIn = true;
	}
	else
	{
		cout << "unknown server (" << sessionID.getTargetCompID() << ")";
	}
	cout << endl;
}

void CFIXAdapter::onLogout( const SessionID &sessionID )
{
	cout << "We are logged out from the ";

	string CompID = sessionID.getTargetCompID();
	if (CompID.compare(m_TradeCompID)==0)
	{
		cout << "trade server";
		m_bTradeLoggedIn = false;
	}
	else if (CompID.compare(m_PriceCompID)==0)
	{
		cout << "market data server";
		m_bPriceLoggedIn = false;
	}
	else
	{
		cout << "unknown server";
	}
	cout << endl;
}

void CFIXAdapter::toAdmin( Message &message, const SessionID &sessionID )
{
	cout << message << 