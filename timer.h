//=============================================================================
//
// タイマー処理 [timer.h]
// Author : GP11A132　43　林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "time.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Time_MAX			(5999)		// MAX時間　単位:秒  例3分=60*3=180秒
#define TIME_DIGIT			(4)			// 桁数


static clock_t start, end;				//タイマーの開始と計算時の時間
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);



double GetTime(void);
int GetMinute(void);
int GetSecond(void);


clock_t *getstart(void);
clock_t *getend(void);