#pragma once
struct Msg;
#include <deque>
//client socket
class CCSock : public CAsyncSocket
{
private:
	BOOL m_bConnected;
	std::deque<Msg> m_deqBuffers;
public:
	UINT m_nLength;
	Msg m_mBuffer;
	CCSock();
	void SetStatus(BOOL stat);
	BOOL IsConnected();
	void PushMsg(Msg msg);
	void FillBuffer(Msg &src);
	void ProcErrorCode(int nErrorCode);
	void PreClose();
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual ~CCSock();
};

