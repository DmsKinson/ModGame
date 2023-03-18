
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#include <afxsock.h>            // MFC socket extensions
#include "LineSegment2.h"
#include "SockMsg.h"


struct Pointf
{
	Pointf();
	Pointf(const Pointf &point);
	Pointf(const FPoint &point);
	Pointf(DOUBLE _x, DOUBLE _y);
	Pointf Mul(DOUBLE num);
	Pointf Add(const Pointf &point);
	Pointf Sub(const Pointf &point);
	DOUBLE Dot(const Pointf &point);
	Pointf Normalize();
	DOUBLE Magnitude();
	DOUBLE x, y;
};


FPoint PointAdd(const FPoint &pt1, const FPoint &pt2);
FPoint PointSub(const FPoint &pt1, const FPoint &pt2);
INT DotMulti(const FPoint &Pt1, const FPoint &Pt2);
FPoint ScalarMulti(const FPoint &Pt1, const DOUBLE Num);
BOOL SegmentIntersect(FPoint p1, FPoint p2, FPoint p3, FPoint p4);
DOUBLE Direction(FPoint p1, FPoint p2, FPoint p3);
BOOL OnSegment(FPoint p1, FPoint p2, FPoint p3);

#define WM_USER_RECVMSG WM_USER + 1
#define NOW_TIME CTime::GetCurrentTime()
#define LOCAL_USER ((CMySocketApp*)AfxGetApp())->m_tszUser
#define LOCAL_IP ((CMySocketApp*)AfxGetApp())->m_cstrHostIP
#define SWEEP_EPSILON 0.001
#define MAX_VALUE 1.7e308