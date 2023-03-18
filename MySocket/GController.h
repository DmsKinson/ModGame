#pragma once
#include <vector>
#include <deque>
#include "GObject.h"
#include "SockMsg.h"
#include "TennisDlg.h"
#include "Selfible.h"
class GBall;
class GBar;

#define UPDATE_PERIOD 30
#define MARGIN 5
#define SAFE_REGION 50
#define DIR_LEFT -1
#define DIR_RIGHT 1
#define SERVER_PLAYER 1
#define CLIENT_PLAYER 0;
#define BLOCK_SIZE sizeof(DataPack)

using namespace std;

class GController
{
private:
	BOOL m_bPausing;
	BOOL m_bGaming;
	BOOL m_bLaunched;	//是否已发球
	GBall *m_ball;
	GBar *m_sbar;
	GBar *m_cbar;
	vector<CString> m_vecPicPath;
	vector<GObject*> m_vecGObjs;
	vector<Selfible*> m_vecpSf;
	deque<DataPack> m_deqPacks;
public:
	UINT m_nCurrPlayer;		//持球玩家	1服务器	0客户端
	CRect m_crActiveRect;
	UINT m_nBuffs;
	UINT m_nObs;
	TennisDlg *m_pDlg;
	GController();
	GController(TennisDlg *Dlg);
	~GController();
	void Init();		
	void Start();
	void Restart();
	void CreateObstacle();
	void CreateBuff();
	void UpdateObjs();
	void PaintObjs(CDC *dc);
	void SendPack();
	void PackObjs();
	void UnpackData();
	void PopAll();
	void PushInDeque(DataPack &Data);
	void ProcToken(UINT Token,CString User);
	void ProcBuff(UINT BuffID,INT pLAYER);
	void SetGaming(BOOL flag);
	void SetLaunched(BOOL flag);
	void PopGObjs(BYTE Index);
	BOOL IsGaming();
	BOOL HasLaunched();
	vector<GObject*>* GetGObjsPtr();
	
};

