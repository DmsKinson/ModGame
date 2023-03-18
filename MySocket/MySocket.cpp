
// MySocket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MySocket.h"
#include "TennisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMySocketApp

BEGIN_MESSAGE_MAP(CMySocketApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMySocketApp construction

CMySocketApp::CMySocketApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMySocketApp object

CMySocketApp theApp;


// CMySocketApp initialization

BOOL CMySocketApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	memset(m_tszUser, 0, USER_LENGTH);
	GetLocalAddress(m_cstrHostIP);		//Get local IP
	TennisDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, _T("Warning: dialog creation failed, so application is terminating unexpectedly.\n"));
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif
	return FALSE;
}

void CMySocketApp::GetLocalAddress(CString &wszAdrr)
{
	TCHAR HostName[USER_LENGTH] = {'\0'};
	char t_hostname[USER_LENGTH] = {'\0'};
	gethostname(t_hostname, sizeof(HostName));// 获得本机主机名.
	UINT nBufferLength = MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, 0);
	MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, MultiByteToWideChar(CP_ACP, 0, t_hostname, -1, HostName, 0)); 
	memcpy(m_tszUser, HostName,nBufferLength*sizeof(TCHAR));
	hostent* hn;
	hn = gethostbyname(t_hostname);//根据本机主机名得到本机ip
	wszAdrr = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式
}







