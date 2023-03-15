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
	cout << message << endl;
	// If this is a login message we need to add the username and password
	string value = message.getHeader().getField(FIX::FIELD::MsgType);
	if (value.compare("A")==0)
	{
		message.getHeader().setField(FIX::Password(m_Password));
		message.getHeader().setField(FIX::Username(m_UserName));
		message.setField(FIX::ResetSeqNumFlag(true));

		// Store the session ID
		string TargetCompID = message.getHeader().getField(FIX::FIELD::TargetCompID);
		if (TargetCompID.compare("LMXBDM")==0)
		{
			m_PriceSessionID = sessionID;
		}
		else if (TargetCompID.compare("LMXBD")==0)
		{
			m_TradeSessionID = sessionID;
		}
	}
}

void CFIXAdapter::toApp( Message &message, const SessionID &sessionID ) 
	throw( DoNotSend )
{
	cout << "toApp:" << message << " - " << sessionID << endl;
}

void CFIXAdapter::fromAdmin( const Message &message, const SessionID &sessionID ) 
	throw( FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon )
{
	cout << message << endl;
	crack(message, sessionID);
}

void CFIXAdapter::fromApp( const Message &message, const SessionID &sessionID ) 
	throw( FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType )
{
	try
	{
		cout << message << endl;
		crack(message, sessionID);
	}
	catch(FIX::UnsupportedMessageType &/*value*/)
	{
		cout << "Not handled message " << message << endl;
	}
}

string CFIXAdapter::GenerateClOrdID()
{
	// This is a 16 char hex number without a leading zero
	CTimeObject dtNow = CTimeObject::Now();

	stringstream ss;
	ss << setfill('0');
	ss << setw(4) << dtNow.GetYear();
	ss << setw(2) << dtNow.GetMonth();
	ss << setw(2) << dtNow.GetDay();
	ss << setw(2) << dtNow.GetHour();
	ss << setw(2) << dtNow.GetMinute();
	ss << setw(2) << dtNow.GetSecond();
	ss << setw(2) << (m_OrderCount++) % 100;

	return ss.str();
}

/*****************************************************************************/
/*          All the available client functions that can be used				 */
/*                  to issue commands to the FIX server						 */
/*****************************************************************************/
bool CFIXAdapter::TestRequest(const string &RequestID)
{
	bool bRet = false;

	if (m_bTradeLoggedIn)
	{
		FIX44::TestRequest message;

		message.set(FIX::TestReqID(RequestID));

		cout << "Sending test request" << endl;
		bRet = FIX::Session::sendToTarget(message, m_TradeSessionID);
	}
	return bRet;
}

bool CFIXAdapter::SendSequenceReset()
{
	bool bRet = false;

	if (m_bTradeLoggedIn)
	{
		FIX44::SequenceReset message;

		message.set(FIX::GapFillFlag(false));

		bRet = FIX::Session::sendToTarget(message, m_TradeSessionID);
	}
	return bRet;
}

bool CFIXAdapter::Logout(const string &Reason)
{
	bool bRet = false;

	if (m_bTradeLoggedIn)
	{
		FIX44::Logout message;
		message.set(FIX::Text(Reason));

		bRet = FIX::Session::sendToTarget(message);
	}
	return bRet;
}

bool CFIXAdapter::NewOrderSingle(const string &Currency, double Quantity, double Price, bool bIsLimit, string &OrderID)
{
	if (!m_bTradeLoggedIn)
	{
		cout << "Not logged in so unable to send new order" << endl;
		return false;
	}

	// Get the unique order ID
	string ClOrdID = GenerateClOrdID();

	FIX::Side	OrderSide;
	if (Quantity > 0)
	{
		OrderSide = FIX::Side_BUY;
	}
	else
	{
		OrderSide = FIX::Side_SELL;
	}

	FIX::OrdType OrderType;
	if (bIsLimit)
	{
		OrderType = FIX::OrdType_LIMIT;
	}
	else
	{
		OrderType = FIX::OrdType_MARKET;
	}

	FIX44::NewOrderSingle message(
		FIX::ClOrdID(ClOrdID),
		OrderSide,				
		FIX::TransactTime(true),
		OrderType);		

	// Convert currency string into ID
	int CurrencyID;
	if (!m_Currencies.GetCur