
#include "stdafx.h"
#include "TimeObject.h"
#include <iostream>
#include <iomanip>

using namespace std;

unsigned int CTimeObject::GetMillisecond()
{
#ifdef _WIN32
	return m_st.wMilliseconds;
#else
	return m_tb.millitm;
#endif // _WIN32
}

unsigned int CTimeObject::GetSecond()
{
#ifdef _WIN32
	return m_st.wSecond;
#else
	return m_tm.tm_sec;
#endif // _WIN32
}

unsigned int CTimeObject::GetMinute()
{
#ifdef _WIN32
	return m_st.wMinute;
#else
	return m_tm.tm_min;
#endif // _WIN32
}

unsigned int CTimeObject::GetHour()
{
#ifdef _WIN32
	return m_st.wHour;
#else
	return m_tm.tm_hour;
#endif // _WIN32
}

unsigned int CTimeObject::GetDay()
{
#ifdef _WIN32
	return m_st.wDay;
#else
	return m_tm.tm_mday;
#endif // _WIN32
}

unsigned int CTimeObject::GetMonth()
{
#ifdef _WIN32
	return m_st.wMonth;
#else
	return m_tm.tm_mon+1;
#endif // _WIN32
}

unsigned int CTimeObject::GetYear()
{
#ifdef _WIN32
	return m_st.wYear;
#else
	return m_tm.tm_year+1900;
#endif // _WIN32
}