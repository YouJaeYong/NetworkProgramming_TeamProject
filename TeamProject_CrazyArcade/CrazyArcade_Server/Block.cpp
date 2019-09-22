#include "Block.h"

CBlock::CBlock(int _iBlockType)
{
	m_iBlockType = _iBlockType;
	m_iObjType = E_OBJ_LIST_TYPE_BLOCK;
}

CBlock::~CBlock()
{
}

void CBlock::Initiailze()
{

}

int CBlock::Update()
{
	CalcRect();
	return 0;
}
bool CBlock::CalcTileIndex(VECTOR2 vPos, int* pX, int* pY)
{
	int iOffsetX = 20;
	int iOffsetY = 40;

	*pX = int(vPos.x - iOffsetX) / TILECX;
	*pY = int(vPos.y - iOffsetY) / TILECY;

	if (*pX < 0 || *pY < 0 || *pX >= TILE_CNTX || *pY >= TILE_CNTY)
		return false;

	return true;
}

void CBlock::CalcRect()
{
	int iOffsetX = 20;
	int iOffsetY = 40;

	int iX = 0;
	int iY = 0;

	if (CalcTileIndex(m_vPos, &iX, &iY) == false)
		return;

	m_rcRectSize.left = -TILECX;
	m_rcRectSize.right = TILECX;

	m_rcRectSize.top = -TILECY;
	m_rcRectSize.bottom = TILECY;


	if (m_iBlockType != E_BLOCK_TYPE_BUSH)
	{
		m_rcRect.left = long(iX * TILECX + iOffsetX) + 2;
		m_rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 2;

		m_rcRect.top = long(iY * TILECY + iOffsetY) + 2;
		m_rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 2;
	}
	else
	{
		m_rcRect.left = long(iX * TILECX + iOffsetX) + 5;
		m_rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 5;

		m_rcRect.top = long(iY * TILECY + iOffsetY) + 5;
		m_rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 5;
	}


	m_vRectCenter.x = float((m_rcRect.left + m_rcRect.right) / 2);
	m_vRectCenter.y = float((m_rcRect.top + m_rcRect.bottom) / 2);
}
