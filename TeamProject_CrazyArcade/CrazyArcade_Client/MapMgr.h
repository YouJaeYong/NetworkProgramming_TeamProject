#pragma once
#include "Include.h"

class CTile;
class CBlock;

class CMapMgr
{
	DECLARE_SINGLETON(CMapMgr)
private:
	CMapMgr();
	~CMapMgr();

public:
	void AddTile(CTile* pTile);
	void AddBlock(CBlock* pTile);
	void Render(void);

	CTile* GetTile(int iIndex);

	vector<CBlock*>* GetVecBlock() { return &m_vecBlock; }
	vector<CBlock*>* GetVecBush() { return &m_vecBush; }
	void DeleteBlock(CBlock* pBlock);
public:
	void LoadMap(void);
	void LoadTile(void);
	void LoadObject(void);


	bool TileReady(void);
	bool BlockReady();

	void MapDestory(void) { TileClear();  ObjectClear(); }
	void SetBlockCnt(int _iCnt) { m_iBlockCnt = _iCnt; }

	void DeleteBush(CBlock* pBlock);
private:
	void TileClear(void);
	void ObjectClear(void);

	static bool BlockSort(CBlock* pDst, CBlock* pSrc);
private:
	vector<CTile*>			m_vecTile;
	vector<CBlock*>			m_vecBlock;
	vector<CBlock*>			m_vecBush;
	int						m_iBlockCnt;
};

