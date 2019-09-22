#pragma once
#include "Include.h"


class CObject
{
protected:
	CObject(int _iID, IMAGE_INFO _tImageInfo);
public:
	virtual ~CObject();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

private:
	void Release(void);
public:
	OBJECT_INFO* GetInfo(void) { return &m_tInfo; }
	void SetVisible(bool _IsVIsible) { m_IsVisible = _IsVIsible; }
	bool IsVisible(void) { return m_IsVisible; }

	IMAGE_INFO GetImageInfo(void) { return m_tImageInfo; }

	void SetDestroy(void) { m_IsDestory = true; }
	bool IsDestory(void) { return m_IsDestory; }

	int GetID(void) { return m_tInfo.iID; }

	E_RENDER_TYPE GetRenderType(void) { return m_eRenderType; }
	E_OBJ_LIST_TYPE GetObjListType(void) { return m_eObjectListType; }

	VECTOR2 GetRectCenter(void) { return  m_vRectCenter; }
	virtual void CalcRect(void);
	bool CalcTileIndex(VECTOR2 vPos, int* pX, int* pY);
	bool CalcTileCenter(VECTOR2 vPos, VECTOR2* vCenter);
public:
	void SetPlayAni(bool _IsAni) { m_IsPlayAni = _IsAni; }
	VECTOR2 GetOffset(void) { return m_vOffset; }
	FRAME	GetFrame(void) { return m_tFrame; }
	void SetFrameScene(int _iScene) { m_tFrame.m_iScene = _iScene; }
protected:
	HDC					m_hDC;
	OBJECT_INFO			m_tInfo;
	IMAGE_INFO			m_tImageInfo;
	FRAME				m_tFrame;
	bool				m_IsDestory;//°´Ã¼ ¼Ò¸ê
	bool				m_IsVisible;

	E_RENDER_TYPE		m_eRenderType;
	E_OBJ_LIST_TYPE		m_eObjectListType;

	RECT				m_rcRectSize;
	VECTOR2				m_vRectCenter;
	bool				m_IsPlayAni;
	VECTOR2				m_vOffset;


};


