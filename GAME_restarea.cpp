//=============================================================================
//
// リラックスエリアの処理 [GAME_restarea.cpp]
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
#include "UI.h"
#include "fade.h"
#include "MapTransparent.h"
#include "timer.h"
#include "other_objects.h"
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
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameRestarea(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	
	InitCamera();
	InitPlayer();


	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	
	if (gamedata->DescriptionFirst == false)
	{
		PlaySound(SOUND_LABEL_BGM_sample003);
	}
	else
	{
		PlaySound(SOUND_LABEL_BGM_sample002);
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameRestarea(void)
{
	UninitCamera();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameRestarea(void)
{
	if (gamedata->DescriptionFirst == false)
	{
		gamedata->start = clock();
		gamedata->pausecheck = true;
	}
	UpdateTittle();
	UpdatePause();
	UpdateBattleUI();
	UpdateBuying();
	if (player[0].pos.z >= -7900.0f&&player[0].pos.z <= -7700.0f&&player[0].pos.x <= -400.0f
		&&player[0].pos.y<50.0f)
	{
		if (GetKeyboardTrigger(DIK_B) && gamedata->pausecheck == false)
		{
			
			if (gamedata->buying_cameramove == 0)
			{
				gamedata->time += (float)((gamedata->end - gamedata->start) / 1000.0);
				gamedata->buying = gamedata->buying ? false : true;
				camera->buying_pos = camera->pos;
				camera->buying_move = camera->pos - D3DXVECTOR3(-500.0f, 50.0f, -7800.0f);
				camera->buying_at_move = camera->at - D3DXVECTOR3(-600.0f, 50.0f, -7800.0f);
			}
			else if (gamedata->buying_cameramove == 1)
			{
				//gamedata->time += (float)((gamedata->end - gamedata->start) / 1000.0);
				gamedata->buying_cameramove = 2;
			}

		}
	}

	

	if (gamedata->pausecheck == false)
	{
		ShowCursor(FALSE);
	}
	else {
		ShowCursor(FALSE);
		//gamedata->start = clock();
		return;
	}


#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

#endif
	// カメラ更新
	UpdateCamera();
	if (gamedata->buying == true)
	{
		
		return;

	}

	if (GetKeyboardTrigger(DIK_G))
	{
		
		camera->shoutchange= camera->shoutchange ? false : true;

	}

	

	UpdateBullet();
	
	UpdateParticle();
	
	// プレイヤーの更新処理
	UpdatePlayer();
	
	
	// エネミーの更新処理
	
	UpdateTransparent();
	
	UpdateRope();
	
	//UpdateRopeParticle();
	
	
	
	// 影の更新処理
	UpdateShadow();
	
	UpdateTime();
	
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGameRestarea_True(void)
{
	// 影の描画処理
	

	DrawPlayer();
	DrawRope();
	DrawBullet();
	
	//DrawTransparent();
	DrawMap();
	DrawBuyingMark();
	DrawShadow();
	DrawParticle();
	
	DrawOtherobjects();
	DrawTime();
	DrawBuying();

}

void DrawGameRestarea(void)
{
	
//#ifdef _DEBUG
//	// デバッグ表示
//	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);
//
//#endif
	
	//DrawGameRestarea_True();
	
	switch (g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGameRestarea_True();
		break;
	}
}


