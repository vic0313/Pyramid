//=============================================================================
//
// ���̃��f������ [other_objects.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_KEY			(5)
#define KEY_RADIUS		(15)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct 
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			touchsize;
	DX11_MODEL			model;				// ���f�����
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
	float				rotspd;				// ��]�X�s�[�h

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	bool				use;

	//float				radius;				//�����蔻��p�̔��a

	float				time;				//���`��Ԃ̎��Ԍv�Z�p
	float				time_plue;
}OBJECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitOtherobjects(void);
void UninitOtherobjects(void);
void UpdateOtherobjects(void);
void DrawOtherobjects(void);

OBJECT *GetKeyObject(void);