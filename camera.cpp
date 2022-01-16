//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "GAME_battlearea.h"
#include <time.h>
#include <Windows.h>
#include	"meshMap.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
#define	POS_Y_CAM		(150.0f)			// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-550.0f)		// カメラの初期位置(Z座標)

#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量

#define VALUE_MOVE_CAMERA_GAMEPDA_X (2.0f)
#define VALUE_MOVE_CAMERA_GAMEPDA_Y (2.0f)

//title用
#define whirling_part		(20)
#define whirling_part_ALL		(whirling_part*2)
#define cam_radius			(3000.0f)
#define cam_Ypos			(500.0f)
#define cam_movespd			(0.1f)
//*****************************************************************************
// グローバル変数
//*****************************************************************************

static CAMERA			g_Camera;					
static PLAYER			*player=GetPlayer();
static CAMERA			*camera = GetCamera();
static GAMEDATA			*gamedata = GetData();
static MESH_MAP			*g_wall = Getwall();

static float x = 0.0f;								//マオスの移動量用
static float y = 0.0f;								//マオスの移動量用
static int buffertime = 0;							//もし最上階のプログラムはゲームじゃないの時、PAUSEを実行のBUFFERTIME
static int	g_ViewPortType = TYPE_FULL_SCREEN;

static float rot = 0.0f;							//プレイヤー前の透明化画像の計算用
static float rott = -D3DX_PI;						//プレイヤー前の透明化画像の計算用
static D3DXVECTOR3 campos[whirling_part_ALL];		//プレイヤー前の透明化画像のPOS

//カメラの注視点の線形移動用の移動座標テーブル
static D3DXVECTOR3 g_MoveTbl_eye[whirling_part_ALL];

