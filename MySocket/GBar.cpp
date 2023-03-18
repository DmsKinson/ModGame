#include "stdafx.h"
#include "GBar.h"
#include "SockMsg.h"
#include "GController.h"

GBar::GBar()
{

}

GBar::GBar(BYTE Index,FPoint Pos, GController *father, FPoint Vel)
	:GObject(Index,Pos,GO_BAR,DEFAULT_WIDTH,DEFAULT_HEIGHT,0, Vel)
{
	m_father = father;
	m_nDirection = 0;
	m_nReverse = 1;
}

void GBar::PaintSelf(CDC *dc)
{
	CBrush brush(BAR_COLOR);
	dc->SelectObject(brush);
	dc->Rectangle(m_crRect);
}

void GBar::UpdateSelf()
{
	static int t = 0;
	t = 1 - t;
	m_crRect.OffsetRect(ScalarMulti(m_ptVel, GetDir()));
	if (m_crRect.left < m_father->m_crActiveRect.left)
		m_crRect.MoveToX(m_father->m_crActiveRect.left);
	if (m_crRect.right > m_father->m_crActiveRect.right)
		m_crRect.MoveToX(m_father->m_crActiveRect.right - m_crRect.Width());
	m_ptPos.SetPoint(m_crRect.CenterPoint().x, m_crRect.CenterPoint().y);
}

DataPack GBar::PackSelf()
{
	DataPack cache;
	cache.Index = m_nIndex;
	cache.Type = GO_BAR;
	cache.Pos.x = m_father->m_crActiveRect.Width() - m_ptPos.x;
	cache.Pos.y = m_father->m_crActiveRect.Height() - m_ptPos.y;
	cache.Abbr = m_crRect.Width();
	cache.Token = TK_UPDATE;
	return cache;
}

void GBar::ChangeSize(INT Length)
{
	m_crRect.left -= Length / 2;
	m_crRect.right += Length / 2;
}

void GBar::SetSize(int Length)
{
	m_crRect.left = m_crRect.CenterPoint().x - Length / 2;
	m_crRect.right = m_crRect.CenterPoint().x + Length / 2;
}

INT GBar::GetDir()
{
	return m_nDirection*m_nReverse;
}

void GBar::SetDir(int dir)
{
	m_nDirection = dir;
}

void GBar::Reverse()
{
	m_nReverse = -m_nReverse;
}

void GBar::OnCollide(FPoint Point, const GObject * Obj)
{
	m_father->m_nCurrPlayer = 1 - m_father->m_nCurrPlayer;
}

void GBar::SetAbbr(INT Abbr)
{
	m_nDirection = Abbr;
}


GBar::~GBar()
{
	m_father = NULL;
}
