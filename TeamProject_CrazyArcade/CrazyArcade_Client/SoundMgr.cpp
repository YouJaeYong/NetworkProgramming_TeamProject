#include "stdafx.h"
#include "SoundMgr.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	m_pSystem = NULL;
	m_iVersion = 0;
	m_fWorldVolume = 1.f;
	m_IsSound = true;

	m_vecFMOD.push_back(&m_pSceneBGM);
	m_vecFMOD.push_back(&m_pEnvironment);
	m_vecFMOD.push_back(&m_pVoice);
	m_vecFMOD.push_back(&m_pEffect);
	m_vecFMOD.push_back(&m_pPlaySound);
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize(void)
{
	m_Result = FMOD_System_Create(&m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_GetVersion(m_pSystem, (unsigned int*)&m_iVersion);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Init(m_pSystem, 64, FMOD_INIT_NORMAL, NULL);
	ErrorCheck(m_Result);

	LoadSoundFile();

}
void CSoundMgr::Update(void)
{
	//볼륨 관련
	if (GetOnceKeyDown(VK_F2)) 
	{
		AllVloumeChange(E_VOLUME_TYPE_UP, 0.01f);
	}
	if (GetOnceKeyDown(VK_F3)) 
	{
		AllVloumeChange(E_VOLUME_TYPE_DOWN, 0.01f);
	}

}
void CSoundMgr::LoadSoundFile(const char* strSoundFolder /*= "../Sound/*.*"*/, const char* strDir  /* ="../Sound/"*/)
{
	_finddata_t fd;

	long handle;
	int iResult = 1;

	handle = _findfirst(strSoundFolder, &fd);		//해당경로 모두 읽어라.
	if (handle == -1)
	{
		return;
	}

	while (iResult != -1)
	{
		TCHAR szName[256];

		ZeroMemory(szName, sizeof(szName));	//메크로 함수.

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
			fd.name, strlen(fd.name), szName, 256);
		//멀티바이트를 유니코드로 변경.

		TCHAR* pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);

		char szFullName[256];

		strcpy_s(szFullName, strDir);
		strcat_s(szFullName, fd.name);

		int iDirResult = IsFileOrDir(szFullName);
		if (iDirResult == 1)
		{
			FMOD_SOUND* pSound;

			m_Result = FMOD_System_CreateSound(m_pSystem, szFullName, FMOD_HARDWARE, 0, &pSound);

			if (m_Result == FMOD_OK)
			{
				m_mapSound.insert(make_pair(pName, pSound));
			}
			else
			{
				delete[] pName;
			}
		}
		else if (iDirResult == 0)
		{
			if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0)
			{
				char szFolderName[256];
				char szNextFullName[256];
				strcpy_s(szFolderName, szFullName);
				strcpy_s(szNextFullName, szFullName);
				strcat_s(szNextFullName, "/*.*");
				strcat_s(szFolderName, "/");
				//폴더면
				LoadSoundFile(szNextFullName, szFolderName);
			}
		}
		iResult = _findnext(handle, &fd);		//모든탐색을 한다.

	}

	_findclose(handle);
	m_Result = FMOD_System_Update(m_pSystem);
	ErrorCheck(m_Result);

}
void CSoundMgr::PlayBGM(TCHAR * SoundKey, bool loop, float fVolume )
{
	if ( IsPlaying(m_pSceneBGM) )
		m_Result = FMOD_Channel_Stop(m_pSceneBGM);
	ErrorCheck(m_Result);

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));

	if (iter == m_mapSound.end())
		return;

	if (loop = true)
		FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);
	else
		FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);


	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE,
		iter->second, 0, &m_pSceneBGM);
	FMOD_Channel_SetVolume(m_pSceneBGM ,  fVolume);

}
void CSoundMgr::PlayEnviroment(TCHAR * SoundKey, float fVolume  , bool _IsPlayingReturn /*= false*/)
{
	if (IsPlaying(m_pEnvironment))
	{
		if (_IsPlayingReturn)return;

		m_Result = FMOD_Channel_Stop(m_pEnvironment);
	}
	ErrorCheck(m_Result);


	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE,
		iter->second, 0, &m_pEnvironment);

	FMOD_Channel_SetVolume(m_pEnvironment, fVolume);
}
void CSoundMgr::PlayVoice(TCHAR * SoundKey, float fVolume, bool _IsPlayingReturn)
{
	if (IsPlaying(m_pVoice))
	{
		if (_IsPlayingReturn)return;

		m_Result = FMOD_Channel_Stop(m_pVoice);
	}
	ErrorCheck(m_Result);


	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE,
		iter->second, 0, &m_pVoice);

	FMOD_Channel_SetVolume(m_pVoice, fVolume);

}
void CSoundMgr::PlayEffect(TCHAR * SoundKey, float fVolume, bool _IsPlayingReturn)
{
	//m_pEffect

	if (IsPlaying(m_pEffect))
	{
		if (_IsPlayingReturn)return;

		m_Result = FMOD_Channel_Stop(m_pEffect);
	}
	ErrorCheck(m_Result);


	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE,
		iter->second, 0, &m_pEffect);

	FMOD_Channel_SetVolume(m_pEffect, fVolume);


}
void CSoundMgr::PlayPlayer(TCHAR * SoundKey, float fVolume, bool _IsPlayingReturn)
{
	if (IsPlaying(m_pPlaySound))
	{
		if (_IsPlayingReturn)return;

		m_Result = FMOD_Channel_Stop(m_pPlaySound);
	}
	ErrorCheck(m_Result);


	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));

	if (iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE,
		iter->second, 0, &m_pPlaySound);

	FMOD_Channel_SetVolume(m_pPlaySound, fVolume);

}
//void CSoundMgr::PlaySound(TCHAR * SoundKey, float fVolume)
//{
//	FMOD_CHANNEL* fmod;
//
//	map<TCHAR*, FMOD_SOUND*>::iterator iter;
//
//	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(SoundKey));
//
//	if (iter == m_mapSound.end())
//		return;
//
//	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_OFF);
//
//	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE,
//		iter->second, 0, &fmod);
//
//	FMOD_Channel_SetVolume(fmod, fVolume);
//
//
//}
void CSoundMgr::StopBGM(void)
{
	m_Result = FMOD_Channel_Stop(m_pSceneBGM);
	ErrorCheck(m_Result);
}

