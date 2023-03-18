#pragma once
#include "GObject.h"
#include "GController.h"
#include "Selfible.h"

#define DEFAULT_RADIUS 15
#define MAX_RADIUS 40
#define BALL_COLOR RGB(28,170,224)
class GBall :
	public GObject,public Selfible
{
private:
	UINT m_nRadius;
	GController *m_father;
	vector<GObject*>  *m_vecObjs;
	CRect m_AfxRect;
public:
	GBall();
	GBall(BYTE Index,FPoint Pos, GController *father,FPoint Vel);
	void WINAPI PaintSelf(CDC *dc);
	DataPack WINAPI PackSelf();
	BOOL GetCollision(const CRect &rect,LineSegment2 &line);
	void SetAbbr(INT Abbr);
	void WINAPI UpdateSelf();
	void Collide(CRect &TempRect);
	void OnCollide(LineSegment2 line, GObject *Obj = NULL);
	void OnCollide(FPoint point, GObject *Obj = NULL);
	~GBall();
};

