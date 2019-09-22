#pragma once
#include "Include.h"

#define GETTIME			CTimeMgr::GetInstance()->GetTime()

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)
private:
	CTimeMgr();
	~CTimeMgr();
public:
	float GetTime(void);

public:
	void InitTime(void);
	void SetTime(void);

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float		m_fTime;
};

