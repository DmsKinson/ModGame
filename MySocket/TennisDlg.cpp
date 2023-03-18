
// MySocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"
#include "TennisDlg.h"
#include "afxdialogex.h"
#include "SockMsg.h"
#include "CSock.h"
#include "GController.h"
#include "GBall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMySocketDlg dialog

TennisDlg::TennisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYSOCKET_DIALOG, pParent)
	, m_cstrLog(_T(""))
	, m_cstrContent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sktCSock = NULL;
	m_sktSSock = NULL;
	m_bIsServer = FALSE;
	m_bIsDown = FALSE;
	m_gcCtl = NULL;
	m_smMsg.Fresh();
}

TennisDlg::~TennisDlg()
{
	m_sktCSock = NULL;
	m_sktSSock = NULL;
	m_gcCtl = NULL;
}


UINT TennisDlg::SendData(void * src,UINT nType,UINT nLength)
{
	if (m_sktCSock != NULL && m_sktCSock->m_hSocket != INVALID_SOCKET )
	{
		Msg cache;
		cache.ctTime = NOW_TIME;
		cache.nType = nType;
		memcpy(cache.tszValue + USER_LENGTH, src, nLength);
		memcpy(cache.tszValue, LOCAL_USER, USER_LENGTH);
		m_sktCSock->m_nLength = sizeof(cache);
		//m_sktCSock->Send(&cache, sizeof(cache));
		m_sktCSock->PushMsg(cache);
		//m_sktCSock->FillBuffer(cache);
		m_sktCSock->AsyncSelect(FD_WRITE);
		//m_sktCSock->SetReady(FALSE);
		return 1;
	}
	else
	{
		return 0;
	}
	
}

BOOL TennisDlg::IsServer()
{
	return m_bIsServer;
}

CRect & TennisDlg::GetPaintRect()
{
	return m_crActiveRect;
}

void TennisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR_IP, m_edtIP);
	DDX_Control(pDX, IDC_EDTTALK, m_edtTalk);
	DDX_Control(pDX, IDC_HOSTIP, m_csHostIP);
	DDX_Text(pDX, IDC_EDTLOG, m_cstrLog);
	DDX_Text(pDX, IDC_EDTCONTENT, m_cstrContent);
	DDX_Control(pDX, IDC_ABORT, m_btnAbort);
	DDX_Control(pDX, IDC_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_HOST, m_btnHost);
	DDX_Control(pDX, IDC_SEND, m_btnSend);
	DDX_Control(pDX, IDC_RESTART, m_btnRestart);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_PAUSE, m_btnPause);
}

BEGIN_MESSAGE_MAP(TennisDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &TennisDlg::OnBnClickedConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEND, &TennisDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_HOST, &TennisDlg::OnBnClickedHost)
	ON_MESSAGE(WM_USER_RECVMSG, &TennisDlg::OnRecvMsg)
	ON_BN_CLICKED(IDC_ABORT, &TennisDlg::OnBnClickedAbort)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, &TennisDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_PAUSE, &TennisDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_RESTART, &TennisDlg::OnBnClickedRestart)
END_MESSAGE_MAP()


// CMySocketDlg message handlers

