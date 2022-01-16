//=============================================================================
//
//  ���w�v�Z���� [collision.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool CollisionBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CollisionBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

float dotProduct(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);
bool RayCast(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *hit, D3DXVECTOR3 *normal);

bool RayHitField(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal);

