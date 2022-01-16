//=============================================================================
//
// ロック処理 [Rope.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "Rope.h"
#include "shadow.h"
#include "renderer.h"
#include "rope.h"
#include "collision.h"
#include "Map.h"
#include "MapTouch.h"
#include "meshMap.h"
#include "meshMapReverse.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_Rope		"data/MODEL/movepoint.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量

#define Rope_SHADOW_SIZE	(0.4f)							// 影の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ROPE				g_Rope[MAX_ROPE];						// プレイヤー
static PLAYER			*player = GetPlayer();
static CAMERA			*camera = GetCamera();
static GAMEDATA			*gamedata = GetData();
static float roty = 0.0f;
//static int	life = 10;
static float bugdeletetime=0.0f;
static float bugdeletetimepule = 0.01f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRope(void)
{
	
	for (int i = 0; i < MAX_ROPE; i++) {
		if (gamedata->modenow == MODE_TITLE)
		{
			LoadModel(MODEL_Rope, &g_Rope[i].model);
		}

		g_Rope[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Rope[i].pos_previous = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Rope[i].rot = D3DXVECTOR3(0.0f, D3DX_PI/4, 0.0f);
		g_Rope[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Rope[i].start_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Rope[i].end_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Rope[i].time = 0.0f;
		g_Rope[i].timeplus = 0.01f;
		g_Rope[i].spd = 0.0f;		// 移動スピードクリア
		g_Rope[i].point_set=true;
		g_Rope[i].point_set_finsh = false;
		g_Rope[i].endpos_Decide=false;
		g_Rope[i].touch_check = false;
		/*D3DXVECTOR3 pos = g_Rope[i].pos;
		pos.y = 0.0f;
		g_Rope[i].shadowIdx = CreateShadow(pos, Rope_SHADOW_SIZE, Rope_SHADOW_SIZE);*/
		g_Rope[i].use = false;
		
		return S_OK;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRope(void)
{
	for (int i = 0; i < MAX_ROPE; i++)
	{

		// モデルの解放処理
		UnloadModel(&g_Rope[i].model);
	}

}

//float roty = 0.0f;
//=============================================================================
// 更新処理
//=============================================================================
void UpdateRope(void)
{
	for (int j = 0; j < MAX_PLAYER; j++)
	{
		if (player[j].use == false)
		{
			continue;
		}
		for (int i = 0; i < MAX_ROPE; i++)
		{
			/*if (g_Rope[i].use == false) {
				continue;
			}*/

			if (g_Rope[i].point_set == false)
			{
				if (camera->shoutchange == true)
				{
					g_Rope[i].rot.y = player[0].rot.y;
				}
				else
				{
					g_Rope[i].rot.y = camera->rot.y+D3DX_PI;

				}
				
				g_Rope[i].touch_check = false;
				
				g_Rope[i].start_pos = player[j].pos;
				g_Rope[i].start_pos.y = player[j].pos.y + (PLAYER_TALL/2 );
				D3DXVECTOR3 aa = camera->at - camera->pos;;
				D3DXVec3Normalize(&aa, &aa);
				aa *= ROPE_LONG;
				float length = (float)(sqrt((aa.x*aa.x) + (aa.z*aa.z) + (aa.y*aa.y)));
				if (camera->shoutchange == true) {
					g_Rope[i].end_pos.x = g_Rope[i].start_pos.x - (float)(sinf(player[j].rot.y)* length*1.5);
					g_Rope[i].end_pos.z = g_Rope[i].start_pos.z - (float)(cosf(player[j].rot.y)* length*1.5);
				}
				else
				{
					g_Rope[i].end_pos.x = g_Rope[i].start_pos.x + (float)(sinf(camera->rot.y)* length*1.5);
					g_Rope[i].end_pos.z = g_Rope[i].start_pos.z + (float)(cosf(camera->rot.y)* length*1.5);
				}
				
				g_Rope[i].end_pos.y = g_Rope[i].start_pos.y + (float)((aa.y)*1.5);


				//g_Player[i].pos.x -= sinf(player[j].rot.y) ;
				//g_Player[i].pos.z -= cosf(player[j].rot.y) ;
				
				g_Rope[i].pos = g_Rope[i].start_pos;
				{
					g_Rope[i].endpos_Decide = false;
					
					MeshwallTouch(2);
					SlopewallTouch(2);
					if (camera->updown_check == false) {
						MeshfieldTouch(2);
					}
					else if (camera->updown_check == true) {
						MeshceilingTouch(2);
					}
					/*g_Rope[i].endpos_Decide = false;*/
				}
				g_Rope[i].point_set = true;
				g_Rope[i].use = true;
				
			}
			

			if (g_Rope[i].use == true)
			{
				if (g_Rope[i].pos != g_Rope[i].end_pos)
				{
					/*g_Rope[i].pos_previous = g_Rope[i].pos;*/

					g_Rope[i].pos += (g_Rope[i].end_pos - g_Rope[i].start_pos) / ROPE_LONG_PART;
				}

				//g_Rope[i].posをstart~end以外させないのため
				if (g_Rope[i].end_pos.x <= g_Rope[i].start_pos.x)
				{
						if (g_Rope[i].end_pos.z <= g_Rope[i].start_pos.z)
						{
							if (g_Rope[i].end_pos.y <= g_Rope[i].start_pos.y)
							{
								if (g_Rope[i].pos.x <= g_Rope[i].end_pos.x
									&&g_Rope[i].pos.y <= g_Rope[i].end_pos.y
									&&g_Rope[i].pos.z <= g_Rope[i].end_pos.z)
								{
									g_Rope[i].pos = g_Rope[i].end_pos;
								}
							}
							else if (g_Rope[i].end_pos.y > g_Rope[i].start_pos.y) {
								if (g_Rope[i].pos.x <= g_Rope[i].end_pos.x
									&&g_Rope[i].pos.y >= g_Rope[i].end_pos.y
									&&g_Rope[i].pos.z <= g_Rope[i].end_pos.z)
								{
									g_Rope[i].pos = g_Rope[i].end_pos;
								}
							}

						}
						else if (g_Rope[i].end_pos.z > g_Rope[i].start_pos.z)
						{
							if (g_Rope[i].end_pos.y <= g_Rope[i].start_pos.y)
							{
								if (g_Rope[i].pos.x <= g_Rope[i].end_pos.x
									&&g_Rope[i].pos.y <= g_Rope[i].end_pos.y
									&&g_Rope[i].pos.z >= g_Rope[i].end_pos.z)
								{
									g_Rope[i].pos = g_Rope[i].end_pos;
								}
							}
							else if (g_Rope[i].end_pos.y > g_Rope[i].start_pos.y) {
								if (g_Rope[i].pos.x <= g_Rope[i].end_pos.x
									&&g_Rope[i].pos.y >= g_Rope[i].end_pos.y
									&&g_Rope[i].pos.z >= g_Rope[i].end_pos.z)
								{
									g_Rope[i].pos = g_Rope[i].end_pos;
								}
							}
						}
					}
				else if (g_Rope[i].end_pos.x > g_Rope[i].start_pos.x)
				{
					if (g_Rope[i].end_pos.z <= g_Rope[i].start_pos.z)
					{
						if (g_Rope[i].end_pos.y <= g_Rope[i].start_pos.y)
						{
							if (g_Rope[i].pos.x >= g_Rope[i].end_pos.x
								&&g_Rope[i].pos.y <= g_Rope[i].end_pos.y
								&&g_Rope[i].pos.z <= g_Rope[i].end_pos.z)
							{
								g_Rope[i].pos = g_Rope[i].end_pos;
							}
						}
						else if (g_Rope[i].end_pos.y > g_Rope[i].start_pos.y) {
							if (g_Rope[i].pos.x >= g_Rope[i].end_pos.x
								&&g_Rope[i].pos.y >= g_Rope[i].end_pos.y
								&&g_Rope[i].pos.z <= g_Rope[i].end_pos.z)
							{
								g_Rope[i].pos = g_Rope[i].end_pos;
							}
						}

					}
					else if (g_Rope[i].end_pos.z > g_Rope[i].start_pos.z)
					{
						if (g_Rope[i].end_pos.y <= g_Rope[i].start_pos.y)
						{
							if (g_Rope[i].pos.x >= g_Rope[i].end_pos.x
								&&g_Rope[i].pos.y <= g_Rope[i].end_pos.y
								&&g_Rope[i].pos.z >= g_Rope[i].end_pos.z)
							{
								g_Rope[i].pos = g_Rope[i].end_pos;
							}
						}
						else if (g_Rope[i].end_pos.y > g_Rope[i].start_pos.y) {
							if (g_Rope[i].pos.x >= g_Rope[i].end_pos.x
								&&g_Rope[i].pos.y >= g_Rope[i].end_pos.y
								&&g_Rope[i].pos.z >= g_Rope[i].end_pos.z)
							{
								g_Rope[i].pos = g_Rope[i].end_pos;
							}
						}
					}
				}
				//もしg_Rope[i].posはg_Rope[i].end_posに到着
				if (g_Rope[i].pos == g_Rope[i].end_pos)
				{
					if (g_Rope[i].touch_check == true)
					{

						g_Rope[i].point_set_finsh = true;

						player[i].jumpOK = false;
						bugdeletetime += bugdeletetimepule;
						//g_Rope[i].touch_check = false;
						if (bugdeletetime > 20) 
						{
							bugdeletetime = 0.0f;
							g_Rope[i].use = false;
							player[j].ropeup_OK = false;
							player[j].ropeuse_OK = false;
							g_Rope[i].point_set_finsh = false;
						}
					}
					else if (g_Rope[i].touch_check == false)
					{
						/*life--;
						if (life <= 0) {
							
							life = 20;
						}*/
						g_Rope[i].use = false;
						player[j].ropeup_OK = false;
						player[j].ropeuse_OK = false;
						g_Rope[i].point_set_finsh = false;
					}
				}
			}
			
				

					
			/*
			if (g_Player[i].jumping == true) {
				float	jumopos = 5.0f;							// XYZ移動量を計算している
				int nowNo = (int)g_Player[i].time;				// 整数分であるテーブル番号を取り出している
				float nowTime = g_Player[i].time - nowNo;		// 時間部分である少数を取り出している
				float jumopos2 = jumopos;
				jumopos *= nowTime;								// 現在の移動量を計算している
				jumopos2 -= jumopos;
				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				g_Player[i].pos.y += jumopos2;
				cam->pos.y += jumopos2;
				cam->at.y += jumopos2;
				g_Player[i].timeplus += 0.001f;
				g_Player[i].time += g_Player[i].timeplus;		// 時間を進めている
				if (g_Player[i].time >= 1 || g_rope[i].point_set_finsh == true) {
					g_Player[i].jumping = false;
					g_Player[i].downOK = true;
					g_Player[i].time = 0;
					g_Player[i].timeplus = 0.01f;

				}
			}
			
			
			*/







			// 影もプレイヤーの位置に合わせる
			/*D3DXVECTOR3 pos = g_Rope[i].pos;
			pos.y = 0.0f;
			SetPositionShadow(g_Rope[i].shadowIdx, pos);*/
		}
	}
	

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRope(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ROPE; i++) {
		if (g_Rope[i].use == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Rope[i].mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_Rope[i].scl.x, g_Rope[i].scl.y, g_Rope[i].scl.z);
			D3DXMatrixMultiply(&g_Rope[i].mtxWorld, &g_Rope[i].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rope[i].rot.y + D3DX_PI, g_Rope[i].rot.x, g_Rope[i].rot.z);
			D3DXMatrixMultiply(&g_Rope[i].mtxWorld, &g_Rope[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Rope[i].pos.x, g_Rope[i].pos.y, g_Rope[i].pos.z);
			D3DXMatrixMultiply(&g_Rope[i].mtxWorld, &g_Rope[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&g_Rope[i].mtxWorld);

			// モデル描画
			DrawModel(&g_Rope[i].model);
		}
	}


	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
ROPE *GetRope(void)
{
	return &g_Rope[0];
}

