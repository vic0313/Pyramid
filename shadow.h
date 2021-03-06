//=============================================================================
//
// e [shadow.h]
// Author : GP11A132 43 ŅŪčô
//
//=============================================================================
#pragma once


//*****************************************************************************
// vg^Cvéū
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseShadow(int nIdxShadow);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);
