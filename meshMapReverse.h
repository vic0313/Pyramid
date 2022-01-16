//=============================================================================
//
// 半透明地図の処理 [meshMapReverse.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitMeshFieldReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ, float startY);
void UninitMeshFieldReverse(void);
void UpdateMeshFieldReverse(void);
void DrawMeshFieldReverse(void);



HRESULT InitMeshWallReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWallReverse(void);
void UpdateMeshWallReverse(void);
void DrawMeshWallReverse(void);

HRESULT InitMeshFieldCeilingReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ);
void UninitMeshFieldCeilingReverse(void);
void UpdateMeshFieldCeilingReverse(void);
void DrawMeshFieldCeilingReverse(void);


HRESULT InitMeshSlopeReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ, int startY, int endY, int slopecheck);
void UninitMeshSlopeReverse(void);
void UpdateMeshSlopeReverse(void);
void DrawMeshSlopeReverse(void);



HRESULT InitSlopeWallReverse(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col);
void UninitSlopeWallReverse(void);
void UpdateSlopeWallReverse(void);
void DrawSlopeWallReverse(void);

void MapTransparentCheckReverse(void);