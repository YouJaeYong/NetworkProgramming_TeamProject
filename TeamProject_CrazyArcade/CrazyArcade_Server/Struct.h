#pragma once

#include "Headers.h"
#include "Character_Server.h"


typedef struct User_Info
{
	SOCKET					sock;
	SOCKADDR_IN				addr;
	CLIENT_INFO				info;
	CCharacter_Server*		pCharacter;

	User_Info()
	{
		pCharacter = NULL;
	}
	void Release()
	{
		if (pCharacter)
			delete pCharacter;
		closesocket(sock);
	}
}USER_INFO;

typedef struct Func_Info
{
	void(*func)(void*);
}FUNC;
