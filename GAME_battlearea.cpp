//=============================================================================
//
// バトルエリアの処理 [GAME_battlearea.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "collision.h"
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
static ENEMY   *enemytypeB = GetEnemyTypeB();
static ENEMY   *enemytypeC = GetEnemyTypeC();
static ENEMY   *enemytypeD = GetEnemyTypeD();
static ENEMY   *enemyBOSS = GetEnemyBOSS();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
static BULLET	*bullet = GetBullet();
static BULLET	*enemybullet1 = GetEnemyBullet1();
static BULLET	*enemybullet2 = GetEnemyBullet2();
static BULLET	*bossbullet = GetEnemyBulletBoss();

static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

//static bool Mapdraw=false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameBattlearea(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	
	InitCamera();
	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	

	PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameBattlearea(void)
{
	// カメラの終了処理
	UninitCamera();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameBattlearea(void)
{
	UpdatePause();
	UpdateBattleUI();
	if (gamedata->gameover == true && gamedata->dying == false)
	{
		
		if(GetKeyboardTrigger(DIK_RETURN))
		{
			SetFade(FADE_OUT, MODE_RESULT);
			gamedata->pausecheck = false;
		}
	}
	
	
	if (gamedata->pausecheck == false)
	{
		ShowCursor(FALSE);
	}
	else {
		ShowCursor(FALSE);
		return;
	}
	
	
	UpdateBullet();

	UpdateParticle();
	if (gamedata->gameover == true)
	{
		start = clock();
		return;
	}


	if (GetKeyboardTrigger(DIK_G))
	{
		camera->shoutchange= camera->shoutchange ? false : true;
	}

	if (GetKeyboardTrigger(DIK_M))
	{
		gamedata->Mapdraw = gamedata->Mapdraw ? false : true;
	}
	

	
	
	if (gamedata->dying == false)
	{
		UpdatePlayer();
	}
	
	UpdateCamera();
	UpdateEnemy();
	UpdateRope();
	UpdateOtherobjects();

	MapTransparentCheck();
	MapTransparentCheckReverse();
	UpdateMap();
	UpdateTransparent();

	UpdateShadow();

	UpdateTime();
	
	HitCheck();
	TouchCheck();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGameBattlearea_True(void)
{
	
	DrawEnemy();
	DrawPlayer();
	DrawRope();
	DrawBullet();
	DrawOtherobjects();
	DrawMap();
	DrawTime();
}

void DrawGameBattlearea(void)
{
	
//#ifdef _DEBUG
//	// デバッグ表示
//	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);
//
//#endif
	
	//DrawGameBattlearea_True();
	
	switch (g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGameBattlearea_True();
		break;
	}
}
//=============================================================================
// BULLET当たり判定処理
//=============================================================================
void HitCheck(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i].use == true) 
		{
			//typeA Hit
			for (int a = 0; a < MAX_ENEMY_TYPEA; a++)
			{
				if(enemytypeA[a].use==false)continue;
				

				bool HitAans = CollisionBB(bullet[i].pos, enemytypeA[a].HitcheckPosA, 
					bullet[i].Hitcheck_size, enemytypeA[a].HitcheckPosA_size);
				if (HitAans == true)
				{
					if (enemytypeA[a].beHit == false) 
					{
						enemytypeA[a].HP -= gamedata->playerAtk;
						bullet[i].use = false;
						enemytypeA[a].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
					
				}

				if (enemytypeA[a].HP <= 0)
				{
					enemytypeA[a].use = false;
					enemytypeA[a].beHit = false;
					enemytypeA[a].Hit_CDtime = HIT_CDTIME;
					enemytypeA[a].HP = ENEMY_TYPEA_MAXHP;
				}

			}
			//typeB Hit
			for (int b = 0; b < MAX_ENEMY_TYPEB; b++)
			{
				if (enemytypeB[b].use == false)continue;
				

				bool HitAans = CollisionBB(bullet[i].pos, enemytypeB[b].HitcheckPosA,
					bullet[i].Hitcheck_size, enemytypeB[b].HitcheckPosA_size);
				bool HitBans = CollisionBB(bullet[i].pos, enemytypeB[b].HitcheckPosB,
					bullet[i].Hitcheck_size, enemytypeB[b].HitcheckPosB_size);
				if (HitAans == true|| HitBans==true)
				{
					if (enemytypeB[b].beHit == false)
					{
						enemytypeB[b].HP -= gamedata->playerAtk;
						bullet[i].use = false;
						enemytypeB[b].beHit = true;
					}
					else
					{
						
						bullet[i].use = false;
					}
					
				}

				if (enemytypeB[b].HP <= 0)
				{
					enemytypeB[b].use = false;
					enemytypeB[b].beHit = false;
					enemytypeB[b].Hit_CDtime = HIT_CDTIME;
					enemytypeB[b].HP = ENEMY_TYPEB_MAXHP;
				}
			}
			//typeC Hit
			for (int c = 0; c < MAX_ENEMY_TYPEC; c++)
			{
				if (enemytypeC[c].use == false)continue;
				

				bool HitAans = CollisionBB(bullet[i].pos, enemytypeC[c].HitcheckPosA,
					bullet[i].Hitcheck_size, enemytypeC[c].HitcheckPosA_size);
				bool HitBans = CollisionBB(bullet[i].pos, enemytypeC[c].HitcheckPosB,
					bullet[i].Hitcheck_size, enemytypeC[c].HitcheckPosB_size);
				if (HitAans == true || HitBans == true)
				{
					if (enemytypeC[c].beHit == false) 
					{
						enemytypeC[c].HP -= gamedata->playerAtk;
						bullet[i].use = false;
						enemytypeC[c].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
					
				}

				if (enemytypeC[c].HP <= 0)
				{
					enemytypeC[c].use = false;
					enemytypeC[c].beHit = false;
					enemytypeC[c].Hit_CDtime = HIT_CDTIME;
					enemytypeC[c].HP = ENEMY_TYPEC_MAXHP;
				}
			}
			//typeD Hit
			for (int d = 0; d < MAX_ENEMY_TYPED; d++)
			{
				if (enemytypeD[d].use == false)continue;
				

				bool HitAans = CollisionBB(bullet[i].pos, enemytypeD[d].HitcheckPosA,
					bullet[i].Hitcheck_size, enemytypeD[d].HitcheckPosA_size);
				bool HitBans = CollisionBB(bullet[i].pos, enemytypeD[d].HitcheckPosB,
					bullet[i].Hitcheck_size, enemytypeD[d].HitcheckPosB_size);
				if (HitAans == true || HitBans == true)
				{
					if (enemytypeD[d].beHit == false)
					{
						enemytypeD[d].HP -= gamedata->playerAtk;
						bullet[i].use = false;
						enemytypeD[d].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
					
				}

				if (enemytypeD[d].HP <= 0)
				{
					
					enemytypeD[d].use = false;
					enemytypeD[d].beHit = false;
					enemytypeD[d].Hit_CDtime = HIT_CDTIME;
					enemytypeD[d].HP = ENEMY_TYPED_MAXHP;
				}
			}
			//BOSS Hit
			for (int s = 0; s < MAX_BOSS; s++)
			{
				if (enemyBOSS[s].use == false)continue;

				bool HitAans = CollisionBB(bullet[i].pos, enemyBOSS[s].HitcheckPosA,
					bullet[i].Hitcheck_size, enemyBOSS[s].HitcheckPosA_size);
				bool HitBans = CollisionBB(bullet[i].pos, enemyBOSS[s].HitcheckPosB,
					bullet[i].Hitcheck_size, enemyBOSS[s].HitcheckPosB_size);
				bool HitCans = CollisionBB(bullet[i].pos, enemyBOSS[s].HitcheckPosC,
					bullet[i].Hitcheck_size, enemyBOSS[s].HitcheckPosC_size);
				if (HitAans == true )
				{
					if (enemyBOSS[s].beHit == false) 
					{
						enemyBOSS[s].HP -= gamedata->playerAtk;
						bullet[i].use = false;
						enemyBOSS[s].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
					
				}
				else if (HitBans == true)
				{
					if (enemyBOSS[s].beHit == false)
					{
					enemyBOSS[s].HP -= gamedata->playerAtk;
					bullet[i].use = false;
					enemyBOSS[s].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
				}
				else if (HitCans == true)
				{
					if (enemyBOSS[s].beHit == false)
					{
					enemyBOSS[s].HP -= gamedata->playerAtk*2;
					bullet[i].use = false;
					enemyBOSS[s].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}
				}

				if (enemyBOSS[s].use == false)continue;
				if (enemyBOSS[s].HP <= 0)
				{
					ReleaseShadow(enemyBOSS[s].shadowIdx);
					enemyBOSS[s].use = false;
					enemyBOSS[s].HP = ENEMY_BOSS_MAXHP;
				}
			}
		}
	}

	for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
	{
		if (enemybullet1[i].use == true)
		{
			//typeA Hit
			for (int i = 0; i < MAX_PLAYER; i++)
			{
				if (player[i].use == false)continue;

				D3DXVECTOR3 ppos = player[i].pos;
				ppos.y += PLAYER_TALL / 2;
				bool HitAans = CollisionBB(enemybullet1[i].pos, ppos,
					enemybullet1[i].Hitcheck_size, player[i].touchsize);
				if (HitAans == true)
				{
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_BULLET_ATK;
						enemybullet1[i].use = false;
						player[i].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}

				}
			}
		}
	}
	for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
	{
		if (enemybullet2[i].use == true)
		{
			//typeA Hit
			for (int i = 0; i < MAX_PLAYER; i++)
			{
				if (player[i].use == false)continue;

				D3DXVECTOR3 ppos = player[i].pos;
				ppos.y += PLAYER_TALL / 2;
				bool HitAans = CollisionBB(enemybullet2[i].pos, ppos,
					enemybullet2[i].Hitcheck_size, player[i].touchsize);
				if (HitAans == true)
				{
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_BULLET_ATK;
						enemybullet2[i].use = false;
						player[i].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}

				}
			}
		}
	}//
	for (int i = 0; i < MAX_ENEMY_BOSS; i++)
	{
		if (bossbullet[i].use == true)
		{
			//typeA Hit
			for (int i = 0; i < MAX_PLAYER; i++)
			{
				if (player[i].use == false)continue;

				D3DXVECTOR3 ppos = player[i].pos;
				ppos.y += PLAYER_TALL / 2;
				bool HitAans = CollisionBB(bossbullet[i].pos, ppos,
					bossbullet[i].Hitcheck_size, player[i].touchsize);
				if (HitAans == true)
				{
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_BULLET_ATK;
						bossbullet[i].use = false;
						player[i].beHit = true;
					}
					else
					{
						bullet[i].use = false;
					}

				}
			}
		}
	}
}

//=============================================================================
// PLAYERとENEMY当たり判定処理
//=============================================================================
void TouchCheck(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (player[i].use == true)
		{
			//typeA Hit
			for (int a = 0; a < MAX_ENEMY_TYPEA; a++)
			{
				if (enemytypeA[a].use == false)continue;


				bool HitAans = CollisionBB(player[i].pos, enemytypeA[a].HitcheckPosA,
					player[i].touchsize, enemytypeA[a].HitcheckPosA_size);
				

				if (HitAans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;
						
						player[i].beHit = true;
					}
					
				}
			}
			//typeB Hit
			for (int b = 0; b < MAX_ENEMY_TYPEB; b++)
			{
				if (enemytypeB[b].use == false)continue;


				bool HitAans = CollisionBB(player[i].pos, enemytypeB[b].HitcheckPosA,
					player[i].touchsize, enemytypeB[b].HitcheckPosA_size);
				bool HitBans = CollisionBB(player[i].pos, enemytypeB[b].HitcheckPosB,
					player[i].touchsize, enemytypeB[b].HitcheckPosB_size);
				if (HitAans == true || HitBans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}
				}
			}
			//typeC Hit
			for (int c = 0; c < MAX_ENEMY_TYPEC; c++)
			{
				if (enemytypeC[c].use == false)continue;


				bool HitAans = CollisionBB(player[i].pos, enemytypeC[c].HitcheckPosA,
					player[i].touchsize, enemytypeC[c].HitcheckPosA_size);
				bool HitBans = CollisionBB(player[i].pos, enemytypeC[c].HitcheckPosB,
					player[i].touchsize, enemytypeC[c].HitcheckPosB_size);
				if (HitAans == true || HitBans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}

				}
			}
			//typeD Hit
			for (int d = 0; d < MAX_ENEMY_TYPED; d++)
			{
				if (enemytypeD[d].use == false)continue;


				bool HitAans = CollisionBB(player[i].pos, enemytypeD[d].HitcheckPosA,
					player[i].touchsize, enemytypeD[d].HitcheckPosA_size);
				bool HitBans = CollisionBB(player[i].pos, enemytypeD[d].HitcheckPosB,
					player[i].touchsize, enemytypeD[d].HitcheckPosB_size);
				if (HitAans == true || HitBans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}

				}
			}
			//BOSS Hit
			for (int s = 0; s < MAX_BOSS; s++)
			{
				if (enemyBOSS[s].use == false)continue;

				bool HitAans = CollisionBB(player[i].pos, enemyBOSS[s].HitcheckPosA,
					player[i].touchsize, enemyBOSS[s].HitcheckPosA_size);
				bool HitBans = CollisionBB(player[i].pos, enemyBOSS[s].HitcheckPosB,
					player[i].touchsize, enemyBOSS[s].HitcheckPosB_size);
				bool HitCans = CollisionBB(player[i].pos, enemyBOSS[s].HitcheckPosC,
					player[i].touchsize, enemyBOSS[s].HitcheckPosC_size);
				if (HitAans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}

				}
				else if (HitBans == true)
				{
					player[i].enemytouch = true;
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}

				}
				else if (HitCans == true)
				{
					if (player[i].enemytouch == false)
					{
						player[i].enemytouch = true;
					}
					player[i].beTouchTime = PLAYER_HIT_CD;
					if (player[i].beHit == false)
					{
						gamedata->playerHPnow -= ENEMY_TOUCH_ATK;

						player[i].beHit = true;
					}

				}
			}
		}
	}

}
//=============================================================================
// ENEMYとPLAYERのHP判定処理
//=============================================================================
//void HPcheck(void)
//{
//	
//		//typeA HP
//		for (int a = 0; a < MAX_ENEMY_TYPEA; a++)
//		{
//			if (enemytypeA[a].use == false)continue;
//			
//			
//		}
//		//typeB HP
//		for (int b = 0; b < MAX_ENEMY_TYPEB; b++)
//		{
//			if (enemytypeB[b].use == false)continue;
//			
//		}
//		//typeC HP
//		for (int c = 0; c < MAX_ENEMY_TYPEC; c++)
//		{
//			if (enemytypeC[c].use == false)continue;
//			
//		}
//		//typeD HP
//		for (int d = 0; d < MAX_ENEMY_TYPED; d++)
//		{
//			if (enemytypeD[d].use == false)continue;
//			
//		}
//		//BOSS HP
//		for (int s = 0; s < MAX_BOSS; s++)
//		{
//			if (enemyBOSS[s].use == false)continue;
//			
//		}
//		//Player HP
//		for (int i = 0; i < MAX_PLAYER; i++)
//		{
//			if (player[i].use == false)continue;
//			if (gamedata->playerHPnow <= 0)
//			{
//				player[i].use = false;
//				if (i + 1 >= MAX_PLAYER)
//				{
//					gamedata->gameover = true;
//					gamedata->playerHPnow = PLAYER_MAXHP_INITIAL;
//					gamedata->playerMAXHP = PLAYER_MAXHP_INITIAL;
//					gamedata->nowplayernum = 0;
//				}
//				else {
//					player[i + 1].use = true;
//					gamedata->nowplayernum++;
//					gamedata->playerHPnow = gamedata->playerMAXHP;
//					SetFade(FADE_OUT, MODE_RESTAREA);
//				}
//			}
//		}
//}