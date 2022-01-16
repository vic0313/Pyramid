//=============================================================================
//
// 計算画面の処理 [GAME_result.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAXHP_SCORE	(1)
#define KEY_SCORE	(50)
#define SKILL_SCORE	(100)
#define PLAYERNUM_SCORE	(200)
#define BOSS_SCORE	(1000)
#define TIME_SCORE	(1)

#define FINAL_SCORE_CHECK_1	(500)
#define FINAL_SCORE_CHECK_2	(1000)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameResult(void);
void UninitGameResult(void);
void UpdateGameResult(void);
void DrawGameResult(void);
void DrawGameResult_True(void);

