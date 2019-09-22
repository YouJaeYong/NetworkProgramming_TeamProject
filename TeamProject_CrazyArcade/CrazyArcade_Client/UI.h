#pragma once
#include "Include.h"
enum E_UI_LEVEL
{
	E_UI_LEVEL_1,
	E_UI_LEVEL_2,
	E_UI_LEVEL_3,
	E_UI_LEVE_END,
};

class CUI
{
public:
	CUI(int iX, int iY, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible = true);
	virtual ~CUI();

public:
	void SetActionFrame(FRAME _tFrame) { m_IsActionFrame = true; m_tFrame = _tFrame; }
	void SetFrame(FRAME _tFrame) { m_tFrame = _tFrame; }
	virtual void Update(void);
	virtual void Render(void);

	void StopFrame(void) { m_IsActionFrame = false; }
	E_UI_LEVEL GetUILevel(void) { return m_eUILevel; }

	void ChangeImage(IMAGE_INFO tImageInfo) { m_tImageInfo = tImageInfo; }
	void ChangeImage(const TCHAR* ImageName);
	void SetPos(int iX, int iY) { m_iX = iX, m_iY = iY; }
	void SetKey(const char* pKey) { strcpy_s(m_strKey, sizeof(m_strKey), pKey); }
	const char* GetKey(void) { return m_strKey; }


	void SetVisible(bool _IsVIsible) { m_IsVisible = _IsVIsible; }
	bool IsVisible(void) { return m_IsVisible; }
	bool IsDestroy(void) { return m_IsDestroy; }

	void SetCallBack(FUNC pFunc) { m_pCallBack.PFUNC = pFunc.PFUNC; }
protected:
	IMAGE_INFO			m_tImageInfo;
	FRAME				m_tFrame;

	bool				m_IsActionFrame;
	int					m_iX;
	int					m_iY;

	E_UI_LEVEL			m_eUILevel;

	char				m_strKey[30];

	bool				m_IsVisible;
	bool				m_IsDestroy;


	FUNC				m_pCallBack;//ÇÔ¼ö

};

