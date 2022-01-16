//=============================================================================
//
// 計算画面の処理 [GAME_result.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "timer.h"
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
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static PLAYER  *player = GetPlayer();
static ENEMY   *enemyboss = GetEnemyBOSS();
static CAMERA *camera = GetCamera();
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameResult(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	
	InitCamera();
	InitPlayer();
	InitResultUI();
	InitShadow();
	
	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	
	if (enemyboss[0].use == true)
	{

		PlaySound(SOUND_LABEL_BGM_sample004);
	}
	else
	{
		PlaySound(SOUND_LABEL_BGM_sample005);
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameResult(void)
{
	UninitCamera();
	UninitPause();
	UninitBattleUI();
	UninitResultUI();
	UninitBuying();
	UninitOtherobjects();
	UninitMap();
	UninitPlayer();
	UninitShadow();
	UninitEnemy();
	UninitTransparent();
	UninitParticle();
	UninitRope();
	UninitBullet();
	UninitTime();
	UninitTittle();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameResult(void)
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
		SetFade(FADE_OUT, MODE_TITLE);
	}
	
	
	// プレイヤーの更新処理
	//UpdatePlayer();
	
	// カメラ更新
	UpdateCamera();
	// エネミーの更新処理
	
	
	UpdateMap();
	
	// 影の更新処理
	//UpdateShadow();
	
	//UpdateBattleUI();
	UpdateResultUI();
	
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGameResult_True(void)
{
	// 影の描画処理
	//DrawShadow();
	DrawPlayer();
	DrawMap();
	//DrawBattleUI();
	
	//DrawPause();
	//DrawTime();
}

void DrawGameResult(void)
{
	
//#ifdef _DEBUG
//	// デバッグ表示
//	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);
//
//#endif
	
	//DrawGameResult_True();
	
	switch (g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGameResult_True();
		break;
	}
}


