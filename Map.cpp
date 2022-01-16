//=============================================================================
//
// 地図の処理 [Map.cpp]
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
#include "meshMap.h"
#include "meshMapReverse.h"
#include "Map.h"
#include "MapTransparent.h"
#include "collision.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	SKY	"data/MODEL/sky.obj"		// 読み込むモデル名
//*****************************************************************************
// グローバル変数
//*****************************************************************************

static PLAYER  *player = GetPlayer();
static ENEMY   *enemytype = GetEnemyTypeA();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
static MAP_POINT	fieldpoint[MAX_MESH_FIELD];
static MAP_POINT	ceilingpoint[MAX_MESH_FIELD_CEILING];
static MAP_POINT	Wallpoint[MAX_MESH_WALL];
static MAP_POINT	fieldslopepoint[MAX_MESH_FIELD_SLOPE];
static MAP_POINT  slopewallpoint[MAX_MESH_SLOPE_WALL];
static MAP_POINT	fieldpoint_reverse[MAX_MESH_FIELD];
static MAP_POINT	ceilingpoint_reverse[MAX_MESH_FIELD_CEILING];
static MAP_POINT	Wallpoint_reverse[MAX_MESH_WALL];
static MAP_POINT	fieldslopepoint_reverse[MAX_MESH_FIELD_SLOPE];
static MAP_POINT  slopewallpoint_reverse[MAX_MESH_SLOPE_WALL];

static int g_nNumMeshField = 0;						// メッシュ壁の数
static int g_nNumMeshCeiling = 0;
static int g_nNumMeshFieldSlope = 0;						// メッシュ壁の数
static int g_nNumMeshSlopeWall = 0;
static int g_nNumMeshWall = 0;						// メッシュ壁の数

static int g_nNumMeshField_reverse = 0;						// メッシュ壁の数
static int g_nNumMeshCeiling_reverse = 0;
static int g_nNumMeshFieldSlope_reverse = 0;						// メッシュ壁の数
static int g_nNumMeshSlopeWall_reverse = 0;
static int g_nNumMeshWall_reverse = 0;						// メッシュ壁の数

static D3DXVECTOR3			pos;				// モデルの位置
static D3DXVECTOR3			rot;				// モデルの向き(回転)
static D3DXVECTOR3			scl;				// モデルの大きさ(スケール
static float				spd;				// 移動スピード
static DX11_MODEL			model;				// モデル情報
static D3DXMATRIX			mtxWorld;			// ワールドマトリックス

//=============================================================================
// 初期化処理
//=============================================================================


