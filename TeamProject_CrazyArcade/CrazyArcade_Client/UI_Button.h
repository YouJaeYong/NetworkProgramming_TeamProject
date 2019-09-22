#pragma once
#include "UI.h"

enum E_BUTTON_TYPE
{
	E_BUTTON_TYPE_LOGIN,
	E_BUTTON_TYPE_EXIT,
	E_BUTTON_TYPE_GAME_START,
	E_BUTTON_TYPE_MAP_1,
	E_BUTTON_TYPE_MAP_2,
	E_BUTTON_TYPE_MAP_3,
	E_BUTTON_TYPE_END
};
class CUI_Button :
	public CUI
{
public:
	CUI_Button(int iX, int iY, IMAGE_INFO _tImageInfo, E_BUTTON_TYPE eButtonType, E_UI_LEVEL _UILevel, bool IsVisible = true);
	virtual ~CUI_Button();
public:
	void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);


private:
	E_BUTTON_TYPE					m_eButtonType;
	RECT							m_rcRect;



	HDC								m_hDC;
};