BOOL TennisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	AddLog(_T("Domitory Game Platform v1.0"),NOW_TIME);
	handle = GetSafeHwnd();
	CString str("HostIP:");
	m_csHostIP.SetWindowTextW(str + pApp->m_cstrHostIP);		//Display local IP
	CRect WndRect, RestartRect;
	GetClientRect(WndRect);
	m_btnRestart.GetWindowRect(RestartRect);
	ScreenToClient(RestartRect);
	UINT TopLeftX = WndRect.TopLeft().x;
	UINT TopLeftY = WndRect.TopLeft().y;
	UINT BottomRightX = RestartRect.BottomRight().x;
	UINT BottomRightY = RestartRect.TopLeft().y - MARGIN;
	m_crActiveRect.SetRect(TopLeftX, TopLeftY, BottomRightX, BottomRightY);
	/*m_crPaintRect.CopyRect(m_crActiveRect);
	m_crPaintRect.OffsetRect(-MARGIN, -MARGIN);*/
	//初始化游戏组件
	if (m_gcCtl == NULL)
		m_gcCtl = new GController(this);
	//m_crActiveRect.CopyRect(m_crActiveRect);
	m_nPort = 1088;		//default port
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void TennisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		
		if (m_gcCtl != NULL)
		{
			dc.Draw3dRect(m_crActiveRect,  RGB(160, 160, 160), RGB(255, 255, 255));
		}	
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR TennisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL TennisDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return true;
	case VK_RETURN:
		if (m_edtTalk.GetWindowTextLengthW() > 0)
			OnBnClickedSend();
		return true;
	case VK_LEFT:
	{
		if (m_gcCtl->IsGaming())
		{
			if (pMsg->message == WM_KEYDOWN && !m_bIsDown)
			{
				AddLog(_T("left down"), NOW_TIME);
				DataPack cache;
				cache.Token = TK_LEFT_DOWN;
				m_bIsDown = TRUE;
				SendData(&cache, TP_TENNIS, sizeof(cache));
			}
			else if (pMsg->message == WM_KEYUP && m_bIsDown)
			{
				AddLog(_T("left up"), NOW_TIME);
				DataPack cache;
				cache.Token = TK_LEFT_UP;
				m_bIsDown = FALSE;
				SendData(&cache, TP_TENNIS, sizeof(cache));
			}
		}
	}
	case VK_RIGHT:
	{
		if (m_gcCtl->IsGaming())
		{
			if (pMsg->message == WM_KEYDOWN && !m_bIsDown)
			{
				DataPack cache;
				cache.Token = TK_RIGHT_DOWN;
				m_bIsDown = TRUE;
				SendData(&cache, TP_TENNIS, sizeof(cache));
			}
			else if (pMsg->message == WM_KEYUP && m_bIsDown)
			{
				DataPack cache;
				cache.Token = TK_RIGHT_UP;
				m_bIsDown = FALSE;
				SendData(&cache, TP_TENNIS, sizeof(cache));
			}
		}
		break;
	}
	case VK_UP:
	{
		if (!m_gcCtl->HasLaunched() && m_gcCtl->IsGaming())
		{
			DataPack cache;
			cache.Token = TK_UP;
			SendData(&cache, TP_TENNIS, sizeof(cache));
		}
		break;
	}
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//收到消息的处理函数
LRESULT TennisDlg::OnRecvMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_smMsg.IsEmpty())
	{
		switch (m_smMsg.GetType())
		{
		case TP_LOG:
			AddLog(m_smMsg.GetModMem(),m_smMsg.GetTime());
			break;
		case TP_TALK:
			AddContent(m_smMsg.GetModMem(),m_smMsg.GetUser(),m_smMsg.GetTime());
			break;
		case TP_QUIT:
			AddLog(m_smMsg.GetModMem(), m_smMsg.GetTime());
			m_sktCSock->PreClose();
			break;
		case TP_TENNIS:
		{
			DataPack cache;
			memcpy(&cache, m_smMsg.GetModMem(), sizeof(cache));
			if(cache.Token<=5)			//游戏控制事件
			{
				m_gcCtl->ProcToken(cache.Token,m_smMsg.GetUser());
			}
			else if(cache.Token<=10 && IsServer())		//键盘响应事件
			{
				m_gcCtl->ProcToken(cache.Token, m_smMsg.GetUser());
			}
			else
			{
				if (!IsServer())				//客户消息队列
				{
					m_gcCtl->PushInDeque(cache);
				}
			}
		}
		default:
			break;
		}
		m_smMsg.Fresh();
	}
	return LRESULT();
}

