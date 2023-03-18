#include "stdafx.h"
#include "GController.h"
#include "GBall.h"
#include "GBar.h"
#include "GBuff.h"
#include "GObstacle.h"
#include "TennisDlg.h"
#include "MySocket.h"
#include "Selfible.h"


void GController::UpdateObjs()
{
	if (m_pDlg->IsServer())
	{
		Selfible *ptr;
		for (int i = 0; i<m_vecpSf.size(); i++)
		{
			ptr = (Selfible*)m_vecpSf.at(i);
			ptr->UpdateSelf();
		}
		char buffer[4] = { '\0' };
	}
}

void GController::SendPack()
{
	while (!m_deqPacks.empty())
	{
		//TRACE("Send\nremain %d\n", m_deqPacks.size());
		DataPack cache;
		cache = m_deqPacks.front();
		if (m_pDlg->SendData(&cache, TP_TENNIS, sizeof(cache)))
			m_deqPacks.pop_front();
	}
}

void GController::PackObjs()
{
	Selfible *ptr;
	for (int i = 0; i < m_vecpSf.size(); i++)
	{
		DataPack cache;
		ptr = (Selfible*)m_vecpSf.at(i);
		m_deqPacks.push_back(ptr->PackSelf());
		//cache = ptr->PackSelf();
	}
}

void GController::UnpackData()
{
	while (!m_deqPacks.empty())
	{
		DataPack cache;
		cache = m_deqPacks.front();
		switch (cache.Token)
		{
		case TK_CREATE:
		{
			switch (cache.Type)
			{
			case GO_BALL:
			{
				GBall *ball = new GBall(cache.Index, cache.Pos, this, cache.Vel);
				m_vecGObjs.push_back(ball);
				m_vecpSf.push_back(ball);
				break;
			}
			case GO_BAR:
			{
				GBar *bar = new GBar(cache.Index, cache.Pos, this, cache.Vel);
				m_vecGObjs.push_back(bar);
				m_vecpSf.push_back(bar);
				break;
			}
			case GO_BUFF:
			{
				GBuff *buff = new GBuff(cache.Index, cache.Pos, cache.Abbr, this, cache.Vel);
				m_vecGObjs.push_back(buff);
				m_vecpSf.push_back(buff);
				break;
			}
			case GO_OBSTACLE:
			{
				GObstacle *obs = new GObstacle(cache.Index, cache.Pos,this,cache.Vel.x,cache.Vel.y, FPoint(0,0), cache.Abbr);
				m_vecGObjs.push_back(obs);
				m_vecpSf.push_back(obs);
				break;
			}
			default:
				break;
			}
			break;
		}
		case TK_UPDATE:
		{
			for (vector<GObject*>::iterator it = m_vecGObjs.begin();it != m_vecGObjs.end();it++)
			{
				//TRACE("cache.index is %d\n", cache.Index);
				//TRACE("*it index is %d\n", (*it)->GetIndex());
				if (cache.Index == (*it)->GetIndex())
				{
					if (cache.Type == GO_BALL)
					{
						//TRACE("UPDATE BALL;\n");
						GBall *ptr = (GBall*)(*it);
						ptr->SetPos(cache.Pos);
						ptr->SetAbbr(cache.Abbr);
						ptr->SetRect(CRect(cache.Pos.x - cache.Abbr, cache.Pos.y - cache.Abbr, cache.Pos.x + cache.Abbr, cache.Pos.y + cache.Abbr));
						continue;
					}
					if (cache.Type == GO_BUFF)
					{
						//TRACE("UPDATE BUFF\n");
						continue;
					}
					if (cache.Type == GO_OBSTACLE)
					{
						//TRACE("UPDATE OBS\n");
						((GObstacle*)(*it))->SetAbbr(cache.Abbr);
						continue;
					}
					if (cache.Type == GO_BAR)
					{
						//TRACE("UPDATE BAR\n");
						GBar *ptr = (GBar*)(*it);
						ptr->SetPos(cache.Pos);
						ptr->SetRect(CRect(cache.Pos.x - cache.Abbr / 2, cache.Pos.y - DEFAULT_HEIGHT/2, cache.Pos.x + cache.Abbr / 2, cache.Pos.y + DEFAULT_HEIGHT / 2));
						ptr->SetSize(cache.Abbr);
						continue;
					}
				}
			}
			break;
		}
		case TK_DEL:
		{
			PopGObjs(cache.Index);
			break;
		}
		default:
			break;
		}
		m_deqPacks.pop_front();
	}
}