HRESULT InitMap(void)
{
	LoadModel(SKY, &model);
	pos = D3DXVECTOR3(0.0f, 10.0f, -5500.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(550.0f, 550.0f, 550.0f);

	//Restarea
	{
		InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 100.0f, 400.0f, -500, -7000, 0.0f);
		SaveFieldPoint(D3DXVECTOR3(-500.0f, 0.0f, -7000.0f), D3DXVECTOR3(500.0f, 0.0f, -9000.0f));
		InitTriangleWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(-1600.0f, -100.0f, -7100.0f), D3DXVECTOR3(1600.0f, -100.0f, -7100.0f),
			D3DXVECTOR3(0.0, 0.0, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		InitTriangleWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(-1500.0f, 0.0f, -4000.0f), D3DXVECTOR3(-1500.0f, 0.0f, -7000.0f),
			D3DXVECTOR3(0.0, 0.0, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		InitTriangleWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(1500.0f, 0.0f, -7000.0f), D3DXVECTOR3(1500.0f, 0.0f, -4000.0f),
			D3DXVECTOR3(0.0, 0.0, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		InitTriangleWall(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(0.0f, 1500.0f, -5500.0f), D3DXVECTOR3(1500.0f, 0.0f, -4000.0f), D3DXVECTOR3(-1500.0f, 0.0f, -4000.0f),
			D3DXVECTOR3(0.0, 0.0, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//
		InitMeshField_Random(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 40, 250.0f,250.0f, -5000, -2000, -60.0f, 1);
		
		//右向の壁
		InitMeshWall(D3DXVECTOR3(500.0f, -100.0f, -8500.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 10, 100.0f, 10.0f);
		SaveWallPoint(D3DXVECTOR3(500.0f, 0.0f, -10000.0f), D3DXVECTOR3(500.0f, -100.0f, -7000.0f), 1);
		//左向の壁
		InitMeshWall(D3DXVECTOR3(-500, -100.0f, -8500.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 10, 100.0f, 10.0f);
		SaveWallPoint(D3DXVECTOR3(-500.0f, 0.0f, -7000.0f), D3DXVECTOR3(-500.0f, -100.0f, -10000.0f), 2);
		//入口
		{
			//
			//右向の壁
			InitMeshWall(D3DXVECTOR3(-150.0f, 0.0f, -6450.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 20, 2, 95.0f, 80.0f);
			SaveWallPoint(D3DXVECTOR3(-150.0f, 160.0f, -7400.0f), D3DXVECTOR3(-150.0f, 0.0f, -5500.0f), 1);
		
			//左向の壁
			InitMeshWall(D3DXVECTOR3(150, 0.0f, -6450.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 20, 2, 95.0f, 80.0f);
			SaveWallPoint(D3DXVECTOR3(150.0f, 160.0f, -5500.0f), D3DXVECTOR3(150.0f, 0.0f, -7400.0f), 2);
			
			//下向の壁
			InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -7000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 10, 2, 30.0f, 80.0f);
			SaveWallPoint(D3DXVECTOR3(-150.0f, 160.0f, -7000.0f), D3DXVECTOR3(150.0f, 0.0f, -7000.0f), 3);
		
			InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 160.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), 3, 4, 100.0f, 100.0f, 150, -7000);
			SaveCeilingPoint(D3DXVECTOR3(150.0f, 160.0f, -7000.0f), D3DXVECTOR3(-150.0f, 160.0f, -7400.0f));
			//
			//左向の壁
			InitMeshWall(D3DXVECTOR3(-200, 0.0f, -6450.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 2, 95.0f, 105.0f);
			SaveWallPoint(D3DXVECTOR3(-200.0f, 210.0f, -5500.0f), D3DXVECTOR3(-200.0f, 0.0f, -7400.0f), 2);
			//右向の壁
			InitMeshWall(D3DXVECTOR3(200.0f, 0.0f, -6450.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 2, 95.0f, 105.0f);
			SaveWallPoint(D3DXVECTOR3(200.0f, 210.0f, -7400.0f), D3DXVECTOR3(200.0f, 0.0f, -5500.0f), 1);

			InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 190.0f, -200, -5500, 210.0f);
			SaveFieldPoint(D3DXVECTOR3(-200.0f, 210.0f, -5500.0f), D3DXVECTOR3(200.0f, 210.0f, -7400.0f));

			//下向の壁
			InitMeshWall(D3DXVECTOR3(-175.0f, 0.0f, -7400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 2, 10.0f, 105.0f);
			SaveWallPoint(D3DXVECTOR3(-200.0f, 210.0f, -7400.0f), D3DXVECTOR3(-150.0f, 0.0f, -7400.0f), 3);

			//下向の壁
			InitMeshWall(D3DXVECTOR3(175.0f, 0.0f, -7400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 2, 10.0f, 105.0f);
			SaveWallPoint(D3DXVECTOR3(150.0f, 210.0f, -7400.0f), D3DXVECTOR3(200.0f, 0.0f, -7400.0f), 3);

			//下向の壁
			InitMeshWall(D3DXVECTOR3(0.0f, 160.0f, -7400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 30.0f, 10.0f);
			SaveWallPoint(D3DXVECTOR3(-150.0f, 210.0f, -7000.0f), D3DXVECTOR3(150.0f, 160.0f, -7000.0f), 3);
		}
	}
	//Battlearea
	{
		//MAPのフレーム  動くじゃダメ
		{
			//地面
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 18, 100.0f, 100.0f, -1500, 300, 0.0f);
				SaveFieldPoint(D3DXVECTOR3(-1500.0f, 0.0f, 300.0f), D3DXVECTOR3(1500.0f, 0.0f, -1500.0f));
				{	//左上
					InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 2, 100.0f, 100.0f, -1500, 1500, 0.0f);
					SaveFieldPoint(D3DXVECTOR3(-1500.0f, 0.0f, 1500.0f), D3DXVECTOR3(-600.0f, 0.0f, 1300.0f));

					InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 1, 100.0f, 170.0f, -1500, 1170, 0.0f);
					SaveFieldPoint(D3DXVECTOR3(-1500.0f, 0.0f, 1170.0f), D3DXVECTOR3(-600.0f, 0.0f, 1000.0f));

					InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 1, 100.0f, 200.0f, -1500, 820, 0.0f);
					SaveFieldPoint(D3DXVECTOR3(-1500.0f, 0.0f, 820.0f), D3DXVECTOR3(-600.0f, 0.0f, 620.0f));


					//右上
					InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 2, 100.0f, 100.0f, 600, 1500, 0.0f);
					SaveFieldPoint(D3DXVECTOR3(600.0f, 0.0f, 1500.0f), D3DXVECTOR3(1500.0f, 0.0f, 1300.0f));


					InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 7, 100.0f, 100.0f, 600, 1000, 0.0f);
					SaveFieldPoint(D3DXVECTOR3(600.0f, 0.0f, 1000.0f), D3DXVECTOR3(1500.0f, 0.0f, 300.0f));
				}
				//天井
				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 640.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 30, 100.0f, 100.0f, 1500, 1500);
				SaveCeilingPoint(D3DXVECTOR3(1500.0f, 640.0f, 1500.0f), D3DXVECTOR3(-1500.0f, 640.0f, -1500.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, -160.0f, FIELD_TOP), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 100, 10, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 640.0f, 1500.0f), D3DXVECTOR3(1500.0f, -160.0f, 1500.0f), 3);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(FIELD_LEFT, -160.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 100, 10, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 640.0f, -1500.0f), D3DXVECTOR3(-1500.0f, -160.0f, 1500.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(FIELD_RIGHT, -160.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 100, 10, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 640.0f, 1500.0f), D3DXVECTOR3(1500.0f, -160.0f, -1500.0f), 2);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 160.0f, FIELD_DOWN), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 10, 6, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(150.0f, 640.0f, -1500.0f), D3DXVECTOR3(-150.0f, 150.0f, -1500.0f), 4);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-825.0f, 0.0f, FIELD_DOWN), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 45, 8, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-150.0f, 640.0f, -1500.0f), D3DXVECTOR3(-1500.0f, 0.0f, -1500.0f), 4);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(825.0f, 0.0f, FIELD_DOWN), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 45, 8, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 640.0f, -1500.0f), D3DXVECTOR3(150.0f, 0.0f, -1500.0f), 4);
			}
			//入口
			{
				//地面
				InitMeshField(D3DXVECTOR3(0.0f, 0.1f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 3, 5, 100.0f, 100.0f, -150, -1350, 0.0f);
				SaveFieldPoint(D3DXVECTOR3(-150.0f, 0.0f, -1350.0f), D3DXVECTOR3(150.0f, 0.0f, -1850.0f));

				//右向の壁
				InitMeshWall(D3DXVECTOR3(-150, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 5, 2, 100.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-150, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 5, 2, 100.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-150.0f, 160.0f, -1850.0f), D3DXVECTOR3(-150.0f, 0.0f, -1350.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(150, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 5, 2, 100.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(150, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 5, 2, 100.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(150.0f, 160.0f, -1350.0f), D3DXVECTOR3(150.0f, 0.0f, -1850.0f), 2);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -1850), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 10, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(150.0f, 160.0f, -1850.0f), D3DXVECTOR3(-150.0f, 0.0f, -1850.0f), 4);

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 160.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), 3, 5, 100.0f, 100.0f, 150, -1350);
				InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 160.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.25f), 3, 5, 100.0f, 100.0f, -150, -1350);
				SaveCeilingPoint(D3DXVECTOR3(150.0f, 160.0f, -1350.0f), D3DXVECTOR3(-150.0f, 160.0f, -1850.0f));
				SaveCeilingPointReverse(D3DXVECTOR3(150.0f, 160.0f, -1350.0f), D3DXVECTOR3(-150.0f, 160.0f, -1850.0f));

				//地面
				InitMeshField(D3DXVECTOR3(0.0f, 210.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 5, 100.0f, 100.0f, -200, -1350, 0.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 210.0f, 0.0f), D3DXVECTOR3(0.0, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 4, 5, 100.0f, 100.0f, -200, -1350, 0.0f);
				SaveFieldPoint(D3DXVECTOR3(-200.0f, 210.0f, -1350.0f), D3DXVECTOR3(200.0f, 210.0f, -1850.0f));
				SaveFieldPointReverse(D3DXVECTOR3(-200.0f, 210.0f, -1350.0f), D3DXVECTOR3(200.0f, 210.0f, -1850.0f));
				//左向の壁
				InitMeshWall(D3DXVECTOR3(-200, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 21, 100.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-200, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 5, 21, 100.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-200.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 0.0f, -1850.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-200.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 0.0f, -1850.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(200, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 21, 100.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(200, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 5, 21, 100.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(200.0f, 210.0f, -1850.0f), D3DXVECTOR3(200.0f, 0.0f, -1350.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(200.0f, 210.0f, -1850.0f), D3DXVECTOR3(200.0f, 0.0f, -1350.0f), 1);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 160.0f, -1350), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 160.0f, -1350), D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 40, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(200.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 160.0f, -1350.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(200.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 160.0f, -1350.0f), 4);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(175.0f, 0.0f, -1350), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 21, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(175.0f, 0.0f, -1350), D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 5, 21, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(200.0f, 210.0f, -1350.0f), D3DXVECTOR3(150.0f, 0.0f, -1350.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(200.0f, 210.0f, -1350.0f), D3DXVECTOR3(150.0f, 0.0f, -1350.0f), 4);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-175.0f, 0.0f, -1350), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 21, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-175.0f, 0.0f, -1350), D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 5, 21, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-150.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 0.0f, -1350.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-150.0f, 210.0f, -1350.0f), D3DXVECTOR3(-200.0f, 0.0f, -1350.0f), 4);

			}
		}

		//1階
		{
			//BOSS エリアの壁
			{
				//地面

				// フィールドの初期化
				InitBOSSField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 24, 24, 50.0f, 50.0f);
				//SaveFieldPoint(D3DXVECTOR3(-600.0f, 0.0f, 1500.0f), D3DXVECTOR3(600.0f, 0.0f, 250.0f));

				//左
				InitMeshWall(D3DXVECTOR3(-600.0f, -160.0f, 875.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 125.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-600.0f, -160.0f, 875.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 125.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 640.0f, 1500.0f), D3DXVECTOR3(-600.0f, -160.0f, 250.0f), 2);

				InitMeshWall(D3DXVECTOR3(-550.0f, -160.0f, 900.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 24, 10, 50.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-550.0f, 0.0f, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f,0.25f), 10, 10, 120.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-550.0f, 640.0f, 300.0f), D3DXVECTOR3(-550.0f, -160.0f, 1500.0f), 1);

				InitMeshWall(D3DXVECTOR3(-300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 10, 8, 50.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f, 0.25f), 10, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-50.0f, 640.0f, 300.0f), D3DXVECTOR3(-550.0f, 0.0f, 300.0f), 4);

				InitMeshWall(D3DXVECTOR3(-325.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 55.0f, 20.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-325.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f,0.25f), 10, 10, 55.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 640.0f, 250.0f), D3DXVECTOR3(-50.0f, 0.0f, 250.0f), 3);

				InitMeshWall(D3DXVECTOR3(-50.0f, 0.0f, 275.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 8, 5.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-50.0f, 0.0f, 275.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 8, 5.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-50.0f, 640.0f, 250.0f), D3DXVECTOR3(-50.0f, 0.0f, 300.0f), 1);

				//右
				InitMeshWall(D3DXVECTOR3(600.0f, -160.0f, 875.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 125.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(600.0f, -160.0f, 875.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 125.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(600.0f, 640.0f, 250.0f), D3DXVECTOR3(600.0f, -160.0f, 1500.0f), 1);

				InitMeshWall(D3DXVECTOR3(550.0f, -160.0f, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 24, 10, 50.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(550.0f, -160.0f, 900.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f,0.25f), 10, 10, 120.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 640.0f, 1500.0f), D3DXVECTOR3(550.0f, -160.0f, 300.0f), 2);

				InitMeshWall(D3DXVECTOR3(300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 10, 8, 50.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f,0.25f), 10, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 640.0f, 300.0f), D3DXVECTOR3(50.0f, 0.0f, 300.0f), 4);

				InitMeshWall(D3DXVECTOR3(325.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 55.0f, 20.0f);
				//InitMeshWallReverse(D3DXVECTOR3(325.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 55.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(50.0f, 640.0f, 250.0f), D3DXVECTOR3(600.0f, 0.0f, 250.0f), 3);

				InitMeshWall(D3DXVECTOR3(50.0f, 0.0f, 275.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 8, 5.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(50.0f, 0.0f, 275.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 8, 5.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(50.0f, 640.0f, 300.0f), D3DXVECTOR3(50.0f, 0.0f, 250.0f), 2);

				//正面
				InitMeshWall(D3DXVECTOR3(0.0f, 200.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 120.0f, 54.0f);
				//InitMeshWallReverse(D3DXVECTOR3(0.0f, 200.0f, 250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 120.0f, 54.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 640.0f, 250.0f), D3DXVECTOR3(600.0f, 200.0f, 250.0f), 3);

				InitMeshWall(D3DXVECTOR3(0.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 2, 8, 50.0f, 80.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 2, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(50.0f, 640.0f, 300.0f), D3DXVECTOR3(-50.0f, 200.0f, 300.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(50.0f, 640.0f, 300.0f), D3DXVECTOR3(-50.0f, 200.0f, 300.0f), 4);

				InitMeshWall(D3DXVECTOR3(0.0f, -160.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 22, 2, 50.0f, 80.0f);
				//InitMeshWallReverse(D3DXVECTOR3(0.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 2, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 0.0f, 300.0f), D3DXVECTOR3(-550.0f, -160.0f, 300.0f), 4);

				//中天井
				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 20.0f, 10.0f, 50, 300);
				InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 20.0f, 10.0f, -50, 300);
				SaveCeilingPoint(D3DXVECTOR3(50.0f, 200.0f, 300.0f), D3DXVECTOR3(-50.0f, 200.0f, 250.0f));
				SaveCeilingPointReverse(D3DXVECTOR3(50.0f, 200.0f, 300.0f), D3DXVECTOR3(-50.0f, 200.0f, 250.0f));
			}

			//落穴(左上)
			{
				//1号地面
				InitMeshField_Random(D3DXVECTOR3(0.0f, -160.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, 36.0f, 5.2f, -1500, 1300, 0.0f, 0);
				//SaveFieldPoint(D3DXVECTOR3(-1500.0f, -160.0f, 1300.0f), D3DXVECTOR3(-600.0f, -160.0f, 1170.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 0.0f, 1300.0f), D3DXVECTOR3(-600.0f, -160.0f, 1300.0f), 3);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 1170.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 0.0f, 1170.0f), D3DXVECTOR3(-1500.0f, -160.0f, 1170.0f), 4);

				//2号地面
				InitMeshField_Random(D3DXVECTOR3(0.0f, -160.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, 36.0f, 7.2f, -1500, 1000, 0.0f,0);
				//SaveFieldPoint(D3DXVECTOR3(-1500.0f, -160.0f, 1000.0f), D3DXVECTOR3(-600.0f, -160.0f, 820.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 0.0f, 1000.0f), D3DXVECTOR3(-600.0f, -160.0f, 1000.0f), 3);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 820.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 0.0f, 820.0f), D3DXVECTOR3(-1500.0f, -160.0f, 820.0f), 4);

				//3号地面
				InitMeshField_Random(D3DXVECTOR3(0.0f, -160.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, 36.0f, 12.8f, -1500, 620, 0.0f, 0);
				//SaveFieldPoint(D3DXVECTOR3(-1500.0f, -160.0f, 620.0f), D3DXVECTOR3(-600.0f, -160.0f, 300.0f));


				//下向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 620.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 0.0f, 620.0f), D3DXVECTOR3(-600.0f, -160.0f, 620.0f), 3);

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1050.0f, -160.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(-600.0f, 0.0f, 300.0f), D3DXVECTOR3(-1500.0f, -160.0f, 300.0f), 4);
			}

			//落穴(右上)
			{
				//1号地面
				InitMeshField_Random(D3DXVECTOR3(0.0f, -160.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, 36.0f, 12.0f, 600, 1300, 0.0f, 0);
				//SaveFieldPoint(D3DXVECTOR3(600.0f, -160.0f, 1300.0f), D3DXVECTOR3(1500.0f, -160.0f, 1000.0f));

				////下向の壁
				//InitMeshWall(D3DXVECTOR3(1050.0f, -160.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				//SaveWallPoint(D3DXVECTOR3(600.0f, 0.0f, 1300.0f), D3DXVECTOR3(1500.0f, -160.0f, 1300.0f), 3);

				////上向の壁
				//InitMeshWall(D3DXVECTOR3(1050.0f, -160.0f, 1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f), 30, 2, 30.0f, 80.0f);
				//SaveWallPoint(D3DXVECTOR3(1500.0f, 0.0f, 1000.0f), D3DXVECTOR3(600.0f, -160.0f, 1000.0f), 4);
			}

			//1号足場
			{
				InitMeshWall(D3DXVECTOR3(750.0f, 0.0f, -1125.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 75.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(750.0f, 0.0f, -1125.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 75.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(750.0f, 250.0f, -750.0f), D3DXVECTOR3(750.0f, 0.0f, -1500.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(750.0f, 250.0f, -750.0f), D3DXVECTOR3(750.0f, 0.0f, -1500.0f), 2);

				InitMeshWall(D3DXVECTOR3(1125.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 75.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1125.0f, 0.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 75.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 250.0f, -750.0f), D3DXVECTOR3(750.0f, 0.0f, -750.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1500.0f, 250.0f, -750.0f), D3DXVECTOR3(750.0f, 0.0f, -750.0f), 4);

				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 75.0f, 75.0f, 750, -750, 250.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 75.0f, 75.0f, 750, 1500, 250.0f);
				SaveFieldPoint(D3DXVECTOR3(750.0f, 250.0f, -750.0f), D3DXVECTOR3(1500.0f, 250.0f, -1500.0f));
				SaveFieldPointReverse(D3DXVECTOR3(750.0f, 250.0f, -750.0f), D3DXVECTOR3(1500.0f, 250.0f, -1500.0f));
			}

			//2号足場
			{
				InitMeshWall(D3DXVECTOR3(350.0f, 0.0f, -1250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 50.0f, 12.0f);
				InitMeshWallReverse(D3DXVECTOR3(350.0f, 0.0f, -1250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 50.0f, 12.0f);
				SaveWallPoint(D3DXVECTOR3(350.0f, 120.0f, -1000.0f), D3DXVECTOR3(350.0f, 0.0f, -1500.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(350.0f, 120.0f, -1000.0f), D3DXVECTOR3(350.0f, 0.0f, -1500.0f), 2);

				InitMeshWall(D3DXVECTOR3(550.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 12.0f);
				InitMeshWallReverse(D3DXVECTOR3(550.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 40.0f, 12.0f);
				SaveWallPoint(D3DXVECTOR3(750.0f, 120.0f, -1000.0f), D3DXVECTOR3(350.0f, 0.0f, -1000.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(750.0f, 120.0f, -1000.0f), D3DXVECTOR3(350.0f, 0.0f, -1000.0f), 4);

				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 50.0f, 350, -1000, 120.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 40.0f, 50.0f, 350, 1500, 120.0f);
				SaveFieldPoint(D3DXVECTOR3(350.0f, 120.0f, -1000.0f), D3DXVECTOR3(7500.0f, 120.0f, -1500.0f));
				SaveFieldPointReverse(D3DXVECTOR3(350.0f, 120.0f, -1000.0f), D3DXVECTOR3(7500.0f, 120.0f, -1500.0f));
			}

			//3号足場
			{
				InitMeshWall(D3DXVECTOR3(650.0f, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 170.0f, 10.0f);
				//InitMeshWallReverse(D3DXVECTOR3(650.0f, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 170.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-200.0f, 50.0f, -1100.0f), D3DXVECTOR3(1500.0f, 0.0f, -1100.0f), 3);

				InitMeshWall(D3DXVECTOR3(650.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 170.0f, 10.0f);
				//InitMeshWallReverse(D3DXVECTOR3(650.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 170.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 50.0f, -500.0f), D3DXVECTOR3(-200.0f, 0.0f, -500.0f), 4);

				InitMeshWall(D3DXVECTOR3(-200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 60.0f, 10.0f);
				//InitMeshWallReverse(D3DXVECTOR3(-200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 60.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-200.0f, 50.0f, -500.0f), D3DXVECTOR3(-200.0f, 0.0f, -1100.0f), 2);

				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 170.0f, 60.0f, -200, -500, 50.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
					//D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 170.0f, 60.0f, -200, -500, 50.0f);
				SaveFieldPoint(D3DXVECTOR3(-200.0f, 50.0f, -500.0f), D3DXVECTOR3(1500.0f, 50.0f, -1100.0f));

			}

			//4号足場
			{
				//
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, -1200, -1000, 125.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, -1200, 1200, 125.0f);
				SaveFieldPoint(D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f));
				SaveFieldPointReverse(D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(-1100.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 12.5f);
				InitMeshWallReverse(D3DXVECTOR3(-1100.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 12.5f);
				SaveWallPoint(D3DXVECTOR3(-1200.0f, 125.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1200.0f, 125.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f), 3);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-1000, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 12.5f);
				InitMeshWallReverse(D3DXVECTOR3(-1000, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 12.5f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-1200, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 12.5f);
				InitMeshWallReverse(D3DXVECTOR3(-1200, 0.0f, -1100.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 12.5f);
				SaveWallPoint(D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 0.0f, -1200.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 0.0f, -1200.0f), 2);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1100.0f, 0.0f, -1000), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 12.5f);
				InitMeshWallReverse(D3DXVECTOR3(-1100.0f, 0.0f, -1000), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 12.5f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 0.0f, -1500.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 0.0f, -1500.0f), 4);
			}

			//5号足場
			{
				//
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 9, 10, 100.0f, 20.0f, 600, 1000, 150.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 9, 10, 100.0f, 20.0f, 600, 1000, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(600.0f, 150.0f, 1000.0f), D3DXVECTOR3(1500.0f, 150.0f, 800.0f));
				SaveFieldPointReverse(D3DXVECTOR3(600.0f, 150.0f, 1000.0f), D3DXVECTOR3(1500.0f, 150.0f, 800.0f));
				//
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 10, 100.0f, 20.0f, 850, 800, 150.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 4, 10, 100.0f, 20.0f, 850, 800, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(850.0f, 150.0f, 800.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f));
				SaveFieldPointReverse(D3DXVECTOR3(850.0f, 150.0f, 800.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f));

				//上向の壁
				InitMeshWall(D3DXVECTOR3(1050.0f, -160.0f, 1000), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 45, 31, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(1050.0f, -160.0f, 1000), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 45, 31, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 150.0f, 1000.0f), D3DXVECTOR3(600.0f, -160.0f, 1000.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1500.0f, 150.0f, 1000.0f), D3DXVECTOR3(600.0f, -160.0f, 1000.0f), 4);

				//下向の壁
				InitMeshWall(D3DXVECTOR3(1050.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1050.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 20, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(850.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 0.0f, 250.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(850.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 0.0f, 250.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(850, 0.0f, 525.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 22, 6, 25.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(850, 0.0f, 525.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 22, 6, 25.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(850.0f, 150.0f, 800.0f), D3DXVECTOR3(850.0f, 0.0f, 250.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(850.0f, 150.0f, 800.0f), D3DXVECTOR3(850.0f, 0.0f, 250.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(1250, 0.0f, 525.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 22, 6, 25.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1250, 0.0f, 525.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 22, 6, 25.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(1250.0f, 150.0f, 250.0f), D3DXVECTOR3(1250.0f, 0.0f, 800.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(1250.0f, 150.0f, 250.0f), D3DXVECTOR3(1250.0f, 0.0f, 800.0f), 1);

			}

			//1号坂道
			{
				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, 1.5f, 1.5f, 600, -1175, 120, 250, 1);
				/*InitMeshSlopeReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 100, 100, 1.5f, 1.5f, 550, -1175, 120, 250, 1);*/
				SaveFieldSlopePoint(D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 250.0f, -1325.0f), 1);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 250.0f, -1325.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 120.0f, -1325.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(750.0f, 250.0f, -1325.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 120.0f, -1325.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 250.0f, -1325.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 120.0f, -1325.0f), 2, 1);
				SaveSlopeWallPointReverse(D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 250.0f, -1325.0f), D3DXVECTOR3(600.0f, 120.0f, -1325.0f), D3DXVECTOR3(750.0f, 120.0f, -1325.0f), 2, 1);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(750.0f, 250.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 120.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 250.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 120.0f, -1175.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(750.0f, 250.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 120.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), 1, 1);
				SaveSlopeWallPointReverse(D3DXVECTOR3(750.0f, 250.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), D3DXVECTOR3(750.0f, 120.0f, -1175.0f), D3DXVECTOR3(600.0f, 120.0f, -1175.0f), 1, 1);
			}
			//2号坂道
			{
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1100.0f, 125.0f, -750), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 12.5f);
				InitMeshWallReverse(D3DXVECTOR3(-1100.0f, 125.0f, -750), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 12.5f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), 4);
				//
				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, 2.5f, 2.0f, -1000, -1000, 0, 125, 2);
				SaveFieldSlopePoint(D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), 2);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f), 2, 2);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-750.0f, 0.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1000.0f), 2, 2);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), 1, 2);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-1000.0f, 125.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), D3DXVECTOR3(-1000.0f, 0.0f, -1200.0f), D3DXVECTOR3(-750.0f, 0.0f, -1200.0f), 1, 2);
			}
			//3号坂道
			{
				//
				//天井
				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 125.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f, -1000, -750);
				InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 125.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f, -1200, -750);
				SaveCeilingPoint(D3DXVECTOR3(-1000.0f, 125.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f));
				SaveCeilingPointReverse(D3DXVECTOR3(-1000.0f, 125.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f));

				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, 2.0f, 2.5f, -1200, -750, 125, 250, 3);
				SaveFieldSlopePoint(D3DXVECTOR3(-1200.0f, 250.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), 3);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -750.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -750.0f), 2, 3);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 250.0f, -750.0f), D3DXVECTOR3(-1000.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1000.0f, 125.0f, -750.0f), 2, 3);

				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1200.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 125.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 125.0f, -750.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-1200.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 125.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), 1, 3);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-1200.0f, 250.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), D3DXVECTOR3(-1200.0f, 125.0f, -750.0f), D3DXVECTOR3(-1200.0f, 125.0f, -1000.0f), 1, 3);
			}
			//4号坂道
			{
				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 25.0f, 50.0f, 600, 800, 0, 150, 3);
				SaveFieldSlopePoint(D3DXVECTOR3(600.0f, 150.0f, 800.0f), D3DXVECTOR3(850.0f, 0.0f, 300.0f), 3);
			}
			//5号坂道
			{
				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 25.0f, 50.0f, 1250, 800, 0, 150, 3);
				SaveFieldSlopePoint(D3DXVECTOR3(1250.0f, 150.0f, 800.0f), D3DXVECTOR3(1500.0f, 0.0f, 300.0f), 3);
			}
			//6号坂道
			{
				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 35.0f, 850, 600, 150, 250, 4);
				SaveFieldSlopePoint(D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 250.0f, 250.0f), 4);

				//左
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 250.0f, 250.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 150.0f, 250.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(850.0f, 250.0f, 250.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 150.0f, 250.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 250.0f, 300.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 150.0f, 300.0f), 2, 4);
				SaveSlopeWallPointReverse(D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 250.0f, 300.0f), D3DXVECTOR3(850.0f, 150.0f, 600.0f), D3DXVECTOR3(850.0f, 150.0f, 300.0f), 2, 4);
				//右
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1250.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 150.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 150.0f, 250.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(1250.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 150.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), 1, 4);
				SaveSlopeWallPointReverse(D3DXVECTOR3(1250.0f, 250.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), D3DXVECTOR3(1250.0f, 150.0f, 250.0f), D3DXVECTOR3(1250.0f, 150.0f, 600.0f), 1, 4);
			}
			//1号柱
			{
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-900.0f, 0.0f, -650), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-900.0f, 0.0f, -650), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 200.0f, -650.0f), D3DXVECTOR3(-800.0f, 0.0f, -650.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 200.0f, -650.0f), D3DXVECTOR3(-800.0f, 0.0f, -650.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-900.0f, 0.0f, -450), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-900.0f, 0.0f, -450), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-800.0f, 200.0f, -450.0f), D3DXVECTOR3(-1000.0f, 0.0f, -450.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-800.0f, 200.0f, -450.0f), D3DXVECTOR3(-1000.0f, 0.0f, -450.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-800, 0.0f, -550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-800, 0.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-800.0f, 200.0f, -650.0f), D3DXVECTOR3(-800.0f, 0.0f, -450.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-800.0f, 200.0f, -650.0f), D3DXVECTOR3(-800.0f, 0.0f, -450.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-1000, 0.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1000, 0.0f, -550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 200.0f, -450.0f), D3DXVECTOR3(-1000.0f, 0.0f, -650.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 200.0f, -450.0f), D3DXVECTOR3(-1000.0f, 0.0f, -650.0f), 2);

			}
			//2号柱
			{
				//下向の壁
				InitMeshWall(D3DXVECTOR3(900.0f, 0.0f, -650), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(900.0f, 0.0f, -650), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(800.0f, 200.0f, -650.0f), D3DXVECTOR3(1000.0f, 0.0f, -650.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(800.0f, 200.0f, -650.0f), D3DXVECTOR3(1000.0f, 0.0f, -650.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(900.0f, 0.0f, -450), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(900.0f, 0.0f, -450), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(1000.0f, 200.0f, -450.0f), D3DXVECTOR3(800.0f, 0.0f, -450.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1000.0f, 200.0f, -450.0f), D3DXVECTOR3(800.0f, 0.0f, -450.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(1000, 0.0f, -550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(1000, 0.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(1000.0f, 200.0f, -650.0f), D3DXVECTOR3(1000.0f, 0.0f, -450.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(1000.0f, 200.0f, -650.0f), D3DXVECTOR3(1000.0f, 0.0f, -450.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(800, 0.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(800, 0.0f, -550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(800.0f, 200.0f, -450.0f), D3DXVECTOR3(800.0f, 0.0f, -650.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(800.0f, 200.0f, -450.0f), D3DXVECTOR3(800.0f, 0.0f, -650.0f), 2);
			}
			//3号柱
			{
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-900.0f, 0.0f, -50), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-900.0f, 0.0f, -50), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 200.0f, -50.0f), D3DXVECTOR3(-800.0f, 0.0f, -50.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 200.0f, -50.0f), D3DXVECTOR3(-800.0f, 0.0f, -50.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-900.0f, 0.0f, 150), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-900.0f, 0.0f, 150), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-800.0f, 200.0f, 150.0f), D3DXVECTOR3(-1000.0f, 0.0f, 150.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-800.0f, 200.0f, 150.0f), D3DXVECTOR3(-1000.0f, 0.0f, 150.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-800, 0.0f, 50.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-800, 0.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-800.0f, 200.0f, -50.0f), D3DXVECTOR3(-800.0f, 0.0f, 150.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-800.0f, 200.0f, -50.0f), D3DXVECTOR3(-800.0f, 0.0f, 150.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-1000, 0.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1000, 0.0f, 50.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-1000.0f, 200.0f, 150.0f), D3DXVECTOR3(-1000.0f, 0.0f, -50.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-1000.0f, 200.0f, 150.0f), D3DXVECTOR3(-1000.0f, 0.0f, -50.0f), 2);

			}
			//4号柱
			{
				//下向の壁
				InitMeshWall(D3DXVECTOR3(900.0f, 0.0f, -50), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(900.0f, 0.0f, -50), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(800.0f, 200.0f, -50.0f), D3DXVECTOR3(1000.0f, 0.0f, -50.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(900.0f, 0.0f, 150), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(900.0f, 0.0f, 150), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(1000.0f, 200.0f, 150.0f), D3DXVECTOR3(800.0f, 0.0f, 150.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(1000, 0.0f, 50.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(1000, 0.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(1000.0f, 200.0f, -50.0f), D3DXVECTOR3(1000.0f, 0.0f, 150.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(800, 0.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(800, 0.0f, 50.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(800.0f, 200.0f, 150.0f), D3DXVECTOR3(800.0f, 0.0f, -50.0f), 2);
			}
			//5号柱
			{
				//下向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -350), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 0.0f, -350), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 200.0f, -350.0f), D3DXVECTOR3(100.0f, 0.0f, -350.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -150), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 0.0f, -150), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 200.0f, -150.0f), D3DXVECTOR3(-100.0f, 0.0f, -150.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(100, 0.0f, -250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(100, 0.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 200.0f, -350.0f), D3DXVECTOR3(100.0f, 0.0f, -150.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-100, 0.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f);
				InitMeshWallReverse(D3DXVECTOR3(-100, 0.0f, -250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 200.0f, -150.0f), D3DXVECTOR3(-100.0f, 0.0f, -350.0f), 2);
			}

		}
		//2階
		{
			//2階の地面
			{

				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 10, 100.0f, 100.0f, -1500, 250, 250.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 30, 10, 100.0f, 100.0f, -1500, 750, 250.0f);
				SaveFieldPoint(D3DXVECTOR3(-1500.0f, 250.0f, 250.0f), D3DXVECTOR3(1500.0f, 250.0f, -750.0f));
				SaveFieldPointReverse(D3DXVECTOR3(-1500.0f, 250.0f, 250.0f), D3DXVECTOR3(1500.0f, 250.0f, -750.0f));
				//天井


				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 10, 100.0f, 100.0f, 1500, 250);
				InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 30, 10, 100.0f, 100.0f, -1500, 250);
				SaveCeilingPoint(D3DXVECTOR3(1500.0f, 200.0f, 250.0f), D3DXVECTOR3(-1500.0f, 200.0f, -750.0f));
				SaveCeilingPointReverse(D3DXVECTOR3(1500.0f, 200.0f, 250.0f), D3DXVECTOR3(-1500.0f, 200.0f, -750.0f));

				//
				InitMeshWall(D3DXVECTOR3(0.0f, 200.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 5, 100.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 200.0f, -750.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 30, 5, 100.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 250.0f, -750.0f), D3DXVECTOR3(1500.0f, 200.0f, -750.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1500.0f, 250.0f, -750.0f), D3DXVECTOR3(1500.0f, 200.0f, -750.0f), 3);

				InitMeshWall(D3DXVECTOR3(0.0f, 200.0f, 250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 5, 100.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 200.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 30, 5, 100.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 250.0f, 250.0f), D3DXVECTOR3(-1500.0f, 200.0f, 250.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1500.0f, 250.0f, 250.0f), D3DXVECTOR3(-1500.0f, 200.0f, 250.0f), 4);

			}

			//1号柱
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, -1500, -150, 500.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(-1500.0f, 500.0f, -150.0f), D3DXVECTOR3(-1300.0f, 500.0f, -350.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-1400.0f, 250.0f, -350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1400.0f, 250.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-1500.0f, 500.0f, -350.0f), D3DXVECTOR3(-1300.0f, 250.0f, -350.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1500.0f, 500.0f, -350.0f), D3DXVECTOR3(-1300.0f, 250.0f, -350.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1400.0f, 250.0f, -150.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1400.0f, 250.0f, -150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-1300.0f, 500.0f, -150.0f), D3DXVECTOR3(-1500.0f, 250.0f, -150.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-1300.0f, 500.0f, -150.0f), D3DXVECTOR3(-1500.0f, 250.0f, -150.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-1300, 250.0f, -250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1300, 250.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-1300.0f, 500.0f, -350.0f), D3DXVECTOR3(-1300.0f, 250.0f, -150.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-1300.0f, 500.0f, -350.0f), D3DXVECTOR3(-1300.0f, 250.0f, -150.0f), 1);

			}
			//2号柱
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, -450, 250, 500.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(-450.0f, 500.0f, 250.0f), D3DXVECTOR3(-250.0f, 500.0f, 50.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-350.0f, 250.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-350.0f, 250.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-450.0f, 500.0f, 50.0f), D3DXVECTOR3(-250.0f, 250.0f, 50.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-450.0f, 500.0f, 50.0f), D3DXVECTOR3(-250.0f, 250.0f, 50.0f), 3);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(-250.0f, 250.0f, 150.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-250.0f, 250.0f, 150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-250.0f, 500.0f, 50.0f), D3DXVECTOR3(-250.0f, 250.0f, 250.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-250.0f, 500.0f, 50.0f), D3DXVECTOR3(-250.0f, 250.0f, 250.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-450, 250.0f, 150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-450, 250.0f, 150.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-450.0f, 500.0f, 250.0f), D3DXVECTOR3(-450.0f, 250.0f, 50.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-450.0f, 500.0f, 250.0f), D3DXVECTOR3(-450.0f, 250.0f, 50.0f), 2);
			}
			//3号柱
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, 250, 250, 500.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(250.0f, 500.0f, 250.0f), D3DXVECTOR3(400.0f, 500.0f, 50.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(350.0f, 250.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(350.0f, 250.0f, 50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(250.0f, 500.0f, 50.0f), D3DXVECTOR3(450.0f, 250.0f, 50.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(250.0f, 500.0f, 50.0f), D3DXVECTOR3(450.0f, 250.0f, 50.0f), 3);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(450.0f, 250.0f, 150.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(450.0f, 250.0f, 150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(450.0f, 500.0f, 50.0f), D3DXVECTOR3(450.0f, 250.0f, 250.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(450.0f, 500.0f, 50.0f), D3DXVECTOR3(450.0f, 250.0f, 250.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(250, 250.0f, 150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(250, 250.0f, 150.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(250.0f, 500.0f, 250.0f), D3DXVECTOR3(250.0f, 250.0f, 50.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(250.0f, 500.0f, 250.0f), D3DXVECTOR3(250.0f, 250.0f, 50.0f), 2);
			}
			//4号柱
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, -100, -500, 500.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(-100.0f, 500.0f, -500.0f), D3DXVECTOR3(100.0f, 500.0f, -700.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 250.0f, -700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 250.0f, -700.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 500.0f, -700.0f), D3DXVECTOR3(100.0f, 250.0f, -700.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 500.0f, -700.0f), D3DXVECTOR3(100.0f, 250.0f, -700.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 250.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 250.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 500.0f, -500.0f), D3DXVECTOR3(-100.0f, 250.0f, -500.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 500.0f, -500.0f), D3DXVECTOR3(-100.0f, 250.0f, -500.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(100, 250.0f, -600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(100, 250.0f, -600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 500.0f, -700.0f), D3DXVECTOR3(100.0f, 250.0f, -500.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 500.0f, -700.0f), D3DXVECTOR3(100.0f, 250.0f, -500.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-100, 250.0f, -600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(-100, 250.0f, -600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 500.0f, -500.0f), D3DXVECTOR3(-100.0f, 250.0f, -700.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 500.0f, -500.0f), D3DXVECTOR3(-100.0f, 250.0f, -700.0f), 2);
			}
			//5号柱
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(1300.0f, 500.0f, -150.0f), D3DXVECTOR3(1500.0f, 500.0f, -350.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(1400.0f, 250.0f, -350), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1400.0f, 250.0f, -350), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(1300.0f, 500.0f, -350.0f), D3DXVECTOR3(1500.0f, 250.0f, -350.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(1300.0f, 500.0f, -350.0f), D3DXVECTOR3(1500.0f, 250.0f, -350.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(1400.0f, 250.0f, -150), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1400.0f, 250.0f, -150), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 500.0f, -150.0f), D3DXVECTOR3(1300.0f, 250.0f, -150.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1500.0f, 500.0f, -150.0f), D3DXVECTOR3(1300.0f, 250.0f, -150.0f), 4);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(1300, 250.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 10, 10, 20.0f, 25.0f);
				InitMeshWallReverse(D3DXVECTOR3(1300, 250.0f, -250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 10, 10, 20.0f, 25.0f);
				SaveWallPoint(D3DXVECTOR3(1300.0f, 500.0f, -150.0f), D3DXVECTOR3(1300.0f, 250.0f, -350.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(1300.0f, 500.0f, -150.0f), D3DXVECTOR3(1300.0f, 250.0f, -350.0f), 2);
			}
			//1号足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 7, 5, 100.0f, 100.0f, -1100, -50, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(-1100.0f, 300.0f, -50.0f), D3DXVECTOR3(-400.0f, 300.0f, -550.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-750.0f, 250.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 7, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(-750.0f, 250.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 7, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(-1100.0f, 300.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-1100.0f, 300.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-750.0f, 250.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 7, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(-750.0f, 250.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 7, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-1100.0f, 250.0f, -50.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-1100.0f, 250.0f, -50.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-400, 250.0f, -300.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(-400, 250.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-1100, 250.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1100, 250.0f, -300.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(-1100.0f, 300.0f, -50.0f), D3DXVECTOR3(-1100.0f, 250.0f, -550.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-1100.0f, 300.0f, -50.0f), D3DXVECTOR3(-1100.0f, 250.0f, -550.0f), 2);
			}
			//2号足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 7, 5, 100.0f, 100.0f, 400, -50, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(1100.0f, 300.0f, -550.0f));
				//下向の壁
				InitMeshWall(D3DXVECTOR3(750.0f, 250.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 7, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(750.0f, 250.0f, -550.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 7, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(1100.0f, 250.0f, -550.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(1100.0f, 250.0f, -550.0f), 3);
				//上向の壁
				InitMeshWall(D3DXVECTOR3(750.0f, 250.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 7, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(750.0f, 250.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 7, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(1100.0f, 300.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1100.0f, 300.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(1100, 250.0f, -300.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(1100, 250.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(1100.0f, 300.0f, -550.0f), D3DXVECTOR3(1100.0f, 250.0f, -50.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(1100.0f, 300.0f, -550.0f), D3DXVECTOR3(1100.0f, 250.0f, -50.0f), 1);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(400, 250.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 100.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(400, 250.0f, -300.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 100.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f), 2);
			}
			//3号足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5, 5, 50.0f, 50.0f, 1250, -1250, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(1250.0f, 300.0f, -1250.0f), D3DXVECTOR3(1500.0f, 300.0f, -1500.0f));
				//上向の壁
				InitMeshWall(D3DXVECTOR3(1375.0f, 250.0f, -1250.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 50.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(1375.0f, 250.0f, -1250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 50.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(1500.0f, 300.0f, -1250.0f), D3DXVECTOR3(1250.0f, 250.0f, -1250.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(1500.0f, 300.0f, -1250.0f), D3DXVECTOR3(1250.0f, 250.0f, -1250.0f), 4);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(1250, 250.0f, -1375.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f), 5, 1, 50.0f, 50.0f);
				InitMeshWallReverse(D3DXVECTOR3(1250, 250.0f, -1375.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.25f), 5, 1, 50.0f, 50.0f);
				SaveWallPoint(D3DXVECTOR3(1250.0f, 300.0f, -1250.0f), D3DXVECTOR3(1250.0f, 250.0f, -1500.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(1250.0f, 300.0f, -1250.0f), D3DXVECTOR3(1250.0f, 250.0f, -1500.0f), 2);
			}
			//4号足場
			{
				//
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 10, 30.0f, 20.0f, 600, 1500, 460.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(600.0f, 460.0f, 1500.0f), D3DXVECTOR3(1500.0f, 460.0f, 1300.0f));
				//下向の壁左
				InitMeshWall(D3DXVECTOR3(775.0f, -180.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 7, 8, 50.0f, 80.0f);
				InitMeshWallReverse(D3DXVECTOR3(775.0f, -180.0f, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 7, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(600.0f, 460.0f, 1300.0f), D3DXVECTOR3(950.0f, -180.0f, 1300.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(600.0f, 460.0f, 1300.0f), D3DXVECTOR3(950.0f, -180.0f, 1300.0f), 3);
				//下向の壁中下
				InitMeshWall(D3DXVECTOR3(1050.0f, -200.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 6, 50.0f, 80.0f);
				InitMeshWallReverse(D3DXVECTOR3(1050.0f, -200.0f, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 4, 6, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(950.0f, 280.0f, 1300.0f), D3DXVECTOR3(1150.0f, -200.0f, 1300.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(950.0f, 280.0f, 1300.0f), D3DXVECTOR3(1150.0f, -200.0f, 1300.0f), 3);
				//下向の壁中上
				InitMeshWall(D3DXVECTOR3(1050.0f, 380.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4, 1, 50.0f, 80.0f);
				InitMeshWallReverse(D3DXVECTOR3(1050.0f, -180.0f, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 4, 1, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(950.0f, 460.0f, 1300.0f), D3DXVECTOR3(1150.0f, 380.0f, 1300.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(950.0f, 460.0f, 1300.0f), D3DXVECTOR3(1150.0f, 380.0f, 1300.0f), 3);
				//下向の壁右
				InitMeshWall(D3DXVECTOR3(1325.0f, -180.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 7, 8, 50.0f, 80.0f);
				InitMeshWallReverse(D3DXVECTOR3(1325.0f, -180.0f, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 7, 8, 50.0f, 80.0f);
				SaveWallPoint(D3DXVECTOR3(1150.0f, 460.0f, 1300.0f), D3DXVECTOR3(1500.0f, -180.0f, 1300.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(1150.0f, 460.0f, 1300.0f), D3DXVECTOR3(1500.0f, -180.0f, 1300.0f), 3);
				//真ん中

				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f, 950, 1500, 280.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 20.0f, 20.0f, 1300, -150, 500.0f);
				SaveFieldPoint(D3DXVECTOR3(950.0f, 280.0f, 1500.0f), D3DXVECTOR3(1150.0f, 280.0f, 1300.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 380.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 10, 10, 20.0f, 20.0f, 1150, 1500);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 380.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 10, 10, 20.0f, 20.0f, 950, 1500);
				SaveCeilingPoint(D3DXVECTOR3(1150.0f, 380.0f, 1500.0f), D3DXVECTOR3(950.0f, 380.0f, 1300.0f));
				//左向の壁
				InitMeshWall(D3DXVECTOR3(1150.0f, 280.0f, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 10, 10, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(1150.0f, 280.0f, 1400.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 10, 10, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(1150.0f, 380.0f, 1500.0f), D3DXVECTOR3(1150.0f, 280.0f, 1300.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(1150.0f, 380.0f, 1500.0f), D3DXVECTOR3(1150.0f, 280.0f, 1300.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(950.0f, 280.0f, 1400.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 10, 10, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(950.0f, 280.0f, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 10, 10, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(950.0f, 380.0f, 1300.0f), D3DXVECTOR3(950.0f, 280.0f, 1500.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(950.0f, 380.0f, 1300.0f), D3DXVECTOR3(950.0f, 280.0f, 1500.0f), 1);
				//下向の壁
				InitMeshWall(D3DXVECTOR3(1050.0f, 280.0f, 1499.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 10, 10, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(1050.0f, 280.0f, 1499.9f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.25f), 10, 10, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(950.0f, 380.0f, 1499.9f), D3DXVECTOR3(1050.0f, 280.0f, 1499.9f), 3);
				SaveWallPointReverse(D3DXVECTOR3(950.0f, 380.0f, 1499.9f), D3DXVECTOR3(1050.0f, 280.0f, 1499.9f), 3);

			}
			//1号坂道
			{

				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 100, 5.0f, 5.0f, -400, -50, 250, 300, 2);
				SaveFieldSlopePoint(D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), 2);

				//上
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f), 2, 2);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 300.0f, -50.0f), D3DXVECTOR3(-350.0f, 250.0f, -50.0f), D3DXVECTOR3(-400.0f, 250.0f, -50.0f), 2, 2);
				//下
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), 1, 2);
				SaveSlopeWallPointReverse(D3DXVECTOR3(-400.0f, 300.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), D3DXVECTOR3(-400.0f, 250.0f, -550.0f), D3DXVECTOR3(-350.0f, 250.0f, -550.0f), 1, 2);
			}
			//2号坂道
			{

				InitMeshSlope(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 100, 5.0f, 5.0f, 350, -50, 250, 300, 1);
				SaveFieldSlopePoint(D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 300.0f, -550.0f), 1);

				//上
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), 1, 1);
				SaveSlopeWallPointReverse(D3DXVECTOR3(400.0f, 300.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), D3DXVECTOR3(400.0f, 250.0f, -50.0f), D3DXVECTOR3(350.0f, 250.0f, -50.0f), 1, 1);
				//下
				InitSlopeWall(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				InitSlopeWallReverse(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f)
					, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
				SaveSlopeWallPoint(D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f), 2, 1);
				SaveSlopeWallPointReverse(D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 300.0f, -550.0f), D3DXVECTOR3(350.0f, 250.0f, -550.0f), D3DXVECTOR3(400.0f, 250.0f, -550.0f), 2, 1);
			}
			//1号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, -1500, -1400, 400.0f);
				InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, 1400, -1400, 400.0f);
				SaveFieldPoint(D3DXVECTOR3(-1500.0f, 400.0f, -1400.0f), D3DXVECTOR3(-1400.0f, 400.0f, -1500.0f));
				SaveFieldPointReverse(D3DXVECTOR3(-1500.0f, 400.0f, -1400.0f), D3DXVECTOR3(-1400.0f, 400.0f, -1500.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, -1400, -1400);
				InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-1400.0f, 350.0f, -1400.0f), D3DXVECTOR3(-1500.0f, 350.0f, -1500.0f));
				SaveCeilingPointReverse(D3DXVECTOR3(-1400.0f, 350.0f, -1400.0f), D3DXVECTOR3(-1500.0f, 350.0f, -1500.0f));

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-1450.0f, 350.0f, -1400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1450.0f, 350.0f, -1400), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-1400.0f, 400.0f, -1400.0f), D3DXVECTOR3(-1500.0f, 350.0f, -1400.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-1400.0f, 400.0f, -1400.0f), D3DXVECTOR3(-1500.0f, 350.0f, -1400.0f), 4);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-1400.0f, 350.0f, -1450.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-1400.0f, 350.0f, -1450.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-1400.0f, 400.0f, -1500.0f), D3DXVECTOR3(-1400.0f, 350.0f, -1400.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-1400.0f, 400.0f, -1500.0f), D3DXVECTOR3(-1400.0f, 350.0f, -1400.0f), 1);

			}
			//2号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, -550, 700, 150.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-550.0f, 150.0f, 700.0f), D3DXVECTOR3(-400.0f, 150.0f, 400.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, -400, 700);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-400.0f, 100.0f, 700.0f), D3DXVECTOR3(-550.0f, 100.0f, 400.0f));
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 100.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 100.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 150.0f, 700.0f), D3DXVECTOR3(-550.0f, 100.0f, 700.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 150.0f, 700.0f), D3DXVECTOR3(-550.0f, 100.0f, 700.0f), 4);
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-550.0f, 150.0f, 400.0f), D3DXVECTOR3(-400.0f, 100.0f, 400.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-550.0f, 150.0f, 400.0f), D3DXVECTOR3(-400.0f, 100.0f, 400.0f), 3);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-400.0f, 100.0f, 550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 15, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-400.0f, 100.0f, 550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 15, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 150.0f, 400.0f), D3DXVECTOR3(-400.0f, 100.0f, 700.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 150.0f, 400.0f), D3DXVECTOR3(-400.0f, 100.0f, 700.0f), 1);

			}
			//3号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 40.0f, -550, 1100, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-550.0f, 300.0f, 1100.0f), D3DXVECTOR3(-400.0f, 300.0f, 700.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 40.0f, -400, 1100);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-400.0f, 250.0f, 1100.0f), D3DXVECTOR3(-550.0f, 250.0f, 700.0f));
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 250.0f, 1100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 250.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 300.0f, 1100.0f), D3DXVECTOR3(-550.0f, 250.0f, 1100.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 300.0f, 1100.0f), D3DXVECTOR3(-550.0f, 250.0f, 1100.0f), 4);
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 250.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 250.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-550.0f, 300.0f, 700.0f), D3DXVECTOR3(-400.0f, 250.0f, 700.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-550.0f, 300.0f, 700.0f), D3DXVECTOR3(-400.0f, 250.0f, 700.0f), 3);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-400.0f, 250.0f, 900.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-400.0f, 250.0f, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 20, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 300.0f, 700.0f), D3DXVECTOR3(-400.0f, 250.0f, 1100.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 300.0f, 700.0f), D3DXVECTOR3(-400.0f, 250.0f, 1100.0f), 1);
			}
			//4号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, -550, 1400, 150.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-550.0f, 150.0f, 1400.0f), D3DXVECTOR3(-400.0f, 150.0f, 1100.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, -400, 1400);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-400.0f, 100.0f, 1400.0f), D3DXVECTOR3(-550.0f, 100.0f, 1100.0f));
				//上向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 100.0f, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 100.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 150.0f, 1400.0f), D3DXVECTOR3(-550.0f, 100.0f, 1400.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 150.0f, 1400.0f), D3DXVECTOR3(-550.0f, 100.0f, 1400.0f), 4);
				//下向の壁
				InitMeshWall(D3DXVECTOR3(-475.0f, 100.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-475.0f, 100.0f, 1100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-550.0f, 150.0f, 1100.0f), D3DXVECTOR3(-400.0f, 100.0f, 1100.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-550.0f, 150.0f, 1100.0f), D3DXVECTOR3(-400.0f, 100.0f, 1100.0f), 3);
				//右向の壁
				InitMeshWall(D3DXVECTOR3(-400.0f, 100.0f, 1250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 15, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-400.0f, 100.0f, 1250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 15, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-400.0f, 150.0f, 1100.0f), D3DXVECTOR3(-400.0f, 100.0f, 1400.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-400.0f, 150.0f, 1100.0f), D3DXVECTOR3(-400.0f, 100.0f, 1400.0f), 1);

			}
			//5号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, -300, 400, 100.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-300.0f, 100.0f, 400.0f), D3DXVECTOR3(-200.0f, 100.0f, 300.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, -200, 400);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-200.0f, 50.0f, 400.0f), D3DXVECTOR3(-300.0f, 50.0f, 300.0f));

				//上向の壁
				InitMeshWall(D3DXVECTOR3(-250.0f, 50.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-250.0f, 50.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-200.0f, 100.0f, 400.0f), D3DXVECTOR3(-300.0f, 50.0f, 400.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(-200.0f, 100.0f, 400.0f), D3DXVECTOR3(-300.0f, 50.0f, 400.0f), 4);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-300.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-300.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-300.0f, 100.0f, 400.0f), D3DXVECTOR3(-300.0f, 50.0f, 300.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-300.0f, 100.0f, 400.0f), D3DXVECTOR3(-300.0f, 50.0f, 300.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(-200.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-200.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-200.0f, 100.0f, 300.0f), D3DXVECTOR3(-200.0f, 50.0f, 400.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-200.0f, 100.0f, 300.0f), D3DXVECTOR3(-200.0f, 50.0f, 400.0f), 1);

			}
			//6号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, -300, 1500, 100.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-300.0f, 100.0f, 1500.0f), D3DXVECTOR3(-100.0f, 100.0f, 1350.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, -100, 1500);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(-100.0f, 50.0f, 1500.0f), D3DXVECTOR3(-300.0f, 50.0f, 1350.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(-200.0f, 50.0f, 1350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-200.0f, 50.0f, 1350.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-300.0f, 100.0f, 1350.0f), D3DXVECTOR3(-100.0f, 50.0f, 1350.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-300.0f, 100.0f, 1350.0f), D3DXVECTOR3(-100.0f, 50.0f, 1350.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(-300, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-300, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-300.0f, 100.0f, 1500.0f), D3DXVECTOR3(-300.0f, 50.0f, 1350.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-300.0f, 100.0f, 1500.0f), D3DXVECTOR3(-300.0f, 50.0f, 1350.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(-100.0f, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-100.0f, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 100.0f, 1350.0f), D3DXVECTOR3(-100.0f, 50.0f, 1500.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 100.0f, 1350.0f), D3DXVECTOR3(-100.0f, 50.0f, 1500.0f), 1);
			}
			//7号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, 200, 400, 100.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(200.0f, 100.0f, 400.0f), D3DXVECTOR3(300.0f, 100.0f, 300.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 10.0f, 10.0f, 300, 400);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(300.0f, 50.0f, 400.0f), D3DXVECTOR3(200.0f, 50.0f, 300.0f));


				//上向の壁
				InitMeshWall(D3DXVECTOR3(250.0f, 50.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(250.0f, 50.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(300.0f, 100.0f, 400.0f), D3DXVECTOR3(200.0f, 50.0f, 400.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(300.0f, 100.0f, 400.0f), D3DXVECTOR3(200.0f, 50.0f, 400.0f), 4);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(200.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(200.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(200.0f, 100.0f, 400.0f), D3DXVECTOR3(200.0f, 50.0f, 300.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(200.0f, 100.0f, 400.0f), D3DXVECTOR3(200.0f, 50.0f, 300.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(300.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 10.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(300.0f, 50.0f, 350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 10.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(300.0f, 100.0f, 300.0f), D3DXVECTOR3(300.0f, 50.0f, 400.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(300.0f, 100.0f, 300.0f), D3DXVECTOR3(300.0f, 50.0f, 400.0f), 1);

			}
			//8号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, 100, 1500, 100.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(100.0f, 100.0f, 1500.0f), D3DXVECTOR3(300.0f, 100.0f, 1350.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, 300, 1500);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(300.0f, 50.0f, 1500.0f), D3DXVECTOR3(100.0f, 50.0f, 1350.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(200.0f, 50.0f, 1350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(200.0f, 50.0f, 1350.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 100.0f, 1350.0f), D3DXVECTOR3(300.0f, 50.0f, 1350.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 100.0f, 1350.0f), D3DXVECTOR3(300.0f, 50.0f, 1350.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(100, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(100, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 100.0f, 1500.0f), D3DXVECTOR3(100.0f, 50.0f, 1350.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 100.0f, 1500.0f), D3DXVECTOR3(100.0f, 50.0f, 1350.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(300.0f, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(300.0f, 50.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(300.0f, 100.0f, 1350.0f), D3DXVECTOR3(300.0f, 50.0f, 1500.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(300.0f, 100.0f, 1350.0f), D3DXVECTOR3(300.0f, 50.0f, 1500.0f), 1);
			}
			//9号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, 400, 700, 150.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(400.0f, 150.0f, 700.0f), D3DXVECTOR3(550.0f, 150.0f, 400.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, 550, 700);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(550.0f, 100.0f, 700.0f), D3DXVECTOR3(400.0f, 100.0f, 400.0f));


				//上向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 100.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 100.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 150.0f, 700.0f), D3DXVECTOR3(400.0f, 100.0f, 700.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(550.0f, 150.0f, 700.0f), D3DXVECTOR3(400.0f, 100.0f, 700.0f), 4);

				//下向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 150.0f, 400.0f), D3DXVECTOR3(550.0f, 100.0f, 400.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 150.0f, 400.0f), D3DXVECTOR3(550.0f, 100.0f, 400.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(400.0f, 100.0f, 550.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 30.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(400.0f, 100.0f, 550.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 30.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 150.0f, 700.0f), D3DXVECTOR3(400.0f, 100.0f, 400.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 150.0f, 700.0f), D3DXVECTOR3(400.0f, 100.0f, 400.0f), 2);
			}
			//10号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 20, 15.0f, 20.0f, 400, 1100, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(400.0f, 300.0f, 1100.0f), D3DXVECTOR3(550.0f, 300.0f, 700.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 20, 15.0f, 20.0f, 550, 1100);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(550.0f, 250.0f, 1100.0f), D3DXVECTOR3(400.0f, 250.0f, 700.0f));


				//上向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 250.0f, 1100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 250.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 300.0f, 1100.0f), D3DXVECTOR3(400.0f, 250.0f, 1100.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(550.0f, 300.0f, 1100.0f), D3DXVECTOR3(400.0f, 250.0f, 1100.0f), 4);

				//下向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 250.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 250.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 300.0f, 700.0f), D3DXVECTOR3(550.0f, 250.0f, 700.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 300.0f, 700.0f), D3DXVECTOR3(550.0f, 250.0f, 700.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(400.0f, 250.0f, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(400.0f, 250.0f, 900.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 20, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 300.0f, 1100.0f), D3DXVECTOR3(400.0f, 250.0f, 700.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 300.0f, 1100.0f), D3DXVECTOR3(400.0f, 250.0f, 700.0f), 2);
			}
			//11号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, 400, 1400, 150.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(400.0f, 150.0f, 1400.0f), D3DXVECTOR3(550.0f, 150.0f, 1100.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 15.0f, 30.0f, 550, 700);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(550.0f, 100.0f, 1400.0f), D3DXVECTOR3(400.0f, 100.0f, 1100.0f));


				//上向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 100.0f, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 100.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(550.0f, 150.0f, 1400.0f), D3DXVECTOR3(400.0f, 100.0f, 1400.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(550.0f, 150.0f, 1400.0f), D3DXVECTOR3(400.0f, 100.0f, 1400.0f), 4);

				//下向の壁
				InitMeshWall(D3DXVECTOR3(475.0f, 100.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(475.0f, 100.0f, 1100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 150.0f, 1100.0f), D3DXVECTOR3(550.0f, 100.0f, 1100.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 150.0f, 1100.0f), D3DXVECTOR3(550.0f, 100.0f, 1100.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(400.0f, 100.0f, 1250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 30.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(400.0f, 100.0f, 1250.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 30.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(400.0f, 150.0f, 1400.0f), D3DXVECTOR3(400.0f, 100.0f, 1100.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(400.0f, 150.0f, 1400.0f), D3DXVECTOR3(400.0f, 100.0f, 1100.0f), 2);
			}
			//12号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, -100, 450, 300.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-100.0f, 300.0f, 450.0f), D3DXVECTOR3(100.0f, 300.0f, 300.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, 100, 450);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(100.0f, 250.0f, 450.0f), D3DXVECTOR3(-100.0f, 250.0f, 300.0f));


				//上向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 250.0f, 450.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 250.0f, 450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 300.0f, 450.0f), D3DXVECTOR3(-100.0f, 250.0f, 450.0f), 4);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 300.0f, 450.0f), D3DXVECTOR3(-100.0f, 250.0f, 450.0f), 4);

				//左向の壁
				InitMeshWall(D3DXVECTOR3(-100.0f, 250.0f, 375.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-100.0f, 250.0f, 375.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 300.0f, 450.0f), D3DXVECTOR3(-100.0f, 250.0f, 300.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 300.0f, 450.0f), D3DXVECTOR3(-100.0f, 250.0f, 300.0f), 2);

				//右向の壁
				InitMeshWall(D3DXVECTOR3(100.0f, 250.0f, 375.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(100.0f, 250.0f, 375.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 300.0f, 300.0f), D3DXVECTOR3(100.0f, 250.0f, 450.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 300.0f, 300.0f), D3DXVECTOR3(100.0f, 250.0f, 450.0f), 1);

			}
			//13号空中足場
			{
				InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, -100, 1500, 200.0f);
				//InitMeshFieldReverse(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -550, -625, 150.0f);
				SaveFieldPoint(D3DXVECTOR3(-100.0f, 200.0f, 1500.0f), D3DXVECTOR3(100.0f, 200.0f, 1350.0f));

				InitMeshFieldCeiling(D3DXVECTOR3(0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 15, 20.0f, 10.0f, 100, 1500);
				//InitMeshFieldCeilingReverse(D3DXVECTOR3(0.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 10, 10.0f, 10.0f, -1500, -1400);
				SaveCeilingPoint(D3DXVECTOR3(100.0f, 150.0f, 1500.0f), D3DXVECTOR3(-100.0f, 150.0f, 1350.0f));

				//下向の壁
				InitMeshWall(D3DXVECTOR3(0.0f, 150.0f, 1350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 20.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(0.0f, 150.0f, 1350.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 20.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 200.0f, 1350.0f), D3DXVECTOR3(100.0f, 150.0f, 1350.0f), 3);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 200.0f, 1350.0f), D3DXVECTOR3(100.0f, 150.0f, 1350.0f), 3);
				//左向の壁
				InitMeshWall(D3DXVECTOR3(-100, 150.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(-100, 150.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(-100.0f, 200.0f, 1500.0f), D3DXVECTOR3(-100.0f, 150.0f, 1350.0f), 2);
				SaveWallPointReverse(D3DXVECTOR3(-100.0f, 200.0f, 1500.0f), D3DXVECTOR3(-100.0f, 150.0f, 1350.0f), 2);
				

				//右向の壁
				InitMeshWall(D3DXVECTOR3(100.0f, 150.0f, 1425.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 5, 15.0f, 10.0f);
				InitMeshWallReverse(D3DXVECTOR3(100.0f, 150.0f, 1425.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 10, 5, 15.0f, 10.0f);
				SaveWallPoint(D3DXVECTOR3(100.0f, 200.0f, 1350.0f), D3DXVECTOR3(100.0f, 150.0f, 1500.0f), 1);
				SaveWallPointReverse(D3DXVECTOR3(100.0f, 200.0f, 1350.0f), D3DXVECTOR3(100.0f, 150.0f, 1500.0f), 1);

			}
		}
		
	}
	
	//BattleareaのBOSSdoor
	//下向の壁
	InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 100.0f, 200.0f);
	SaveWallPoint(D3DXVECTOR3(-50.0f, 200.0f, 250.0f), D3DXVECTOR3(50.0f, 0.0f, 250.0f), 3);


	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMap(void)
{
	UnloadModel(&model);

	UninitMeshField();
	UninitBOSSField();
	UninitMeshField_Random();
	UninitMeshFieldCeiling();
	UninitSlopeWall();
	UninitMeshSlope();
	UninitMeshWall();
	UninitTriangleWall();
	UninitMeshFieldReverse();
	UninitMeshFieldCeilingReverse();
	UninitSlopeWallReverse();
	UninitMeshSlopeReverse();
	UninitMeshWallReverse();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap(void)
{
	


	// 地面処理の更新
	UpdateMeshField();

	// 地面処理の更新
	UpdateBOSSField();

	UpdateMeshField_Random();
	//天井の更新処理
	UpdateMeshFieldCeiling();

	UpdateSlopeWall();
		

	// 地面処理の更新
	UpdateMeshSlope();

	// 壁処理の更新
	UpdateMeshWall();


	// 地面処理の更新
	UpdateMeshFieldReverse();

	//天井の更新処理
	UpdateMeshFieldCeilingReverse();

	UpdateSlopeWallReverse();

	// 地面処理の更新
	UpdateMeshSlopeReverse();

	// 壁処理の更新
	UpdateMeshWallReverse();

	UpdateTriangleWall();

	
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawMap(void)
{
	switch (gamedata->modenow)
	{
	case MODE_TITLE:
	{

		DrawMeshMapRestarea();
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// カリング無効
			SetCullingMode(CULL_MODE_NONE);

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y + D3DX_PI, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);



				// ワールドマトリックスの設定
				SetWorldMatrix(&mtxWorld);

				// モデル描画
				DrawModel(&model);
		}
	}
		break;
	case MODE_RESTAREA:
	{
		DrawMeshMapRestarea();
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// カリング無効
			SetCullingMode(CULL_MODE_NONE);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y + D3DX_PI, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// モデル描画
			DrawModel(&model);
		}

	}
		break;
	case MODE_BATTLEAREA:
	{
		DrawMeshMapBattlearea();
	}
		break;
	case MODE_RESULT:
	{
		DrawMeshMapRestarea();
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// カリング無効
			SetCullingMode(CULL_MODE_NONE);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y + D3DX_PI, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// モデル描画
			DrawModel(&model);
		}
	}
		break;
	default:
		break;
	}
	

	
}




//=============================================================================
// 地面の頂点座標を保存する
//=============================================================================

//pos0=左上座標,pos3=右下座標
void SaveFieldPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;
	pos1.x = pos3.x;
	pos2.x = pos0.x;
	fieldpoint[g_nNumMeshField].pos0 = pos0;
	fieldpoint[g_nNumMeshField].pos1 = pos1;
	fieldpoint[g_nNumMeshField].pos2 = pos2;
	fieldpoint[g_nNumMeshField].pos3 = pos3;
	fieldpoint[g_nNumMeshField].posplayer = pos0;
	g_nNumMeshField++;
}

void SaveFieldPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;
	pos1.x = pos3.x;
	pos2.x = pos0.x;
	fieldpoint_reverse[g_nNumMeshField].pos0 = pos0;
	fieldpoint_reverse[g_nNumMeshField].pos1 = pos1;
	fieldpoint_reverse[g_nNumMeshField].pos2 = pos2;
	fieldpoint_reverse[g_nNumMeshField].pos3 = pos3;
	fieldpoint_reverse[g_nNumMeshField].posplayer = pos0;
	g_nNumMeshField_reverse++;
}

//=============================================================================
// 天井の頂点座標を保存する
//=============================================================================

//pos0=左上座標,pos3=右下座標
void SaveCeilingPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;
	pos1.x = pos3.x;
	pos2.x = pos0.x;
	ceilingpoint[g_nNumMeshCeiling].pos0 = pos0;
	ceilingpoint[g_nNumMeshCeiling].pos1 = pos1;
	ceilingpoint[g_nNumMeshCeiling].pos2 = pos2;
	ceilingpoint[g_nNumMeshCeiling].pos3 = pos3;
	ceilingpoint[g_nNumMeshCeiling].posplayer = pos0;
	g_nNumMeshCeiling++;
}

void SaveCeilingPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;
	pos1.x = pos3.x;
	pos2.x = pos0.x;
	ceilingpoint_reverse[g_nNumMeshCeiling].pos0 = pos0;
	ceilingpoint_reverse[g_nNumMeshCeiling].pos1 = pos1;
	ceilingpoint_reverse[g_nNumMeshCeiling].pos2 = pos2;
	ceilingpoint_reverse[g_nNumMeshCeiling].pos3 = pos3;
	ceilingpoint_reverse[g_nNumMeshCeiling].posplayer = pos0;
	g_nNumMeshCeiling_reverse++;
}
//=============================================================================
// 壁の頂点座標を保存する
//=============================================================================

//pos0=左上座標,pos3=右下座標		slopecheck=1 ->右向けの壁(左の壁);2 ->左向けの壁(右の壁);3 ->下向けの壁(上の壁);4 ->上向けの壁(下の壁)
void SaveWallPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3, int check)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;
	
	//if slopecheck=1 ->右向けの壁(左の壁)
	//if slopecheck=2 ->左向けの壁(右の壁)
	//if slopecheck=3 ->下向けの壁(上の壁)
	//if slopecheck=4 ->上向けの壁(下の壁)
	switch (check)
	{
	case 1:
		pos1.z = pos3.z;
		pos2.z = pos0.z;
		break;
	case 2:
		pos1.z = pos3.z;
		pos2.z = pos0.z;
		break;
	case 3:
		pos1.x = pos3.x;
		pos2.x = pos0.x;
		break;

	case 4:
		pos1.x = pos3.x;
		pos2.x = pos0.x;
		break;
	default:
		break;
	}
	
	Wallpoint[ g_nNumMeshWall].pos0 = pos0;
	Wallpoint[ g_nNumMeshWall].pos1 = pos1;
	Wallpoint[ g_nNumMeshWall].pos2 = pos2;
	Wallpoint[ g_nNumMeshWall].pos3 = pos3;
	Wallpoint[g_nNumMeshWall].check = check;
	g_nNumMeshWall++;
}

void SaveWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3, int check)
{

	D3DXVECTOR3 pos1 = pos0;
	D3DXVECTOR3 pos2 = pos3;

	//if slopecheck=1 ->右向けの壁(左の壁)
	//if slopecheck=2 ->左向けの壁(右の壁)
	//if slopecheck=3 ->下向けの壁(上の壁)
	//if slopecheck=4 ->上向けの壁(下の壁)
	switch (check)
	{
	case 1:
		pos1.z = pos3.z;
		pos2.z = pos0.z;
		break;
	case 2:
		pos1.z = pos3.z;
		pos2.z = pos0.z;
		break;
	case 3:
		pos1.x = pos3.x;
		pos2.x = pos0.x;
		break;

	case 4:
		pos1.x = pos3.x;
		pos2.x = pos0.x;
		break;
	default:
		break;
	}

	Wallpoint_reverse[g_nNumMeshWall].pos0 = pos0;
	Wallpoint_reverse[g_nNumMeshWall].pos1 = pos1;
	Wallpoint_reverse[g_nNumMeshWall].pos2 = pos2;
	Wallpoint_reverse[g_nNumMeshWall].pos3 = pos3;
	Wallpoint_reverse[g_nNumMeshWall].check = check;
	g_nNumMeshWall_reverse++;
}

//=============================================================================
// 坂道の地面の頂点座標を保存する
//=============================================================================

//pos0=左上座標,pos3=右下座標 if slopecheck=1 ->左下から右上の坂道;2 ->右下から左上の坂道;3 ->上上から下下の坂道;4 ->下上から上下の坂道
void SaveFieldSlopePoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3,int check)
{
	D3DXVECTOR3 pos1 = pos3;
	D3DXVECTOR3 pos2 = pos0;
	//if slopecheck=1 ->左下から右上の坂道
	//if slopecheck=2 ->右下から左上の坂道
	//if slopecheck=3 ->上上から下下の坂道
	//if slopecheck=4 ->下上から上下の坂道
	switch (check)
	{
	case 1:
		pos1.z = pos0.z;
		pos2.z = pos3.z;
		pos1.y = pos3.y;
		pos2.y = pos0.y;
		break;
		
	case 2:	
		pos1.z = pos0.z;
		pos2.z = pos3.z;
		pos1.y = pos3.y;
		pos2.y = pos0.y;
		break;
	case 3:
		pos1.z = pos0.z;
		pos2.z = pos3.z;
		pos1.y = pos0.y;
		pos2.y = pos3.y;
		break;
	case 4:
		pos1.z = pos0.z;
		pos2.z = pos3.z;
		pos1.y = pos0.y;
		pos2.y = pos3.y;
		break;
	default:
		break;
	}
	fieldslopepoint[g_nNumMeshFieldSlope].pos0 = pos0;
	fieldslopepoint[g_nNumMeshFieldSlope].pos1 = pos1;
	fieldslopepoint[g_nNumMeshFieldSlope].pos2 = pos2;
	fieldslopepoint[g_nNumMeshFieldSlope].pos3 = pos3;
	fieldslopepoint[g_nNumMeshFieldSlope].check = check;
	g_nNumMeshFieldSlope++;
	
}


//=============================================================================
// 坂道の壁の頂点座標を保存する
//=============================================================================

//pos0=左上座標,pos3=右下座標 check1=1->下から上見え左の壁;2->下から上見え右の壁	check2=1 ->左下から右上の坂道;2->右下から左上の坂道;3 ->上上から下下の坂道;4 ->下上から上下の坂道		
void SaveSlopeWallPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2)
{
	slopewallpoint[g_nNumMeshSlopeWall].pos0 = pos0;
	slopewallpoint[g_nNumMeshSlopeWall].pos1 = pos1;
	slopewallpoint[g_nNumMeshSlopeWall].pos2 = pos2;
	slopewallpoint[g_nNumMeshSlopeWall].pos3 = pos3;
		//if check2=1 ->左下から右上の坂道
		//if check2=2 ->右下から左上の坂道
		//if check2=3 ->上上から下下の坂道
		//if check2=4 ->下上から上下の坂道

		//if check1=1->下から上見え左の壁
		//if check1=2->下から上見え右の壁
	slopewallpoint[g_nNumMeshSlopeWall].check1 = check1;
	slopewallpoint[g_nNumMeshSlopeWall].check2 = check2;
	g_nNumMeshSlopeWall++;
	//if slopecheck=1 ->左下から右上の坂道
				//if slopecheck=2 ->右下から左上の坂道
				//if slopecheck=3 ->上上から下下の坂道
				//if slopecheck=4 ->下上から上下の坂道

}


void SaveSlopeWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2)
{
	slopewallpoint_reverse[g_nNumMeshSlopeWall].pos0 = pos0;
	slopewallpoint_reverse[g_nNumMeshSlopeWall].pos1 = pos1;
	slopewallpoint_reverse[g_nNumMeshSlopeWall].pos2 = pos2;
	slopewallpoint_reverse[g_nNumMeshSlopeWall].pos3 = pos3;
	//if check2=1 ->左下から右上の坂道
	//if check2=2 ->右下から左上の坂道
	//if check2=3 ->上上から下下の坂道
	//if check2=4 ->下上から上下の坂道

	//if check1=1->下から上見え左の壁
	//if check1=2->下から上見え右の壁
	slopewallpoint_reverse[g_nNumMeshSlopeWall].check1 = check1;
	slopewallpoint_reverse[g_nNumMeshSlopeWall].check2 = check2;
	g_nNumMeshSlopeWall_reverse++;
	//if slopecheck=1 ->左下から右上の坂道
				//if slopecheck=2 ->右下から左上の坂道
				//if slopecheck=3 ->上上から下下の坂道
				//if slopecheck=4 ->下上から上下の坂道

}



//=============================================================================
// 地面の情報を取得
//=============================================================================
MAP_POINT *GetFieldPoint(void)
{
	return &(fieldpoint[0]);
}

