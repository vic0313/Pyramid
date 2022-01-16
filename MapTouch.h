//=============================================================================
//
// 地図の当たり判定処理 [MapTouch.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MeshfieldTouch(int check);
void MeshceilingTouch( int check);
void MeshwallTouch(int check);
void SlopewallTouch( int check);

bool CameraRayTouch_Field(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber);
bool CameraRayTouch_Wall(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber);
bool CameraRayTouch_Ceiling(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber);
bool CameraRayTouch_Slope(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber);
bool CameraRayTouch_SlopeWall(D3DXVECTOR3 playerpos, D3DXVECTOR3 camerapos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal, int *meshnumber);