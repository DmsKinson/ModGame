#include "stdafx.h"
#include "GBuff.h"
#include "resource.h"
GBuff::GBuff()
{
}

GBuff::GBuff(BYTE Index,FPoint Pos, UINT Kind, GController *father, FPoint Vel)
	:GObject(Index,Pos, GO_BUFF,0,0,Kind, Vel)
{
	m_nKind = Kind;
	m_father = father;
	m_imgPic.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP0 + Kind);
	m_crRect.SetRect(Pos.x - m_imgPic.GetWidth() / 2, Pos.y - m_imgPic.GetHeight() / 2, Pos.x + m_imgPic.GetWidth() / 2, Pos.y + m_imgPic.GetHeight() / 2);
}

void GBuff::PaintSelf(CDC *dc)
{
	m_imgPic.Draw(dc->GetSafeHdc(), m_crRect);
}

void GBuff::UpdateSelf()
{
}

DataPack GBuff::PackSelf()
{
	DataPack cache;
	cache.Index = m_nIndex;
	cache.Type = GO_BUFF;
	cache.Pos.x = m_father->m_crActiveRect.Width() - m_ptPos.x;
	cache.Pos.y = m_father->m_crActiveRect.Height() - m_ptPos.y;
	cache.Token = TK_UPDATE;
	return cache;
}

void GBuff::OnCollide(FPoint Point, const GObject * Obj)
{
	DataPack cache;
	cache.Index = m_nIndex;
	cache.Type = GO_BUFF;
	cache.Token = TK_DEL;
	m_father->PushInDeque(cache);
	m_father->m_nBuffs--;
	m_father->ProcBuff(m_nKind,m_father->m_nCurrPlayer);
	m_father->PopGObjs(m_nIndex);
}

GBuff::~GBuff()
{
	m_father = NULL;
	m_imgPic.Detach();
	m_imgPic.Destroy();
}
