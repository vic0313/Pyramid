//=============================================================================
//
// バトルエリアの処理 [GAME_battlearea.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameBattlearea(void);
void UninitGameBattlearea(void);
void UpdateGameBattlearea(void);
void DrawGameBattlearea(void);
void DrawGameBattlearea_True(void);

void HitCheck(void);
void TouchCheck(void);