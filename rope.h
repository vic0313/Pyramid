//=============================================================================
//
// ロック処理 [rope.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ROPE		(1)					// プレイヤーの数
#define ROPE_LONG_PART	(20)
#define ROPE_LONG		(300.0f)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ROPE
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			pos_previous;

	D3DXVECTOR3			start_pos;				// モデルの位置
	
	D3DXVECTOR3			end_pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	float				spd;				// 移動スピード

	DX11_MODEL			model;				// モデル情報

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	int					shadowIdx;			// 影のインデックス番号

	bool				use;				// 生きるかどうか

	float				time;				//JUMPの時間計算用
	float				timeplus;				//JUMPの時間計算用

	bool				point_set;
	bool				point_set_finsh;
	bool				endpos_Decide;
	bool				touch_check;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRope(void);
void UninitRope(void);
void UpdateRope(void);
void DrawRope(void);

ROPE *GetRope(void);
