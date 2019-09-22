#pragma once
typedef struct tagImageInfo
{
	int            iImage_Width;
	int            iImage_Height;
	int            iImageScale;

	TCHAR         strImageName[MIN_STR];
	UINT         m_iRGB;

	tagImageInfo()
	{
		iImage_Width = 0;
		iImage_Height = 0;
		iImageScale = 1;
		//strImageName = NULL;
		memset(strImageName, 0, sizeof(strImageName));
		m_iRGB = RGB(255, 255, 255);
	}

	tagImageInfo(TCHAR* _strImageName, int _iWidth, int _iHeight, UINT _iRGB, int _Scale = 1)
	{
		iImage_Width = _iWidth;
		iImage_Height = _iHeight;
		iImageScale = _Scale;
		//strImageName = _strImageName;
		lstrcpy(strImageName, _strImageName);
		m_iRGB = _iRGB;
	}

	tagImageInfo tagImageInfo::operator =(tagImageInfo& ImageInfo)
	{
		iImage_Width = ImageInfo.iImage_Width;
		iImage_Height = ImageInfo.iImage_Height;
		iImageScale = ImageInfo.iImageScale;
		m_iRGB = ImageInfo.m_iRGB;
		lstrcpy(strImageName, ImageInfo.strImageName);
		return *this;
	}

}IMAGE_INFO;


typedef struct tagFrame
{
	int            m_iCurFrame;
	int            m_iScene;
	int            m_iMaxFrame;
	int            m_iFirstFrame;
	float         m_fFrameTime;
	float         m_fMaxFrameTime;

	tagFrame()
	{
		m_iFirstFrame = 0;
		m_iCurFrame = 0;
		m_iMaxFrame = 0;
		m_iScene = 0;
		m_fFrameTime = 0;
		m_fMaxFrameTime = 0;
	}
	tagFrame(int iCurFrame, int iScene, int iMaxFrame, float fFrameTime, float fMaxFrameTime)
	{
		m_iFirstFrame = m_iCurFrame = iCurFrame;
		m_iScene = iScene;
		m_iMaxFrame = iMaxFrame;
		m_fFrameTime = fFrameTime;
		m_fMaxFrameTime = fMaxFrameTime;
	}
}FRAME;
typedef struct tagVector2
{
	float      x;
	float      y;

	tagVector2() { x = 0.f, y = 0.f; }
	tagVector2(float fX, float fY) { x = fX, y = fY; }

	tagVector2 operator * (float fNum)
	{
		tagVector2 vec;
		vec.x = x * fNum;
		vec.y = y * fNum;
		return vec;
	}
	tagVector2 operator + (float fNum)
	{
		tagVector2 vec;
		vec.x = x + fNum;
		vec.y = y + fNum;
		return vec;
	}
	tagVector2 operator + (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x + _vec.x;
		vec.y = y + _vec.y;
		return vec;
	}
	tagVector2 operator - (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x - _vec.x;
		vec.y = y - _vec.y;
		return vec;
	}
	tagVector2 operator - (float fNum)
	{
		tagVector2 vec;
		vec.x = x - fNum;
		vec.y = y - fNum;
		return vec;
	}
	tagVector2 operator * (tagVector2 _vec)
	{
		tagVector2 vec;
		vec.x = x * _vec.x;
		vec.y = y * _vec.y;
		return vec;
	}

	tagVector2 operator += (tagVector2 vec2)
	{
		x += vec2.x;
		y += vec2.y;
		return *this;
	}
	tagVector2 operator -= (tagVector2 vec2)
	{
		x -= vec2.x;
		y -= vec2.y;
		return *this;
	}

	tagVector2 operator *= (tagVector2 vec2)
	{
		x *= vec2.x;
		y *= vec2.y;
		return *this;
	}

	bool operator == (tagVector2 vec2)
	{
		if (x == vec2.x && y == vec2.y)return true;
		return false;
	}
	bool operator != (tagVector2 vec2)
	{
		if (x != vec2.x || y != vec2.y)return true;
		return false;
	}
	float Length(void)
	{
		float fLen = sqrtf(x * x + y * y);
		return fLen;
	}
	tagVector2 Nomalize(void)
	{
		x = x / Length();
		y = y / Length();
		return *this;
	}
	float Dot(tagVector2 vec2)
	{
		float fx = x * vec2.x;
		float fy = y * vec2.y;
		return fx + fy;
	}
}VECTOR2;


