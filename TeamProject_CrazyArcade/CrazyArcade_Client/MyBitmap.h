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
	HDC				m_DC;//���� DC
	HDC				m_MemDC; //���� ���۸��� ���� DC
	HBITMAP			m_Bitmap;
	HBITMAP			m_OldBitmap;
};

