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

	FMOD_BOOL IsPlaying(FMOD_CHANNEL* pChannel);//���� �÷��������� üũ
	void Release(void);
private:
	map<TCHAR*, FMOD_SOUND*>		m_mapSound;		// ���� ����.
	FMOD_SYSTEM*					m_pSystem;		// FMOD �ý��� ������
	FMOD_RESULT						m_Result;		// ���� ������
	size_t							m_iVersion;

private:
	FMOD_CHANNEL*					m_pSceneBGM;//�� BGM
	FMOD_CHANNEL*					m_pEnvironment;
	FMOD_CHANNEL*					m_pVoice;
	FMOD_CHANNEL*					m_pEffect;//���ٱ�
	FMOD_CHANNEL*					m_pGameSound;

	FMOD_CHANNEL*					m_pPlaySound;
	vector<FMOD_CHANNEL**>			m_vecFMOD;
private:
	//����
	bool							m_IsSound;
	float							m_fWorldVolume;
};

