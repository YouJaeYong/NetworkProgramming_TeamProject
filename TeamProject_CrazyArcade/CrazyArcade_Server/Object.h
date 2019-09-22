#pragma once
#include "Headers.h"
#include "Math.h"

class CObject
{
public:
	CObject();
	~CObject();
public:
	virtual void Initiailze() {};
	virtual int Update() { return 0; };

	void		SetID(int ID) { m_iID = ID; }
	int			GetID() { return m_iID; }
	int			GetObjType() { return m_iObjType; }
	VECTOR2*	GetPos() { return &m_vPos; }
	RECT		GetRect() { return m_rcRect; }
protected:
	VECTOR2		m_vPos;
	int			m_iID;

	int			m_iObjType;
	RECT		m_rcRect;
	RECT		m_rcRectSize;
};

