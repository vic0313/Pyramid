//=============================================================================
//
// ���b�N���� [rope.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ROPE		(1)					// �v���C���[�̐�
#define ROPE_LONG_PART	(20)
#define ROPE_LONG		(300.0f)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ROPE
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			pos_previous;

	D3DXVECTOR3			start_pos;				// ���f���̈ʒu
	
	D3DXVECTOR3			end_pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h

	DX11_MODEL			model;				// ���f�����

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	bool				use;				// �����邩�ǂ���

	float				time;				//JUMP�̎��Ԍv�Z�p
	float				timeplus;				//JUMP�̎��Ԍv�Z�p

	bool				point_set;
	bool				point_set_finsh;
	bool				endpos_Decide;
	bool				touch_check;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRope(void);
void UninitRope(void);
void UpdateRope(void);
void DrawRope(void);

ROPE *GetRope(void);
