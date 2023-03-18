#include "stdafx.h"
#include "MySocket.h"
#include "SockMsg.h"

Msg::Msg(TCHAR* Value,TCHAR* User, UINT Length, UINT Type, CTime Time)
{
	memset(tszValue, 0, sizeof(tszValue));
	memcpy(tszValue+USER_LENGTH, Value, Length);
	memcpy(tszValue, User, USER_LENGTH);
	nType = Type;
	ctTime = Time;
}

Msg::Msg(const Msg & msg)
{
	memset(tszValue, 0, sizeof(tszValue));
	memcpy(this, &msg, sizeof(msg));
}

Msg::Msg()
{
	memset(tszValue, 0, sizeof(tszValue));
	nType = 0;
}

DataPack::DataPack()
{
	Token = 0;
	Index = 0;
	Pos.SetPoint(0, 0);
	Vel.SetPoint(0, 0);
	Abbr = 0;
	Type = 0;
}

SockMsg::SockMsg()
{
	m_mValue.ctTime = NOW_TIME;
	m_mValue.nType = 0;
	memset(m_mValue.tszValue, 0, sizeof(m_mValue.tszValue));
}

SockMsg::SockMsg(Msg smMsg)
{
	Fresh();
	Assign(smMsg);
}


SockMsg::~SockMsg()
{
}

void SockMsg::Fresh()
{
	memset(m_mValue.tszValue, 0, sizeof(m_mValue.tszValue));
	m_mValue.ctTime = CTime();
	m_mValue.nType = 0;
}

void SockMsg::Assign(const Msg & src)
{
	Fresh();
	memcpy(&m_mValue, &src, sizeof(src));
}

BOOL SockMsg::IsEmpty()
{
	return m_mValue.nType == 0;
}

TCHAR* SockMsg::GetModMem()
{
	return m_mValue.tszValue+USER_LENGTH;
}

CString SockMsg::GetUser()
{
	TCHAR cache[128] = { '\0' };
	memcpy(cache, m_mValue.tszValue, USER_LENGTH);
	return CString(cache);
}

int SockMsg::GetType()
{
	return m_mValue.nType;
}

CTime SockMsg::GetTime()
{
	return m_mValue.ctTime;
}

