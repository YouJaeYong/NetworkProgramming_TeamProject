#pragma once

#include "Include.h"

class CMyBitmap;

class CBitmapMgr
{
	DECLARE_SINGLETON(CBitmapMgr)
private:
	CBitmapMgr();
	~CBitmapMgr();
public:
	void LoadImageByScene(E_SCENE_TYPE _eSceneType);
	void LoadBmp(const TCHAR* pKey, const TCHAR* pFileName);
	CMyBitmap* GetBitmap(const TCHAR* pKey);
	HDC			GetMemDC(const TCHAR* pKey);

private:
	void ReleaseImage(void);
private:
	map<const TCHAR*, CMyBitmap*>		m_MapBit;
};

