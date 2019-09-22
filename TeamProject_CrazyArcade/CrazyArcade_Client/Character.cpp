#include "stdafx.h"
#include "Character.h"
#include "GameMgr.h"
#include "TimeMgr.h"
#include "NetMgr.h"
#include "ClientMgr.h"
#include "Client.h"
#include "CollisionMgr.h"
#include "Bubble.h"
#include "SoundMgr.h"
CCharacter::CCharacter(const char* szID, int iStartingIndex, IMAGE_INFO _ImageInfo)
	: CObject(0, _ImageInfo)
{
	strcpy_s(m_szID, sizeof(m_szID), szID);

	SetStartingPoint(iStartingIndex);
	m_ePrevCharacterState = E_CHARACTER_STATE_END;
	m_eObjectListType = E_OBJ_LIST_TYPE_PLAYER;
	m_eRenderType = E_RENDER_TYPE_SORT_OBJECT;

	m_eMoveState = E_MOVE_STAND;
	m_ePrevMoveState = E_MOVE_STAND;

	m_IsControlPossible = true;
	m_IsCollBanana = false;
	Initialize();

	m_fSoundDelayTime = 1.f;
	m_fSoundMaxTime = 1.f;

	m_fInfoSendTime = 0.0f;

	m_IsSetPos = false;

	m_bKey = 0;
}

CCharacter::~CCharacter()
{
}

void CCharacter::Initialize(void)
{
	m_rcRectSize.left = long(-TILECX / 2);
	m_rcRectSize.right = long(+TILECX / 2);

	m_rcRectSize.top = long(-TILECY / 4);
	m_rcRectSize.bottom = long(+TILECY / 4);


	m_IsPossibleKick = false;
	m_fDelayTime = 0.f;
	m_fKickDelayTime = 0.05f;

	m_tCharacterInfo.InitCharacter();
}

void CCharacter::Update(void)
{
	/*if (CGameMgr::GetInstance()->GetGameState() != E_GAME_STATE_RPOGRESS)
	return;*/


	CCollisionMgr::GetInstance()->CollisionPlayerToItem(this);
	CCollisionMgr::GetInstance()->CollisionPlayerToPlayer(this);//Drowning상태에서만


	CalcRect();

	if (m_IsSetPos == true)
	{
		m_IsSetPos = false;

		RECT rc;
		rc.left = 40;
		rc.top = 30;
		rc.right = 600;
		rc.bottom = 530;

		//충돌 체크
		VECTOR2 vMovePos = m_tNextInfoMsg.vPos;
		VECTOR2 vDestPos = vMovePos;
		m_tInfo.vPos = vDestPos;
		m_tInfo.m_vDir = m_tNextInfoMsg.vDir;
		m_bKey = m_tNextInfoMsg.cKey;
		SetState((E_CHARACTER_STATE)m_tNextInfoMsg.iState);
		m_eMoveState = (E_MOVE_STATE)m_tNextInfoMsg.iMoveState;
		SetVisible(m_tNextInfoMsg.IsVisible);
	}
	Move();
	FrameMove();

	if (m_IsPossibleKick == false)
	{
		m_fDelayTime += GETTIME;

		if (m_fDelayTime > m_fKickDelayTime)
		{
			m_IsPossibleKick = true;
			m_fDelayTime = 0.f;
		}
	}



	//상태 및 위치 정보 1초에 30번 보내주자
	m_fInfoSendTime += 30 * GETTIME;
	if (m_fInfoSendTime >= 1.0f)
	{
		//보내기
		if (CClientMgr::GetInstance()->GetMe()->GetCharacter() == this)
		{
			CHAR_INFO_MSG   info;
			strcpy_s(info.szID, GetPlayerID());
			info.vPos = GetInfo()->vPos;
			info.vDir = GetInfo()->m_vDir;
			info.iState = GetState();
			info.iMoveState = m_eMoveState;
			info.cKey = m_bKey;
			info.IsVisible = m_IsVisible;
			if (m_tPrevInfoMsg != info)
			{
				m_fInfoSendTime = 0.0f;
				//키정보
				CNetMgr::GetInstance()->OnRequestCharacterInfo(info);
				m_tPrevInfoMsg = info;
			}
		}
	}
}

void CCharacter::Render(void)
{
}