void GController::PopAll()
{
	m_vecpSf.clear();
	m_vecGObjs.clear();
}


void GController::PaintObjs(CDC *dc)
{
	Selfible *ptr;
	for (INT i = 0;i<m_vecpSf.size();i++)
	{
		ptr = (Selfible*)m_vecpSf.at(i);
		ptr->PaintSelf(dc);
	}
}

void GController::PushInDeque(DataPack & Data)
{
	m_deqPacks.push_back(Data);
}

void GController::ProcToken(UINT Token,CString User)
{
	switch (Token)
	{
	case TK_START:
		Start();
		//TRACE("start\n");
		break;
	case TK_PAUSE:
		SetGaming(!IsGaming());
		//TRACE("pause\n");
		break;
	case TK_RESTART:
		Init();
		Start();
		//TRACE("restart\n");
		break;
	case TK_SWIN:
		if(!m_pDlg->IsServer())
			AfxMessageBox(_T("You Lose!"));
		else
			AfxMessageBox(_T("You Win!"));
		break;
	case TK_CWIN:
		if(!m_pDlg->IsServer())
			AfxMessageBox(_T("You Win!"));
		else
			AfxMessageBox(_T("You Lose!"));
		break;
	case TK_LEFT_DOWN:
		if (User == CString(LOCAL_USER))
		{
			TRACE("SERVER\n");
			m_sbar->SetDir(DIR_LEFT);
		}
		else
		{
			TRACE("CLIENT\n");
			m_cbar->SetDir(DIR_RIGHT);
		}
		//TRACE("left down\n");
		break;
	case TK_RIGHT_DOWN:
		if (User == CString(LOCAL_USER))
		{
			m_sbar->SetDir(DIR_RIGHT);
		}
		else
		{
			m_cbar->SetDir(DIR_LEFT);
		}
		//TRACE("right down\n");
		break;
	case TK_RIGHT_UP:
	case TK_LEFT_UP:
		//TRACE("key up\n");
		if (User == CString(LOCAL_USER))
		{
			m_sbar->SetDir(0);
		}
		else
		{
			m_cbar->SetDir(0);
		}
		break;
	case TK_UP:
		if (!HasLaunched())
		{
			if (User == CString(LOCAL_USER) && m_nCurrPlayer == 1)
			{
				m_ball->SetSpeed(FPoint(0, -3).Add(FPoint(rand() % 4 - 2, rand() % 4 - 2)));
				m_bLaunched = TRUE;
			}
			if (User != CString(LOCAL_USER) && m_nCurrPlayer == 0)
			{
				m_ball->SetSpeed(FPoint(0, 3).Add(FPoint(rand() % 4 - 2, rand() % 4 - 2)));
				m_bLaunched = TRUE;
			}
		}
		break;
	default:
		break;
	}
}

void GController::ProcBuff(UINT BuffID,INT Player)
{
	switch (BuffID)
	{
	case BALL_SPEED_UP:
		m_ball->ChangeSpeed(1.5);
		break;
	case BALL_SPEED_DOWN:
		m_ball->ChangeSpeed(0.8);
		break;
	case BALL_SIZE_UP:
		m_ball->SetAbbr(5);
		break;
	case BALL_SIZE_DOWN:
		m_ball->SetAbbr(-5);
		break;
	case BAR_SIZE_UP:
		if (Player == SERVER_PLAYER)
		{
			m_sbar->ChangeSize(10);
		}
		else
		{
			m_cbar->ChangeSize(10);
		}
		break;
	case BAR_SIZE_DOWN:
		if (Player == SERVER_PLAYER)
		{
			m_sbar->ChangeSize(-10);
		}
		else
		{
			m_cbar->ChangeSize(-10);
		}
		break;
	case BAR_SPEED_UP:
		if (Player == SERVER_PLAYER)
		{
			m_sbar->ChangeSpeed(1.2);
		}
		else
		{
			m_cbar->ChangeSpeed(1.2);
		}
		break;
	case BAR_SPEED_DOWN:
		if (Player == SERVER_PLAYER)
		{
			m_sbar->ChangeSpeed(0.8);
		}
		else
		{
			m_cbar->ChangeSpeed(0.8);
		}
		break;
	case DIR_REVERSE:
		if (Player == SERVER_PLAYER)
		{
			m_sbar->Reverse();
		}
		else
		{
			m_cbar->Reverse();
		}
		break;
	default:
		break;
	}
}