typedef struct tagItemInfo
{
	int         iCurBubble;//사용중인 물풍선
	int         iMaxBubble;
	int         iSpeedCnt;
	int         iPowerCnt;
	int         iBananaCnt;
	int         iDartCnt;
	int         iPinCnt;
	int         iMineCnt;
	bool      IsKick;

	tagItemInfo()
	{
		InitItem();
	}
	void InitItem(void)
	{
		iCurBubble = 0;
		iMaxBubble = 1;
		iSpeedCnt = 0;
		iPowerCnt = 1;

		iBananaCnt = 3;
		iDartCnt = 3;
		iMineCnt = 3;
		iPinCnt = 1;
		IsKick = false;
	}
	void EatItemBubble(void)
	{
		++iMaxBubble;
		iMaxBubble = min(6, iMaxBubble);
	}
	void EatItemSpeed(void)
	{
		iSpeedCnt++;
		iSpeedCnt = min(6, iSpeedCnt);
	}
	void EatItemPower(void)
	{
		++iPowerCnt;
		iPowerCnt = min(6, iPowerCnt);
	}
	void EatItemMaxPower(void)
	{
		iPowerCnt = max(6, iPowerCnt);
	}
	void EatItemMaxSpeed(void)
	{
		iSpeedCnt = max(6, iSpeedCnt);
	}
	void EatItemKick(void)
	{
		IsKick = false;
	}
}ITEM_INFO;
typedef struct tagCharacterInfo
{
	float            m_fSpeed;
	float            m_fInitSpeed;
	float             fRatio;
	ITEM_INFO         m_tItemInfo;

	void EatItem(E_ITEMTYPE eItemType)
	{
		fRatio = 10.0f;
		switch (eItemType)
		{
		case E_ITEMTYPE_BUBBLE:
			m_tItemInfo.EatItemBubble();
			break;
		case E_ITEMTYPE_POWER:
			m_tItemInfo.EatItemPower();
			break;
		case E_ITEMTYPE_SPEED:
			m_tItemInfo.EatItemSpeed();
			m_fSpeed = m_fInitSpeed + fRatio * m_tItemInfo.iSpeedCnt;
			break;
		case E_ITEMTYPE_MAXPOWER:
			m_tItemInfo.EatItemMaxPower();
			break;
		case E_ITEMTYPE_MAXSPEED:
			m_tItemInfo.EatItemMaxSpeed();
			m_fSpeed = m_fInitSpeed + fRatio * m_tItemInfo.iSpeedCnt;
			break;
		case E_ITEMTYPE_KICK:
			m_tItemInfo.EatItemKick();
			break;
		}
	}
	void InitCharacter(void)
	{
		m_fInitSpeed = 65.00f;
		m_fSpeed = m_fInitSpeed;
		m_tItemInfo.InitItem();
	}
	tagCharacterInfo()
	{
		m_fInitSpeed = 65.00f;
		m_fSpeed = m_fInitSpeed;
	}
}CHARACTER_INFO;




typedef struct tagFunc
{
	void(*PFUNC) (void);
}FUNC;



class CStringCmp
{
private:
	const TCHAR*   m_pString;

public:
	explicit CStringCmp(const TCHAR* pKey)
		:m_pString(pKey) { }

	~CStringCmp() {}

public:

	template<typename T>
	bool   operator() (T data)
	{
		return !lstrcmp(data.first, m_pString);
	}

};

typedef struct tagObjectInfo
{
	// 서버 공용=================
	int               iID;
	VECTOR2            vPos;
	//===============

	VECTOR2            m_vDir;
	RECT            rcRect;
}OBJECT_INFO;