void CCharacter::Stop(void)
{
	SetMoveState(E_MOVE_STAND);
}

void CCharacter::Move(void)
{
	if (m_eCharacterState == E_CHARACTER_STATE_DEATH)
		return;//죽으면 움직이지 못한다.


	if (m_eMoveState != m_ePrevMoveState)//이동 처음 시작
	{
		switch (m_eMoveState)
		{
		case E_MOVE_STAND:
		{
			if (m_eCharacterState != E_CHARACTER_STATE_DROWNING)
				m_tFrame.m_iCurFrame = m_tFrame.m_iFirstFrame;
		}
		break;
		case E_MOVE_LEFT:
			SetDir(E_DIR_TYPE_LEFT);
			break;
		case E_MOVE_RIGHT:
			SetDir(E_DIR_TYPE_RIGHT);
			break;
		case E_MOVE_UP:
			SetDir(E_DIR_TYPE_UP);
			break;
		case E_MOVE_DOWN:
			SetDir(E_DIR_TYPE_DOWN);
			break;
		}
		m_ePrevMoveState = m_eMoveState;
	}

	if (m_eMoveState != E_MOVE_STAND)//이동
	{
		if (m_eCharacterState < E_CHARACTER_STATE_DROWNING)
		{
			m_tFrame.m_fFrameTime += GETTIME;
			if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
			{
				m_tFrame.m_fFrameTime = 0;
				m_tFrame.m_iCurFrame++;
				if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
				{
					CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Character_Fixed.wav", g_fVolume);
					m_tFrame.m_iCurFrame = m_tFrame.m_iFirstFrame;
				}
			}
		}
		CharacterMove();//실제 이동
	}
}

void CCharacter::FrameMove(void)
{
	if (m_eCharacterState > E_CHARACTER_STATE_STAND_LEFT)
	{
		m_tFrame.m_fFrameTime += GETTIME;
		if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
		{
			m_tFrame.m_fFrameTime = 0;
			m_tFrame.m_iCurFrame++;
			if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
			{
				switch (m_eCharacterState)
				{
				case E_CHARACTER_STATE_DROWNING:
					Dead();
					break;
				case E_CHARACTER_STATE_REVIVE:
					SetDir(m_tInfo.m_vDir);//현재 방향으로 다시 시작
					break;
				case E_CHARACTER_STATE_DEATH: {
					Destory();
					break; }
				}
				m_tFrame.m_iCurFrame = m_tFrame.m_iFirstFrame;
			}
		}
	}
}

void CCharacter::SetState(E_CHARACTER_STATE _eState)
{
	m_eCharacterState = _eState;
	m_fSoundDelayTime = m_fSoundMaxTime;
	switch (m_eCharacterState)
	{
	case E_CHARACTER_STATE_DEATH:
		CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Character_Die.wav", g_fVolume);
		break;
	case E_CHARACTER_STATE_REVIVE:
		CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Character_Revival.wav", g_fVolume);
		break;
	}
}

