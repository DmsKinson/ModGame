#pragma once


#include "SockMsg.h"
#include "SSock.h"
#include <deque>
using namespace std;

typedef deque<Msg> MsgDeque;
class CBridgeSock :public CAsyncSocket
{
private:
	Msg m_mBuffer;
	std::deque<Msg> m_deqBuffers;
public:
	UINT m_nLength;
	CSSock *m_ssFather;
	CBridgeSock();
	CBridgeSock(CSSock *ssFather);
	void PreClose();
	void PushMsg(Msg msg);
	void FillBuffer(Msg &src);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual ~CBridgeSock();
};
