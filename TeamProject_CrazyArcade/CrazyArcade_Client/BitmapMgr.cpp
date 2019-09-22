#include "stdafx.h"
#include "BitmapMgr.h"
#include "MyBitmap.h"

IMPLEMENT_SINGLETON(CBitmapMgr)

CBitmapMgr::CBitmapMgr()
{
}


CBitmapMgr::~CBitmapMgr()
{
	ReleaseImage();
}

void CBitmapMgr::LoadImageByScene(E_SCENE_TYPE _eSceneType)
{
	//ÀÌ¹ÌÁö ¼Ò¸ê
	ReleaseImage();
	switch (_eSceneType)
	{
	case E_SCENE_TYPE_LOGIN:
		LoadBmp(L"BackBuffer", L"./Resource/00.Logo/Logo_Bg.bmp");
		LoadBmp(L"Logo", L"./Resource/00.Logo/Logo_Bg.bmp");
		LoadBmp(L"Game_Start", L"./Resource/00.Logo/Logo_Button_Start.bmp");

		LoadBmp(L"Exit_Button", L"./Resource/00.Logo/exit3.bmp");
		LoadBmp(L"Start_Button", L"./Resource/00.Logo/start3.bmp");
		LoadBmp(L"Edit_Button", L"./Resource/00.Logo/edit3.bmp");
		break;
	case E_SCENE_TYPE_LOBBY:
		LoadBmp(L"BackBuffer", L"./Resource/00.Logo/Logo_Bg.bmp");
		LoadBmp(L"Lobby", L"./Resource/01.Lobby/Lobby.bmp");
		LoadBmp(L"Lobby_Image_Empty", L"./Resource/01.Lobby/Lobby_Image_Empty.bmp");
		LoadBmp(L"Lobby_Bazzi", L"./Resource/01.Lobby/Lobby_Bazzi.bmp");
		LoadBmp(L"Lobby_Dao", L"./Resource/01.Lobby/Lobby_Dao.bmp");
		LoadBmp(L"Lobby_Item", L"./Resource/01.Lobby/Lobby_Item.bmp");


		LoadBmp(L"Lobby_Sel_0", L"./Resource/01.Lobby/Loddy_Image_SelectMap_0.bmp");
		LoadBmp(L"Lobby_Sel_1", L"./Resource/01.Lobby/Loddy_Image_SelectMap_1.bmp");
		LoadBmp(L"Lobby_Sel_2", L"./Resource/01.Lobby/Loddy_Image_SelectMap_2.bmp");
		LoadBmp(L"Lobby_Sel_3", L"./Resource/01.Lobby/Loddy_Image_SelectMap_3.bmp");


		LoadBmp(L"Lobby_Map_0", L"./Resource/01.Lobby/Lobby_Image_Map_0.bmp");
		LoadBmp(L"Lobby_Map_1", L"./Resource/01.Lobby/Lobby_Image_Map_1.bmp");
		LoadBmp(L"Lobby_Map_2", L"./Resource/01.Lobby/Lobby_Image_Map_2.bmp");
		LoadBmp(L"Lobby_Map_3", L"./Resource/01.Lobby/Lobby_Image_Map_3.bmp");


		LoadBmp(L"Lobby_Button_Start", L"./Resource/01.Lobby/Lobby_Button_Start.bmp");


		break;
	case E_SCENE_TYPE_TEST:
	case E_SCENE_TYPE_INGAME:
		LoadBmp(L"BackBuffer", L"./Resource/00.Logo/Logo_Bg.bmp");
		LoadBmp(L"InGame_Bg", L"./Resource/03.Ingame/InGame_Bg.bmp");
		LoadBmp(L"InGame_Image_Empty", L"./Resource/03.Ingame/InGame_Image_Empty.bmp");
		LoadBmp(L"InGame_Image_Num", L"./Resource/03.Ingame/InGame_Image_Num.bmp");
		LoadBmp(L"InGame_Image_Word", L"./Resource/03.Ingame/InGame_Image_Word.bmp");
		LoadBmp(L"InGame_Bazzi", L"./Resource/03.Ingame/InGame_Bazzi.bmp");
		LoadBmp(L"InGame_Dao", L"./Resource/03.Ingame/InGame_Dao.bmp");


		LoadBmp(L"Character_Dao", L"./Resource/03.Ingame/Character/Character_Bot.bmp");
		LoadBmp(L"Character_Bazzi", L"./Resource/03.Ingame/Character/Character_Player.bmp");
		LoadBmp(L"Bubble", L"./Resource/03.Ingame/Bubble/Bubble.bmp");
		LoadBmp(L"BubbleFlow", L"./Resource/03.Ingame/Bubble/BubbleFlow.bmp");
		LoadBmp(L"Item", L"./Resource/03.Ingame/Item/Item.bmp");
		LoadBmp(L"Dart", L"./Resource/03.Ingame/Item/Dart.bmp");



		//Map 0
		LoadBmp(L"Map_0_Box_-1", L"./Resource/03.Ingame/Map_0/Box_-1.bmp");
		LoadBmp(L"Map_0_Box_0", L"./Resource/03.Ingame/Map_0/Box_0.bmp");
		LoadBmp(L"Map_0_Box_1", L"./Resource/03.Ingame/Map_0/Box_1.bmp");
		LoadBmp(L"Map_0_Box_2", L"./Resource/03.Ingame/Map_0/Box_2.bmp");
		LoadBmp(L"Map_0_Box_3", L"./Resource/03.Ingame/Map_0/Box_3.bmp");
		LoadBmp(L"Map_0_Box_4", L"./Resource/03.Ingame/Map_0/Box_4.bmp");
		LoadBmp(L"Map_0_Box_5", L"./Resource/03.Ingame/Map_0/Box_5.bmp");
		LoadBmp(L"Map_0_Box_6", L"./Resource/03.Ingame/Map_0/Box_6.bmp");
		LoadBmp(L"Map_0_Box_7", L"./Resource/03.Ingame/Map_0/Box_7.bmp");

		LoadBmp(L"Map_0_Tile", L"./Resource/03.Ingame/Map_0/Tile.bmp");
		LoadBmp(L"Map_1_Tile", L"./Resource/03.Ingame/Map_1/Tile.bmp");
		LoadBmp(L"Map_2_Tile", L"./Resource/03.Ingame/Map_2/Tile.bmp");

		LoadBmp(L"Alpha", L"./Resource/03.Ingame/Alpha.bmp");


		LoadBmp(L"Map_1_Box_0", L"./Resource/03.Ingame/Map_1/Box_0.bmp");
		LoadBmp(L"Map_1_Box_1", L"./Resource/03.Ingame/Map_1/Box_1.bmp");
		LoadBmp(L"Map_1_Box_2", L"./Resource/03.Ingame/Map_1/Box_2.bmp");
		LoadBmp(L"Map_1_Box_3", L"./Resource/03.Ingame/Map_1/Box_3.bmp");
		LoadBmp(L"Map_1_Box_4", L"./Resource/03.Ingame/Map_1/Box_4.bmp");
		LoadBmp(L"Map_1_Box_5", L"./Resource/03.Ingame/Map_1/Box_5.bmp");
		LoadBmp(L"Map_1_Box_6", L"./Resource/03.Ingame/Map_1/Box_6.bmp");

		LoadBmp(L"Map_2_Box_0", L"./Resource/03.Ingame/Map_2/Box_0.bmp");
		LoadBmp(L"Map_2_Box_1", L"./Resource/03.Ingame/Map_2/Box_1.bmp");
		LoadBmp(L"Map_2_Box_2", L"./Resource/03.Ingame/Map_2/Box_2.bmp");
		LoadBmp(L"Map_2_Box_3", L"./Resource/03.Ingame/Map_2/Box_3.bmp");
		LoadBmp(L"Map_2_Box_4", L"./Resource/03.Ingame/Map_2/Box_4.bmp");
		LoadBmp(L"Map_2_Box_5", L"./Resource/03.Ingame/Map_2/Box_5.bmp");
		LoadBmp(L"Map_2_Box_6", L"./Resource/03.Ingame/Map_2/Box_6.bmp");
		LoadBmp(L"Map_2_Box_7", L"./Resource/03.Ingame/Map_2/Box_7.bmp");
		LoadBmp(L"Map_2_Box_8", L"./Resource/03.Ingame/Map_2/Box_8.bmp");
		LoadBmp(L"Map_2_Box_9", L"./Resource/03.Ingame/Map_2/Box_9.bmp");
		LoadBmp(L"Map_2_Box_10", L"./Resource/03.Ingame/Map_2/Box_10.bmp");
		LoadBmp(L"Map_2_Box_11", L"./Resource/03.Ingame/Map_2/Box_11.bmp");

		break;

	case E_SCENE_TYPE_OBJECT_EDIT:
	case E_SCENE_TYPE_TILE_EDIT:
		LoadBmp(L"BackBuffer", L"../Resource/00.Logo/Logo_Bg.bmp");
		LoadBmp(L"InGame_Bg", L"../Resource/03.Ingame/InGame_Bg.bmp");

		LoadBmp(L"Map_0_Tile", L"../Resource/03.Ingame/Map_0/Tile.bmp");
		LoadBmp(L"Map_1_Tile", L"../Resource/03.Ingame/Map_1/Tile.bmp");
		LoadBmp(L"Map_2_Tile", L"../Resource/03.Ingame/Map_2/Tile.bmp");

		//Map
		LoadBmp(L"Box_-1", L"../Resource/03.Ingame/Map_0/Box_-1.bmp");
		LoadBmp(L"Map_0_Box_0", L"../Resource/03.Ingame/Map_0/Box_0.bmp");
		LoadBmp(L"Map_0_Box_1", L"../Resource/03.Ingame/Map_0/Box_1.bmp");
		LoadBmp(L"Map_0_Box_2", L"../Resource/03.Ingame/Map_0/Box_2.bmp");
		LoadBmp(L"Map_0_Box_3", L"../Resource/03.Ingame/Map_0/Box_3.bmp");
		LoadBmp(L"Map_0_Box_4", L"../Resource/03.Ingame/Map_0/Box_4.bmp");
		LoadBmp(L"Map_0_Box_5", L"../Resource/03.Ingame/Map_0/Box_5.bmp");
		LoadBmp(L"Map_0_Box_6", L"../Resource/03.Ingame/Map_0/Box_6.bmp");
		LoadBmp(L"Map_0_Box_7", L"../Resource/03.Ingame/Map_0/Box_7.bmp");




		LoadBmp(L"Map_1_Box_0", L"../Resource/03.Ingame/Map_1/Box_0.bmp");
		LoadBmp(L"Map_1_Box_1", L"../Resource/03.Ingame/Map_1/Box_1.bmp");
		LoadBmp(L"Map_1_Box_2", L"../Resource/03.Ingame/Map_1/Box_2.bmp");
		LoadBmp(L"Map_1_Box_3", L"../Resource/03.Ingame/Map_1/Box_3.bmp");
		LoadBmp(L"Map_1_Box_4", L"../Resource/03.Ingame/Map_1/Box_4.bmp");
		LoadBmp(L"Map_1_Box_5", L"../Resource/03.Ingame/Map_1/Box_5.bmp");
		LoadBmp(L"Map_1_Box_6", L"../Resource/03.Ingame/Map_1/Box_6.bmp");

		LoadBmp(L"Map_2_Box_0", L"../Resource/03.Ingame/Map_2/Box_0.bmp");
		LoadBmp(L"Map_2_Box_1", L"../Resource/03.Ingame/Map_2/Box_1.bmp");
		LoadBmp(L"Map_2_Box_2", L"../Resource/03.Ingame/Map_2/Box_2.bmp");
		LoadBmp(L"Map_2_Box_3", L"../Resource/03.Ingame/Map_2/Box_3.bmp");
		LoadBmp(L"Map_2_Box_4", L"../Resource/03.Ingame/Map_2/Box_4.bmp");
		LoadBmp(L"Map_2_Box_5", L"../Resource/03.Ingame/Map_2/Box_5.bmp");

		LoadBmp(L"Map_2_Box_6", L"../Resource/03.Ingame/Map_2/Box_6.bmp");
		LoadBmp(L"Map_2_Box_7", L"../Resource/03.Ingame/Map_2/Box_7.bmp");
		LoadBmp(L"Map_2_Box_8", L"../Resource/03.Ingame/Map_2/Box_8.bmp");
		LoadBmp(L"Map_2_Box_9", L"../Resource/03.Ingame/Map_2/Box_9.bmp");
		LoadBmp(L"Map_2_Box_10", L"../Resource/03.Ingame/Map_2/Box_10.bmp");
		LoadBmp(L"Map_2_Box_11", L"../Resource/03.Ingame/Map_2/Box_11.bmp");

		break;
	}
}

void CBitmapMgr::LoadBmp(const TCHAR * pKey, const TCHAR * pFileName)
{
	m_MapBit.insert(make_pair(pKey, (new CMyBitmap)->LoadBmp((TCHAR*)pFileName)));
}

CMyBitmap * CBitmapMgr::GetBitmap(const TCHAR * pKey)
{
	map<const TCHAR*, CMyBitmap*>::iterator iter = m_MapBit.find(pKey);

	if (iter == m_MapBit.end())return NULL;

	return iter->second;
}

HDC CBitmapMgr::GetMemDC(const TCHAR * pKey)
{
	map<const TCHAR*, CMyBitmap*>::iterator iter = m_MapBit.begin();
	map<const TCHAR*, CMyBitmap*>::iterator iter_end = m_MapBit.end();
	for (iter; iter != m_MapBit.end(); ++iter)
	{
		if (lstrcmp(iter->first, pKey) == 0)
			return iter->second->GetMemDC();
	}
	return NULL;

}

void CBitmapMgr::ReleaseImage(void)
{
	map<const TCHAR*, CMyBitmap*>::iterator iter = m_MapBit.begin();

	for (; iter != m_MapBit.end(); ++iter)
	{
		delete iter->second;
	}
	m_MapBit.clear();
}