//todo
void GController::PopGObjs(BYTE Index)
{
	int len = m_vecGObjs.size();
	vector<GObject*>::iterator oit = m_vecGObjs.begin();
	vector<Selfible*>::iterator sit = m_vecpSf.begin();
	for (; oit!=m_vecGObjs.end() && sit!=m_vecpSf.end(); oit++,sit++)
	{
		BYTE temp = (*oit)->GetIndex();
		if (Index == temp)
		{
			delete *oit;
			m_vecGObjs.erase(oit);
			m_vecpSf.erase(sit);
			break;
		}
	}
}



vector<GObject*>* GController::GetGObjsPtr()
{
	return &m_vecGObjs;
}

GController::GController()
{
}

GController::GController(TennisDlg *Dlg)
{
	m_pDlg = Dlg;
	Init();
	//init paint rect
	m_crActiveRect = m_pDlg->GetPaintRect();
}

void GController::Init()
{
	m_bGaming = FALSE;
	m_bLaunched = FALSE;
	m_bPausing = FALSE;
	m_nBuffs = 0;
	m_nObs = 0;
	m_vecGObjs.clear();
	m_vecpSf.clear();
	m_deqPacks.clear();
}

//TODO
void GController::Start()
{
	if (m_pDlg->IsServer())
	{
		srand(time(0));
		m_nCurrPlayer = rand() % 2;
		if (m_nCurrPlayer)
		{
			BYTE Index = rand() % 256;
			FPoint Pos(m_crActiveRect.CenterPoint().x, m_crActiveRect.bottom - DEFAULT_HEIGHT - DEFAULT_RADIUS);
			FPoint Vel(0, -5);
			GBall *ball = new GBall(Index,Pos, this, Vel);
			m_ball = ball;
			DataPack cache;
			cache.Index = Index;
			cache.Type = GO_BALL;
			cache.Token = TK_CREATE;
			Pos.x = m_crActiveRect.Width() - Pos.x;
			Pos.y = m_crActiveRect.Height() - Pos.y;
			cache.Pos = Pos;
			cache.Abbr = DEFAULT_RADIUS;
			m_vecGObjs.push_back(ball);
			m_vecpSf.push_back(ball);
			m_deqPacks.push_back(cache);
		}
		else
		{
			BYTE Index = rand() % 256;
			FPoint Pos(m_crActiveRect.CenterPoint().x, m_crActiveRect.top + DEFAULT_HEIGHT + DEFAULT_RADIUS );
			FPoint Vel(0, 5);
			GBall *ball = new GBall(Index,Pos, this, Vel);
			m_ball = ball;
			DataPack cache;
			cache.Index = Index;
			cache.Type = GO_BALL;
			cache.Token = TK_CREATE;
			Pos.x = m_crActiveRect.Width() - Pos.x;
			Pos.y = m_crActiveRect.Height() - Pos.y;
			cache.Pos = Pos;
			cache.Abbr = DEFAULT_RADIUS;
			m_vecGObjs.push_back(ball);
			m_vecpSf.push_back(ball);
			m_deqPacks.push_back(cache);
		}
		//SendPack();
		BYTE Index = rand() % 256;
		FPoint Pos(m_crActiveRect.CenterPoint().x, m_crActiveRect.bottom - DEFAULT_HEIGHT / 2);
		FPoint Vel(5, 0);
		GBar *bar1 = new GBar(Index,Pos, this, Vel);
		m_sbar = bar1;
		DataPack cache1;
		cache1.Index = Index;
		cache1.Type = GO_BAR;
		cache1.Token = TK_CREATE;
		Pos.x = m_crActiveRect.Width() - Pos.x;
		Pos.y = m_crActiveRect.Height() - Pos.y;
		cache1.Abbr = DEFAULT_WIDTH;
		cache1.Pos = Pos;
		m_deqPacks.push_back(cache1);

		Index = rand() % 256;
		Pos = FPoint(m_crActiveRect.CenterPoint().x, m_crActiveRect.top + DEFAULT_HEIGHT / 2);
		Vel = FPoint(5, 0);
		GBar *bar2 = new GBar(Index,Pos, this, Vel);
		m_cbar = bar2;
		DataPack cache2;
		cache2.Index = Index;
		cache2.Type = GO_BAR;
		cache2.Token = TK_CREATE;
		Pos.x = m_crActiveRect.Width() - Pos.x;
		Pos.y = m_crActiveRect.Height() - Pos.y;
		cache2.Abbr = DEFAULT_WIDTH;
		cache2.Pos = Pos;

		m_deqPacks.push_back(cache2);

		m_vecGObjs.push_back(bar1);
		m_vecGObjs.push_back(bar2);
		m_vecpSf.push_back(bar1);
		m_vecpSf.push_back(bar2);
	}
	SendPack();
	m_pDlg->SetTimer(GAME_EVENT, UPDATE_PERIOD, NULL);		//Start game timer
	m_bGaming = TRUE;
	m_pDlg->m_btnPause.SetWindowTextW(_T("Pause"));
}

