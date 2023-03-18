#include "stdafx.h"
#include "GBall.h"
#include "GBar.h"
#include "SockMsg.h"
#include "GBuff.h"
#include "GObstacle.h"

GBall::GBall()
{
}

GBall::GBall(BYTE Index,FPoint Pos, GController *father, FPoint Vel)
	:GObject(Index,Pos,GO_BALL,2*DEFAULT_RADIUS, 2*DEFAULT_RADIUS, DEFAULT_RADIUS,Vel)
{
	m_nColor = BALL_COLOR;
	m_nRadius = DEFAULT_RADIUS;
	m_father = father;
	m_vecObjs = father->GetGObjsPtr();
	m_AfxRect.CopyRect(&(m_father->m_crActiveRect));
}

void GBall::PaintSelf(CDC *dc)
{
	CBrush brush(m_nColor);
	dc->SelectObject(brush);
	dc->Ellipse(m_crRect);
}



DataPack GBall::PackSelf()
{
	DataPack cache;
	cache.Index = m_nIndex;
	cache.Type = GO_BALL;
	cache.Pos.x = m_father->m_crActiveRect.Width() - m_ptPos.x;
	cache.Pos.y = m_father->m_crActiveRect.Height() - m_ptPos.y;
	cache.Abbr = m_nRadius;
	cache.Token = TK_UPDATE;
	return cache;
}



BOOL GBall::GetCollision(const CRect & rect, LineSegment2 & line)
{
	FPoint points[4];
	points[0].SetPoint(rect.left, rect.top);
	points[1].SetPoint(rect.left, rect.bottom);
	points[2].SetPoint(rect.right, rect.bottom);
	points[3].SetPoint(rect.right, rect.top);
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			FPoint APoint(m_ptPos.x + m_nRadius, m_ptPos.y);
			FPoint endPos(APoint);
			endPos.Add(m_ptVel);
			if (SegmentIntersect(APoint, endPos, points[0], points[1]))
			{
				line.m_cpStart = points[0];
				line.m_cpEnd = points[1];
				return TRUE;
			}
			break;
		}
		case 1:
		{
			FPoint APoint(m_ptPos.x, m_ptPos.y-m_nRadius);
			FPoint endPos(APoint);
			endPos.Add(m_ptVel);
			if (SegmentIntersect(APoint, endPos, points[1], points[2]))
			{
				line.m_cpStart = points[1];
				line.m_cpEnd = points[2];
				return TRUE;
			}
			break;
		}
		case 2:
		{
			FPoint APoint(m_ptPos.x - m_nRadius, m_ptPos.y);
			FPoint endPos(APoint);
			endPos.Add(m_ptVel);
			if (SegmentIntersect(APoint, endPos, points[2], points[3]))
			{
				line.m_cpStart = points[2];
				line.m_cpEnd = points[3];
				return TRUE;
			}
			break;
		}
		case 3:
		{
			FPoint APoint(m_ptPos.x , m_ptPos.y + m_nRadius);
			FPoint endPos(APoint);
			endPos.Add(m_ptVel);
			if (SegmentIntersect(APoint, endPos, points[3], points[0]))
			{
				line.m_cpStart = points[3];
				line.m_cpEnd = points[0];
				return TRUE;
			}
			break;
		}
		default:
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		FPoint PtoVC(m_ptPos-points[i]);
		DOUBLE dis = PtoVC.Magnitude();
		if (dis <= m_nRadius)
		{
			line.m_cpStart = FPoint(-1,-1);
			line.m_cpEnd = points[i];
			return TRUE;
		}
	}
	return FALSE;
}

void GBall::SetAbbr(INT Abbr)
{
	m_nRadius += Abbr;
	if (m_nRadius > 0 && m_nRadius < MAX_RADIUS)
	{
		m_crRect.SetRect(m_ptPos.x - m_nRadius, m_ptPos.y - m_nRadius, m_ptPos.x + m_nRadius, m_ptPos.y + m_nRadius);
	}
	else
	{
		m_nRadius -= Abbr;
	}
}

