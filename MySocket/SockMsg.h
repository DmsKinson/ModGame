#pragma once
#define USER_LENGTH 32

#define TP_WUZIQI 1
#define TP_PAODEKUAI 2
#define TP_MINE 3
#define TP_LOG 4
#define TP_QUIT 5
#define TP_TALK 6
#define TP_TENNIS 7

#define TK_START 1
#define TK_PAUSE 2
#define TK_RESTART 3
#define TK_SWIN 4
#define TK_CWIN 5
#define TK_LEFT_DOWN 6
#define TK_LEFT_UP 7
#define TK_RIGHT_DOWN 8
#define TK_RIGHT_UP	9
#define TK_UP 10
#define TK_UPDATE 11
#define TK_DEL 12
#define TK_CREATE 13

struct Msg
{
	Msg(TCHAR* Value,TCHAR* User, UINT Length, UINT Type, CTime Time);
	Msg(const Msg &msg);
	Msg();
	CTime ctTime;
	UINT nType;
	TCHAR tszValue[1024];
};

struct DataPack
{
	DataPack();
	CHAR Token;		//type of pack
	BYTE Index;
	UINT Type;		//type of GObject
	FPoint Pos;
	FPoint Vel;
	INT Abbr;		//specific abbr  //GBall:radius  //Gbar:null  //GBuff:kind   //GObs:intensity
};

struct BarData
{
	UINT PosX, PosY;
	CRect Rect;
};

class SockMsg
{
private:
	Msg m_mValue;
public:
	SockMsg();
	SockMsg(Msg smMsg);
	~SockMsg();
	void Fresh();
	void Assign(const Msg& src);
	BOOL IsEmpty();
	TCHAR* GetModMem();
	CString GetUser();
	int GetType();
	CTime GetTime();
};

