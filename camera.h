//=============================================================================
//
// カメラ処理 [camera.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <d3dx9.h>
#include "renderer.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CameraPosX_MAX	(1490.0f)
#define	CameraPosX_MIN	(-1490.0f)
#define	CameraPosZ_MAX	(1490.0f)
#define	CameraPosZ_MIN	(-1490.0f)
#define	CameraPosY_MAX	(630.0f)
#define	CameraPosY_MIN	(50.0f)

#define	CameraAtPosY_MAX	(100.0f)
#define	CameraAtPosY_MIN	(1.0f)

#define LENMAX		(300.0f);
#define	LENMIN		(100.0f);

#define	VIEW_ANGLE		(D3DXToRadian(55.0f))							// ビュー平面の視野角

#define MOUSE_SET_POINTX (500)
#define MOUSE_SET_POINTY (400)

#define camera_start_Ypos (30.0f)
#define camera_radius (100.0f)
#define camera_Ypos_center (camera_start_Ypos + camera_radius)
#define camera_at_radius (200.0f)
#define camera__at_Ypos_center (60.0f)
#define CAMERA_DISTANCE_MIN	(150.0f)
#define CAMERA_DISTANCE_MAX	(camera_radius+CAMERA_DISTANCE_MIN)
#define CAMERA_AT_DISTANCE (200.0f)
#define CAMERA_AT_ONEPOS_DISTANCE (200.0f)
#define CAMERA_POS_DISTANCE (9.0f)
#define ONECAMERA_YPOS	(60)
#define ONECAMERA_AT_YPOS_MAX	(500)
#define ONECAMERA_AT_YPOS_MIN	(300)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct CAMERA
{
	D3DXVECTOR3			pos;			// カメラの視点(位置)
	D3DXVECTOR3			at;				// カメラの注視点
	D3DXVECTOR3			playerpos;
	D3DXVECTOR3			up;				// カメラの上方向ベクトル
	D3DXVECTOR3			rot;			// カメラの回転
	float			xyrot_MAX;
	float				len;			// カメラの視点と注視点の距離

	D3DXMATRIX			mtxView;		// ビューマトリックス
	D3DXMATRIX			mtxInvView;		// ビューマトリックス
	D3DXMATRIX			mtxProjection;	// プロジェクションマトリックス

	bool				updown_check;
	bool				shoutchange;	//TRUE=普通の視点

	float				time_at;		//注視点用の時間
	float				time_eye;		//注視点用の時間

	D3DXVECTOR3			left_pos;
	D3DXVECTOR3			right_pos;
	D3DXVECTOR3			top_pos;
	D3DXVECTOR3			down_pos;

	D3DXVECTOR3			buying_pos;
	D3DXVECTOR3			buying_move;
	D3DXVECTOR3			buying_at_move;

};

enum {
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_NONE,
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA *GetCamera(void);

void SetViewPort(int type);
int GetViewPortType(void);