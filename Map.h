//=============================================================================
//
// �n�}�̏��� [Map.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef struct
{
	D3DXVECTOR3			pos0;				// ����̈ʒu
	D3DXVECTOR3			pos1;				// �E��̈ʒu
	D3DXVECTOR3			pos2;				// �����̈ʒu
	D3DXVECTOR3			pos3;				// �����̈ʒu
	D3DXVECTOR3			posplayer;
	int					check;
	int					check1;
	int					check2;
	bool				ok;
}MAP_POINT;
//*****************************************************************************
// �v���g�^�C�v�錾
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
//if slopecheck=1 ->�E�����̕�(���̕�)			-D3DX_PI * 0.50f
	//if slopecheck=2 ->�������̕�(�E�̕�)		D3DX_PI * 0.50f
	//if slopecheck=3 ->�������̕�(��̕�)		0.0f
	//if slopecheck=4 ->������̕�(���̕�)		D3DX_PI

	//if skypecheck=1->NO��
	//if skypecheck=2->YES��
void SaveFieldSlopePoint(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int check);
//if slopecheck=1 ->��������E��̍⓹
	//if slopecheck=2 ->�E�����獶��̍⓹
	//if slopecheck=3 ->��ォ�牺���̍⓹
	//if slopecheck=4 ->���ォ��㉺�̍⓹
void SaveSlopeWallPoint(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2);
		//if check2=1 ->��������E��̍⓹
		//if check2=2 ->�E�����獶��̍⓹
		//if check2=3 ->����������̍⓹
		//if check2=4 ->�㉺���牺��̍⓹

		//if check1=1->������㌩�����̕�
		//if check1=2->������㌩���E�̕�

void SaveFieldPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveCeilingPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3);
void SaveWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos3, int check);
void SaveSlopeWallPointReverse(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, int check1, int check2);
