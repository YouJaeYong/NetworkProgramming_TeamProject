#pragma once
#include "Protocol.h"

class CCharacter_Server
{
public:
	CCharacter_Server();
	~CCharacter_Server();

public:
	CHAR_INFO_MSG*		GetInfo() { return &m_tInfo; }
	void		SetInfo(const CHAR_INFO_MSG& chinfo) { m_tInfo = chinfo; }

	void Update();
	VECTOR2* GetVecPrevPos() { return &m_vPrevPos; }
	RECT	GetRect() { return m_rcRect; }
	void CalcRect();
private:
	CHAR_INFO_MSG		m_tInfo;
	VECTOR2		m_vPrevPos;
	RECT		m_rcRect;
	RECT		m_rcRectSize;
};

