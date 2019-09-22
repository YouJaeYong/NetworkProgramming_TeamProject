#pragma once
extern HWND				g_hWnd;
extern HWND				g_hEdit;
extern HWND				g_hEdit2;


extern HDC				g_HDC;
extern	CRITICAL_SECTION	g_cs;


const int				WINCX = 800;
const int				WINCY = 600;

const int				TILECX = 40;
const int				TILECY = 40;

const int				TILE_CNTX = 15;
const int				TILE_CNTY = 13;

const float				g_fVolume = 0.1f;
#define MIN_STR			40
#define PURE			=0

const DWORD				BLOCK_TYPE_BASIC = 0x00000001;
const DWORD				BLOCK_TYPE_PUSH = 0x00000002;
const DWORD				BLOCK_TYPE_BUSH = 0x00000004;
const DWORD				BLOCK_TYPE_STATIC = 0x00000008;//∞Ì¡§




#define  KEY_UP		0x00000001
#define  KEY_DOWN	0x00000002
#define  KEY_LEFT	0x00000004
#define  KEY_RIGHT	0x00000008