// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
//TransParentBlt �� ����ϱ� ���ؼ� ���̺귯���� �߰��� �־�� ��..
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <queue>	
#include<list>
using namespace std;
#pragma comment (lib, "fmodex_vc.lib")
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"



//#ifdef _DEBUG
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#endif