MAP_POINT *GetFieldPointReverse(void)
{
	return &(fieldpoint_reverse[0]);
}
//=============================================================================
// 天井の情報を取得
//=============================================================================
MAP_POINT *GetCeilingPoint(void)
{
	return &(ceilingpoint[0]);
}

MAP_POINT *GetCeilingPointReverse(void)
{
	return &(ceilingpoint_reverse[0]);
}
//=============================================================================
// 壁の情報を取得
//=============================================================================
MAP_POINT *GetWallPoint(void)
{
	return &(Wallpoint[0]);
}

MAP_POINT *GetWallPointReverse(void)
{
	return &(Wallpoint_reverse[0]);
}
//=============================================================================
// 坂道の地面の情報を取得
//=============================================================================
MAP_POINT *GetFieldSlopePoint(void)
{
	return &(fieldslopepoint[0]);
}

MAP_POINT *GetFieldSlopePointReverse(void)
{
	return &(fieldslopepoint_reverse[0]);
}
//=============================================================================
// 坂道の壁の情報を取得
//=============================================================================
MAP_POINT *GetSlopeWallPoint(void)
{
	return &(slopewallpoint[0]);
}

MAP_POINT *GetSlopeWallPointReverse(void)
{
	return &(slopewallpoint_reverse[0]);
}