void CSoundMgr::AllVloumeChange(E_VOLUME_TYPE VolumneType, float _fnterval)
{

	for (size_t i = 0; i < m_vecFMOD.size(); ++i)
	{
		if (IsPlaying(*m_vecFMOD[i]))
		{
			float fVolume = 0.f;
			FMOD_Channel_GetVolume(*m_vecFMOD[i], &fVolume);

			switch (VolumneType)
			{
			case E_VOLUME_TYPE_UP:
				fVolume += _fnterval;
				break;
			case E_VOLUME_TYPE_DOWN:
				fVolume -= _fnterval;
				break;
			}

			fVolume = min(1, fVolume);
			fVolume = max(0, fVolume);
			FMOD_Channel_SetVolume(*m_vecFMOD[i], fVolume);
		}
	}
	

}

void CSoundMgr::LoadFolder(const char * strFolder)
{
}
void CSoundMgr::ErrorCheck(FMOD_RESULT _result)
{
	if (_result != FMOD_OK)
	{
		cout << "에러 " << endl;
		return;
	}
}

int CSoundMgr::IsFileOrDir(const char * path)
{
	_finddatai64_t c_file;
	intptr_t hFile;
	int result;

	if ((hFile = _findfirsti64(path, &c_file)) == -1L)
		result = -1; // 파일 또는 디렉토리가 없으면 -1 반환
	else
		if (c_file.attrib & _A_SUBDIR)
			result = 0; // 디렉토리면 0 반환
		else
			result = 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

	_findclose(hFile);

	return result;
}

FMOD_BOOL CSoundMgr::IsPlaying(FMOD_CHANNEL* pChannel)
{
	FMOD_BOOL IsPlaying = FALSE;
	if (pChannel == NULL) return IsPlaying;
	FMOD_Channel_IsPlaying(pChannel, &IsPlaying);
	return IsPlaying;
}

void CSoundMgr::Release(void)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_mapSound.end();

	for (iter; iter != iter_end; ++iter)
	{
		m_Result = FMOD_Sound_Release(iter->second);

		ErrorCheck(m_Result);

		delete[](iter->first);

		iter = m_mapSound.erase(iter);
		iter_end = m_mapSound.end();

		if (iter == iter_end)
			break;
	}

	m_Result = FMOD_System_Close(m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Release(m_pSystem);
	ErrorCheck(m_Result);
}
