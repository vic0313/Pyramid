//=============================================================================
//
// メイン処理 [main.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(960)			// ウインドウの幅
#define SCREEN_HEIGHT	(540)			// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define FIELD_W			(3000.0f)			// フィールドの広さ
#define FIELD_H			(3000.0f)			// フィールドの広さ
#define FIELD_TOP		( FIELD_H/2)		// フィールドの上
#define FIELD_DOWN		(-FIELD_H/2)		// フィールドの下
#define FIELD_LEFT		(-FIELD_W/2)		// フィールドの左
#define FIELD_RIGHT		( FIELD_W/2)		// フィールドの右
#define PLAYERNUM		(3)

#define PLAYER_MAXHP_INITIAL	(100)
#define PLAYER_MAXHP_MAX	(150)
#define PLAYER_ATK_INITIAL	(10)
#define PLAYER_ATK_MAX	(50)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,							// タイトル画面
	MODE_RESTAREA,							// ゲーム説明画面
	MODE_BATTLEAREA,								// ゲーム画面
	MODE_RESULT,							// リザルト画面
	MODE_MAX
} MODE;

// 補間用のデータ構造体を定義
struct INTERPOLATION_DATA
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scl;		// 拡大縮小
	float		frame;		// 実行フレーム数 ( dt = 1.0f/frame )
};

struct GAMEDATA 
{
	float	playerMAXHP;
	float	playerHPnow;
	float	playerAtk;
	bool	pausecheck;	//true=pause状態；
	bool	gameover;
	int		nowplayernum;
	MODE	modenow;
	bool	bigjumpset;
	bool	dying;
	int		getkey;
	int		playernum;
	bool	Mapdraw;
	bool	DescriptionFirst;
	bool	Description;
	float	time;
	long 	start;
	long 	end;
	bool	drawopen;
	bool	buying;
	int		buying_cameramove;
	bool	twojump;
	int		money;
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char* GetDebugStr(void);
MODE GetMode(void);
//MODE *GetMode(void);

void SetMode(MODE mode);

GAMEDATA *GetData(void);