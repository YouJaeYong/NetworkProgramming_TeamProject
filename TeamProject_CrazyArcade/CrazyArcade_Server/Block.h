#pragma once
#include "Object.h"
#include "Define.h"
#include "Enum.h"


class CBlock :
	public CObject
{
public:
	CBlock(int _iBlockType);
	virtual ~CBlock();
public:
	virtual void Initiailze();
	virtual int Update();

	void SetItemType(int _item) { m_iItemType = _item;}
	int GetBlockType() { return m_iBlockType; }
private:
	bool CalcTileIndex(VECTOR2	vPos, int* pX, int* pY);
	void CalcRect();

private:
	VECTOR2				m_vRectCenter;
	int					m_iBlockType;
	int					m_iItemType;

};

