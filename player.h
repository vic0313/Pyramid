//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "GAME_battlearea.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�
#define PLAYER_SIZE		(18.0f)
#define PLAYER_TALL		(70.0f)
#define PLAYER_PARTS_MAX	(13)								// �v���C���[�̃p�[�c�̐�

#define PLAYER_FIRST_POS_X  (0.0f)
#define PLAYER_FIRST_POS_Z  (-1750.0f)
#define PLAYER_FIRST_POS_Y  (0.0f)

#define PLAYER_SIZE2		(40.0f)//�Ǔ������p
#define PLAYER_HIT_CD		(0.5f)

#define PLAYER_SHADOW_SIZE	(1.5f)							// �e�̑傫��
#define PLAYER_SHADOW_SIZE_MAX	(0.0f)							// �e�̑傫��
#define PLAYER_SHADOW_SIZE_YPOS_MAX	(110.0f)							// �e�̑傫��
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
 struct PLAYER
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu

	//�Ǔ������p
	D3DXVECTOR3			left_pos;
	D3DXVECTOR3			right_pos;
	D3DXVECTOR3			top_pos;
	D3DXVECTOR3			down_pos;
	D3DXVECTOR3			center_pos;

	D3DXVECTOR3			downpos;
	D3DXVECTOR3			ropepos;
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			rot_move;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			touchsize;
	bool				beHit;
	bool				enemytouch;
	float				beHitTime;
	float				beHitTime_plus;
	float				beTouchTime;
	float				beTouchTime_plus;
	float				spd;				// �ړ��X�s�[�h
	
	DX11_MODEL			model;				// ���f�����

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	bool				use;				// �����邩�ǂ���
	//float				radius;				// ���a

	int				wall_touch;

	float				time;				//JUMP�̎��Ԍv�Z�p
	float				timeplus;				//JUMP�̎��Ԍv�Z�p
	float				time_bigjump;				//JUMP�̎��Ԍv�Z�p
	float				time_bigjumpplus;				//JUMP�̎��Ԍv�Z�p
	float				time_down;				//JUMP�̎��Ԍv�Z�p
	float				time_downplus;				//JUMP�̎��Ԍv�Z�p
	float				time_rope;				//JUMP�̎��Ԍv�Z�p
	float				time_ropeplus;				//JUMP�̎��Ԍv�Z�p
	float				time_gunmod_use;
	bool				movecheck;
	bool				touchcheck;
	bool				touchcheck_field;
	bool				jumpOK;
	bool				jumping;
	bool				bigjumpOK;
	bool				bigjumping;
	bool				bigjumping_UP;
	bool				downOK;
	bool				namidause;
	D3DXVECTOR3			camera_pos;
	D3DXVECTOR3			camera_pos2;
	bool				ropeuse_OK;
	bool				ropeup_OK;
	int					ropemode;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	int					load;				// �p�[�c�̓ǂݍ���
	INTERPOLATION_DATA	*tbl_adr;			// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	float				move_time;			// ���s����

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	D3DXQUATERNION		Quaternion;

	//D3DXVECTOR3			UpVector;			// �����������Ă��鏊
	
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);


