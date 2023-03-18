#pragma once
#include "GObject.h"
#include "Selfible.h"
#define DEFAULT_WIDTH 70
#define DEFAULT_HEIGHT 15
#define BAR_COLOR RGB(170,25,177)
class GController;

class GBar :
	public GObject,public Selfible
{
private:
	CRect m_crSelf;
	GController *m_father;
	INT m_nDirection;	//ÒÆ¶¯·½Ïò	
	INT m_nReverse;
public:
	GBar();
	GBar(BYTE Index,FPoint Pos,GController *father, FPoint Vel);
	void WINAPI PaintSelf(CDC *dc);
	void WINAPI UpdateSelf();
	DataPack WINAPI PackSelf();
	void ChangeSize(INT Length);
	void SetSize(int Length);
	INT GetDir();
	void SetDir(int dir);
	void Reverse();
	void OnCollide(FPoint Point, const GObject *Obj);
	void virtual SetAbbr(INT Abbr);
	~GBar();
};

