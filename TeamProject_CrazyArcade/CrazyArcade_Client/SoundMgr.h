#pragma once
#include "Include.h"

enum E_VOLUME_TYPE
{
	E_VOLUME_TYPE_UP , 
	E_VOLUME_TYPE_DOWN , 
	E_VOLUME_TYPE_END 
};
class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	CSoundMgr();
	virtual ~CSoundMgr();

public:
	void Initialize(void);
	void Update(void);
	void LoadSoundFile(const char* strSoundFolder = "../Sound/*.*" , const char* strDir = "../Sound/");

	void PlayBGM(TCHAR* SoundKey, bool loop, float fVolume);
	void PlayEnviroment(TCHAR* SoundKey, float fVolume , bool _IsPlayingReturn = false);
	void PlayVoice(TCHAR* SoundKey, float fVolume, bool _IsPlayingReturn = false);
	void PlayEffect(TCHAR* SoundKey, float fVolume, bool _IsPlayingReturn = false);
	void PlayPlayer(TCHAR* SoundKey, float fVolume, bool _IsPlayingReturn = false);

	//void PlaySound(TCHAR* SoundKey, float fVolume);
	void StopBGM(void);

	void  AllVloumeChange(E_VOLUME_TYPE VolumneType , float _fnterval );
private:
	void LoadFolder(const char* strFolder);
	void ErrorCheck(FMOD_RESULT _result);
	int IsFileOrDir(const char* path);

	FMOD_BOOL IsPlaying(FMOD_CHANNEL* pChannel);//현재 플레이중인지 체크
	void Release(void);
private:
	map<TCHAR*, FMOD_SOUND*>		m_mapSound;		// 사운드 관리.
	FMOD_SYSTEM*					m_pSystem;		// FMOD 시스템 포인터
	FMOD_RESULT						m_Result;		// 에러 관리용
	size_t							m_iVersion;

private:
	FMOD_CHANNEL*					m_pSceneBGM;//씬 BGM
	FMOD_CHANNEL*					m_pEnvironment;
	FMOD_CHANNEL*					m_pVoice;
	FMOD_CHANNEL*					m_pEffect;//물줄기
	FMOD_CHANNEL*					m_pGameSound;

	FMOD_CHANNEL*					m_pPlaySound;
	vector<FMOD_CHANNEL**>			m_vecFMOD;
private:
	//조작
	bool							m_IsSound;
	float							m_fWorldVolume;
};

