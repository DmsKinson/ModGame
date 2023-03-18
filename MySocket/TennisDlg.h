
// MySocketDlg.h : header file
//

#pragma once
#define GAME_EVENT 1
#define CONNECT_EVENT 2
#define SEND_EVENT 3

#include "SockMsg.h"
#include "CSock.h"
#include "SSock.h"
#include "afxcmn.h"
#include "afxwin.h"
class GController;

// TennisDlg dialog
class TennisDlg : public CDialogEx
{
// Construction
private:
	int m_nTryCount;
	UINT m_nPort;
	BOOL m_bIsServer;
	BOOL m_bIsDown;
	CRect m_crActiveRect;
	CRect m_crPaintRect;
	CCSock *m_sktCSock;
	CSSock *m_sktSSock;
	GController *m_gcCtl;
public:
	TennisDlg(CWnd* pParent = NULL);	// standard constructor
	~TennisDlg();
	UINT SendData(void *src,UINT nType,UINT nLength);
	SockMsg m_smMsg;
	CString m_cstrServerAddr;
	HWND handle;
	BOOL IsServer();
	CRect& GetPaintRect();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSOCKET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	CEdit m_edtTalk;
	CStatic m_csHostIP;
	CEdit m_edtIP;
	void AddLog(CString cstrLog,CTime ctTime);
	void ClearLog();
	void AddContent(CString cstrVal, CString cstrUsrName,CTime ctTime);
	void ClearContent();
	CString m_cstrLog;
	CString m_cstrContent;
	CButton m_btnAbort;
	CButton m_btnConnect;
	CButton m_btnHost;
	CButton m_btnSend;
	CButton m_btnRestart;
	afx_msg void OnBnClickedAbort();
	afx_msg void OnClose();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedHost();
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);
	CButton m_btnStart;
	CButton m_btnPause;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
