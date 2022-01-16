//=============================================================================
//
// �e���� [bullet.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET		(30)	// �e�ő吔
#define	MAX_ENEMY_BULLET_1		(30)	// �e�ő吔
#define	MAX_ENEMY_BULLET_2		(30)	// �e�ő吔
#define	MAX_ENEMY_BOSS		(100)	// �e�ő吔
#define	BOSS_BULLET_ONENUM		(5)	// �e�ő吔
#define BULLET_WIDTH	(20)
#define BULLET_HEIGHT	(20)
#define BULLET_LENGTH	(20)
//bigbullet
#define BULLET_CHECK_RADIUS	(20)

#define TEXTURE_BULLET_SIZE_X (20)
#define TEXTURE_BULLET_SIZE_Y (20)
#define TEXTURE_BULLETS_MAX			(1)			// �e�N�X�`���̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 pos2;							// �����㏸���E�̈ʒu
	D3DXVECTOR3 particlepos;					// �ʒu
	D3DXVECTOR3 scl;							// �X�P�[��
	MATERIAL	material;						// �}�e���A��
	D3DXVECTOR3 Hitcheck_size;
	bool		use;							// �g�p���Ă��邩�ǂ���
	bool		touchparticle;					// �g�p���Ă��邩�ǂ���

	D3DXVECTOR3 rot;							// �p�x
	float		spd;							// �ړ���
	bool		touchcheck;						//�ǂƒn�ʓ����蔻��p
	D3DXVECTOR3 oneshoutmove;					//�v���C���[��shootingmode�ύX�̎��A�e�̈ړ���
	bool		oneshoutcheck;					//�v���C���[��shootingmode�ύX�̎��A�e�̈ړ��̏����p

	float				time_down;				//�����̎��Ԍv�Z�p
	float				time_downplus;			//�����̎��Ԍv�Z�p
	float				time_up;				//�㏸�̎��Ԍv�Z�p
	float				timeplus_up;			//�㏸�̎��Ԍv�Z�p
	float				time_Split;				//����̎��Ԍv�Z�p
	float				timeplus_Split;			//����̎��Ԍv�Z�p

	bool		drawcheck;						//�J����CHECK�p�Aif==true�@�e�̕`�悵�܂�
	D3DXVECTOR3 left_pos;						// �J����CHECK�p�̈ʒu
	D3DXVECTOR3 right_pos;						// �J����CHECK�p�̈ʒu
	D3DXVECTOR3 front_pos;						// �J����CHECK�p�̈ʒu
	D3DXVECTOR3 back_pos;						// �J����CHECK�p�̈ʒu
	D3DXVECTOR3 top_pos;						// �J����CHECK�p�̈ʒu
	D3DXVECTOR3 down_pos;						// �J����CHECK�p�̈ʒu
} BULLET;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int type);

BULLET *GetBullet(void);

BULLET *GetEnemyBullet1(void);

BULLET *GetEnemyBullet2(void);
BULLET *GetEnemyBulletBoss(void);

void BulletDrawCheck(void);