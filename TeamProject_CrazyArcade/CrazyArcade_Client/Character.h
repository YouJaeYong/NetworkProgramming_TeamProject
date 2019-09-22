#pragma once
#include "Object.h"
#include "Protocol.h"

enum E_DIR_TYPE
{
	E_DIR_TYPE_LEFT,
	E_DIR_TYPE_RIGHT,
	E_DIR_TYPE_UP,
	E_DIR_TYPE_DOWN,
};
class CBubble;
class CCharacter :
	public CObject
{
protected:
	CCharacter(const char* szID, int iStartingIndex, IMAGE_INFO _ImageInfo);
public:
	virtual ~CCharacter();


public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	int GetState(void) { return m_eCharacterState; }
	CHARACTER_INFO* GetCharacterInfo(void) { return &m_tCharacterInfo; }

	void Stop(void);
	virtual void Move(void);
	virtual void FrameMove(void);
	virtual void SetState(E_CHARACTER_STATE _eState);
	virtual void SetDir(E_DIR_TYPE eDir);
	virtual E_MOVE_STATE SetDir(VECTOR2 vDir);


	void SetMoveState(int _State);
	const char* GetPlayerID(void) { return m_szID; }

	// 물풍선 생성
	void CreateBubble(void);
	void Kick(CBubble* pBubble);
	void UsePin(void);
	void UseBanana(void);
	void UseMine(void);
	void UseDart(void);
	void CollisionFlow(void);//풍선 맞음
	void Sliding(void);//바나나 밟으면

	void Dead(void);
	void Revive();
	void SetTeam(int _Team) { m_iTeam = _Team; }
	int GetTeam(void) { return m_iTeam; }


	bool IsControlPossible(void) { return m_IsControlPossible; }
	bool IsCollBanana(void) { return m_IsCollBanana; }


	void SetInfoMsg(CHAR_INFO_MSG& info);
	void SetInitPos(bool Istype) { m_IsSetPos = Istype; }
	int GetMoveState() { return (int)m_eMoveState; }

	void SetKey(char key) { m_bKey = key; };
	char GetKey() { return m_bKey; }

	E_CHARACTER_TYPE GetCharacterType() { return m_eCharacterType; }
private:
	void CharacterMove(void);
	void Destory(bool _IsNetSend = true);
	virtual void CalcRect(void);
	virtual void SetStartingPoint(int iStartIndex);

protected:
	E_CHARACTER_STATE				m_ePrevCharacterState;
	E_CHARACTER_STATE				m_eCharacterState;
	CHARACTER_INFO					m_tCharacterInfo;
	E_MOVE_STATE					m_eMoveState;
	E_MOVE_STATE					m_ePrevMoveState;

	char							m_szID[30];//ID

	bool							m_IsPossibleKick;//킥 딜레이용
	float							m_fDelayTime;
	float							m_fKickDelayTime;
	int								m_iTeam;

	E_CHARACTER_TYPE				m_eCharacterType;
	bool							m_IsControlPossible;//컨트롤 가능
	bool							m_IsCollBanana;


	float							m_fSoundDelayTime;
	float							m_fSoundMaxTime;

	float							m_fInfoSendTime;


	bool							m_IsSetPos;
	CHAR_INFO_MSG					m_tNextInfoMsg;
	CHAR_INFO_MSG					m_tPrevInfoMsg;

	char							m_bKey;//누르고 있는 키

};

