//=============================================================================
//
// 他のモデル処理 [other_objects.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_KEY			(5)
#define KEY_RADIUS		(15)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct 
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3			touchsize;
	DX11_MODEL			model;				// モデル情報
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	float				rotspd;				// 回転スピード

	int					shadowIdx;			// 影のインデックス番号

	bool				use;

	//float				radius;				//当たり判定用の半径

	float				time;				//線形補間の時間計算用
	float				time_plue;
}OBJECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitOtherobjects(void);
void UninitOtherobjects(void);
void UpdateOtherobjects(void);
void DrawOtherobjects(void);

OBJECT *GetKeyObject(void);