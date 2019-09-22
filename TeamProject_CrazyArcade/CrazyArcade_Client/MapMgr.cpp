#include "stdafx.h"
#include "MapMgr.h"
#include "Tile.h"
#include "GameMgr.h"
#include "Block.h"
#include "SceneMgr.h"
IMPLEMENT_SINGLETON(CMapMgr)

CMapMgr::CMapMgr()
{
}


CMapMgr::~CMapMgr()
{
	TileClear();
	ObjectClear();
}

void CMapMgr::AddTile(CTile * pTile)
{
	if (pTile)
	{
		m_vecTile.push_back(pTile);
	}

}

void CMapMgr::AddBlock(CBlock * pBlock)
{
	if (pBlock)
	{
		pBlock->Initialize();
		if (pBlock->GetBlockType() == E_BLOCK_TYPE_BUSH)
		{
			m_vecBush.push_back(pBlock);
		}
		m_vecBlock.push_back(pBlock);
		sort(m_vecBlock.begin(), m_vecBlock.end(), BlockSort);
		//부쉬는 앞으로 떙기자 부쉬 먼저 체크 

	}

}

void CMapMgr::Render(void)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
		SAFE_FUNC(m_vecTile[i], Render());


	if (CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_OBJECT_EDIT)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
			SAFE_FUNC(m_vecBlock[i], Render());
	}



}

CTile* CMapMgr::GetTile(int iIndex)
{
	if (iIndex < 0 || int(m_vecTile.size()) <= iIndex) return NULL;

	return m_vecTile[iIndex];
}

void CMapMgr::DeleteBlock(CBlock * pBlock)
{
	vector<CBlock*>::iterator iter = find(m_vecBlock.begin(), m_vecBlock.end(), pBlock);

	if (iter != m_vecBlock.end())
	{
		delete *iter;
		m_vecBlock.erase(iter);
	}

}

void CMapMgr::LoadMap(void)
{
	LoadTile();
	LoadObject();
}

void CMapMgr::LoadTile(void)
{
	//로드
	HANDLE		hFile = NULL;
	DWORD	dwByte = 0;
	if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)
	{
		hFile = CreateFile(L"./Data/Map1_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)
	{
		hFile = CreateFile(L"./Data/Map2_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)
	{
		hFile = CreateFile(L"./Data/Map3_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (hFile == NULL)
	{
		MessageBox(g_hWnd, L"Tile_Load 실패", NULL, 0);
		return;
	}

	TileClear();//타일 초기화

				//int iMapType = 0;
				//ReadFile(hFile, &iMapType, sizeof(int), &dwByte, NULL);//맵 타입

	for (int i = 0; i < TILE_CNTY; ++i)
	{
		for (int j = 0; j < TILE_CNTX; ++j)
		{
			int iOption = 0;
			int iX = 0;
			int iY = 0;
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &iX, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &iY, sizeof(int), &dwByte, NULL);

			CTile* pTile = new CTile(iX, iY);
			pTile->SetFrame(iOption);

			switch (CGameMgr::GetInstance()->GetMapType())
			{
			case E_MAP_TYPE_1:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_0_Tile", 40, 40, RGB(255, 2, 2)));
				break;
			case E_MAP_TYPE_2:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_1_Tile", 40, 40, RGB(255, 2, 2)));
				break;
			case E_MAP_TYPE_3:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_2_Tile", 40, 40, RGB(255, 2, 2)));
				break;
			default:
				pTile->SetImageInfo(IMAGE_INFO(L"Box_-1", 40, 40, RGB(255, 2, 2)));
				break;
			}
			AddTile(pTile);
		}
	}
	CloseHandle(hFile);
}

void CMapMgr::LoadObject(void)//서버에서 처리하자 !
{
	HANDLE		hFile = NULL;
	DWORD	dwByte = 0;
	if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)
	{
		hFile = CreateFile(L"./Data/Map1_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)
	{
		hFile = CreateFile(L"./Data/Map2_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)
	{
		hFile = CreateFile(L"./Data/Map3_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if (hFile == NULL)
	{
		MessageBox(g_hWnd, L"Block_Load 실패", NULL, 0);
		return;
	}
	ObjectClear();

	int iCnt = 0;
	ReadFile(hFile, &iCnt, sizeof(int), &dwByte, NULL);//블럭 갯수

	for (int i = 0; i < iCnt; ++i)
	{
		int iBlockType = 0;
		float fX = 0.f;//위치
		float fY = 0.f;
		TCHAR szKey[MIN_STR];
		int iWidth = 0;
		int iHeight = 0;
		UINT iRGB = 0;

		ReadFile(hFile, &iBlockType, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &fX, sizeof(float), &dwByte, NULL);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, NULL);
		ReadFile(hFile, &szKey, sizeof(TCHAR) * MIN_STR, &dwByte, NULL);
		ReadFile(hFile, &iWidth, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iHeight, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iRGB, sizeof(UINT), &dwByte, NULL);


		CBlock* pBlock = new CBlock(0, fX, fY, IMAGE_INFO(szKey, iWidth, iHeight, iRGB), (E_BLOCK_TYPE)iBlockType);
		AddBlock(pBlock);
	}

	sort(m_vecBlock.begin(), m_vecBlock.end(), BlockSort);


	CloseHandle(hFile);
}

bool CMapMgr::TileReady(void)
{
	if (m_vecTile.size() == TILE_CNTX * TILE_CNTY)return true;
	return false;
}

bool CMapMgr::BlockReady(void)
{
	if (m_vecBlock.size() == m_iBlockCnt) {
		printf("%d\n", m_vecBlock.size());
		return true; }
	return false;
}

void CMapMgr::DeleteBush(CBlock * pBlock)
{
	if (pBlock)
	{
		vector<CBlock*>	::iterator iter = find(m_vecBush.begin(), m_vecBush.end(), pBlock);
		if (iter != m_vecBush.end())
		{
			m_vecBush.erase(iter);
		}
	}
}

void CMapMgr::TileClear(void)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		delete m_vecTile[i];
	}
	m_vecTile.clear();
}

void CMapMgr::ObjectClear(void)
{
	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		delete m_vecBlock[i];
	}
	m_vecBlock.clear();

}

bool CMapMgr::BlockSort(CBlock * pDst, CBlock * pSrc)
{
	if (pSrc->GetBlockType() != E_BLOCK_TYPE_BUSH &&  pDst->GetBlockType() == E_BLOCK_TYPE_BUSH)return true;
	return false;
}
