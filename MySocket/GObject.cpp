#include "stdafx.h"
#include "GObject.h"
#include "SockMsg.h"

GObject::GObject()
{
}

GObject::GObject(BYTE Index,FPoint Pos, UINT Type, UINT Width, UINT Height, INT Abbr, FPoint Vel)
{
	m_nIndex = Index;
	m_ptPos = Pos;
	m_ptVel = Vel;
	m_nType = Type;
	m_nAbbr = Abbr;
	m_crRect.SetRect(Pos.x-Width/2,Pos.y-Height/2,Pos.x+Width/2,Pos.y+Height/2);
}

void GObject::SetPos(FPoint Pos)
{
	m_ptPos = Pos;
}

void GObject::ChangeSpeed(UINT Rate)
{
	FPoint bk = m_ptVel;
	m_ptVel = ScalarMulti(m_ptVel, Rate);
	if (m_ptVel.Magnitude() <= 1)
		m_ptVel = bk;
}

void GObject::SetSpeed(FPoint Vel)
{
	m_ptVel = Vel;
}

void GObject::SetAbbr(INT Abbr)
{
}

void GObject::SetRect(CRect rect)
{
	m_crRect = rect;
}

FPoint & GObject::GetPos()
{
	return m_ptPos;
}

FPoint & GObject::GetVel()
{
	return m_ptVel;
}

CRect & GObject::GetRect()
{
	return m_crRect;
}

void GObject::OnCollide(FPoint Point, const GObject * Obj)
{
}

BOOL GObject::operator==(const GObject & Obj) const
{
	return m_nIndex == Obj.GetIndex();
}

UINT GObject::GetIndex() const
{
	return m_nIndex;
}

UINT GObject::GetType() const
{
	return m_nType;
}

INT GObject::GetAbbr() const
{
	return m_nAbbr;
}

GObject::~GObject()
{
}
