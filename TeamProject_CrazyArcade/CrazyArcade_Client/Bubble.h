#pragma once
#include "Object.h"

class CCharacter;

class CBubble :
	public CObject
{
private:
	CBubble(int iID, VECTOR2 vPos, IMAGE_INFO _tImage, CCharacter* pOwner, int iPower, float fLifeTime);
public:
	virtual ~CBubble();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	static CBubble* Create(int iID, VECTOR2 vPos, CCharacter* pOwner, int iPower, float fLifeTime = 3.f);

	virtual void CalcRect(void);
	vector<CObject*>		GetVecNotReady(void) { return m_vecObject; }

	void SetKick(bool _IsKick, VECTOR2 _vDir);
	bool IsKick(void) { return m_IsKick; }

	void CreateBubbleFlow();

	bool IsCreateFlow(void) { return m_IsCreateFlow; }
	float GetKickSpeed(void) { return m_fKickSpeed; }

	void BoomBubble();//¹°Ç³¼± Æø¹ß
private:
	void KickMove(void);
	void Release(void);
private:
	CCharacter*				m_pOwner;

	float					m_fCurTime;
	float					m_fLifeTime;

	bool					m_IsKick;
	float					m_fKickSpeed;

	int						m_iPower;
	bool					m_IsCreateFlow;
	vector<CObject*>		m_vecObject;

};

