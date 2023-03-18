#pragma once
#include "GObject.h"
#include "GController.h"
#include "Selfible.h"
class GObstacle :
	public GObject,public Selfible
{
private:
	INT m_nIntensity;
	GController *m_father;
	static COLORREF COLORS[5];
public:
	GObstacle();
	GObstacle(BYTE Index,FPoint Pos,GController *father,UINT Width,UINT Height,FPoint Vel,INT Intensity = -1);
	void WINAPI PaintSelf(CDC *dc);
	void WINAPI UpdateSelf();
	DataPack WINAPI PackSelf();
	void virtual SetAbbr(INT Abbr);
	void OnCollide(FPoint Point, const GObject *Obj);
	~GObstacle();
};