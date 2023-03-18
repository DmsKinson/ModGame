#pragma once
#include "GObject.h"
#include "GController.h"
#include "Selfible.h"

#define BALL_SPEED_UP 0
#define BALL_SPEED_DOWN 2
#define BALL_SIZE_UP 7
#define BALL_SIZE_DOWN 4
#define BAR_SPEED_UP 1
#define BAR_SPEED_DOWN 3
#define BAR_SIZE_UP 8
#define BAR_SIZE_DOWN 5
#define DIR_REVERSE 6
class GBuff :
	public GObject,public Selfible
{
private:
	UINT m_nKind;
	GController *m_father;
	CImage m_imgPic;
public:
	GBuff();
	GBuff(BYTE Index,FPoint Pos, UINT Kind, GController *father, FPoint Vel);
	void WINAPI PaintSelf(CDC *dc);
	void WINAPI UpdateSelf();
	DataPack WINAPI PackSelf();
	void OnCollide(FPoint Point, const GObject *Obj);
	~GBuff();
};