void CCharacter::SetDir(E_DIR_TYPE eDir)
{
	E_CHARACTER_STATE eCharacterState = E_CHARACTER_STATE_END;

	switch (eDir)
	{
	case E_DIR_TYPE_LEFT:
		m_tInfo.m_vDir = VECTOR2(-1.f, 0.f);
		eCharacterState = E_CHARACTER_STATE_STAND_LEFT;
		break;
	case E_DIR_TYPE_RIGHT:
		m_tInfo.m_vDir = VECTOR2(+1.f, 0.f);
		eCharacterState = E_CHARACTER_STATE_STAND_RIGHT;
		break;
	case E_DIR_TYPE_UP:
		m_tInfo.m_vDir = VECTOR2(0.f, -1.f);
		eCharacterState = E_CHARACTER_STATE_STAND_UP;
		break;
	case E_DIR_TYPE_DOWN:
		m_tInfo.m_vDir = VECTOR2(0.f, 1.f);
		eCharacterState = E_CHARACTER_STATE_STAND_DOWN;
		break;
	}
	if (m_eCharacterState != E_CHARACTER_STATE_DROWNING)
	{
		SetState(eCharacterState);
	}
	m_tInfo.m_vDir.Nomalize();
}
E_MOVE_STATE CCharacter::SetDir(VECTOR2 vDir)
{
	if (m_tInfo.m_vDir == VECTOR2(-1.f, 0.f))
	{
		SetDir(E_DIR_TYPE_LEFT);
		return E_MOVE_LEFT;
	}
	else if (m_tInfo.m_vDir == VECTOR2(1.f, 0.f))
	{
		SetDir(E_DIR_TYPE_RIGHT);
		return E_MOVE_RIGHT;
	}
	else if (m_tInfo.m_vDir == VECTOR2(0.f, -1.f))
	{
		SetDir(E_DIR_TYPE_UP);
		return E_MOVE_UP;
	}
	else if (m_tInfo.m_vDir == VECTOR2(0.f, 1.f))
	{
		SetDir(E_DIR_TYPE_DOWN);
		return E_MOVE_DOWN;
	}
	return E_MOVE_STAND;
}
void CCharacter::SetMoveState(int _State)
{
	m_eMoveState = (E_MOVE_STATE)_State;

}
void CCharacter::CreateBubble(void)
{
	if (m_tCharacterInfo.m_tItemInfo.iCurBubble >= m_tCharacterInfo.m_tItemInfo.iMaxBubble
		|| m_eCharacterState >= E_CHARACTER_STATE_DROWNING)
		return;
	if (CCollisionMgr::GetInstance()->CollisionPosToObject(m_tInfo.vPos, E_OBJ_LIST_TYPE_BUBBLE) == false)
	{
		//물풍선 생성
		VECTOR2 vTileCenter;
		CalcTileCenter(VECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y + m_vOffset.y), &vTileCenter);

		CREATE_OBJECT_MSG CreateMsg;
		strcpy_s(CreateMsg.szID, m_szID);
		CreateMsg.vCreatePos = vTileCenter;
		CreateMsg.vDir = m_tInfo.m_vDir;//다크 때문에
		CreateMsg.eCreateType = E_OBJECT_CREATE_TYPE_BUBBLE;

		CNetMgr::GetInstance()->OnRequestCreateObject(CreateMsg);
	}
}
void CCharacter::Kick(CBubble* pBubble)
{
	if (CClientMgr::GetInstance()->GetMe()->GetCharacter() != this)
		return;

	if (pBubble == NULL || pBubble->IsKick() == true || m_IsPossibleKick == false)return;



	if (m_tCharacterInfo.m_tItemInfo.IsKick == true)
	{
		bool m_IsMovePossible = true;

		VECTOR2 vDestPos;
		VECTOR2 vMovePos = pBubble->GetInfo()->vPos + m_tInfo.m_vDir * pBubble->GetKickSpeed();

		RECT rc;
		rc.left = 40;
		rc.top = 60;
		rc.right = 600;
		rc.bottom = 540;

		if (CCollisionMgr::GetInstance()->ScreenOutCheck(m_tInfo.vPos, vMovePos, &vDestPos, rc)
			|| CCollisionMgr::GetInstance()->CollisionObjectToBlock(pBubble, vDestPos, &vDestPos)
			|| CCollisionMgr::GetInstance()->CollisionObjectToBubble(pBubble, vDestPos, &vDestPos)
			)
			m_IsMovePossible = false;

		if (m_IsMovePossible)
		{
			CCharacter* pMe = CClientMgr::GetInstance()->GetMe()->GetCharacter();
			if (pMe == this)
			{
				m_IsPossibleKick = false;
				//pBubble->SetKick(true, m_tInfo.m_vDir);
				BUBBLE_KICK_MSG msg;
				msg.vPos = pBubble->GetInfo()->vPos;
				msg.vDir = m_tInfo.m_vDir;
				msg.iID = pBubble->GetID();
				strcpy_s(msg.szID, m_szID);
				//서버로 전달
				CNetMgr::GetInstance()->OnRequestBubbleKick(msg);
			}
		}
	}
}

