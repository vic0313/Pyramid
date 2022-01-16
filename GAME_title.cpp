//=============================================================================
//
// タイトル画面の処理 [GAME_tittle.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "input.h"

#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "shadow.h"
#include "rope.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "particle.h"
#include "bullet.h"
#include "fade.h"
#include "UI.h"
#include "other_objects.h"
#include "MapTransparent.h"
#include "timer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static PLAYER  *player = GetPlayer();
static ENEMY   *enemytypeA = GetEnemyTypeA();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
static MESH_MAP *wall = Getwall();
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameTitle(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	
	InitCamera();
	InitShadow();
	InitPlayer();
	InitTittle();
	InitEnemy();

	InitPause();
	InitBattleUI();
	InitBuying();

	InitRope();
	InitBullet();
	InitParticle();
	InitOtherobjects();
	InitTransparent();
	InitTime();
	{
		gamedata->pausecheck = false;
		gamedata->nowplayernum = 0;
		gamedata->playerMAXHP = PLAYER_MAXHP_INITIAL;
		gamedata->playerHPnow = PLAYER_MAXHP_INITIAL;
		gamedata->playerAtk = PLAYER_ATK_INITIAL;
		gamedata->gameover = false;
		gamedata->bigjumpset = false;
		gamedata->dying = false;
		gamedata->getkey =0;
		gamedata->playernum=PLAYERNUM;
		gamedata->Mapdraw = false;
		gamedata->DescriptionFirst = false;
		gamedata->Description = false;
		gamedata->time = 0.0f;
		gamedata->drawopen = false;
		gamedata->buying = false;
		gamedata->buying_cameramove = 0;
		gamedata->twojump=false;
		gamedata->money = 0;
	}
	wall[152].use = true;


	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	

	PlaySound(SOUND_LABEL_BGM_sample000);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameTitle(void)
{
	UninitCamera();

	
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameTitle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

#endif
	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える
		SetFade(FADE_OUT, MODE_RESTAREA);
	}
	// ゲームパッドで入力処理
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_RESTAREA);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_RESTAREA);
	}
	
	
	
	// カメラ更新
	UpdateCamera();

	UpdateMap();
	UpdateTittle();
	
	
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGameTitle_True(void)
{
	
	DrawMap();
	DrawTittle();
}

void DrawGameTitle(void)
{
	
//#ifdef _DEBUG
//	// デバッグ表示
//	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);
//
//#endif
	
	//DrawGameTitle_True();
	
	switch (g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGameTitle_True();
		break;
	}
}

