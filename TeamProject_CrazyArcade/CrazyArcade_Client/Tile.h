#pragma once
#include "Include.h"
class CTile
{
public:
	CTile(int iX, int iY);
	~CTile();

public:
	void SetImageInfo(IMAGE_INFO _ImageInfo) { m_tImageInfo = _ImageInfo; }
	void SetImageName(TCHAR* strName);
	void SetFrame(FRAME _tFrame) { m_tFrame = _tFrame; }
	void SetFrame(int _iFrame);
	void Render(void);

	IMAGE_INFO GetImageInfo(void) { return m_tImageInfo; }
	FRAME	  GetFrame(void) { return m_tFrame; }
	int		  GetPosX(void) { return m_iX; }
	int		  GetPosY(void) { return m_iY; }
private:
	IMAGE_INFO			m_tImageInfo;
	FRAME				m_tFrame;
	int					m_iX;
	int					m_iY;
};

