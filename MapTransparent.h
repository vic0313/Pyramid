//=============================================================================
//
// プレイヤー前の透明画像の処理 [MapTransparent.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"

#include "input.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define MAX_Transparent_wall				(100)
#define MAX_Transparent_field				(100)
#define MAX_Transparent						(MAX_Transparent_wall)

#define Transparent_pos				(10.5f)


//*****************************************************************************
// 構造体定義
//*****************************************************************************




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitTransparent(void);
HRESULT MakeVertex(void);
void UninitTransparent(void);
void UpdateTransparent(void);
void DrawTransparent(void);