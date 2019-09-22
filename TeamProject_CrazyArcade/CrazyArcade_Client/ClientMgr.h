#pragma once
#include "Include.h"

class CClient;
class CClientMgr
{
	DECLARE_SINGLETON(CClientMgr)
private:
	CClientMgr();
	~CClientMgr();
public:
	void AddClient(CClient* pClient);
	CClient* FindClient(const char* szID);
	void SetMe(CClient* pClient) { m_pMe = pClient; }
	CClient* GetMe(void) { return m_pMe; }
	vector<CClient*>* GetClient(void) { return &m_vecClient; }
public:
	vector<CClient*>			m_vecClient;
	CClient*					m_pMe;
};