void CCharacter::UsePin(void)
{
	if (m_eCharacterState != E_CHARACTER_STATE_DROWNING)return;

	if (m_tCharacterInfo.m_tItemInfo.iPinCnt - 1 >= 0)
	{
		m_tCharacterInfo.m_tItemInfo.iPinCnt--;
		Revive();
	}
}
void CCharacter::UseBanana(void)
{
	if (m_eCharacterState >= E_CHARACTER_STATE_DROWNING)return;

	if (m_tCharacterInfo.m_tItemInfo.iBananaCnt - 1 >= 0)
	{
		//서버 전달
		CREATE_OBJECT_MSG msg;

		msg.iID = 0;
		strcpy_s(msg.szID, m_szID);
		msg.vCreatePos = m_tInfo.vPos;
		msg.vDir = m_tInfo.m_vDir;
		msg.eCreateType = E_OBJECT_CREATE_TYPE_BANANA;
		CNetMgr::GetInstance()->OnRequestCreateObject(msg);
	}
}
void CCharacter::UseMine(void)
{
	if (m_eCharacterState >= E_CHARACTER_STATE_DROWNING)return;

	if (m_tCharacterInfo.m_tItemInfo.iMineCnt - 1 >= 0)
	{
		//서버 전달
		CREATE_OBJECT_MSG msg;

		msg.iID = 0;
		strcpy_s(msg.szID, m_szID);
		msg.vCreatePos = m_tInfo.vPos;
		msg.vDir = m_tInfo.m_vDir;
		msg.eCreateType = E_OBJECT_CREATE_TYPE_MINE;
		CNetMgr::GetInstance()->OnRequestCreateObject(msg);

	}
}
void CCharacter::UseDart(void)
{
	if (m_eCharacterState >= E_CHARACTER_STATE_DROWNING)return;

	if (m_tCharacterInfo.m_tItemInfo.iDartCnt - 1 >= 0)
	{
		//서버 전달
		CREATE_OBJECT_MSG msg;

		msg.iID = 0;
		strcpy_s(msg.szID, m_szID);
		msg.vCreatePos = m_tInfo.vPos;
		msg.vDir = m_tInfo.m_vDir;
		msg.eCreateType = E_OBJECT_CREATE_TYPE_DART;
		CNetMgr::GetInstance()->OnRequestCreateObject(msg);
	}
}
void CCharacter::CollisionFlow(void)
{
	switch (m_eCharacterState)
	{
	case E_CHARACTER_STATE_STAND_UP:
	case E_CHARACTER_STATE_STAND_DOWN:
	case E_CHARACTER_STATE_STAND_RIGHT:
	case E_CHARACTER_STATE_STAND_LEFT:
		SetState(E_CHARACTER_STATE_DROWNING);
		break;
	}
}
void CCharacter::Sliding(void)
{
	CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Item_Banana_Off.wav", g_fVolume);

	m_IsCollBanana = true;
	//해당 방향으로 벽에 충돌할때까지 이동 빠른 스피드로
	SetMoveState(SetDir(m_tInfo.m_vDir));
}
void CCharacter::Revive()
{
	if (m_eCharacterState != E_CHARACTER_STATE_DROWNING)
		return;
	SetState(E_CHARACTER_STATE_REVIVE);
	Stop();

}

void CCharacter::SetInfoMsg(CHAR_INFO_MSG& info)
{
	m_tNextInfoMsg = info;
}

void CCharacter::Dead(void)
{
	SetState(E_CHARACTER_STATE_DEATH);
	Stop();

}
void CCharacter::CharacterMove(void)
{
	SetVisible(true);
	RECT rc;
	rc.left = 40;
	rc.top = 30;
	rc.right = 600;
	rc.bottom = 530;


	float fSpeed = m_tCharacterInfo.m_fSpeed;
	if (m_IsCollBanana)
	{
		fSpeed = 500;
	}

	if (m_eCharacterState == E_CHARACTER_STATE_DROWNING)
	{
		fSpeed = fSpeed * 0.1f;
	}

	VECTOR2 vMovePos = m_tInfo.vPos + m_tInfo.m_vDir * fSpeed * GETTIME;
	VECTOR2 vDestPos = vMovePos;
	if (CCollisionMgr::GetInstance()->CollisionObjectToBlock(this, vMovePos, &vDestPos))
	{
		m_IsCollBanana = false;
	}
	if (CCollisionMgr::GetInstance()->ScreenOutCheck(m_tInfo.vPos, vDestPos, &vDestPos, rc))
	{
		m_IsCollBanana = false;
	}
	if (CCollisionMgr::GetInstance()->CollisionObjectToBubble(this, vDestPos, &vDestPos))
	{
		m_IsCollBanana = false;
	}

	m_tInfo.vPos = vDestPos;
}

