#include "stdafx.h"
#include "CSGuardian.h"

CCSGuardian::CCSGuardian(CRIT_SECTION &CS):
	m_CS(CS)
{
#ifdef _WIN
	m_CS.Lock();
#elseif
	m_CS.Stu