void GBall::UpdateSelf()
{
	if (!m_father->HasLaunched())
	{
		int x = m_father->GetGObjsPtr()->at(2- (m_father->m_nCurrPlayer))->GetPos().x;
		m_crRect.MoveToX(x - m_nRadius);
		m_ptPos.SetPoint(m_crRect.CenterPoint().x, m_ptPos.y);
	}
	else
	{
		m_crRect.OffsetRect(m_ptVel);
		m_ptPos.Offset(m_ptVel);
		Collide(m_crRect);
	}
}

void GBall::Collide(CRect &TempRect)
{
	///collision on border
	if (TempRect.left < m_AfxRect.left)
	{
		m_ptVel.x = -m_ptVel.x;
		TempRect.MoveToX(m_AfxRect.left);
	}
	if (TempRect.right > m_AfxRect.right)
	{
		m_ptVel.x = -m_ptVel.x;
		TempRect.MoveToX(m_AfxRect.right - 2*m_nRadius);
	}
	if (TempRect.top < m_AfxRect.top)
	{
		m_father->SetGaming(FALSE);
		DataPack cache;
		cache.Token = TK_SWIN;
		m_father->PushInDeque(cache);
	}
	if (TempRect.bottom > m_AfxRect.bottom)
	{
		m_father->SetGaming(FALSE);
		DataPack cache;
		cache.Token = TK_CWIN;
		m_father->PushInDeque(cache);
	}
	///collision on other g-objs
	for (int i = 1; i < m_vecObjs->size(); i++)
	{
		GObject *cache = m_vecObjs->at(i);
		FPoint ray = cache->GetPos() - m_ptPos;
		if (DotMulti(m_ptVel, ray) > 0)
		{
			
			if (cache->GetType() == GO_BUFF)
			{
				INT rx = abs(cache->GetPos().x - m_ptPos.x);
				INT ry = abs(cache->GetPos().y - m_ptPos.y);
				INT dx = min(rx, cache->GetRect().Width()*0.5);
				INT dy = min(ry, cache->GetRect().Height()*0.5);
				if ((dx - rx)*(dx - rx) + (dy - ry)*(dy - ry) <= m_nRadius*m_nRadius)		
				{
					((GBuff*)cache)->OnCollide(FPoint(), this);
				}
			}
			if (cache->GetType() == GO_OBSTACLE)
			{
				LineSegment2 temp ;
				if (GetCollision(cache->GetRect(),temp))
				{
					((GObstacle*)cache)->OnCollide(FPoint(), this);
					OnCollide(temp, cache);
				}
			}
			if (cache->GetType() == GO_BAR)
			{
				LineSegment2 temp;
				CRect barPlain = cache->GetRect();
				barPlain.DeflateRect(0, 0, 0, cache->GetRect().Height()/2);
				if (GetCollision(barPlain, temp))
				{
					((GBar*)cache)->OnCollide(FPoint(), this);
					OnCollide(temp, cache);
				}
			}
		}
	}
}
void GBall::OnCollide(LineSegment2 line, GObject *Obj)
{
	if (line.m_cpStart.x == -1 && line.m_cpStart.y == -1)
	{
		m_ptVel.x = -m_ptVel.x;
		m_ptVel.y = -m_ptVel.y;
	}
	FPoint vLine = line.m_cpEnd - line.m_cpStart;
	DOUBLE proj = m_ptVel.Proj(vLine);
	Pointf nor = vLine.Normalize();
	nor.Mul(proj);
	m_ptVel.x = nor.x - m_ptVel.x;
	m_ptVel.y = nor.y - m_ptVel.y;
	m_ptVel.Add(nor);
}

void GBall::OnCollide(FPoint point, GObject * Obj)
{
	LineSegment2 line(point,m_ptPos);
	OnCollide(line, Obj);
}


GBall::~GBall()
{
	m_father = NULL;
	m_vecObjs = NULL;
}
