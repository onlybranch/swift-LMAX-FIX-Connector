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

	unsigned int				m_Orde