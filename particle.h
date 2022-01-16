//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_PARTICLE_ROPE		(512)		// �p�[�e�B�N���ő吔
#define	MAX_PARTICLE_BULLET		(1000)		// �p�[�e�B�N���ő吔
#define	MAX_PARTICLE_TWOJUMP		(20)		// �p�[�e�B�N���ő吔
#define	MAX_PARTICLE_DIED		(100)


#define	PARTICLE_BULLET_NUM		(5)		// �p�[�e�B�N���ő吔

#define TWOJUMP_RADIUS		(30)
#define TWOJUMP_YPLUE		(0.01f)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ��]
	D3DXVECTOR3		scale;			// �X�P�[��
	D3DXVECTOR3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nIdxShadow;		// �eID
	int				nLife;			// ����
	bool			bUse;			// �g�p���Ă��邩�ǂ���

	int						g_TexNo;// �e�N�X�`���ԍ�
	D3DXMATRIX				g_mtxWorldParticle;// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posBase;// �r���{�[�h�����ʒu
	float				time;				//JUMP�̎��Ԍv�Z�p
	float				timeplus;			//JUMP�̎��Ԍv�Z�p
	D3DXVECTOR3				moveway;
	//bool			usecheck;
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT InitRopeParticle(void);
//void UninitRopeParticle(void);
//void UpdateRopeParticle(void);
//void DrawRopeParticle(void);

//HRESULT InitBulletParticle(void);
//void UninitBulletParticle(void);
//void UpdateBulletParticle(void);
//void DrawBulletParticle(void);

HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

PARTICLE *GetTwojumpParticle(void);
int SetBulletParticle(D3DXVECTOR3 bulletpos);
int SetDieParticle(D3DXVECTOR3 diedpos);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