void CCharacter::Destory(bool _IsNetSend)
{

	if (m_IsDestory)return;

	CClient* pMe = CClientMgr::GetInstance()->GetMe();
	if (pMe == NULL)return;
	if (pMe->GetCharacter()->GetState() == E_CHARACTER_STATE_DEATH) {
		if (pMe->GetClientInfo()->cTeam == E_TEAM_TYPE_BLUE)
			CNetMgr::GetInstance()->OnRequestGameOver(E_GAME_STATE_RED_WIN);
		else if (pMe->GetClientInfo()->cTeam == E_TEAM_TYPE_RED)
			CNetMgr::GetInstance()->OnRequestGameOver(E_GAME_STATE_BLUE_WIN);
	}

	m_IsDestory = true;

	/*vector<CClient*> vecClient = *CClientMgr::GetInstance()->GetClient();

	for (size_t i = 0; i < vecClient.size(); ++i)
	{
	if (this == vecClient[i]->GetCharacter())
	{
	vecClient[i]->SetCharacter('D');
	break;
	}
	}*/
}




void CCharacter::CalcRect(void)
{
	m_vOffset.y = 20;

	m_tInfo.rcRect.left = (long)(m_tInfo.vPos.x + m_rcRectSize.left);
	m_tInfo.rcRect.right = (long)(m_tInfo.vPos.x + m_rcRectSize.right);

	m_tInfo.rcRect.top = (long)(m_tInfo.vPos.y + m_rcRectSize.top + m_vOffset.y);
	m_tInfo.rcRect.bottom = (long)(m_tInfo.vPos.y + m_rcRectSize.bottom + m_vOffset.y);

	m_vRectCenter.x = float(m_tInfo.rcRect.left + m_tInfo.rcRect.right / 2);
	m_vRectCenter.y = float(m_tInfo.rcRect.top + m_tInfo.rcRect.bottom / 2);

}

void CCharacter::SetStartingPoint(int iStartIndex)
{
	if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)
	{
		switch (iStartIndex)
		{
		case 0:
			m_tInfo.vPos.x = 40;
			m_tInfo.vPos.y = 50;
			SetDir(E_DIR_TYPE_DOWN);
			break;
		case 1:
			m_tInfo.vPos.x = 600;
			m_tInfo.vPos.y = 88;
			SetDir(E_DIR_TYPE_LEFT);
			break;
		case 2:
			m_tInfo.vPos.x = 80;
			m_tInfo.vPos.y = 530;
			SetDir(E_DIR_TYPE_UP);
			break;
		case 3:
			m_tInfo.vPos.x = 600;
			m_tInfo.vPos.y = 530;
			SetDir(E_DIR_TYPE_UP);
			break;
		}
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)
	{
		switch (iStartIndex)
		{
		case 0:
			m_tInfo.vPos.x = 40;
			m_tInfo.vPos.y = 520;

			SetDir(E_DIR_TYPE_UP);

			break;
		case 1:
			m_tInfo.vPos.x = 160;
			m_tInfo.vPos.y = 250;
			SetDir(E_DIR_TYPE_DOWN);
			break;
		case 2:
			m_tInfo.vPos.x = 600;
			m_tInfo.vPos.y = 40;
			SetDir(E_DIR_TYPE_DOWN);
			break;
		case 3:
			m_tInfo.vPos.x = 480;
			m_tInfo.vPos.y = 340;
			SetDir(E_DIR_TYPE_UP);
			break;
		}
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)
	{
		switch (iStartIndex)
		{
		case 0:
			m_tInfo.vPos.x = 160;
			m_tInfo.vPos.y = 160;
			SetDir(E_DIR_TYPE_DOWN);
			break;
		case 1:
			m_tInfo.vPos.x = 160;
			m_tInfo.vPos.y = 400;
			SetDir(E_DIR_TYPE_UP);
			break;
		case 2:
			m_tInfo.vPos.x = 480;
			m_tInfo.vPos.y = 400;
			SetDir(E_DIR_TYPE_UP);
			break;
		case 3:
			m_tInfo.vPos.x = 480;
			m_tInfo.vPos.y = 160;
			SetDir(E_DIR_TYPE_DOWN);
			break;
		}
	}
}