void TennisDlg::AddLog(CString cstrLog, CTime ctTime)
{
	UpdateData(TRUE);
	CString cache = ctTime.Format(_T("%Y/%m/%d %H:%M:%S "))+_T("\r\n") + cstrLog + _T("\r\n");
	m_cstrLog += cache;
	UpdateData(FALSE);
}

void TennisDlg::ClearLog()
{
	UpdateData(TRUE);
	m_cstrLog = "";
	UpdateData(FALSE);
}

void TennisDlg::AddContent(CString cstrVal,CString cstrUsrName,CTime ctTime)
{
	UpdateData(TRUE);
	CString cache = ctTime.Format(_T("%Y/%m/%d %H:%M:%S ")) + cstrUsrName + _T("\r\n") + cstrVal + _T("\r\n");
	m_cstrContent += cache;
	UpdateData(FALSE);
}

void TennisDlg::ClearContent()
{
	UpdateData(TRUE);
	m_cstrContent = "";
	UpdateData(FALSE);
}


void TennisDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int i = 0;
	if (nIDEvent == CONNECT_EVENT)
	{
		if (m_sktCSock != NULL && m_sktCSock->m_hSocket == INVALID_SOCKET)		//re-create valid socket
		{
			BOOL bFlag = m_sktCSock->Create(0, SOCK_STREAM, FD_CONNECT);
			if (!bFlag)
			{
				m_sktCSock->ProcErrorCode(GetLastError());
				m_sktCSock->Close();
				return;
			}
		}
		m_sktCSock->Connect(m_cstrServerAddr, m_nPort);
		m_nTryCount++;
		if (m_nTryCount > 10 || m_sktCSock->IsConnected())
		{
			KillTimer(CONNECT_EVENT);
			if (m_nTryCount > 10)
				AfxMessageBox(_T("Out of time!"));
			return;
		}
	}
	if (nIDEvent == GAME_EVENT)
	{
		if (m_gcCtl->IsGaming())
		{
			CClientDC dc(this);
			CDC dcMem;
			CBitmap canvas;
			dcMem.CreateCompatibleDC(&dc);
			canvas.CreateCompatibleBitmap(&dc, m_crActiveRect.Width(), m_crActiveRect.Height());
			dcMem.SelectObject(canvas);
			dcMem.FillSolidRect(m_crActiveRect, RGB(255, 255, 255));
			if (IsServer())
			{
				m_gcCtl->CreateBuff();
				m_gcCtl->CreateObstacle();
				m_gcCtl->PaintObjs(&dcMem);
				m_gcCtl->UpdateObjs();
				m_gcCtl->PackObjs();
				m_gcCtl->SendPack();
				////i++;
			}
			if(!IsServer())
			{
				m_gcCtl->UnpackData();
				m_gcCtl->PaintObjs(&dcMem);
				//m_gcCtl->PopAll();
			}
			dc.BitBlt(0, 0, m_crActiveRect.Width(), m_crActiveRect.Height(), &dcMem, 0, 0, SRCCOPY);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void TennisDlg::OnBnClickedAbort()
{
	if (IsServer())
	{
		if (m_sktSSock != NULL)
		{
			m_sktSSock->PreClose();
			m_sktSSock = NULL;
			m_bIsServer = FALSE;
			m_btnConnect.EnableWindow(TRUE);
			m_btnHost.EnableWindow(TRUE);
		}
	}
	else
	{
		if (m_sktCSock != NULL)
		{
			CString t_cstr(" Quit");
			t_cstr = LOCAL_IP + t_cstr;
			Msg cache(t_cstr.GetBuffer(), LOCAL_USER, t_cstr.GetLength() * sizeof(TCHAR), TP_QUIT, NOW_TIME);
			m_sktCSock->Send(&cache, sizeof(cache));
			m_sktCSock->PreClose();
			AddLog(_T("Connection interrupt!"), NOW_TIME);
			m_sktCSock = NULL;
		}
	}
}

void TennisDlg::OnBnClickedHost()
{
	
	if (m_sktSSock == NULL)
		m_sktSSock = new CSSock();
	if (m_sktSSock->m_hSocket == INVALID_SOCKET)		//re-create valid socket
	{
		BOOL bFlag = m_sktSSock->Create(1088, SOCK_STREAM, FD_ACCEPT);
		if (!bFlag)
		{
			m_sktSSock->ProcErrorCode(GetLastError());
			m_sktSSock->PreClose();
			return;
		}
	}
	CMySocketApp* pApp = (CMySocketApp*)AfxGetApp();
	m_cstrServerAddr = pApp->m_cstrHostIP;
	if (!m_sktSSock->Listen())
	{
		if (m_sktSSock->GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("Error"));
			m_sktSSock->PreClose();
		}
	}
	///Become a server;
	m_bIsServer = TRUE;		
	AddLog(_T("Waiting for connecting... ") , NOW_TIME);
	///create and connect self-client 
	if (m_sktCSock == NULL)
		m_sktCSock = new CCSock();
	///Set timer to connect
	SetTimer(CONNECT_EVENT, 1000, NULL);		
	m_nTryCount = 0;
	///Forbid button
	m_btnConnect.EnableWindow(FALSE);
	m_btnHost.EnableWindow(FALSE);
}

void TennisDlg::OnBnClickedSend()
{
	if (m_sktCSock != NULL && m_sktCSock->IsConnected())
	{
		CString cache;
		m_edtTalk.GetWindowTextW(cache);
		Msg temp(cache.GetBuffer(), LOCAL_USER, cache.GetLength() * sizeof(TCHAR), TP_TALK, NOW_TIME);
		m_sktCSock->m_nLength = sizeof(temp);
		m_sktCSock->PushMsg(temp);
		//m_sktCSock->FillBuffer(temp);
		m_sktCSock->AsyncSelect(FD_WRITE);
		m_edtTalk.SetWindowTextW(_T(""));
	}
	else
	{
		AfxMessageBox(_T(" No Connection."));
	}
}

void TennisDlg::OnBnClickedConnect()
{
	if (m_sktCSock == NULL)
	{
		AfxSocketInit();
		m_sktCSock = new CCSock();
	}
	m_edtIP.GetWindowText(m_cstrServerAddr);	//Get target IP
	SetTimer(CONNECT_EVENT, 1000, NULL);		//Set timer to connect

	m_nTryCount = 0;
}


void TennisDlg::OnClose()
{
	OnBnClickedAbort();
	CDialogEx::OnClose();
}

//todo
void TennisDlg::OnBnClickedStart()
{
	if (m_sktCSock != NULL)
	{
		if (m_gcCtl->IsGaming())
			return;
		if (IsServer())
		{
			DataPack cache;
			cache.Token = TK_START;
			///send start signal to client
			SendData(&cache, TP_TENNIS, sizeof(cache));
			//m_gcCtl->Start();
		}
		else
		{
			AfxMessageBox(_T("Host is father!"));
		}
	}
	else
	{
		AfxMessageBox(_T("No Connection!"));
	}
	
}

//todo
void TennisDlg::OnBnClickedPause()
{
	if (m_sktCSock != NULL)
	{
		DataPack cache;
		cache.Token = TK_PAUSE;
		///send signal to client
		SendData(&cache, TP_TENNIS, sizeof(cache));
	}
	else
	{
		AfxMessageBox(_T("No Connection!"));
	}
}

//todo
void TennisDlg::OnBnClickedRestart()
{
	if (m_sktCSock != NULL)
	{
		if (m_gcCtl->IsGaming())
			return;
		if (IsServer())
		{
			DataPack cache;
			cache.Token = TK_RESTART;
			///send signal to client
			SendData(&cache, TP_TENNIS, sizeof(cache));
		}
		else
		{
			AfxMessageBox(_T("Host is father!"));
		}
	}

	else
	{
		AfxMessageBox(_T("No Connection!"));
	}
	
}
