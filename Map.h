//=============================================================================
//
// 地図の処理 [Map.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************

typedef struct
{
	D3DXVECTOR3			pos0;				// 左上の位置
	D3DXVECTOR3			pos1;				// 右上の位置
	D3DXVECTOR3			pos2;				// 左下の位置
	D3DXVECTOR3			pos3;				// 左下の位置
	D3DXVECTOR3			posplayer;
	int					check;
	int					check1;
	int					check2;
	bool				ok;
}MAP_POINT;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

MAP_POINT *GetFieldPoint(void);
MAP_POINT *GetWallPoint(void);
MAP_POINT *GetFieldSlopePoint(void);
MAP_POINT *GetSlopeWallPoint(void);
MAP_POINT *GetCeilingPoint(void);
MAP_POINT *GetFieldPointReverse(void);
MAP_POINT *GetWallPointReverse(void);
MAP_POINT *GetSlopeWallPointReverse(void);
MAP_POINT *GetCeilingPointReverse(void);

void SaveFieldPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveCeilingPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveWallPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3, int check);
//if slopecheck=1 ->右向けの壁(左の壁)			-D3DX_PI * 0.50f
	//if slopecheck=2 ->左向けの壁(右の壁)		D3DX_PI * 0.50f
	//if slopecheck=3 ->下向けの壁(上の壁)		0.0f
	//if slopecheck=4 ->上向けの壁(下の壁)		D3DX_PI

	//if skypecheck=1->NO空中
	//if skypecheck=2->YES空中
void SaveFieldSlopePoint(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int check);
//if slopecheck=1 ->左下から右上の坂道
	//if slopecheck=2 ->右下から左上の坂道
	//if slopecheck=3 ->上上から下下の坂道
	//if slopecheck=4 ->下上から上下の坂道
void SaveSlopeWallPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2);
		//if check2=1 ->左下から右上の坂道
		//if check2=2 ->右下から左上の坂道
		//if check2=3 ->下下から上上の坂道
		//if check2=4 ->上下から下上の坂道

		//if check1=1->下から上見え左の壁
		//if check1=2->下から上見え右の壁

void SaveFieldPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveCeilingPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3, int check);
void SaveSlopeWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2);
