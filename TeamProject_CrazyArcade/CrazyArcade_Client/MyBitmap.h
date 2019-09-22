#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();
public:
	inline HDC		GetMemDC(void)
	{
		return m_MemDC;

	}
public:
	CMyBitmap*		LoadBmp(TCHAR* pFileName);
private:
	HDC				m_DC;//실제 DC
	HDC				m_MemDC; //더블 버퍼링을 위한 DC
	HBITMAP			m_Bitmap;
	HBITMAP			m_OldBitmap;
};

