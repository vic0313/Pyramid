//=============================================================================
//
// 地図の当たり判定処理 [MapTouch.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "player.h"
#include "camera.h"
#include "rope.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static MAP_POINT	*g_ceilingpoint = GetCeilingPoint();
static MAP_POINT	*g_wallpoint = GetWallPoint();
static MAP_POINT *g_slopewallpoint = GetSlopeWallPoint();
static MAP_POINT	*g_fieldslopepoint = GetFieldSlopePoint();
static GAMEDATA			*gamedata = GetData();
static MESH_MAP *g_wall = Getwall();

static BULLET	*g_bullet = GetBullet();
static BULLET	*enemy_bullet1 = GetEnemyBullet1();
static BULLET	*enemy_bullet2 = GetEnemyBullet2();
static BULLET	*boss_bullet = GetEnemyBulletBoss();
static PLAYER  *player = GetPlayer();
static CAMERA  *camera= GetCamera();
static ROPE		*rope = GetRope();

static ENEMY *enemytypeA = GetEnemyTypeA();
static ENEMY *enemytypeB= GetEnemyTypeB();
static ENEMY *enemytypeC = GetEnemyTypeC();
static ENEMY *enemytypeD = GetEnemyTypeD();

//=============================================================================
// 地面と当たり判定処理
// 
// 
//=============================================================================
void MeshfieldTouch(int check) {
	switch (check)
	{
	case 1://player TOUCH
	{
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (player[i].use == false)continue;
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (player[i].pos.x >= g_fieldslopepoint[j].pos0.x &&player[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&player[i].pos.z >= g_fieldslopepoint[j].pos3.z &&player[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = player[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - player[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = player[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (player[i].downpos.y >= (HitPos.y - 10.0) && player[i].pos.y <= (HitPos.y + 1.0f))
					{
						D3DXVECTOR3 pos = player[i].pos;
						switch (g_fieldslopepoint[j].check)
						{
						case 1:player[i].wall_touch = 1;
							break;
						case 2:player[i].wall_touch = 2;
							break;
						case 3:player[i].wall_touch = 3;
							break;
						case 4:player[i].wall_touch = 4;
							break;
						default:
							break;
						}
						
						player[i].pos.y = HitPos.y + 1.0f;
						camera->pos.y += HitPos.y+1.0f-pos.y ;
						camera->at.y+= HitPos.y + 1.0f - pos.y;
						player[i].jumpOK = true;
						player[i].bigjumpOK = true;
						player[i].time_downplus = 0.01f;
						player[i].time_down = 0;
						player[i].touchcheck_field = true;
						player[i].bigjumping = false;
					}
					//if (pos.y >= (HitPos.y - 2.0) && pos.y <= (HitPos.y + 2.0f))
					//{
					//	pos.y = HitPos.y + 2.0f;
					//	player[i].downOK = false;
					//}
				}


			}


			//平台TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD; j++) {
				if (player[i].pos.x >= g_fieldpoint[j].pos0.x &&player[i].pos.x <= g_fieldpoint[j].pos3.x
					&&player[i].pos.z >= g_fieldpoint[j].pos3.z &&player[i].pos.z <= g_fieldpoint[j].pos0.z)
				{
					if (player[i].downpos.y >= (g_fieldpoint[j].pos0.y - 5.0f) && player[i].pos.y <= (g_fieldpoint[j].pos0.y + 1.0f))
					{
						// レイキャストして足元の高さを求める
						D3DXVECTOR3 HitPosition;		//交点
						D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
						D3DXVECTOR3 start = player[i].pos;
						D3DXVECTOR3 end = player[i].pos;
						// 少し上から、ズドーンと下へレイを飛ばす
						start.y += 100.0f;
						end.y -= 1000.0f;

						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							player[i].pos.y = HitPosition.y + 1.0f;
							player[i].jumpOK = true;
							player[i].bigjumpOK = true;
							player[i].time_downplus = 0.01f;
							player[i].time_down = 0;
							player[i].touchcheck_field = true;
							player[i].bigjumping = false;
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							player[i].pos.y = HitPosition.y + 1.0f;
							player[i].jumpOK = true;
							player[i].bigjumpOK = true;
							player[i].time_downplus = 0.01f;
							player[i].time_down = 0;
							player[i].touchcheck_field = true;
							player[i].bigjumping = false;
						}

					}
				}
			}

			////地面以下の判定
			if (player[i].pos.x >= -550.0f&&player[i].pos.x <= 550.0f
				&&player[i].pos.z >= 300.0f&&player[i].pos.z <= 1500.0f)
			{
				if (player[i].pos.y <= BOSS_WAVE_AMPLITUDE)
				{
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					//bool ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
					D3DXVECTOR3 poss = player[i].pos;

					bool ans = RayHitBOSSField(player[i].pos, &HitPosition, &Normal);
					if (player[i].namidause == true)
					{
						if (player[i].downpos.y >= (HitPosition.y - 50.0f) && player[i].pos.y <= (HitPosition.y + 1.0f))
						{

							player[i].pos.y = HitPosition.y + 1.0f;
							/*if (poss.y < player[i].pos.y)
							{*/
							camera->pos.y += player[i].pos.y - poss.y;
							//}

							player[i].downOK = true;
							player[i].movecheck = false;
							//player[i].jumpOK = false;
							player[i].bigjumpOK = true;
							player[i].time_downplus = 0.01f;
							player[i].time_down = 0;
							player[i].touchcheck_field = true;
							player[i].bigjumping = false;
						}if (player[i].pos.y >= (HitPosition.y + 5.0f))
						{
							player[i].movecheck = true;
						}
					}
					else if (player[i].namidause == false)
					{
						player[i].movecheck = true;
						if (player[i].pos.y <= 1.0f)
						{
							player[i].pos.y = 1.0f;
							player[i].time_downplus = 0.01f;
							player[i].time_down = 0;
							//player[i].downOK = false;
							//player[i].jumpOK = true;
							player[i].bigjumpOK = true;
							player[i].touchcheck_field = true;
							player[i].bigjumping = false;
						}/*
						else {
							player[i].downOK = true;
						}*/
					}
				}
				
			}
			 
				
			


		}
		break;
	}
	case 2: {

		for (int i = 0; i < MAX_ROPE; i++)
		{

			if (rope[i].endpos_Decide == false)
			{
				//坂道TOUCHの判定
				for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++)
				{
					D3DXVECTOR3		nor, nor2;
					D3DXVECTOR3		vec1;
					D3DXVECTOR3		vec2;
					float			d1, d2;
					bool			check = false;
					int				check1 = 0;
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_fieldslopepoint[j].pos1 - g_fieldslopepoint[j].pos0;
						vec2 = g_fieldslopepoint[j].pos2 - g_fieldslopepoint[j].pos0;
						crossProduct(&nor, &vec2, &vec1);
						D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_fieldslopepoint[j].pos2 - g_fieldslopepoint[j].pos1;
						vec2 = g_fieldslopepoint[j].pos3 - g_fieldslopepoint[j].pos1;
						crossProduct(&nor2, &vec2, &vec1);
						D3DXVec3Normalize(&nor2, &nor2);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 start = rope[i].start_pos;
					D3DXVECTOR3 end = rope[i].end_pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
						vec1 = start - g_fieldslopepoint[j].pos0;
						vec2 = end - g_fieldslopepoint[j].pos0;
						d1 = dotProduct(&vec1, &nor);
						d2 = dotProduct(&vec2, &nor);
						if ((d1 * d2) > 0.0f)
						{
							// 当たっている可能性は無い
							check = false;
						}
						else if ((d1 * d2) <= 0.0f)
						{
							check = true;
							check1 = 1;
						}

						if (check == false) {
							// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
							vec1 = start - g_fieldslopepoint[j].pos3;
							vec2 = end - g_fieldslopepoint[j].pos3;
							d1 = dotProduct(&vec1, &nor2);
							d2 = dotProduct(&vec2, &nor2);
							if ((d1 * d2) > 0.0f)
							{
								// 当たっている可能性は無い
								check = false;
							}
							else if ((d1 * d2) <= 0.0f)
							{
								check = true;
							}

						}
						if (check == true) {
							if (check1 == 1) {
								vec1 = start - g_fieldslopepoint[j].pos0;
								vec2 = end - g_fieldslopepoint[j].pos0;
								d1 = dotProduct(&vec1, &nor);
								d2 = dotProduct(&vec2, &nor);
							}
							else if (check1 == 0) {
								vec1 = start - g_fieldslopepoint[j].pos3;
								vec2 = end - g_fieldslopepoint[j].pos3;
								d1 = dotProduct(&vec1, &nor2);
								d2 = dotProduct(&vec2, &nor2);
							}
							d1 = (float)fabs(d1);		//絶対値を求めている
							d2 = (float)fabs(d2);		//絶対値を求めている
							float a = d1 / (d1 + d2);							// 内分比
							D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
							D3DXVECTOR3		p3 = g_fieldslopepoint[j].pos0 + vec3;						// 交点
							HitPosition = p3;											// 求めた交点を入れておく
						}
					}
					//check HitPositionはstartとendの中かどうか
					//check HitPositionはstartとendの中かどうか
					bool check2 = false;

					if (rope[i].end_pos.x <= rope[i].start_pos.x) {

						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}
					else if (rope[i].end_pos.x > rope[i].start_pos.x)
					{
						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}

					if (check2 == true)
					{
						if (HitPosition.x >= g_fieldslopepoint[j].pos0.x &&HitPosition.x <= g_fieldslopepoint[j].pos3.x
							&&HitPosition.z >= g_fieldslopepoint[j].pos3.z &&HitPosition.z <= g_fieldslopepoint[j].pos0.z)
						{
							rope[i].end_pos.x = HitPosition.x;
							rope[i].end_pos.z = HitPosition.z;
							float x = g_fieldslopepoint[j].pos3.x - rope[i].end_pos.x;
							float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
							float z = rope[i].end_pos.z - g_fieldslopepoint[j].pos3.z;
							float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
							float  y_all = 0.0f;

							//if slopecheck=1 [i].左下から右上の坂道
							//if slopecheck=2 [i].右下から左上の坂道
							//if slopecheck=3 [i].上上から下下の坂道
							//if slopecheck=4 [i].下上から上下の坂道

							switch (g_fieldslopepoint[j].check)
							{
							case 1:
								y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
								HitPosition.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
								break;
							case 2:
								y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
								HitPosition.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
								break;
							case 3:
								y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
								HitPosition.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
								break;
							case 4:
								y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
								HitPosition.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
								break;
							default:
								break;
							}

							rope[i].end_pos.y = HitPosition.y + 1.0f;
							rope[i].touch_check = true;
							rope[i].endpos_Decide = true;

						}
					}
					if (rope[i].touch_check == true) { break; }
				}

				//平台TOUCHの判定
				if (rope[i].endpos_Decide == false)
				{

					for (int j = 0; j < MAX_MESH_FIELD; j++)
					{

						D3DXVECTOR3		nor, nor2;
						D3DXVECTOR3		vec1;
						D3DXVECTOR3		vec2;
						float			d1, d2;
						bool			check = false;
						int				check1 = 0;
						{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
							vec1 = g_fieldpoint[j].pos1 - g_fieldpoint[j].pos0;
							vec2 = g_fieldpoint[j].pos2 - g_fieldpoint[j].pos0;
							crossProduct(&nor, &vec2, &vec1);
							D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

						}
						{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
							vec1 = g_fieldpoint[j].pos2 - g_fieldpoint[j].pos1;
							vec2 = g_fieldpoint[j].pos3 - g_fieldpoint[j].pos1;
							crossProduct(&nor2, &vec2, &vec1);
							D3DXVec3Normalize(&nor2, &nor2);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

						}
						// レイキャストして足元の高さを求める
						D3DXVECTOR3 HitPosition;		//交点
						D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
						D3DXVECTOR3 start = rope[i].start_pos;
						D3DXVECTOR3 end = rope[i].end_pos;
						// 少し上から、ズドーンと下へレイを飛ばす
						{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
							vec1 = start - g_fieldpoint[j].pos0;
							vec2 = end - g_fieldpoint[j].pos0;
							d1 = dotProduct(&vec1, &nor);
							d2 = dotProduct(&vec2, &nor);
							if ((d1 * d2) > 0.0f)
							{
								// 当たっている可能性は無い
								check = false;
							}
							else if ((d1 * d2) <= 0.0f)
							{
								check = true;
								check1 = 1;
							}

							if (check == false) {
								// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
								vec1 = start - g_fieldpoint[j].pos3;
								vec2 = end - g_fieldpoint[j].pos3;
								d1 = dotProduct(&vec1, &nor2);
								d2 = dotProduct(&vec2, &nor2);
								if ((d1 * d2) > 0.0f)
								{
									// 当たっている可能性は無い
									check = false;
								}
								else if ((d1 * d2) <= 0.0f)
								{
									check = true;
								}

							}
							if (check == true)
							{
								if (check1 == 1) {
									vec1 = start - g_fieldpoint[j].pos0;
									vec2 = end - g_fieldpoint[j].pos0;
									d1 = dotProduct(&vec1, &nor);
									d2 = dotProduct(&vec2, &nor);
								}
								else if (check1 == 0) {
									vec1 = start - g_fieldpoint[j].pos3;
									vec2 = end - g_fieldpoint[j].pos3;
									d1 = dotProduct(&vec1, &nor2);
									d2 = dotProduct(&vec2, &nor2);
								}
								d1 = (float)fabs(d1);		//絶対値を求めている
								d2 = (float)fabs(d2);		//絶対値を求めている
								float a = d1 / (d1 + d2);							// 内分比
								D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
								D3DXVECTOR3		p3 = g_fieldpoint[j].pos0 + vec3;						// 交点
								HitPosition = p3;											// 求めた交点を入れておく
							}
						}

						//check HitPositionはstartとendの中かどうか
						bool check2 = false;

						if (rope[i].end_pos.x <= rope[i].start_pos.x) {

							if (rope[i].end_pos.z <= rope[i].start_pos.z)
							{
								if (rope[i].end_pos.y <= rope[i].start_pos.y)
								{
									if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
										&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
										&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
								else if (rope[i].end_pos.y > rope[i].start_pos.y) {
									if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
										&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
										&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}

							}
							else if (rope[i].end_pos.z > rope[i].start_pos.z)
							{
								if (rope[i].end_pos.y <= rope[i].start_pos.y)
								{
									if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
										&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
										&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
								else if (rope[i].end_pos.y > rope[i].start_pos.y) {
									if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
										&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
										&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
							}
						}
						else if (rope[i].end_pos.x > rope[i].start_pos.x)
						{
							if (rope[i].end_pos.z <= rope[i].start_pos.z)
							{
								if (rope[i].end_pos.y <= rope[i].start_pos.y)
								{
									if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
										&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
										&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
								else if (rope[i].end_pos.y > rope[i].start_pos.y) {
									if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
										&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
										&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}

							}
							else if (rope[i].end_pos.z > rope[i].start_pos.z)
							{
								if (rope[i].end_pos.y <= rope[i].start_pos.y)
								{
									if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
										&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
										&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
								else if (rope[i].end_pos.y > rope[i].start_pos.y) {
									if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
										&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
										&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
									{
										check2 = true;
									}
								}
							}
						}

						if (check2 == true)
						{
							if (HitPosition.x >= g_fieldpoint[j].pos0.x &&HitPosition.x <= g_fieldpoint[j].pos3.x
								&&HitPosition.z >= g_fieldpoint[j].pos3.z &&HitPosition.z <= g_fieldpoint[j].pos0.z
								&&HitPosition.y >= (g_fieldpoint[j].pos3.y-1.0f) &&HitPosition.y <= (g_fieldpoint[j].pos0.y+1.0f))
							{
								rope[i].end_pos = HitPosition;
								rope[i].end_pos.y = HitPosition.y + 1.0f;
								rope[i].endpos_Decide = true;
								rope[i].touch_check = true;
							}

						}
						if (rope[i].touch_check == true) { break; }
					}
				}

			}
		}
		break;
	}
	case 3:
	{
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_bullet[i].use == false) { break; }
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (g_bullet[i].pos.x >= g_fieldslopepoint[j].pos0.x &&g_bullet[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&g_bullet[i].pos.z >= g_fieldslopepoint[j].pos3.z &&g_bullet[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = g_bullet[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - g_bullet[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = g_bullet[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (g_bullet[i].pos.y <= (HitPos.y + BULLET_CHECK_RADIUS) && g_bullet[i].pos.y >= (HitPos.y- BULLET_CHECK_RADIUS))
					{
						
						//g_bullet[i].touchparticle = true;
						g_bullet[i].particlepos = g_bullet[i].pos;
						g_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;
						
						g_bullet[i].touchcheck = true;
					}
					
				}


			}


			//平台TOUCHの判定
			for (int j = 1; j < MAX_MESH_FIELD; j++) {
				if (g_bullet[i].pos.y <= (g_fieldpoint[j].pos0.y + BULLET_CHECK_RADIUS) && g_bullet[i].pos.y >= (g_fieldpoint[j].pos0.y- BULLET_CHECK_RADIUS)) {
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = g_bullet[i].pos;
					D3DXVECTOR3 end = g_bullet[i].pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					start.y += 100.0f;
					end.y -= 1000.0f;
					if (g_bullet[i].pos.x >= g_fieldpoint[j].pos0.x &&g_bullet[i].pos.x <= g_fieldpoint[j].pos3.x
						&&g_bullet[i].pos.z >= g_fieldpoint[j].pos3.z &&g_bullet[i].pos.z <= g_fieldpoint[j].pos0.z) {
						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							g_bullet[i].particlepos = g_bullet[i].pos;
							g_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;
							
							g_bullet[i].touchcheck = true;
							//g_bullet[i].touchparticle = true;
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							g_bullet[i].particlepos = g_bullet[i].pos;
							g_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;
							g_bullet[i].touchcheck = true;
							//g_bullet[i].touchparticle = true;
						}

					}
				}
			}
			//地面以下の判定
			if (g_bullet[i].pos.y <= (0.0f+ BULLET_CHECK_RADIUS)) {
				g_bullet[i].particlepos = g_bullet[i].pos;
				g_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;
				
				g_bullet[i].touchcheck = true;
				//g_bullet[i].touchparticle = true;
			}
		}
		//enemyのbullet1
		for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
		{
			if (enemy_bullet1[i].use == false) { break; }
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (enemy_bullet1[i].pos.x >= g_fieldslopepoint[j].pos0.x &&enemy_bullet1[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&enemy_bullet1[i].pos.z >= g_fieldslopepoint[j].pos3.z &&enemy_bullet1[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = enemy_bullet1[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - enemy_bullet1[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = enemy_bullet1[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (enemy_bullet1[i].pos.y <= (HitPos.y + BULLET_CHECK_RADIUS) && enemy_bullet1[i].pos.y >= (HitPos.y - BULLET_CHECK_RADIUS))
					{

						//enemy_bullet1[i].touchparticle = true;
						enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
						enemy_bullet1[i].particlepos.y -= BULLET_CHECK_RADIUS;

						enemy_bullet1[i].touchcheck = true;
					}

				}


			}


			//平台TOUCHの判定
			for (int j = 1; j < MAX_MESH_FIELD; j++) {
				if (enemy_bullet1[i].pos.y <= (g_fieldpoint[j].pos0.y + BULLET_CHECK_RADIUS) && enemy_bullet1[i].pos.y >= (g_fieldpoint[j].pos0.y - BULLET_CHECK_RADIUS)) {
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = enemy_bullet1[i].pos;
					D3DXVECTOR3 end = enemy_bullet1[i].pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					start.y += 100.0f;
					end.y -= 1000.0f;
					if (enemy_bullet1[i].pos.x >= g_fieldpoint[j].pos0.x &&enemy_bullet1[i].pos.x <= g_fieldpoint[j].pos3.x
						&&enemy_bullet1[i].pos.z >= g_fieldpoint[j].pos3.z &&enemy_bullet1[i].pos.z <= g_fieldpoint[j].pos0.z) {
						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
							enemy_bullet1[i].particlepos.y -= BULLET_CHECK_RADIUS;

							enemy_bullet1[i].touchcheck = true;
							//enemy_bullet1[i].touchparticle = true;
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
							enemy_bullet1[i].particlepos.y -= BULLET_CHECK_RADIUS;
							enemy_bullet1[i].touchcheck = true;
							//enemy_bullet1[i].touchparticle = true;
						}

					}
				}
			}
			//地面以下の判定
			if (enemy_bullet1[i].pos.y <= (0.0f + BULLET_CHECK_RADIUS)) {
				enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
				enemy_bullet1[i].particlepos.y -= BULLET_CHECK_RADIUS;

				enemy_bullet1[i].touchcheck = true;
				//enemy_bullet1[i].touchparticle = true;
			}
		}

		for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
		{
			if (enemy_bullet2[i].use == false) { break; }
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (enemy_bullet2[i].pos.x >= g_fieldslopepoint[j].pos0.x &&enemy_bullet2[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&enemy_bullet2[i].pos.z >= g_fieldslopepoint[j].pos3.z &&enemy_bullet2[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = enemy_bullet2[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - enemy_bullet2[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = enemy_bullet2[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (enemy_bullet2[i].pos.y <= (HitPos.y + BULLET_CHECK_RADIUS) && enemy_bullet2[i].pos.y >= (HitPos.y - BULLET_CHECK_RADIUS))
					{

						//enemy_bullet2[i].touchparticle = true;
						enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
						enemy_bullet2[i].particlepos.y -= BULLET_CHECK_RADIUS;

						enemy_bullet2[i].touchcheck = true;
					}

				}


			}


			//平台TOUCHの判定
			for (int j = 1; j < MAX_MESH_FIELD; j++) {
				if (enemy_bullet2[i].pos.y <= (g_fieldpoint[j].pos0.y + BULLET_CHECK_RADIUS) && enemy_bullet2[i].pos.y >= (g_fieldpoint[j].pos0.y - BULLET_CHECK_RADIUS)) {
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = enemy_bullet2[i].pos;
					D3DXVECTOR3 end = enemy_bullet2[i].pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					start.y += 100.0f;
					end.y -= 1000.0f;
					if (enemy_bullet2[i].pos.x >= g_fieldpoint[j].pos0.x &&enemy_bullet2[i].pos.x <= g_fieldpoint[j].pos3.x
						&&enemy_bullet2[i].pos.z >= g_fieldpoint[j].pos3.z &&enemy_bullet2[i].pos.z <= g_fieldpoint[j].pos0.z) {
						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
							enemy_bullet2[i].particlepos.y -= BULLET_CHECK_RADIUS;

							enemy_bullet2[i].touchcheck = true;
							//enemy_bullet2[i].touchparticle = true;
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
							enemy_bullet2[i].particlepos.y -= BULLET_CHECK_RADIUS;
							enemy_bullet2[i].touchcheck = true;
							//enemy_bullet2[i].touchparticle = true;
						}

					}
				}
			}
			//地面以下の判定
			if (enemy_bullet2[i].pos.y <= (0.0f + BULLET_CHECK_RADIUS)) {
				enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
				enemy_bullet2[i].particlepos.y -= BULLET_CHECK_RADIUS;

				enemy_bullet2[i].touchcheck = true;
				//enemy_bullet2[i].touchparticle = true;
			}
		}

		//
		for (int i = 0; i < MAX_ENEMY_BOSS; i++)
		{
			if (boss_bullet[i].use == false) { break; }
			
			//平台TOUCHの判定
			for (int j = 1; j < MAX_MESH_FIELD; j++) {
				if (boss_bullet[i].pos.y <= (g_fieldpoint[j].pos0.y + BULLET_CHECK_RADIUS) && boss_bullet[i].pos.y >= (g_fieldpoint[j].pos0.y - BULLET_CHECK_RADIUS)) {
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = boss_bullet[i].pos;
					D3DXVECTOR3 end = boss_bullet[i].pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					start.y += 100.0f;
					end.y -= 1000.0f;
					if (boss_bullet[i].pos.x >= g_fieldpoint[j].pos0.x &&boss_bullet[i].pos.x <= g_fieldpoint[j].pos3.x
						&&boss_bullet[i].pos.z >= g_fieldpoint[j].pos3.z &&boss_bullet[i].pos.z <= g_fieldpoint[j].pos0.z) {
						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							boss_bullet[i].particlepos = boss_bullet[i].pos;
							boss_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;

							boss_bullet[i].touchcheck = true;
							//boss_bullet[i].touchparticle = true;
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							boss_bullet[i].particlepos = boss_bullet[i].pos;
							boss_bullet[i].particlepos.y -= BULLET_CHECK_RADIUS;
							boss_bullet[i].touchcheck = true;
							//boss_bullet[i].touchparticle = true;
						}

					}
				}
			}
		}
	}break;
	case 4://player TOUCH
	{
		for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
		{
			if (enemytypeA[i].use == false)continue;
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (enemytypeA[i].pos.x >= g_fieldslopepoint[j].pos0.x &&enemytypeA[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&enemytypeA[i].pos.z >= g_fieldslopepoint[j].pos3.z &&enemytypeA[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = enemytypeA[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - enemytypeA[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = enemytypeA[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (enemytypeA[i].pos.y >= (HitPos.y - 10.0) && enemytypeA[i].pos.y <= (HitPos.y + 1.0f))
					{
						D3DXVECTOR3 pos = enemytypeA[i].pos;
						
						enemytypeA[i].pos.y = HitPos.y + 1.0f;
						
					}
					//if (pos.y >= (HitPos.y - 2.0) && pos.y <= (HitPos.y + 2.0f))
					//{
					//	pos.y = HitPos.y + 2.0f;
					//	enemytypeA[i].downOK = false;
					//}
				}


			}


			//平台TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD; j++) {
				if (enemytypeA[i].pos.x >= g_fieldpoint[j].pos0.x &&enemytypeA[i].pos.x <= g_fieldpoint[j].pos3.x
					&&enemytypeA[i].pos.z >= g_fieldpoint[j].pos3.z &&enemytypeA[i].pos.z <= g_fieldpoint[j].pos0.z)
				{
					if (enemytypeA[i].pos.y >= (g_fieldpoint[j].pos0.y - 5.0f) && enemytypeA[i].pos.y <= (g_fieldpoint[j].pos0.y + 1.0f))
					{
						// レイキャストして足元の高さを求める
						D3DXVECTOR3 HitPosition;		//交点
						D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
						D3DXVECTOR3 start = enemytypeA[i].pos;
						D3DXVECTOR3 end = enemytypeA[i].pos;
						// 少し上から、ズドーンと下へレイを飛ばす
						start.y += 100.0f;
						end.y -= 1000.0f;

						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemytypeA[i].pos.y = HitPosition.y + 1.0f;
							
						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemytypeA[i].pos.y = HitPosition.y + 1.0f;
							
						}

					}
				}
			}

		}
		for (int i = 0; i < MAX_ENEMY_TYPED; i++)
		{
			if (enemytypeD[i].use == false)continue;
			//坂道TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++) {
				if (enemytypeD[i].pos.x >= g_fieldslopepoint[j].pos0.x &&enemytypeD[i].pos.x <= g_fieldslopepoint[j].pos3.x
					&&enemytypeD[i].pos.z >= g_fieldslopepoint[j].pos3.z &&enemytypeD[i].pos.z <= g_fieldslopepoint[j].pos0.z) {
					D3DXVECTOR3 HitPos;
					HitPos = enemytypeD[i].pos;
					float x = g_fieldslopepoint[j].pos3.x - enemytypeD[i].pos.x;
					float x_all = g_fieldslopepoint[j].pos3.x - g_fieldslopepoint[j].pos0.x;
					float z = enemytypeD[i].pos.z - g_fieldslopepoint[j].pos3.z;
					float z_all = g_fieldslopepoint[j].pos0.z - g_fieldslopepoint[j].pos3.z;
					float  y_all = 0.0f;

					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
					switch (g_fieldslopepoint[j].check)
					{
					case 1:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(x / x_all));
						break;
					case 2:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(x / x_all));
						break;
					case 3:
						y_all = g_fieldslopepoint[j].pos0.y - g_fieldslopepoint[j].pos3.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y + (y_all*(z / z_all));
						break;
					case 4:
						y_all = g_fieldslopepoint[j].pos3.y - g_fieldslopepoint[j].pos0.y;
						HitPos.y = g_fieldslopepoint[j].pos3.y - (y_all*(z / z_all));
						break;
					default:
						break;
					}
					if (enemytypeD[i].pos.y >= (HitPos.y - 10.0) && enemytypeD[i].pos.y <= (HitPos.y + 1.0f))
					{
						D3DXVECTOR3 pos = enemytypeD[i].pos;

						enemytypeD[i].pos.y = HitPos.y + 1.0f;

					}
					//if (pos.y >= (HitPos.y - 2.0) && pos.y <= (HitPos.y + 2.0f))
					//{
					//	pos.y = HitPos.y + 2.0f;
					//	enemytypeD[i].downOK = false;
					//}
				}


			}


			//平台TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD; j++) {
				if (enemytypeD[i].pos.x >= g_fieldpoint[j].pos0.x &&enemytypeD[i].pos.x <= g_fieldpoint[j].pos3.x
					&&enemytypeD[i].pos.z >= g_fieldpoint[j].pos3.z &&enemytypeD[i].pos.z <= g_fieldpoint[j].pos0.z)
				{
					if (enemytypeD[i].pos.y >= (g_fieldpoint[j].pos0.y - 5.0f) && enemytypeD[i].pos.y <= (g_fieldpoint[j].pos0.y + 1.0f))
					{
						// レイキャストして足元の高さを求める
						D3DXVECTOR3 HitPosition;		//交点
						D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
						D3DXVECTOR3 start = enemytypeD[i].pos;
						D3DXVECTOR3 end = enemytypeD[i].pos;
						// 少し上から、ズドーンと下へレイを飛ばす
						start.y += 100.0f;
						end.y -= 1000.0f;

						bool ans1 = RayCast(g_fieldpoint[j].pos0, g_fieldpoint[j].pos2, g_fieldpoint[j].pos1, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemytypeD[i].pos.y = HitPosition.y + 1.0f;

						}
						ans1 = RayCast(g_fieldpoint[j].pos1, g_fieldpoint[j].pos2, g_fieldpoint[j].pos3, start, end, &HitPosition, &Normal);
						if (ans1)
						{
							enemytypeD[i].pos.y = HitPosition.y + 1.0f;

						}

					}
				}
			}

		}
		break;
	}
	default:
		break;
	}
	
		
}

//=============================================================================
// 天井と当たり判定処理
// 
// 
//=============================================================================
void MeshceilingTouch( int check)
{
	switch (check)
	{
	case 1: {
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			//天井TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_CEILING; j++) {

				if (player[i].pos.x >= g_ceilingpoint[j].pos3.x &&player[i].pos.x <= g_ceilingpoint[j].pos0.x
					&&player[i].pos.z >= g_ceilingpoint[j].pos3.z &&player[i].pos.z <= g_ceilingpoint[j].pos0.z)
				{
					if (((player[i].pos.y + PLAYER_TALL) >= (g_ceilingpoint[j].pos0.y - 1.0f) && (player[i].pos.y + PLAYER_TALL) <= (g_ceilingpoint[j].pos0.y + 50.0f)))
					{
						player[i].pos.y = g_ceilingpoint[j].pos3.y - PLAYER_TALL - 1.0f;
						player[i].touchcheck_field = true;
					}
					else if (player[i].ropepos.y <= (g_ceilingpoint[j].pos0.y - 1.0f) && (player[i].pos.y + PLAYER_TALL) >= (g_ceilingpoint[j].pos0.y - 1.0f))
					{
						player[i].pos.y = g_ceilingpoint[j].pos3.y - PLAYER_TALL - 1.0f;
						player[i].touchcheck_field = true;
					}

				}
			}
			//天井以上の判定

			if ((player[i].pos.y + PLAYER_TALL) >= (640.0f - 1.0f))
			{

				player[i].pos.y = (640.0f - 1.0f - PLAYER_TALL);
				player[i].touchcheck_field = true;
			}
		}
		break;
	}
	case 2: {
		for (int i = 0; i < MAX_ROPE; i++)
		{

			if (rope[i].endpos_Decide == false)
			{
				//天井TOUCHの判定
				for (int j = 0; j < MAX_MESH_FIELD_CEILING; j++)
				{
					D3DXVECTOR3		nor, nor2;
					D3DXVECTOR3		vec1;
					D3DXVECTOR3		vec2;
					float			d1, d2;
					bool			check = false;
					int				check1 = 0;
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_ceilingpoint[j].pos1 - g_ceilingpoint[j].pos0;
						vec2 = g_ceilingpoint[j].pos2 - g_ceilingpoint[j].pos0;
						crossProduct(&nor, &vec2, &vec1);
						D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_ceilingpoint[j].pos2 - g_ceilingpoint[j].pos1;
						vec2 = g_ceilingpoint[j].pos3 - g_ceilingpoint[j].pos1;
						crossProduct(&nor2, &vec2, &vec1);
						D3DXVec3Normalize(&nor2, &nor2);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
					// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = rope[i].start_pos;
					D3DXVECTOR3 end = rope[i].end_pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
						vec1 = start - g_ceilingpoint[j].pos0;
						vec2 = end - g_ceilingpoint[j].pos0;
						d1 = dotProduct(&vec1, &nor);
						d2 = dotProduct(&vec2, &nor);
						if ((d1 * d2) > 0.0f)
						{
							// 当たっている可能性は無い
							check = false;
						}
						else if ((d1 * d2) <= 0.0f)
						{
							check = true;
							check1 = 1;
						}

						if (check == false) {
							// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
							vec1 = start - g_ceilingpoint[j].pos3;
							vec2 = end - g_ceilingpoint[j].pos3;
							d1 = dotProduct(&vec1, &nor2);
							d2 = dotProduct(&vec2, &nor2);
							if ((d1 * d2) > 0.0f)
							{
								// 当たっている可能性は無い
								check = false;
							}
							else if ((d1 * d2) <= 0.0f)
							{
								check = true;
							}
						}
						if (check == true)
						{
							if (check1 == 1) {
								vec1 = start - g_ceilingpoint[j].pos0;
								vec2 = end - g_ceilingpoint[j].pos0;
								d1 = dotProduct(&vec1, &nor);
								d2 = dotProduct(&vec2, &nor);
							}
							else if (check1 == 0) {
								vec1 = start - g_ceilingpoint[j].pos3;
								vec2 = end - g_ceilingpoint[j].pos3;
								d1 = dotProduct(&vec1, &nor2);
								d2 = dotProduct(&vec2, &nor2);
							}
							d1 = (float)fabs(d1);		//絶対値を求めている
							d2 = (float)fabs(d2);		//絶対値を求めている
							float a = d1 / (d1 + d2);							// 内分比
							D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
							D3DXVECTOR3		p3 = g_ceilingpoint[j].pos0 + vec3;						// 交点
							HitPosition = p3;											// 求めた交点を入れておく
						}
					}
					
					//check HitPositionはstartとendの中かどうか
					bool check2 = false;

					if (rope[i].end_pos.x <= rope[i].start_pos.x) {

						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}
					else if (rope[i].end_pos.x > rope[i].start_pos.x)
					{
						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}

					if (check2 == true)
					{

						if (HitPosition.x >= g_ceilingpoint[j].pos3.x &&HitPosition.x <= g_ceilingpoint[j].pos0.x
							&&HitPosition.z >= g_ceilingpoint[j].pos3.z &&HitPosition.z <= g_ceilingpoint[j].pos0.z
							&&HitPosition.y <= (g_ceilingpoint[j].pos0.y+1.0f) &&HitPosition.y >= (g_ceilingpoint[j].pos0.y-1.0f))
						{
							rope[i].end_pos = HitPosition;
							rope[i].end_pos.y = HitPosition.y-1.0f;
							rope[i].touch_check = true;
							rope[i].endpos_Decide = true;
						}

					}

					if (rope[i].touch_check == true) { break; }
				}
				
			}
		}
			
		break;
	}
	case 3: {
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_bullet[i].use == false) { break; }
			//天井TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_CEILING; j++) {

				if (g_bullet[i].pos.x >= g_ceilingpoint[j].pos3.x &&g_bullet[i].pos.x <= g_ceilingpoint[j].pos0.x
					&&g_bullet[i].pos.z >= g_ceilingpoint[j].pos3.z &&g_bullet[i].pos.z <= g_ceilingpoint[j].pos0.z)
				{
					if (((g_bullet[i].pos.y) >= (g_ceilingpoint[j].pos0.y - BULLET_CHECK_RADIUS) 
						&& (g_bullet[i].pos.y ) <= (g_ceilingpoint[j].pos0.y + BULLET_CHECK_RADIUS )))
					{
						g_bullet[i].particlepos = g_bullet[i].pos;
						g_bullet[i].particlepos.y += BULLET_CHECK_RADIUS;
						g_bullet[i].touchcheck = true;
						//g_bullet[i].touchparticle = true;
					}
					

				}
			}
			//天井以上の判定

			if ((g_bullet[i].pos.y ) >= (640.0f - BULLET_CHECK_RADIUS))
			{
				//g_bullet[i].touchparticle = true;
				g_bullet[i].particlepos = g_bullet[i].pos;
				g_bullet[i].particlepos.y += BULLET_CHECK_RADIUS;
				
				g_bullet[i].touchcheck = true;
			}
		}
		//enemyのbullet1
		for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
		{
			if (enemy_bullet1[i].use == false) { break; }
			//天井TOUCHの判定
			for (int j = 0; j < MAX_MESH_FIELD_CEILING; j++) {

				if (enemy_bullet1[i].pos.x >= g_ceilingpoint[j].pos3.x &&enemy_bullet1[i].pos.x <= g_ceilingpoint[j].pos0.x
					&&enemy_bullet1[i].pos.z >= g_ceilingpoint[j].pos3.z &&enemy_bullet1[i].pos.z <= g_ceilingpoint[j].pos0.z)
				{
					if (((enemy_bullet1[i].pos.y) >= (g_ceilingpoint[j].pos0.y - BULLET_CHECK_RADIUS)
						&& (enemy_bullet1[i].pos.y) <= (g_ceilingpoint[j].pos0.y + BULLET_CHECK_RADIUS)))
					{
						enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
						enemy_bullet1[i].particlepos.y += BULLET_CHECK_RADIUS;
						enemy_bullet1[i].touchcheck = true;
						//enemy_bullet1[i].touchparticle = true;
					}


				}
			}
			//天井以上の判定

			if ((enemy_bullet1[i].pos.y) >= (640.0f - BULLET_CHECK_RADIUS))
			{
				//enemy_bullet1[i].touchparticle = true;
				enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
				enemy_bullet1[i].particlepos.y += BULLET_CHECK_RADIUS;

				enemy_bullet1[i].touchcheck = true;
			}
		}
		break;
	}
	default:
		break;
	}
	
}
//=============================================================================
// 壁と当たり判定処理
// 
// 
//=============================================================================
void MeshwallTouch( int check) {
	
		switch (check)
		{
		case 1: {
			
			for (int i = 0; i < MAX_PLAYER; i++)
			{
				
					//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++)
				{
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)
					if (j == 152)
					{
						if (g_wall[152].use == false)break;
					}
					
					
					switch (g_wallpoint[j].check)
					{
					case 1:
						if (player[i].pos.z <= g_wallpoint[j].pos3.z&&player[i].pos.z >= g_wallpoint[j].pos0.z
							&& player[i].pos.y < g_wallpoint[j].pos0.y && (player[i].pos.y + PLAYER_TALL) >= g_wallpoint[j].pos3.y
							&&player[i].pos.x >= g_wallpoint[j].pos0.x)
						{
							if (player[i].wall_touch!=2) 
							{
								if ((player[i].pos.x - PLAYER_SIZE) <= (g_wallpoint[j].pos0.x + 1.0f))
								{
									player[i].pos.x = (g_wallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
									player[i].touchcheck = true;
								}
								else if ((player[i].ropepos.x - PLAYER_SIZE) >= (g_wallpoint[j].pos0.x + 1.0f)
									&& (player[i].pos.x - PLAYER_SIZE) <= (g_wallpoint[j].pos0.x + 1.0f))
								{
									player[i].pos.x = (g_wallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
									player[i].touchcheck = true;
								}
							}
							

						}
						break;
					case 2:
						if (player[i].pos.z <= g_wallpoint[j].pos0.z&&player[i].pos.z >= g_wallpoint[j].pos3.z
							&& player[i].pos.y < g_wallpoint[j].pos0.y && (player[i].pos.y + PLAYER_TALL) >= g_wallpoint[j].pos3.y
							&&player[i].pos.x <= g_wallpoint[j].pos0.x)
						{
							if (player[i].wall_touch != 1)
							{
								if ((player[i].pos.x + PLAYER_SIZE) >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									player[i].pos.x = (g_wallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
									player[i].touchcheck = true;
								}
								else if ((player[i].ropepos.x + PLAYER_SIZE) <= (g_wallpoint[j].pos0.x - 1.0f)
									&& (player[i].pos.x + PLAYER_SIZE) >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									player[i].pos.x = (g_wallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
									player[i].touchcheck = true;
								}
							}

						}
						break;
					case 3:
						if (player[i].pos.x <= g_wallpoint[j].pos3.x&&player[i].pos.x >= g_wallpoint[j].pos0.x
							&& player[i].pos.y < g_wallpoint[j].pos0.y && (player[i].pos.y + PLAYER_TALL) >= g_wallpoint[j].pos3.y
							&&player[i].pos.z <= g_wallpoint[j].pos0.z)
						{
							if (player[i].wall_touch != 3)
							{
								if ((player[i].pos.z + PLAYER_SIZE) >= (g_wallpoint[j].pos0.z - 1.0f))
								{
									player[i].pos.z = (g_wallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
									player[i].touchcheck = true;
								}
								else if ((player[i].ropepos.z + PLAYER_SIZE) <= (g_wallpoint[j].pos0.z - 1.0f)
									&& (player[i].pos.z + PLAYER_SIZE) >= (g_wallpoint[j].pos0.z - 1.0f))
								{
									player[i].pos.z = (g_wallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
									player[i].touchcheck = true;
								}
							}
						}
						break;
					case 4:
						if (player[i].pos.x <= g_wallpoint[j].pos0.x&&player[i].pos.x >= g_wallpoint[j].pos3.x
							&& player[i].pos.y < g_wallpoint[j].pos0.y && (player[i].pos.y + PLAYER_TALL) >= g_wallpoint[j].pos3.y
							&&player[i].pos.z >= g_wallpoint[j].pos0.z)
						{
							if (player[i].wall_touch != 4)
							{
								if ((player[i].pos.z - PLAYER_SIZE) <= (g_wallpoint[j].pos0.z + 1.0f))
								{
									player[i].pos.z = (g_wallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
									player[i].touchcheck = true;
								}
								else if ((player[i].ropepos.z - PLAYER_SIZE) >= (g_wallpoint[j].pos0.z + 1.0f)
									&& (player[i].pos.z - PLAYER_SIZE) <= (g_wallpoint[j].pos0.z + 1.0f))
								{
									player[i].pos.z = (g_wallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
									player[i].touchcheck = true;
								}
							}
						}
						break;

					default:
						break;
					}
				}
				
			}
		}
				break;
		case 2: {
			for (int i = 0; i < MAX_ROPE; i++)
			{
				if (rope[i].endpos_Decide == false) 
				{
					//壁TOUCHの判定
					for (int j = 0; j < MAX_MESH_WALL; j++)
					{
						D3DXVECTOR3		nor, nor2;
						D3DXVECTOR3		vec1;
						D3DXVECTOR3		vec2;
						float			d1, d2;
						bool			check = false;
						int				check1 = 0;
						{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
							vec1 = g_wallpoint[j].pos1 - g_wallpoint[j].pos0;
							vec2 = g_wallpoint[j].pos2 - g_wallpoint[j].pos0;
							crossProduct(&nor, &vec2, &vec1);
							D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

						}
						{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
							vec1 = g_wallpoint[j].pos2 - g_wallpoint[j].pos1;
							vec2 = g_wallpoint[j].pos3 - g_wallpoint[j].pos1;
							crossProduct(&nor2, &vec2, &vec1);
							D3DXVec3Normalize(&nor2, &nor2);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

						}
						// レイキャストして足元の高さを求める
						D3DXVECTOR3 HitPosition;		//交点
						D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
						D3DXVECTOR3 start = rope[i].start_pos;
						D3DXVECTOR3 end = rope[i].end_pos;
						// 少し上から、ズドーンと下へレイを飛ばす
							// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
							vec1 = start - g_wallpoint[j].pos0;
							vec2 = end - g_wallpoint[j].pos0;
							d1 = dotProduct(&vec1, &nor);
							d2 = dotProduct(&vec2, &nor);
							if ((d1 * d2) > 0.0f)
							{
								// 当たっている可能性は無い
								check = false;
							}
							else if ((d1 * d2) <= 0.0f)
							{
								check = true;
								check1 = 1;
							}

							if (check == false) {
								// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
								vec1 = start - g_wallpoint[j].pos3;
								vec2 = end - g_wallpoint[j].pos3;
								d1 = dotProduct(&vec1, &nor2);
								d2 = dotProduct(&vec2, &nor2);
								if ((d1 * d2) > 0.0f)
								{
									// 当たっている可能性は無い
									check = false;
								}
								else if ((d1 * d2) <= 0.0f)
								{
									check = true;
								}

							}
							if (check == true)
							{
								if (check1 == 1) {
									vec1 = start - g_wallpoint[j].pos0;
									vec2 = end - g_wallpoint[j].pos0;
									d1 = dotProduct(&vec1, &nor);
									d2 = dotProduct(&vec2, &nor);
								}
								else if (check1 == 0) {
									vec1 = start - g_wallpoint[j].pos3;
									vec2 = end - g_wallpoint[j].pos3;
									d1 = dotProduct(&vec1, &nor2);
									d2 = dotProduct(&vec2, &nor2);
								}
								d1 = (float)fabs(d1);		//絶対値を求めている
								d2 = (float)fabs(d2);		//絶対値を求めている
								float a = d1 / (d1 + d2);							// 内分比
								D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
								D3DXVECTOR3		p3 = g_wallpoint[j].pos0 + vec3;						// 交点
								HitPosition = p3;											// 求めた交点を入れておく
							}
						
						

						//check HitPositionはstartとendの中かどうか
						bool check2=false;
						
							if (rope[i].end_pos.x <= rope[i].start_pos.x) {

								if (rope[i].end_pos.z <= rope[i].start_pos.z )
								{
									if (rope[i].end_pos.y <= rope[i].start_pos.y)
									{
										if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
											&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
											&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
									else if (rope[i].end_pos.y > rope[i].start_pos.y) {
										if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
											&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
											&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
									
								}
								else if (rope[i].end_pos.z > rope[i].start_pos.z)
								{
									if (rope[i].end_pos.y <= rope[i].start_pos.y)
									{
										if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
											&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
											&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
									else if (rope[i].end_pos.y > rope[i].start_pos.y) {
										if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
											&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
											&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
								}
							}
							else if (rope[i].end_pos.x > rope[i].start_pos.x)
							{
								if (rope[i].end_pos.z <= rope[i].start_pos.z)
								{
									if (rope[i].end_pos.y <= rope[i].start_pos.y)
									{
										if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
											&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
											&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
									else if (rope[i].end_pos.y > rope[i].start_pos.y) {
										if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
											&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
											&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}

								}
								else if (rope[i].end_pos.z > rope[i].start_pos.z)
								{
									if (rope[i].end_pos.y <= rope[i].start_pos.y)
									{
										if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
											&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
											&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
									else if (rope[i].end_pos.y > rope[i].start_pos.y) {
										if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
											&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
											&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
										{
											check2 = true;
										}
									}
								}
							}
							
						
						if (check2 == true)
						{
							switch (g_wallpoint[j].check)
							{
							case 1:
								if (HitPosition.z <= g_wallpoint[j].pos3.z&&HitPosition.z >= g_wallpoint[j].pos0.z
									&& HitPosition.y <= g_wallpoint[j].pos0.y && (HitPosition.y ) >= g_wallpoint[j].pos3.y
									&&HitPosition.x <= (g_wallpoint[j].pos0.x+1.0f)&& HitPosition.x >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									rope[i].end_pos = HitPosition;
									rope[i].end_pos.x = HitPosition.x+1.0f;
									rope[i].endpos_Decide = true;
									rope[i].touch_check = true;
								}
								break;
							case 2:
								if (HitPosition.z <= g_wallpoint[j].pos0.z&&HitPosition.z >= g_wallpoint[j].pos3.z
									&& HitPosition.y <= g_wallpoint[j].pos0.y && (HitPosition.y ) >= g_wallpoint[j].pos3.y
									&&HitPosition.x <= (g_wallpoint[j].pos0.x + 1.0f) && HitPosition.x >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									rope[i].end_pos = HitPosition;
									rope[i].end_pos.x = HitPosition.x - 1.0f;
									rope[i].endpos_Decide = true;
									rope[i].touch_check = true;
								}
								break;
							case 3:
								if (HitPosition.x <= g_wallpoint[j].pos3.x&&HitPosition.x >= g_wallpoint[j].pos0.x
									&& HitPosition.y < g_wallpoint[j].pos0.y && (HitPosition.y ) >= g_wallpoint[j].pos3.y
									&&HitPosition.z <= (g_wallpoint[j].pos0.z+1.0) && HitPosition.z >= (g_wallpoint[j].pos0.z - 1.0))
								{
									rope[i].end_pos = HitPosition;
									rope[i].end_pos.z = HitPosition.z - 1.0f;
									rope[i].endpos_Decide = true;
									rope[i].touch_check = true;
								}
								break;
							case 4:
								if (HitPosition.x <= g_wallpoint[j].pos0.x&&HitPosition.x >= g_wallpoint[j].pos3.x
									&& HitPosition.y < g_wallpoint[j].pos0.y && (HitPosition.y ) >= g_wallpoint[j].pos3.y
									&&HitPosition.z <= (g_wallpoint[j].pos0.z + 1.0) && HitPosition.z >= (g_wallpoint[j].pos0.z - 1.0))
								{
									rope[i].end_pos = HitPosition;
									rope[i].end_pos.z = HitPosition.z + 1.0f;
									rope[i].endpos_Decide = true;
									rope[i].touch_check = true;
								}
								break;

							default:
								break;
							}
						}

						if (rope[i].touch_check == true) { break; }
					}
				}
				
			}
			break;
		}
		case 3: {
			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (g_bullet[i].use == false) { break; }
					//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++) {
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)

					switch (g_wallpoint[j].check)
					{
					case 1:
						if (g_bullet[i].pos.z <= g_wallpoint[j].pos3.z&&g_bullet[i].pos.z >= g_wallpoint[j].pos0.z
							&& g_bullet[i].pos.y < g_wallpoint[j].pos0.y && (g_bullet[i].pos.y ) >= g_wallpoint[j].pos3.y
							)
						{
							if ((g_bullet[i].pos.x ) <= (g_wallpoint[j].pos0.x )
								&& g_bullet[i].pos.x >= g_wallpoint[j].pos0.x- BULLET_CHECK_RADIUS)
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x -= BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}

						}
						break;
					case 2:
						if (g_bullet[i].pos.z <= g_wallpoint[j].pos0.z&&g_bullet[i].pos.z >= g_wallpoint[j].pos3.z
							&& g_bullet[i].pos.y < g_wallpoint[j].pos0.y && (g_bullet[i].pos.y ) >= g_wallpoint[j].pos3.y
							)
						{
							if ((g_bullet[i].pos.x ) >= (g_wallpoint[j].pos0.x ) 
								&& g_bullet[i].pos.x <= (g_wallpoint[j].pos0.x+ BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x += BULLET_CHECK_RADIUS;
								
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					case 3:
						if (g_bullet[i].pos.x <= g_wallpoint[j].pos3.x&&g_bullet[i].pos.x >= g_wallpoint[j].pos0.x
							&& g_bullet[i].pos.y < g_wallpoint[j].pos0.y && (g_bullet[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((g_bullet[i].pos.z ) >= (g_wallpoint[j].pos0.z )
								&& g_bullet[i].pos.z <= (g_wallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z += BULLET_CHECK_RADIUS;
								
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					case 4:
						if (g_bullet[i].pos.x <= g_wallpoint[j].pos0.x&&g_bullet[i].pos.x >= g_wallpoint[j].pos3.x
							&& g_bullet[i].pos.y < g_wallpoint[j].pos0.y && (g_bullet[i].pos.y ) >= g_wallpoint[j].pos3.y
							)
						{
							if ((g_bullet[i].pos.z) <= (g_wallpoint[j].pos0.z )
								&& g_bullet[i].pos.z >= (g_wallpoint[j].pos0.z-BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z -= BULLET_CHECK_RADIUS;
								
								g_bullet[i].touchcheck = true;
							}
						}
						break;

					default:
						break;
					}
				}
			}
			//enemyのbullet1
			for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
			{
				if (enemy_bullet1[i].use == false) { break; }
				//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++) {
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)

					switch (g_wallpoint[j].check)
					{
					case 1:
						if (enemy_bullet1[i].pos.z <= g_wallpoint[j].pos3.z&&enemy_bullet1[i].pos.z >= g_wallpoint[j].pos0.z
							&& enemy_bullet1[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet1[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet1[i].pos.x) <= (g_wallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& enemy_bullet1[i].pos.x >= g_wallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}

						}
						break;
					case 2:
						if (enemy_bullet1[i].pos.z <= g_wallpoint[j].pos0.z&&enemy_bullet1[i].pos.z >= g_wallpoint[j].pos3.z
							&& enemy_bullet1[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet1[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet1[i].pos.x) >= (g_wallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& enemy_bullet1[i].pos.x <= (g_wallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x += BULLET_CHECK_RADIUS;

								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					case 3:
						if (enemy_bullet1[i].pos.x <= g_wallpoint[j].pos3.x&&enemy_bullet1[i].pos.x >= g_wallpoint[j].pos0.x
							&& enemy_bullet1[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet1[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet1[i].pos.z) >= (g_wallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& enemy_bullet1[i].pos.z <= (g_wallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z += BULLET_CHECK_RADIUS;

								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					case 4:
						if (enemy_bullet1[i].pos.x <= g_wallpoint[j].pos0.x&&enemy_bullet1[i].pos.x >= g_wallpoint[j].pos3.x
							&& enemy_bullet1[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet1[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet1[i].pos.z) <= (g_wallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& enemy_bullet1[i].pos.z >= (g_wallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z -= BULLET_CHECK_RADIUS;

								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;

					default:
						break;
					}
				}
			}

			for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
			{
				if (enemy_bullet2[i].use == false) { break; }
				//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++) {
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)

					switch (g_wallpoint[j].check)
					{
					case 1:
						if (enemy_bullet2[i].pos.z <= g_wallpoint[j].pos3.z&&enemy_bullet2[i].pos.z >= g_wallpoint[j].pos0.z
							&& enemy_bullet2[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet2[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet2[i].pos.x) <= (g_wallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& enemy_bullet2[i].pos.x >= g_wallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}

						}
						break;
					case 2:
						if (enemy_bullet2[i].pos.z <= g_wallpoint[j].pos0.z&&enemy_bullet2[i].pos.z >= g_wallpoint[j].pos3.z
							&& enemy_bullet2[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet2[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet2[i].pos.x) >= (g_wallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& enemy_bullet2[i].pos.x <= (g_wallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x += BULLET_CHECK_RADIUS;

								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					case 3:
						if (enemy_bullet2[i].pos.x <= g_wallpoint[j].pos3.x&&enemy_bullet2[i].pos.x >= g_wallpoint[j].pos0.x
							&& enemy_bullet2[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet2[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet2[i].pos.z) >= (g_wallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& enemy_bullet2[i].pos.z <= (g_wallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z += BULLET_CHECK_RADIUS;

								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					case 4:
						if (enemy_bullet2[i].pos.x <= g_wallpoint[j].pos0.x&&enemy_bullet2[i].pos.x >= g_wallpoint[j].pos3.x
							&& enemy_bullet2[i].pos.y < g_wallpoint[j].pos0.y && (enemy_bullet2[i].pos.y) >= g_wallpoint[j].pos3.y
							)
						{
							if ((enemy_bullet2[i].pos.z) <= (g_wallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& enemy_bullet2[i].pos.z >= (g_wallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z -= BULLET_CHECK_RADIUS;

								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;

					default:
						break;
					}
				}
			}
		}break;
		case 4: {

			for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
			{

				//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++)
				{
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)

					if (j == 152)
					{

						if (g_wall[152].use == false)break;
					}

					switch (g_wallpoint[j].check)
					{
					case 1:
						if (enemytypeA[i].pos.z <= g_wallpoint[j].pos3.z&&enemytypeA[i].pos.z >= g_wallpoint[j].pos0.z
							&& enemytypeA[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeA[i].pos.y ) >= g_wallpoint[j].pos3.y
							&&enemytypeA[i].pos.x >= g_wallpoint[j].pos0.x)
						{
							
								if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.x + 1.0f))
								{
									enemytypeA[i].pos.x = (g_wallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
								}
								else if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.x + 1.0f)
									&& (enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.x + 1.0f))
								{
									enemytypeA[i].pos.x = (g_wallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
								}
							


						}
						break;
					case 2:
						if (enemytypeA[i].pos.z <= g_wallpoint[j].pos0.z&&enemytypeA[i].pos.z >= g_wallpoint[j].pos3.z
							&& enemytypeA[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeA[i].pos.y ) >= g_wallpoint[j].pos3.y
							&&enemytypeA[i].pos.x <= g_wallpoint[j].pos0.x)
						{
								if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									enemytypeA[i].pos.x = (g_wallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
								}
								else if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.x - 1.0f)
									&& (enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.x - 1.0f))
								{
									enemytypeA[i].pos.x = (g_wallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
								}
							

						}
						break;
					case 3:
						if (enemytypeA[i].pos.x <= g_wallpoint[j].pos3.x&&enemytypeA[i].pos.x >= g_wallpoint[j].pos0.x
							&& enemytypeA[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeA[i].pos.y ) >= g_wallpoint[j].pos3.y
							&&enemytypeA[i].pos.z <= g_wallpoint[j].pos0.z)
						{
							
								if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.z - 1.0f))
								{
									enemytypeA[i].pos.z = (g_wallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
								}
								else if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.z - 1.0f)
									&& (enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.z - 1.0f))
								{
									enemytypeA[i].pos.z = (g_wallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
								}
							
						}
						break;
					case 4:
						if (enemytypeA[i].pos.x <= g_wallpoint[j].pos0.x&&enemytypeA[i].pos.x >= g_wallpoint[j].pos3.x
							&& enemytypeA[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeA[i].pos.y ) >= g_wallpoint[j].pos3.y
							&&enemytypeA[i].pos.z >= g_wallpoint[j].pos0.z)
						{
								if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.z + 1.0f))
								{
									enemytypeA[i].pos.z = (g_wallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
								}
								else if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) >= (g_wallpoint[j].pos0.z + 1.0f)
									&& (enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_wallpoint[j].pos0.z + 1.0f))
								{
									enemytypeA[i].pos.z = (g_wallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
								}
							
						}
						break;

					default:
						break;
					}
				}

			}

			for (int i = 0; i < MAX_ENEMY_TYPED; i++)
			{

				//壁の判定
				for (int j = 0; j < MAX_MESH_WALL; j++)
				{
					//if check=1 ->右向けの壁(左の壁)
					//if check=2 ->左向けの壁(右の壁)
					//if check=3 ->下向けの壁(上の壁)
					//if check=4 ->上向けの壁(下の壁)

					if (g_wall[152].use == false)break;

					switch (g_wallpoint[j].check)
					{
					case 1:
						if (enemytypeD[i].pos.z <= g_wallpoint[j].pos3.z&&enemytypeD[i].pos.z >= g_wallpoint[j].pos0.z
							&& enemytypeD[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeD[i].pos.y) >= g_wallpoint[j].pos3.y
							&&enemytypeD[i].pos.x >= g_wallpoint[j].pos0.x)
						{

							if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.x + 1.0f))
							{
								enemytypeD[i].pos.x = (g_wallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.x + 1.0f)
								&& (enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.x + 1.0f))
							{
								enemytypeD[i].pos.x = (g_wallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}



						}
						break;
					case 2:
						if (enemytypeD[i].pos.z <= g_wallpoint[j].pos0.z&&enemytypeD[i].pos.z >= g_wallpoint[j].pos3.z
							&& enemytypeD[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeD[i].pos.y) >= g_wallpoint[j].pos3.y
							&&enemytypeD[i].pos.x <= g_wallpoint[j].pos0.x)
						{
							if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.x - 1.0f))
							{
								enemytypeD[i].pos.x = (g_wallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.x - 1.0f)
								&& (enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.x - 1.0f))
							{
								enemytypeD[i].pos.x = (g_wallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}


						}
						break;
					case 3:
						if (enemytypeD[i].pos.x <= g_wallpoint[j].pos3.x&&enemytypeD[i].pos.x >= g_wallpoint[j].pos0.x
							&& enemytypeD[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeD[i].pos.y) >= g_wallpoint[j].pos3.y
							&&enemytypeD[i].pos.z <= g_wallpoint[j].pos0.z)
						{

							if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.z - 1.0f))
							{
								enemytypeD[i].pos.z = (g_wallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.z - 1.0f)
								&& (enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.z - 1.0f))
							{
								enemytypeD[i].pos.z = (g_wallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}

						}
						break;
					case 4:
						if (enemytypeD[i].pos.x <= g_wallpoint[j].pos0.x&&enemytypeD[i].pos.x >= g_wallpoint[j].pos3.x
							&& enemytypeD[i].pos.y < g_wallpoint[j].pos0.y && (enemytypeD[i].pos.y) >= g_wallpoint[j].pos3.y
							&&enemytypeD[i].pos.z >= g_wallpoint[j].pos0.z)
						{
							if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.z + 1.0f))
							{
								enemytypeD[i].pos.z = (g_wallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) >= (g_wallpoint[j].pos0.z + 1.0f)
								&& (enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_wallpoint[j].pos0.z + 1.0f))
							{
								enemytypeD[i].pos.z = (g_wallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}

						}
						break;

					default:
						break;
					}
				}

			}
		}
				break;
		default:
			break;
		}
	
	
	
}
//=============================================================================
// 坂道の壁と当たり判定処理
// 
// 
//=============================================================================
void SlopewallTouch( int check)
{
	switch (check)
	{
	case 1: {
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (player[i].use == false)continue;
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((player[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (player[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	player[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.z - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (player[i].pos.z - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								player[i].pos.z = (g_slopewallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.z - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (player[i].pos.z - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((player[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (player[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	player[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.z + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (player[i].pos.z + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								player[i].pos.z = (g_slopewallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.z + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (player[i].pos.z + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								player[i].pos.z = (g_slopewallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - player[i].pos.x) / x);
						if (player[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	player[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.z + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (player[i].pos.z + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								player[i].pos.z = (g_slopewallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.z + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (player[i].pos.z + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								player[i].pos.z = (g_slopewallpoint[j].pos0.z - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}

						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - player[i].pos.x) / x);

						if (player[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	player[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.z - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (player[i].pos.z - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								player[i].pos.z = (g_slopewallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.z - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (player[i].pos.z - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								player[i].pos.z = (g_slopewallpoint[j].pos0.z + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((player[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (player[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	player[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.x + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (player[i].pos.x + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x + 20.0f))
							{

								player[i].pos.x = (g_slopewallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.x + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (player[i].pos.x + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((player[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (player[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	player[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.x - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (player[i].pos.x - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x + 20.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.x - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (player[i].pos.x - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - player[i].pos.z) / z);
						if (player[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	player[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.x - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (player[i].pos.x - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x -20.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.x - PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (player[i].pos.x - PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x + PLAYER_SIZE + 1.0f);
								player[i].touchcheck = true;
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - player[i].pos.z) / z);
						if (player[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	player[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&player[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&player[i].pos.y <= y_max)
						{
							if ((player[i].pos.x + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (player[i].pos.x + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x +20.0f))
							{

								player[i].pos.x = (g_slopewallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
							else if ((player[i].ropepos.x + PLAYER_SIZE) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (player[i].pos.x + PLAYER_SIZE) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								player[i].pos.x = (g_slopewallpoint[j].pos0.x - PLAYER_SIZE - 1.0f);
								player[i].touchcheck = true;
							}
						}
						break;
						
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}
		break;
	}
	case 2: {

		for (int i = 0; i < MAX_ROPE; i++)
		{
			if (rope[i].endpos_Decide == false)
			{
				float y_max = 0.0f;
				float y = 0.0f;
				float x = 0.0f;
				float z = 0.0f;
				//壁の判定
				for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
				{
					
					D3DXVECTOR3		nor, nor2;
					D3DXVECTOR3		vec1;
					D3DXVECTOR3		vec2;
					float			d1, d2;
					bool			check = false;
					int				check1 = 0;
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_slopewallpoint[j].pos1 - g_slopewallpoint[j].pos0;
						vec2 = g_slopewallpoint[j].pos2 - g_slopewallpoint[j].pos0;
						crossProduct(&nor, &vec2, &vec1);
						D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
					{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
						vec1 = g_slopewallpoint[j].pos2 - g_slopewallpoint[j].pos1;
						vec2 = g_slopewallpoint[j].pos3 - g_slopewallpoint[j].pos1;
						crossProduct(&nor2, &vec2, &vec1);
						D3DXVec3Normalize(&nor2, &nor2);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)

					}
						// レイキャストして足元の高さを求める
					D3DXVECTOR3 HitPosition;		//交点
					D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
					D3DXVECTOR3 start = rope[i].start_pos;
					D3DXVECTOR3 end = rope[i].end_pos;
					// 少し上から、ズドーンと下へレイを飛ばす
					{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
						vec1 = start - g_slopewallpoint[j].pos0;
						vec2 = end - g_slopewallpoint[j].pos0;
						d1 = dotProduct(&vec1, &nor);
						d2 = dotProduct(&vec2, &nor);
						if ((d1 * d2) > 0.0f)
						{
							// 当たっている可能性は無い
							check = false;
						}
						else if ((d1 * d2) <= 0.0f)
						{
							check = true;
							check1 = 1;
						}

						if (check == false) {
							// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
							vec1 = start - g_slopewallpoint[j].pos3;
							vec2 = end - g_slopewallpoint[j].pos3;
							d1 = dotProduct(&vec1, &nor2);
							d2 = dotProduct(&vec2, &nor2);
							if ((d1 * d2) > 0.0f)
							{
								// 当たっている可能性は無い
								check = false;
							}
							else if ((d1 * d2) <= 0.0f)
							{
								check = true;
							}
						}

						if (check == true)
						{
							if (check1 == 1) {
								vec1 = start - g_slopewallpoint[j].pos0;
								vec2 = end - g_slopewallpoint[j].pos0;
								d1 = dotProduct(&vec1, &nor);
								d2 = dotProduct(&vec2, &nor);
							}
							else if (check1 == 0) {
								vec1 = start - g_slopewallpoint[j].pos3;
								vec2 = end - g_slopewallpoint[j].pos3;
								d1 = dotProduct(&vec1, &nor2);
								d2 = dotProduct(&vec2, &nor2);
							}
							d1 = (float)fabs(d1);		//絶対値を求めている
							d2 = (float)fabs(d2);		//絶対値を求めている
							float a = d1 / (d1 + d2);							// 内分比
							D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
							D3DXVECTOR3		p3 = g_slopewallpoint[j].pos0 + vec3;						// 交点
							HitPosition = p3;											// 求めた交点を入れておく
						}
					}


					//check HitPositionはstartとendの中かどうか
					bool check2 = false;

					if (rope[i].end_pos.x <= rope[i].start_pos.x) {

						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x >= rope[i].end_pos.x&& HitPosition.x <= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}
					else if (rope[i].end_pos.x > rope[i].start_pos.x)
					{
						if (rope[i].end_pos.z <= rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z >= rope[i].end_pos.z&& HitPosition.z <= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}

						}
						else if (rope[i].end_pos.z > rope[i].start_pos.z)
						{
							if (rope[i].end_pos.y <= rope[i].start_pos.y)
							{
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y >= rope[i].end_pos.y&& HitPosition.y <= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
							else if (rope[i].end_pos.y > rope[i].start_pos.y) {
								if (HitPosition.x <= rope[i].end_pos.x&& HitPosition.x >= rope[i].start_pos.x
									&&HitPosition.y <= rope[i].end_pos.y&& HitPosition.y >= rope[i].start_pos.y
									&&HitPosition.z <= rope[i].end_pos.z&& HitPosition.z >= rope[i].start_pos.z)
								{
									check2 = true;
								}
							}
						}
					}

					if (check2 == true)
					{


						y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
						//壁の種類
						switch (g_slopewallpoint[j].check2)
						{
							//if check2=1 ->左下から右上の坂道
						case 1:
							x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
							switch (g_slopewallpoint[j].check1)
							{
								//if check1=1->下から上見え左の壁
							case 1:
								y_max = g_slopewallpoint[j].pos3.y + y * ((HitPosition.x - g_slopewallpoint[j].pos3.x) / x);
								if (HitPosition.x >= g_slopewallpoint[j].pos3.x	&&	HitPosition.x <= g_slopewallpoint[j].pos0.x
									&&HitPosition.y >= g_slopewallpoint[j].pos3.y	&&HitPosition.y <= y_max)
								{
									if ((HitPosition.z) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (HitPosition.z) >= (g_slopewallpoint[j].pos0.z - 1.0f))
									{

										HitPosition.z = (g_slopewallpoint[j].pos0.z + 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									}
								}
								break;
								//if check1=2->下から上見え右の壁
							case 2:
								y_max = g_slopewallpoint[j].pos0.y + y * ((HitPosition.x - g_slopewallpoint[j].pos0.x) / x);
								if (HitPosition.x >= g_slopewallpoint[j].pos0.x	&&	HitPosition.x <= g_slopewallpoint[j].pos3.x
									&&HitPosition.y >= g_slopewallpoint[j].pos0.y	&&HitPosition.y <= y_max)
								{
									if ((HitPosition.z) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (HitPosition.z) <= (g_slopewallpoint[j].pos0.z + 1.0f))
									{
										HitPosition.z = (g_slopewallpoint[j].pos0.z - 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									}

								}
								break;
							default:
								break;
							}
							break;
							//if check2=2 ->右下から左上の坂道
						case 2:
							x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
							switch (g_slopewallpoint[j].check1)
							{
								//if check1=1->下から上見え左の壁
							case 1:
								y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - HitPosition.x) / x);
								if (HitPosition.x >= g_slopewallpoint[j].pos0.x	&&	HitPosition.x <= g_slopewallpoint[j].pos3.x
									&&HitPosition.y >= g_slopewallpoint[j].pos3.y	&&HitPosition.y <= y_max
									&& (HitPosition.z) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (HitPosition.z) <= (g_slopewallpoint[j].pos0.z + 1.0f))
								{
										HitPosition.z = (g_slopewallpoint[j].pos0.z - 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;

								}
								break;
								//if check1=2->下から上見え右の壁
							case 2:
								y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - HitPosition.x) / x);

								if (HitPosition.x >= g_slopewallpoint[j].pos3.x	&&	HitPosition.x <= g_slopewallpoint[j].pos0.x
									&&HitPosition.y >= g_slopewallpoint[j].pos3.y	&&HitPosition.y <= y_max
									&& (HitPosition.z) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (HitPosition.z) >= (g_slopewallpoint[j].pos0.z - 1.0f))
								{
										HitPosition.z = (g_slopewallpoint[j].pos0.z + 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									
								}
								break;
							default:
								break;
							}
							break;
							//if check2=3 ->上上から下下の坂道
						case 3:
							z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
							switch (g_slopewallpoint[j].check1)
							{
								//if check1=1->下から上見え左の壁
							case 1:
								y_max = g_slopewallpoint[j].pos3.y + y * ((HitPosition.z - g_slopewallpoint[j].pos3.z) / z);
								if (HitPosition.z >= g_slopewallpoint[j].pos3.z	&&	HitPosition.z <= g_slopewallpoint[j].pos0.z
									&&HitPosition.y >= g_slopewallpoint[j].pos3.y	&&HitPosition.y <= y_max
									&& (HitPosition.x) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (HitPosition.x) <= (g_slopewallpoint[j].pos0.x + 1.0f))
								{
										HitPosition.x = (g_slopewallpoint[j].pos0.x - 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									
								}
								break;
								//if check1=2->下から上見え右の壁
							case 2:
								y_max = g_slopewallpoint[j].pos0.y + y * ((HitPosition.z - g_slopewallpoint[j].pos0.z) / z);
								if (HitPosition.z >= g_slopewallpoint[j].pos0.z	&&	HitPosition.z <= g_slopewallpoint[j].pos3.z
									&&HitPosition.y >= g_slopewallpoint[j].pos0.y	&&HitPosition.y <= y_max
									&& (HitPosition.x) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (HitPosition.x) >= (g_slopewallpoint[j].pos0.x + 1.0f))
								{
										HitPosition.x = (g_slopewallpoint[j].pos0.x + 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									

								}
								break;
							default:
								break;
							}
							break;
							//if check2=4 ->下上から上下の坂道
						case 4:
							z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
							switch (g_slopewallpoint[j].check1)
							{
								//if check1=1->下から上見え左の壁
							case 1:
								y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - HitPosition.z) / z);
								if (HitPosition.z >= g_slopewallpoint[j].pos3.z	&&	HitPosition.z <= g_slopewallpoint[j].pos0.z
									&&HitPosition.y >= g_slopewallpoint[j].pos0.y	&&HitPosition.y <= y_max
									&& (HitPosition.x) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (HitPosition.x) <= (g_slopewallpoint[j].pos0.x + 1.0f))
								{
										HitPosition.x = (g_slopewallpoint[j].pos0.x - 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									
								}
								break;
								//if check1=2->下から上見え右の壁
							case 2:
								y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - HitPosition.z) / z);
								if (HitPosition.z >= g_slopewallpoint[j].pos0.z	&&	HitPosition.z <= g_slopewallpoint[j].pos3.z
									&&HitPosition.y >= g_slopewallpoint[j].pos3.y	&&HitPosition.y <= y_max
									&& (HitPosition.x) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (HitPosition.x) >= (g_slopewallpoint[j].pos0.x + 1.0f))
								{
									HitPosition.x = (g_slopewallpoint[j].pos0.x + 1.0f);
										rope[i].end_pos = HitPosition;
										rope[i].endpos_Decide = true;
										rope[i].touch_check = true;
									

								}
								break;
							default:
								break;
							}
							break;
						default:
							break;
						}
					}
					if (rope[i].touch_check == true) { break; }
				}
			}
		}
		break;
	}
	case 3: {
		for (int i = 0; i < MAX_BULLET; i++)
		{
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			if (g_bullet[i].use == false) { break; }
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_bullet[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (g_bullet[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	g_bullet[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&g_bullet[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.z ) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.z ) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS ))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z -= BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_bullet[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (g_bullet[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	g_bullet[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&g_bullet[i].pos.y  >= g_slopewallpoint[j].pos0.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.z ) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.z ) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS ))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z += BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - g_bullet[i].pos.x) / x);
						if (g_bullet[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	g_bullet[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&g_bullet[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.z ) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.z ) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS ))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z += BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - g_bullet[i].pos.x) / x);

						if (g_bullet[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	g_bullet[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&g_bullet[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.z ) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& (g_bullet[i].pos.z ) >= (g_slopewallpoint[j].pos0.z- BULLET_CHECK_RADIUS ))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.z -= BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_bullet[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (g_bullet[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	g_bullet[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&g_bullet[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.x ) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.x ) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x += BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_bullet[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (g_bullet[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	g_bullet[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&g_bullet[i].pos.y >= g_slopewallpoint[j].pos0.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.x ) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.x ) >= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS ))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x -= BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - g_bullet[i].pos.z) / z);
						if (g_bullet[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	g_bullet[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&g_bullet[i].pos.y >= g_slopewallpoint[j].pos0.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.x ) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.x ) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x += BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - g_bullet[i].pos.z) / z);
						if (g_bullet[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	g_bullet[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&g_bullet[i].pos.y >= g_slopewallpoint[j].pos3.y	&&g_bullet[i].pos.y <= y_max)
						{
							if ((g_bullet[i].pos.x ) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS) 
								&& (g_bullet[i].pos.x ) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS))
							{
								//g_bullet[i].touchparticle = true;
								g_bullet[i].particlepos = g_bullet[i].pos;
								g_bullet[i].particlepos.x -= BULLET_CHECK_RADIUS;
								g_bullet[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}
		//enemyのbullet1
		for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
		{
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			if (enemy_bullet1[i].use == false) { break; }
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemy_bullet1[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (enemy_bullet1[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemy_bullet1[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z -= BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemy_bullet1[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (enemy_bullet1[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemy_bullet1[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z += BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - enemy_bullet1[i].pos.x) / x);
						if (enemy_bullet1[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemy_bullet1[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z += BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - enemy_bullet1[i].pos.x) / x);

						if (enemy_bullet1[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemy_bullet1[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.z -= BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemy_bullet1[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (enemy_bullet1[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemy_bullet1[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x += BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemy_bullet1[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (enemy_bullet1[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemy_bullet1[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.x) >= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - enemy_bullet1[i].pos.z) / z);
						if (enemy_bullet1[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemy_bullet1[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x += BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - enemy_bullet1[i].pos.z) / z);
						if (enemy_bullet1[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemy_bullet1[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemy_bullet1[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet1[i].pos.y <= y_max)
						{
							if ((enemy_bullet1[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& (enemy_bullet1[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet1[i].touchparticle = true;
								enemy_bullet1[i].particlepos = enemy_bullet1[i].pos;
								enemy_bullet1[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet1[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}

		for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
		{
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			if (enemy_bullet2[i].use == false) { break; }
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemy_bullet2[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (enemy_bullet2[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemy_bullet2[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z -= BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemy_bullet2[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (enemy_bullet2[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemy_bullet2[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z += BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - enemy_bullet2[i].pos.x) / x);
						if (enemy_bullet2[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemy_bullet2[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z += BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - enemy_bullet2[i].pos.x) / x);

						if (enemy_bullet2[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemy_bullet2[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.z) <= (g_slopewallpoint[j].pos0.z + BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.z) >= (g_slopewallpoint[j].pos0.z - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.z -= BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemy_bullet2[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (enemy_bullet2[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemy_bullet2[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x += BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemy_bullet2[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (enemy_bullet2[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemy_bullet2[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.x) >= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - enemy_bullet2[i].pos.z) / z);
						if (enemy_bullet2[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemy_bullet2[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x += BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - enemy_bullet2[i].pos.z) / z);
						if (enemy_bullet2[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemy_bullet2[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemy_bullet2[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemy_bullet2[i].pos.y <= y_max)
						{
							if ((enemy_bullet2[i].pos.x) <= (g_slopewallpoint[j].pos0.x + BULLET_CHECK_RADIUS)
								&& (enemy_bullet2[i].pos.x) >= (g_slopewallpoint[j].pos0.x - BULLET_CHECK_RADIUS))
							{
								//enemy_bullet2[i].touchparticle = true;
								enemy_bullet2[i].particlepos = enemy_bullet2[i].pos;
								enemy_bullet2[i].particlepos.x -= BULLET_CHECK_RADIUS;
								enemy_bullet2[i].touchcheck = true;
							}
						}
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}

	}break;
	case 4: {
		for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
		{
			if (enemytypeA[i].use == false)continue;
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemytypeA[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (enemytypeA[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemytypeA[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemytypeA[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
							}
							else if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemytypeA[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (enemytypeA[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemytypeA[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemytypeA[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
							}
							else if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - enemytypeA[i].pos.x) / x);
						if (enemytypeA[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemytypeA[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemytypeA[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
							}
							else if ((enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (enemytypeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPEA_POSA_X - 1.0f);
							}

						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - enemytypeA[i].pos.x) / x);

						if (enemytypeA[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemytypeA[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemytypeA[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
							}
							else if ((enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (enemytypeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								enemytypeA[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPEA_POSA_X + 1.0f);
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemytypeA[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (enemytypeA[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemytypeA[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemytypeA[i].pos.y  >= g_slopewallpoint[j].pos3.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 20.0f))
							{

								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
							}
							else if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemytypeA[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (enemytypeA[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemytypeA[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemytypeA[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x + 20.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
							}
							else if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - enemytypeA[i].pos.z) / z);
						if (enemytypeA[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemytypeA[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemytypeA[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x - 20.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
							}
							else if ((enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (enemytypeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPEA_POSA_X + 1.0f);
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - enemytypeA[i].pos.z) / z);
						if (enemytypeA[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemytypeA[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemytypeA[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&enemytypeA[i].pos.y <= y_max)
						{
							if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x + 20.0f))
							{

								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
							}
							else if ((enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (enemytypeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								enemytypeA[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPEA_POSA_X - 1.0f);
							}
						}
						break;

					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}

		for (int i = 0; i < MAX_ENEMY_TYPED; i++)
		{
			if (enemytypeD[i].use == false)continue;
			float y_max = 0.0f;
			float y = 0.0f;
			float x = 0.0f;
			float z = 0.0f;
			//壁の判定
			for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
			{
				y = (float)fabs(g_slopewallpoint[j].pos1.y - g_slopewallpoint[j].pos0.y);
				//壁の種類
				switch (g_slopewallpoint[j].check2)
				{
					//if check2=1 ->左下から右上の坂道
				case 1:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemytypeD[i].pos.x - g_slopewallpoint[j].pos3.x) / x);
						if (enemytypeD[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemytypeD[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemytypeD[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemytypeD[i].pos.x - g_slopewallpoint[j].pos0.x) / x);
						if (enemytypeD[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemytypeD[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemytypeD[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=2 ->右下から左上の坂道
				case 2:
					x = (float)fabs(g_slopewallpoint[j].pos3.x - g_slopewallpoint[j].pos0.x);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.x - enemytypeD[i].pos.x) / x);
						if (enemytypeD[i].pos.x >= g_slopewallpoint[j].pos0.x	&&	enemytypeD[i].pos.x <= g_slopewallpoint[j].pos3.x
							&&enemytypeD[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f) && (enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 20.0f))
							{
								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z - 1.0f)
								&& (enemytypeD[i].pos.z + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 1.0f))
							{
								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z - ENEMY_TYPED_POSA_X - 1.0f);
							}

						}
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.x - enemytypeD[i].pos.x) / x);

						if (enemytypeD[i].pos.x >= g_slopewallpoint[j].pos3.x	&&	enemytypeD[i].pos.x <= g_slopewallpoint[j].pos0.x
							&&enemytypeD[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f) && (enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z - 20.0f))
							{

								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.z + 1.0f)
								&& (enemytypeD[i].pos.z - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.z + 1.0f))
							{
								enemytypeD[i].pos.z = (g_slopewallpoint[j].pos0.z + ENEMY_TYPED_POSA_X + 1.0f);
							}
						}
						break;
					default:
						break;
					}
					break;
					//if check2=3 ->上上から下下の坂道
				case 3:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((enemytypeD[i].pos.z - g_slopewallpoint[j].pos3.z) / z);
						if (enemytypeD[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemytypeD[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemytypeD[i].pos.y >= g_slopewallpoint[j].pos3.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 20.0f))
							{

								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}
						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((enemytypeD[i].pos.z - g_slopewallpoint[j].pos0.z) / z);
						if (enemytypeD[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemytypeD[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemytypeD[i].pos.y >= g_slopewallpoint[j].pos0.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x + 20.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}

						}
						break;
					default:
						break;
					}
					break;
					//if check2=4 ->下上から上下の坂道
				case 4:
					z = (float)fabs(g_slopewallpoint[j].pos3.z - g_slopewallpoint[j].pos0.z);
					switch (g_slopewallpoint[j].check1)
					{
						//if check1=1->下から上見え左の壁
					case 1:
						y_max = g_slopewallpoint[j].pos3.y + y * ((g_slopewallpoint[j].pos3.z - enemytypeD[i].pos.z) / z);
						if (enemytypeD[i].pos.z >= g_slopewallpoint[j].pos0.z	&&	enemytypeD[i].pos.z <= g_slopewallpoint[j].pos3.z
							&&enemytypeD[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos3.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f) && (enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x - 20.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}
							else if ((enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x + 1.0f)
								&& (enemytypeD[i].pos.x - ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 1.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x + ENEMY_TYPED_POSA_X + 1.0f);
							}

						}
						break;
						//if check1=2->下から上見え右の壁
					case 2:
						y_max = g_slopewallpoint[j].pos0.y + y * ((g_slopewallpoint[j].pos0.z - enemytypeD[i].pos.z) / z);
						if (enemytypeD[i].pos.z >= g_slopewallpoint[j].pos3.z	&&	enemytypeD[i].pos.z <= g_slopewallpoint[j].pos0.z
							&&enemytypeD[i].pos.y + PLAYER_TALL >= g_slopewallpoint[j].pos0.y	&&enemytypeD[i].pos.y <= y_max)
						{
							if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f) && (enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x + 20.0f))
							{

								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}
							else if ((enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) <= (g_slopewallpoint[j].pos0.x - 1.0f)
								&& (enemytypeD[i].pos.x + ENEMY_TYPED_POSA_X) >= (g_slopewallpoint[j].pos0.x - 1.0f))
							{
								enemytypeD[i].pos.x = (g_slopewallpoint[j].pos0.x - ENEMY_TYPED_POSA_X - 1.0f);
							}
						}
						break;

					default:
						break;
					}
					break;
				default:
					break;
				}
			}

		}
		break;
	}
	default:
		break;
	}
	
}


//=============================================================================
// カメラと壁、地面と当たり判定処理
// 
// 
//=============================================================================
bool CameraRayTouch_Field(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal,int *meshnumber) 
{
	
		D3DXVECTOR3 start = camerapos;
		D3DXVECTOR3 end = playerpos;
		end.y += PLAYER_TALL;
		D3DXVECTOR3 p0 = g_fieldpoint[*meshnumber].pos0;
		D3DXVECTOR3 p1 = g_fieldpoint[*meshnumber].pos1;
		D3DXVECTOR3 p2 = g_fieldpoint[*meshnumber].pos2;
		D3DXVECTOR3 p3 = g_fieldpoint[*meshnumber].pos3;
		bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
		if (ans)
		{
			
			return true;
		}
		ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
		if (ans)
		{
			
			return true;
		}
	
	return false;
}
bool CameraRayTouch_Wall(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber)
{
	
		D3DXVECTOR3 start = camerapos;
		D3DXVECTOR3 end = playerpos;
		end.y += PLAYER_TALL;
		D3DXVECTOR3 p0 = g_wallpoint[*meshnumber].pos0;
		D3DXVECTOR3 p1 = g_wallpoint[*meshnumber].pos1;
		D3DXVECTOR3 p2 = g_wallpoint[*meshnumber].pos2;
		D3DXVECTOR3 p3 = g_wallpoint[*meshnumber].pos3;
		bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
		if (ans)
		{
			return true;
		}
		ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
		if (ans)
		{
			return true;
		}
	
	return false;
	
}
bool CameraRayTouch_Ceiling(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber)
{

	
		D3DXVECTOR3 start = camerapos;
		D3DXVECTOR3 end = playerpos;
		end.y += PLAYER_TALL;
		D3DXVECTOR3 p0 = g_ceilingpoint[ *meshnumber].pos0;
		D3DXVECTOR3 p1 = g_ceilingpoint[ *meshnumber].pos1;
		D3DXVECTOR3 p2 = g_ceilingpoint[ *meshnumber].pos2;
		D3DXVECTOR3 p3 = g_ceilingpoint[ *meshnumber].pos3;
		bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
		if (ans)
		{
			
			return true;
		}
		ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
		if (ans)
		{
			
			return true;
		}
	
	return false;

	
}
bool CameraRayTouch_Slope(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber)
{
	
		D3DXVECTOR3 start = playerpos;
		D3DXVECTOR3 end = camerapos;
		start.y += 3.0f;
		D3DXVECTOR3 p0 = g_fieldslopepoint[*meshnumber].pos0;
		D3DXVECTOR3 p1 = g_fieldslopepoint[*meshnumber].pos1;
		D3DXVECTOR3 p2 = g_fieldslopepoint[*meshnumber].pos2;
		D3DXVECTOR3 p3 = g_fieldslopepoint[*meshnumber].pos3;
		bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
		if (ans)
		{
			return true;
		}
		ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
		if (ans)
		{
			return true;
		}
	
	return false;
	
}
bool CameraRayTouch_SlopeWall(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber)
{
	
		D3DXVECTOR3 start = camerapos;
		D3DXVECTOR3 end = playerpos;
		D3DXVECTOR3 p0 = g_slopewallpoint[ *meshnumber].pos0;
		D3DXVECTOR3 p1 = g_slopewallpoint[ *meshnumber].pos1;
		D3DXVECTOR3 p2 = g_slopewallpoint[ *meshnumber].pos2;
		D3DXVECTOR3 p3 = g_slopewallpoint[ *meshnumber].pos3;
		if (g_slopewallpoint[*meshnumber].check1 == 1)
		{
			bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
			if (ans)
			{
				return true;
			}
		}
		if (g_slopewallpoint[*meshnumber].check1 == 2)
		{
			bool ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
			if (ans)
			{
				return true;
			}
		}
	
	return false;
	
}