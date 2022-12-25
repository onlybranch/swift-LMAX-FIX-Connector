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
		FIX::FileLogFactory		logFact