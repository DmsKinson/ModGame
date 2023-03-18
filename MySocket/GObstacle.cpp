#include "stdafx.h"
#include "GObstacle.h"
#include "SockMsg.h"

COLORREF GObstacle::COLORS[5] = { RGB(118,84,41),RGB(234,218,66),RGB(225,177,95),RGB(225,209,116),RGB(105,66,30)};

GObstacle::GObstacle()
{
}


GObstacle::GObstacle(BYTE Index, FPoint Pos, GController *father,UINT Width,UINT Height,FPoint Vel,INT Intensity)
	:GObject(Index,Pos, GO_OBSTACLE,Width,Height,Intensity, Vel)
{
	m_father = father;
	m_nIntensity = Intensity;
	m_crRect.SetRect(m_ptPos.x - Width / 2, m_ptPos.y - Height / 2, m_ptPos.x + Width / 2, m_ptPos.y + Height / 2);
	m_nColor = Intensity == -1 ? COLORS[4] : COLORS[Intensity];
}

void GObstacle::PaintSelf(CDC *dc)
{
	m_nColor = m_nIntensity == -1 ? COLORS[4] : COLORS[m_nIntensity];
	CBrush brush(m_nColor);
	dc->SelectObject(brush);
	dc->Rectangle(m_crRect);
}

void GObstacle::UpdateSelf()
{
	/*for (INT i = 0; i < m_nNum; i++)
	{
		m_cpPoints[i].Offset(m_ptVel);
	}*/
}

DataPack GObstacle::PackSelf()
{
	DataPack cache;
	cache.Index = m_nIndex;
	cache.Type = GO_OBSTACLE;
	cache.Pos.x = m_father->m_crActiveRect.Width() - m_ptPos.x;
	cache.Pos.y = m_father->m_crActiveRect.Height() - m_ptPos.y;
	cache.Abbr = m_nIntensity;
	cache.Token = TK_UPDATE;
	return cache;
}

void GObstacle::SetAbbr(INT Abbr)
{
	m_nIntensity = Abbr;
}

void GObstacle::OnCollide(FPoint Point, const GObject *Obj)
{
	if (m_nIntensity != -1)
	{
		m_nIntensity--;
		if (m_nIntensity < 1)
		{
			DataPack cache;
			cache.Index = m_nIndex;
			cache.Type = GO_OBSTACLE;
			cache.Token = TK_DEL;
			m_father->PushInDeque(cache);
			m_father->m_nObs--;
			m_father->PopGObjs(m_nIndex);
		}
	}
}

GObstacle::~GObstacle()
{
	m_father = NULL;
}
