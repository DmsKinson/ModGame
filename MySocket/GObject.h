#pragma once
#define GO_BALL 1
#define GO_BUFF 2
#define GO_BAR 3
#define GO_OBSTACLE 4
class GObject
{
protected:
	BYTE m_nIndex;			//unique index
	FPoint m_ptPos;			//position
	UINT m_nType;			//type of object
	INT m_nAbbr;
	FPoint m_ptVel;			//velocity
	COLORREF m_nColor;		//color
	CRect m_crRect;
public:
	GObject();
	GObject(BYTE Index,FPoint Pos, UINT Type, UINT Width, UINT Height, INT Abbr,FPoint Vel);
	void virtual SetPos(FPoint Pos);
	void virtual ChangeSpeed(UINT Rate);
	void virtual SetSpeed(FPoint Vel);
	void virtual SetAbbr(INT Abbr);
	void virtual SetRect(CRect rect);
	CRect& GetRect();
	FPoint& GetPos();
	FPoint& GetVel();
	void virtual OnCollide(FPoint Point,const GObject *Obj);
	BOOL operator == (const GObject &Obj) const;
	UINT GetIndex() const;
	UINT GetType() const;
	INT GetAbbr() const;
	virtual ~GObject();
};