//カメラの注視点の線形移動用の移動スピードテーブル
static float	g_MoveSpd_eye[whirling_part_ALL];

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, (camera_Ypos_center+ PLAYER_FIRST_POS_Y), POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, PLAYER_TALL, 0.0f);
	g_Camera.playerpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.updown_check = false;		//false:視線は下
	g_Camera.shoutchange = true;

	g_Camera.left_pos = g_Camera.pos;
	g_Camera.right_pos = g_Camera.pos;
	g_Camera.top_pos = g_Camera.pos;
	g_Camera.down_pos = g_Camera.pos;

	g_Camera.buying_pos = g_Camera.pos;
	g_Camera.buying_move = g_Camera.pos;
	g_Camera.buying_at_move = g_Camera.pos;

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);

	//カメラの注視点用の時間をクリア
	g_Camera.time_at = 0.0f;

	//カメラの注視点用の時間をクリア
	g_Camera.time_eye = 0.0f;

	{
		float a = (SCREEN_HEIGHT / 2)/(tanf(VIEW_ANGLE/2));
		float b = (float)sqrt((SCREEN_HEIGHT / 2) * (SCREEN_HEIGHT / 2) + (SCREEN_WIDTH / 2)*(SCREEN_WIDTH / 2));
		float xxzz = (float)atan(b/a);
		g_Camera.xyrot_MAX = xxzz;
	}

	for (int i = 0; i < whirling_part; i++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, cam_Ypos, -5500.0f);
		rot += D3DX_PI / (whirling_part);
		
		pos.x -= sinf(rot)*cam_radius;
		pos.z -= cosf(rot)*cam_radius;

		campos[i] = pos;
		
	}
	for (int i = 0; i < whirling_part; i++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, cam_Ypos, -5500.0f);
		rott += D3DX_PI / (whirling_part);
		
		pos.x -= sinf(rott)*cam_radius;
		pos.z -= cosf(rott)*cam_radius;

		campos[i+ whirling_part] = pos;
	}
	for (int i = 0; i < whirling_part_ALL; i++)
	{
		g_MoveTbl_eye[i] = campos[i];
		g_MoveSpd_eye[i]= cam_movespd;
	}
	
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	switch (gamedata->modenow)
	{
	case MODE_TITLE:
		{
			{
				int nowNo = (int)g_Camera.time_eye;			
				int maxNo = (sizeof(g_MoveTbl_eye) / sizeof(D3DXVECTOR3)); 
				int nextNo = (nowNo + 1) % maxNo;			
				D3DXVECTOR3	pos = g_MoveTbl_eye[nextNo] - g_MoveTbl_eye[nowNo];
				float nowTime = g_Camera.time_eye - nowNo;	
				pos *= nowTime;									// 現在の移動量を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				g_Camera.pos = g_MoveTbl_eye[nowNo] + pos;

				g_Camera.time_eye += g_MoveSpd_eye[nowNo];		// 時間を進めている
				if ((int)g_Camera.time_eye >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Camera.time_eye -= maxNo;				
				}
			}
		g_Camera.at = D3DXVECTOR3(0.0f, 750.0f, -5500.0f);
		}
		break;
	case MODE_RESTAREA:
	{

		if (gamedata->buying == true)
		{
			if (gamedata->buying_cameramove == 0)
			{
				if (g_Camera.pos == D3DXVECTOR3(-500.0f, 50.0f, -7800.0f))
				{
					gamedata->buying_cameramove = 1;
				}
				g_Camera.pos -= g_Camera.buying_move / 40;
				g_Camera.at -= g_Camera.buying_at_move / 40;


				{
					if (g_Camera.pos.x >= -501.0f&&g_Camera.pos.x <= -499.0f)
					{
						g_Camera.pos.x = -500.0f;
					}
					if (g_Camera.pos.z >= -7801.0f&&g_Camera.pos.z <= -7799.0f)
					{
						g_Camera.pos.z = -7800.0f;
					}
					if (g_Camera.pos.y >= 49.0f&&g_Camera.pos.y <= 51.0f)
					{
						g_Camera.pos.y = 50.0f;
					}
				}
				{
					if (g_Camera.at.x >= -601.0f&&g_Camera.at.x <= -599.0f)
					{
						g_Camera.at.x = -600.0f;
					}
					if (g_Camera.at.z >= -7801.0f&&g_Camera.at.z <= -7799.0f)
					{
						g_Camera.at.z = -7800.0f;
					}
					if (g_Camera.at.y >= 49.0f&&g_Camera.at.y <= 51.0f)
					{
						g_Camera.at.y = 50.0f;
					}
				}
			}
			else if (gamedata->buying_cameramove == 2)
			{
				if (g_Camera.pos == g_Camera.buying_pos)
				{
					gamedata->buying_cameramove = 0;
					gamedata->buying = false;
					gamedata->start = clock();
				}
				g_Camera.pos += g_Camera.buying_move / 40;
				g_Camera.at += g_Camera.buying_at_move / 40;


				{
					if (g_Camera.pos.x >= g_Camera.buying_pos.x-1.0f&&g_Camera.pos.x <= g_Camera.buying_pos.x + 1.0f)
					{
						g_Camera.pos.x = g_Camera.buying_pos.x;
					}
					if (g_Camera.pos.z >= g_Camera.buying_pos.z - 1.0f&&g_Camera.pos.z <= g_Camera.buying_pos.z + 1.0f)
					{
						g_Camera.pos.z = g_Camera.buying_pos.z;
					}
					if (g_Camera.pos.y >= g_Camera.buying_pos.y - 1.0f&&g_Camera.pos.y <= g_Camera.buying_pos.y + 1.0f)
					{
						g_Camera.pos.y = g_Camera.buying_pos.y;
					}
				}
				
			}
			


			return;
		}

		if (player[0].use == true)
		{
			HWND titlecheck = GetActiveWindow();		//このプログラムのTITLE
			HWND OK = GetForegroundWindow();			//最上のプログラムのTITLE
			if (titlecheck == OK)
			{
				POINT point, pointwindow;
				GetCursorPos(&point);			//今モニター上、mouseの座標
				RECT rect;
				GetWindowRect(titlecheck, &rect);
				pointwindow.x = rect.left + (rect.right - rect.left) / 2;
				pointwindow.y = rect.top + (rect.bottom - rect.top) / 2;
				x = float(point.x - pointwindow.x);
				y = float(point.y - pointwindow.y);
				SetCursorPos(pointwindow.x, pointwindow.y);			//mouseの座標reset
				buffertime = 0;
			}
			else {

				buffertime++;
				if (buffertime >= 2) {
					gamedata->pausecheck = true;
				}
			}

			if (g_Camera.shoutchange == true)
			{
				player[0].camera_pos.x = player[0].pos.x - (CAMERA_DISTANCE_MIN*(sinf(g_Camera.rot.y)));
				player[0].camera_pos.z = player[0].pos.z - (CAMERA_DISTANCE_MIN*(cosf(g_Camera.rot.y)));
				player[0].camera_pos2.x = player[0].pos.x - (CAMERA_DISTANCE_MAX*(sinf(g_Camera.rot.y)));
				player[0].camera_pos2.z = player[0].pos.z - (CAMERA_DISTANCE_MAX*(cosf(g_Camera.rot.y)));

				// カメラの注視点と主人公に追従させる
				g_Camera.at.x = player[0].pos.x + (CAMERA_AT_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.at.z = player[0].pos.z + (CAMERA_AT_DISTANCE*(cosf(g_Camera.rot.y)));

				//パソコンmouse
				{

					{
						float y_move = y / 10;
						float xx = player[0].camera_pos.x - player[0].camera_pos2.x;
						float zz = player[0].camera_pos.z - player[0].camera_pos2.z;
						if (g_Camera.pos.y <= (camera_Ypos_center + player[0].pos.y)) {

							g_Camera.pos.y -= y_move;
							if (g_Camera.pos.y <= (player[0].pos.y + camera_start_Ypos + 1.0f))
							{
								g_Camera.pos.y = (player[0].pos.y + camera_start_Ypos + 1.0f);
							}
							float y_move2 = (camera_Ypos_center + player[0].pos.y) - g_Camera.pos.y;
							float y_proportion = ((sqrtf((camera_radius* camera_radius) - (y_move2*y_move2))) / (camera_radius));
							g_Camera.at.y = player[0].pos.y + camera__at_Ypos_center + camera_at_radius * (1 - y_proportion);
							float x_pos = player[0].camera_pos.x - xx * y_proportion;
							float z_pos = player[0].camera_pos.z - zz * y_proportion;
							float x_move = (xx*xx) - (y_move2*y_move2);
							float z_move = (zz*zz) - (y_move2*y_move2);
							float vx, vz;
							vx = x_pos - g_Camera.at.x;
							vz = z_pos - g_Camera.at.z;
							g_Camera.len = sqrtf(vx * vx + vz * vz);

						}
						else if (g_Camera.pos.y > (camera_Ypos_center + player[0].pos.y)) {
							g_Camera.pos.y -= y_move;
							if (g_Camera.pos.y >= (player[0].pos.y + camera_Ypos_center + camera_radius - 1.0f))
							{
								g_Camera.pos.y = (player[0].pos.y + camera_Ypos_center + camera_radius - 1.0f);
							}
							float y_move2 = g_Camera.pos.y - (camera_Ypos_center + player[0].pos.y);
							float y_proportion = ((sqrtf((camera_radius* camera_radius) - (y_move2*y_move2))) / (camera_radius));
							g_Camera.at.y = player[0].pos.y + camera__at_Ypos_center - camera_at_radius * (1 - y_proportion);
							float x_pos = player[0].camera_pos.x - xx * y_proportion;
							float z_pos = player[0].camera_pos.z - zz * y_proportion;
							float vx, vz;
							vx = x_pos - g_Camera.at.x;
							vz = z_pos - g_Camera.at.z;
							g_Camera.len = sqrtf(vx * vx + vz * vz);
						}
					}
				}

				{
					//パソコンMOUSE
					{
						g_Camera.rot.y += (D3DX_PI*(x / 2000));
						g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
						g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
						
						
					}
					//ゲームパッド
					/*
					{
						g_Camera.rot.y += (D3DX_PI*(x));
						g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
						g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
					}*/

				}

			}
			else
			{//第一人視点mode
				float y_move = y / 5;
				// カメラの注視点と主人公に追従させる
				g_Camera.at.x = player[0].pos.x + (CAMERA_AT_ONEPOS_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.at.z = player[0].pos.z + (CAMERA_AT_ONEPOS_DISTANCE*(cosf(g_Camera.rot.y)));
				g_Camera.at.y -= y_move;
				//パソコンmouse
				g_Camera.rot.y += (D3DX_PI*(x / 2000));
				g_Camera.pos.x = player[0].pos.x + (CAMERA_POS_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.pos.z = player[0].pos.z + (CAMERA_POS_DISTANCE*(cosf(g_Camera.rot.y)));
				g_Camera.pos.y = player[0].pos.y + ONECAMERA_YPOS;

				if (g_Camera.at.y >= (g_Camera.pos.y + ONECAMERA_AT_YPOS_MAX))
				{
					g_Camera.at.y = (g_Camera.pos.y + ONECAMERA_AT_YPOS_MAX);
				}
				else if (g_Camera.at.y <= (g_Camera.pos.y - ONECAMERA_AT_YPOS_MIN)) {
					g_Camera.at.y = (g_Camera.pos.y - ONECAMERA_AT_YPOS_MIN);
				}
			}

			//ROPE用　視点の上下判定
			if (g_Camera.pos.y >= g_Camera.at.y)
			{
				g_Camera.updown_check = false;
			}
			else
			{
				g_Camera.updown_check = true;
			}


			//MAP外判定
			{
				if (g_Camera.pos.y >= CameraPosY_MAX)
				{
					g_Camera.pos.y = CameraPosY_MAX;
				}
				else if (g_Camera.pos.y <= CameraPosY_MIN)
				{
					g_Camera.pos.y = CameraPosY_MIN;
				}

				//入口中の判定
				if (player[0].pos.x >= -150.0f&&player[0].pos.x <= 150.0f
					&&player[0].pos.z >= -7400.0f&&player[0].pos.z <= -7000.0f
					&&player[0].pos.y<200.0f)
				{

					if (g_Camera.pos.y > 140.0f)
					{
						g_Camera.pos.y = 140.0f;
					}
					if (g_Camera.pos.x > 140.0f)
					{
						g_Camera.pos.x = 140.0f;
					}
					else if (g_Camera.pos.x < -140.0f)
					{
						g_Camera.pos.x = -140.0f;
					}
					if (g_Camera.pos.z > -6980.0f)
					{
						g_Camera.pos.z = -6980.0f;
					}

				}
				if (player[0].pos.x <= -400.0f&&player[0].pos.z >= -7400.0f)
				{
					if (g_Camera.pos.y < 210.0f&&g_Camera.pos.x >= -210.0f)
					{
						g_Camera.pos.x = -210.0f;
					}
				}
				else if (player[0].pos.x >= 400.0f&&player[0].pos.z >= -7400.0f)
				{
					if (g_Camera.pos.y < 210.0f&&g_Camera.pos.x <= 210.0f)
					{
						g_Camera.pos.x = 210.0f;
					}
				}

			}
		}
	}
		break;
	case MODE_BATTLEAREA:
	{
		if (gamedata->dying == true)return;
		
		if (player[0].use == true) 
		{ 
			HWND titlecheck = GetActiveWindow();		//このプログラムのTITLE
			HWND OK = GetForegroundWindow();			//最上のプログラムのTITLE
			if (titlecheck == OK)
			{
				POINT point, pointwindow;
				GetCursorPos(&point);			//今モニター上、mouseの座標
				RECT rect;
				GetWindowRect(titlecheck, &rect);
				pointwindow.x = rect.left + (rect.right - rect.left) / 2;
				pointwindow.y = rect.top + (rect.bottom - rect.top) / 2;
				x = float(point.x - pointwindow.x);
				y = float(point.y - pointwindow.y);
				SetCursorPos(pointwindow.x, pointwindow.y);			//mouseの座標reset
				buffertime = 0;
			}
			else {

				buffertime++;
				if (buffertime >= 2) {
					gamedata->pausecheck = true;
				}
			}

			if (g_Camera.shoutchange == true)
			{
				player[0].camera_pos.x = player[0].pos.x - (CAMERA_DISTANCE_MIN*(sinf(g_Camera.rot.y)));
				player[0].camera_pos.z = player[0].pos.z - (CAMERA_DISTANCE_MIN*(cosf(g_Camera.rot.y)));
				player[0].camera_pos2.x = player[0].pos.x - (CAMERA_DISTANCE_MAX*(sinf(g_Camera.rot.y)));
				player[0].camera_pos2.z = player[0].pos.z - (CAMERA_DISTANCE_MAX*(cosf(g_Camera.rot.y)));

				// カメラの注視点と主人公に追従させる
				g_Camera.at.x = player[0].pos.x + (CAMERA_AT_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.at.z = player[0].pos.z + (CAMERA_AT_DISTANCE*(cosf(g_Camera.rot.y)));

				//パソコンmouse
				{
					{
						float y_move = y / 10;
						float xx = player[0].camera_pos.x - player[0].camera_pos2.x;
						float zz = player[0].camera_pos.z - player[0].camera_pos2.z;
						if (g_Camera.pos.y <= (camera_Ypos_center + player[0].pos.y)) {

							g_Camera.pos.y -= y_move;
							if (g_Camera.pos.y <= (player[0].pos.y + camera_start_Ypos + 1.0f))
							{
								g_Camera.pos.y = (player[0].pos.y + camera_start_Ypos + 1.0f);
							}
							float y_move2 = (camera_Ypos_center + player[0].pos.y) - g_Camera.pos.y;
							float y_proportion = ((sqrtf((camera_radius* camera_radius) - (y_move2*y_move2))) / (camera_radius));
							g_Camera.at.y = player[0].pos.y + camera__at_Ypos_center + camera_at_radius * (1 - y_proportion);
							float x_pos = player[0].camera_pos.x - xx * y_proportion;
							float z_pos = player[0].camera_pos.z - zz * y_proportion;
							float x_move = (xx*xx) - (y_move2*y_move2);
							float z_move = (zz*zz) - (y_move2*y_move2);
							float vx, vz;
							vx = x_pos - g_Camera.at.x;
							vz = z_pos - g_Camera.at.z;
							g_Camera.len = sqrtf(vx * vx + vz * vz);

						}
						else if (g_Camera.pos.y > (camera_Ypos_center + player[0].pos.y)) {
							g_Camera.pos.y -= y_move;
							if (g_Camera.pos.y >= (player[0].pos.y + camera_Ypos_center + camera_radius - 1.0f))
							{
								g_Camera.pos.y = (player[0].pos.y + camera_Ypos_center + camera_radius - 1.0f);
							}
							float y_move2 = g_Camera.pos.y - (camera_Ypos_center + player[0].pos.y);
							float y_proportion = ((sqrtf((camera_radius* camera_radius) - (y_move2*y_move2))) / (camera_radius));
							g_Camera.at.y = player[0].pos.y + camera__at_Ypos_center - camera_at_radius * (1 - y_proportion);
							float x_pos = player[0].camera_pos.x - xx * y_proportion;
							float z_pos = player[0].camera_pos.z - zz * y_proportion;
							float vx, vz;
							vx = x_pos - g_Camera.at.x;
							vz = z_pos - g_Camera.at.z;
							g_Camera.len = sqrtf(vx * vx + vz * vz);
						}
					}
				}

				{
					//パソコンMOUSE
					{
						g_Camera.rot.y += (D3DX_PI*(x / 2000));
						g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
						g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;

						float vx, vz;
						vx = player[0].pos.x - g_Camera.pos.x;
						vz = player[0].pos.z - g_Camera.pos.z;
						float xlong = sqrtf(vx * vx + vz * vz);
						g_Camera.playerpos = g_Camera.pos + ((g_Camera.at - g_Camera.pos)*(xlong / (xlong + CAMERA_AT_DISTANCE)));
					}
					
				}

			}
			else
			{//第一人視点mode
				float y_move = y / 5;
				// カメラの注視点と主人公に追従させる
				g_Camera.at.x = player[0].pos.x + (CAMERA_AT_ONEPOS_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.at.z = player[0].pos.z + (CAMERA_AT_ONEPOS_DISTANCE*(cosf(g_Camera.rot.y)));
				g_Camera.at.y -= y_move;
				//パソコンmouse
				g_Camera.rot.y += (D3DX_PI*(x / 2000));
				g_Camera.pos.x = player[0].pos.x + (CAMERA_POS_DISTANCE*(sinf(g_Camera.rot.y)));
				g_Camera.pos.z = player[0].pos.z + (CAMERA_POS_DISTANCE*(cosf(g_Camera.rot.y)));
				g_Camera.pos.y = player[0].pos.y + ONECAMERA_YPOS;

				if (g_Camera.at.y >= (g_Camera.pos.y + ONECAMERA_AT_YPOS_MAX))
				{

					g_Camera.at.y = (g_Camera.pos.y + ONECAMERA_AT_YPOS_MAX);
				}
				else if (g_Camera.at.y <= (g_Camera.pos.y - ONECAMERA_AT_YPOS_MIN)) {
					g_Camera.at.y = (g_Camera.pos.y - ONECAMERA_AT_YPOS_MIN);
				}
			}

			//ROPE用　視点の上下判定
			if (g_Camera.pos.y >= g_Camera.at.y)
			{
				g_Camera.updown_check = false;
			}
			else
			{
				g_Camera.updown_check = true;
			}
			//MAP外判定
			{
				if (g_Camera.pos.x <= CameraPosX_MIN)
				{
					g_Camera.pos.x = CameraPosX_MIN;
				}
				else if (g_Camera.pos.x >= CameraPosX_MAX)
				{
					g_Camera.pos.x = CameraPosX_MAX;

				}
				if (g_Camera.pos.z <= CameraPosZ_MIN)
				{
					if (player[0].pos.x >= 150.0f || player[0].pos.x <= -150.0f)
					{
						g_Camera.pos.z = CameraPosZ_MIN;
					}
					else if (player[0].pos.x <= 150.0f&&player[0].pos.x >= -150.0f
						&&player[0].pos.y >= 160.0f)
					{
						g_Camera.pos.z = CameraPosZ_MIN;
					}
				}
				else if (g_Camera.pos.z >= CameraPosZ_MAX)
				{
					g_Camera.pos.z = CameraPosZ_MAX;

				}
				if (g_Camera.pos.y >= CameraPosY_MAX)
				{
					g_Camera.pos.y = CameraPosY_MAX;
				}
				else if (g_Camera.pos.y <= CameraPosY_MIN)
				{
					g_Camera.pos.y = CameraPosY_MIN;
				}

				//入口中の判定
				if (player[0].pos.x >= -150.0f&&player[0].pos.x <= 150.0f
					&&player[0].pos.z <= -1350.0f&&player[0].pos.y <= 160.0f)
				{
					if (player[0].pos.z <= -1350.0f) {
						if (g_Camera.pos.y > 140.0f)
						{
							g_Camera.pos.y = 140.0f;
						}
					}
					if (g_Camera.pos.x > 140.0f)
					{
						g_Camera.pos.x = 140.0f;
					}
					else if (g_Camera.pos.x < -140.0f)
					{
						g_Camera.pos.x = -140.0f;
					}
					if (g_Camera.pos.z < -1840.0f)
					{
						g_Camera.pos.z = -1840.0f;
					}
				}

				//boss エリア壁判定
				{
					if (g_wall[152].use == true)
					{
						if (player[0].pos.x >= -600.0f&&player[0].pos.x <= 600.0f)
						{
							if (g_Camera.pos.z > 240.0f) {
								g_Camera.pos.z = 240.0f;
							}
						}
						else if (player[0].pos.x <= -600.0f&&player[0].pos.z >= 250.0f)
						{
							if (g_Camera.pos.x > -610.0f) {
								g_Camera.pos.x = -610.0f;
							}
						}else if (player[0].pos.x >= 600.0f&&player[0].pos.z >= 250.0f)
						{
							if (g_Camera.pos.x < 610.0f) {
								g_Camera.pos.x = 610.0f;
							}
						}

					}
					else
					{
						if (player[0].pos.x >= -550.0f &&player[0].pos.x <= -50.0f
							&&player[0].pos.z >= 300.0f)
						{
							if (g_Camera.pos.x < -540.0f) {
								g_Camera.pos.x = -540.0f;
							}
							if (g_Camera.pos.z < 310.0f) {
								g_Camera.pos.z = 310.0f;
							}

						}
						else if (player[0].pos.x <= 550.0f &&player[0].pos.x >= 50.0f
							&&player[0].pos.z >= 300.0f)
						{
							if (g_Camera.pos.x > 540.0f) {
								g_Camera.pos.x = 540.0f;
							}
							if (g_Camera.pos.z < 310.0f) {
								g_Camera.pos.z = 310.0f;
							}
						}
						else if (player[0].pos.x >= -100.0f &&player[0].pos.x <= 100.0f
							&&player[0].pos.z >= 300.0f&&g_Camera.pos.y >= 250.0f)
						{
							if (g_Camera.pos.z < 310.0f) {
								g_Camera.pos.z = 310.0f;
							}
						}

						if (player[0].pos.x <= -600.0f&&player[0].pos.z >= 250.0f)
						{
							if (g_Camera.pos.x > -610.0f) {
								g_Camera.pos.x = -610.0f;
							}
						}
						else if (player[0].pos.x >= 600.0f&&player[0].pos.z >= 250.0f)
						{
							if (g_Camera.pos.x < 610.0f) {
								g_Camera.pos.x = 610.0f;
							}
						}
						else if (player[0].pos.x <= 600.0f&&player[0].pos.x >= 50.0f
							&&player[0].pos.z <= 250.0f)
						{
							if (g_Camera.pos.z > 240.0f) {
								g_Camera.pos.z = 240.0f;
							}
						}
						else if (player[0].pos.x >= -600.0f&&player[0].pos.x <= -50.0f
							&&player[0].pos.z <= 250.0f)
						{
							if (g_Camera.pos.z > 240.0f) {
								g_Camera.pos.z = 240.0f;
							}
						}
						else if (player[0].pos.x >= -50.0f&&player[0].pos.x <= 50.0f
							&&player[0].pos.z <= 250.0f)
						{
							if (g_Camera.pos.z > 240.0f&&g_Camera.pos.y > 250.0f) {
								g_Camera.pos.z = 240.0f;
							}
						}
					}
					
				}
				//2階下の天井の判定
				{
					if (player[0].pos.z >= -750.0f&&player[0].pos.z <= 250.0f
						&&player[0].pos.y < 220.0f)
					{
						if (g_Camera.pos.y > 240.0f) {
							g_Camera.pos.y = 240.0f;
						}
					}
				}
				//右上穴判定
				{
					if (player[0].pos.z >= 1300.0f&&player[0].pos.z <= 1500.0f
						&&player[0].pos.x >= 950.0f&&player[0].pos.x <= 1150.0f
						&&player[0].pos.y<380.0f&&player[0].pos.y > 250.0f)
					{
						if (g_Camera.pos.y > 370.0f) {
							g_Camera.pos.y = 370.0f;
						}
					}
				}
			}

		}
	}
		break;
	case MODE_RESULT:
		{
		g_Camera.pos = D3DXVECTOR3(0.0f, 30.0f, -8350.0f);
		g_Camera.at = D3DXVECTOR3(0.0f, 70.0f, -8250.0f);
		}
		break;
	default:
		break;
	}
	
	g_Camera.left_pos = g_Camera.pos;
	g_Camera.right_pos = g_Camera.pos;
	g_Camera.top_pos = g_Camera.pos;
	g_Camera.down_pos = g_Camera.pos;

	g_Camera.left_pos.x -= cosf(g_Camera.rot.y)*PLAYER_SIZE2;
	g_Camera.left_pos.y += PLAYER_TALL / 2;
	g_Camera.left_pos.z += sinf(g_Camera.rot.y)*PLAYER_SIZE2;

	g_Camera.right_pos.x += cosf(g_Camera.rot.y)*PLAYER_SIZE2;
	g_Camera.right_pos.y += PLAYER_TALL / 2;
	g_Camera.right_pos.z -= sinf(g_Camera.rot.y)*PLAYER_SIZE2;

	g_Camera.top_pos.y += (PLAYER_TALL / 2) + PLAYER_SIZE2;

	g_Camera.down_pos.y += (PLAYER_TALL / 2) - PLAYER_SIZE2;
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	SetViewMatrix(&g_Camera.mtxView);

	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}


//= ============================================================================
// ビューポートの設定
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// ビューポート設定
	switch (g_ViewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;


	}
	g_ImmediateContext->RSSetViewports(1, &vp);
}

int GetViewPortType(void)
{
	return g_ViewPortType;
}
