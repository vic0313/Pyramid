//=============================================================================
//
// UI処理 [UI.h]
// Author : GP11A132　43　林尚頡
//
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99999)		// スコアの最大値
#define SCORE_DIGIT			(5)			// 桁数

#define	HP_PRICE			(50)
#define	ATK_PRICE			(80)
#define	TWOJUMP_PRICE		(100)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PAUSE {
	bool					g_Use;						// true:使っている  false:未使用
	float					g_w, g_h;					// 幅と高さ
	D3DXVECTOR3				g_Pos;						// ポリゴンの座標
	int						g_TexNo;					// テクスチャ番号
	ID3D11Buffer			*g_VertexBuffer;
	D3DXCOLOR				color;

	 D3DXMATRIX				mtxWorld;
	 D3DXVECTOR3 scl;			// スケール
	 MATERIAL	material;		// マテリアル
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTittle(void);
void UninitTittle(void);
void UpdateTittle(void);
void DrawTittle(void);

HRESULT InitBuying(void);
void UninitBuying(void);
void UpdateBuying(void);
void DrawBuying(void);
void DrawBuyingMark(void);

HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);

HRESULT InitResultUI(void);
void UninitResultUI(void);
void UpdateResultUI(void);
void DrawResultUI(void);