GController::~GController()
{
	m_pDlg = NULL;
	m_ball = NULL;
	m_cbar = NULL;
	m_sbar = NULL;
	m_deqPacks.clear();
	m_vecGObjs.clear();
	m_vecpSf.clear();
	m_vecPicPath.clear();
}

BOOL GController::IsGaming()
{
	return m_bGaming;
}

void GController::SetGaming(BOOL flag)
{
	m_bGaming = flag;
	if (!flag)
		m_pDlg->m_btnPause.SetWindowText(_T("Resume"));
	else
		m_pDlg->m_btnPause.SetWindowText(_T("Pause"));
}



void GController::SetLaunched(BOOL flag)
{
	m_bLaunched = flag;
}

BOOL GController::HasLaunched()
{
	return m_bLaunched;
}

void GController::CreateObstacle()
{
	static long lastTime = time(0);
	if (m_nObs < 3 && time(0) - lastTime > 10)
	{
		m_nObs++;
		UINT UpBorder = m_crActiveRect.Height()/4;
		UINT DownBorder = UpBorder * 3;
		UINT x =200+ rand() % (m_crActiveRect.Width()-200);
		UINT y =UpBorder+ rand() % (DownBorder - UpBorder);
		UINT width = rand() % 151 + 50;
		UINT height = rand() % 151 + 50;
		INT Intensity = rand() % 5 - 1;
		BYTE Index = rand() % 256;
		GObstacle *ob = new GObstacle(Index,FPoint(x, y), this,width,height ,FPoint(0,0), Intensity);
		m_vecGObjs.push_back(ob);
		m_vecpSf.push_back(ob);

		DataPack cache;
		cache.Token = TK_CREATE;
		cache.Type = GO_OBSTACLE;
		cache.Index = Index;
		cache.Pos.x = m_crActiveRect.right - x;
		cache.Pos.y = m_crActiveRect.bottom - y;
		cache.Vel.x = width;
		cache.Vel.y = height;
		cache.Abbr = Intensity;
		PushInDeque(cache);
		lastTime = time(0);
	}
}

void GController::CreateBuff()
{
	static long lastTime = time(0);
	if (m_nBuffs < 5 && time(0) - lastTime>7)
	{
		m_nBuffs++;
		UINT UpBorder = m_crActiveRect.Height() / 4;
		UINT DownBorder = UpBorder * 3;
		UINT x = rand() % (m_crActiveRect.Width() - 200);
		UINT y = UpBorder + rand() % (DownBorder - UpBorder);

		UINT Kind = rand() % 9;
		BYTE Index = rand() % 256;
		GBuff *buff = new GBuff(Index, FPoint(x, y), Kind, this, FPoint(0, 0));
		m_vecGObjs.push_back(buff);
		m_vecpSf.push_back(buff);

		DataPack cache;
		cache.Token = TK_CREATE;
		cache.Type = GO_BUFF;
		cache.Index = Index;
		cache.Pos.x = m_crActiveRect.right- x;
		cache.Pos.y = m_crActiveRect.bottom- y;
		cache.Vel.x = 0;
		cache.Vel.y = 0;
		cache.Abbr = Kind;
		PushInDeque(cache);
		lastTime = time(0);
	}
}

void GController::Restart()
{
	Init();
	Start();
}
