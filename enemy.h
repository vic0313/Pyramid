//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY_TYPEA		(7)					// �G�l�~�[�̐�
#define MAX_ENEMY_TYPEB		(8)
#define MAX_ENEMY_TYPEC		(3)
#define MAX_ENEMY_TYPED		(3)
#define MAX_BOSS			(1)

#define ENEMY_TYPEA_MAXHP	(50.0)
#define ENEMY_TYPEB_MAXHP	(50.0)
#define ENEMY_TYPEC_MAXHP	(50.0)
#define ENEMY_TYPED_MAXHP	(50.0)
#define ENEMY_BOSS_MAXHP	(500.0f)

#define ENEMY_TYPEA_ATK	(50.0)
#define ENEMY_TYPEB_ATK	(50.0)
#define ENEMY_TYPEC_ATK	(50.0)
#define ENEMY_TYPED_ATK	(50.0)
#define ENEMY_BOSS_ATK	(50.0)
//shoutpos�p
#define ENEMY_TYPEA_SHOUTPOS_X	(50.0)
#define ENEMY_TYPEA_SHOUTPOS_Y	(50.0)
#define ENEMY_TYPEA_SHOUTPOS_Z	(50.0)

#define ENEMY_TYPEB_SHOUTPOS_X	(50.0)
#define ENEMY_TYPEB_SHOUTPOS_Y	(50.0)
#define ENEMY_TYPEB_SHOUTPOS_Z	(50.0)

#define ENEMY_TYPEC_SHOUTPOS_X	(50.0)
#define ENEMY_TYPEC_SHOUTPOS_Y	(50.0)
#define ENEMY_TYPEC_SHOUTPOS_Z	(50.0)

#define ENEMY_BOSS_SHOUTPOS_X	(50.0)
#define ENEMY_BOSS_SHOUTPOS_Y	(300.0)
#define ENEMY_BOSS_SHOUTPOS_Z	(200.0)
//hitcheck�p���POSA�̔��a
#define ENEMY_TYPEA_POSA_X	(12.0f)
#define ENEMY_TYPEA_POSA_Y	(25.0f)
#define ENEMY_TYPEA_POSA_Z	(12.0f)

#define ENEMY_TYPEB_POSA_X	(35.0)
#define ENEMY_TYPEB_POSA_Y	(17.0)
#define ENEMY_TYPEB_POSA_Z	(35.0)

#define ENEMY_TYPEC_POSA_X	(25.0)
#define ENEMY_TYPEC_POSA_Y	(25.0)
#define ENEMY_TYPEC_POSA_Z	(25.0)

#define ENEMY_TYPED_POSA_X	(45.0f)
#define ENEMY_TYPED_POSA_Y	(50.0f)
#define ENEMY_TYPED_POSA_Z	(45.0f)

#define ENEMY_BOSS_POSA_X	(250.0)
#define ENEMY_BOSS_POSA_Y	(50.0)
#define ENEMY_BOSS_POSA_Z	(250.0)

//hitcheck�p���POSB�̍���
#define ENEMY_TYPEB_POSB_YPOS	(40.0)
#define ENEMY_TYPEC_POSB_YPOS	(35.0)
#define ENEMY_TYPED_POSB_YPOS	(70.0)
#define ENEMY_BOSS_POSB_YPOS	(200.0)
//hitcheck�p���POSB��xpos
#define ENEMY_TYPED_POSB_XPOS	(45.0)
//hitcheck�p���POSB�̔��a
#define ENEMY_TYPEB_POSB_X	(30.0f)
#define ENEMY_TYPEB_POSB_Y	(30.0f)
#define ENEMY_TYPEB_POSB_Z	(30.0f)

#define ENEMY_TYPEC_POSB_X	(30.0f)
#define ENEMY_TYPEC_POSB_Y	(7.5f)
#define ENEMY_TYPEC_POSB_Z	(80.0f)

#define ENEMY_TYPED_POSB_X	(100.0f)
#define ENEMY_TYPED_POSB_Y	(50.0f)
#define ENEMY_TYPED_POSB_Z	(45.0f)

#define ENEMY_BOSS_POSB_X	(150.0f)
#define ENEMY_BOSS_POSB_Y	(50.0f)
#define ENEMY_BOSS_POSB_Z	(150.0f)


//hitcheck�p��OPOSC�̍���
#define ENEMY_BOSS_POSC_YPOS	(350.0)
//hitcheck�p��OPOSC�̔��a
#define ENEMY_BOSS_POSC_X	(50.0)
#define ENEMY_BOSS_POSC_Y	(50.0)
#define ENEMY_BOSS_POSC_Z	(50.0)

//beHIT�̎��̖��Gtime
#define	HIT_CDTIME	(1.0)
#define	HIT_CDTIME_PLUE (0.1f)
#define	HIT_CDTIME_BOSS	(3.0)
//���S��̏d������
#define	REBIRTH_TYPEA_TIME	(20.0)
#define	REBIRTH_TYPEB_TIME	(50.0)
#define	REBIRTH_TYPEC_TIME	(30.0)
#define	REBIRTH_TYPED_TIME	(50.0)
#define	REBIRTH_TIME_PLUE (0.1f)



//�{���͈�
#define ENEMY_TYPEA_RANG	(400.0f)
#define ENEMY_TYPEA_RANG_Y	(200.0f)
#define ENEMY_TYPEB_RANG	(500.0f)
#define ENEMY_TYPEB_RANG_Y	(300.0f)
#define ENEMY_TYPEC_RANG	(1000.0f)
#define ENEMY_TYPEC_RANG_Y	(100.0f)
#define ENEMY_TYPED_RANG	(300.0f)
#define ENEMY_TYPED_RANG_Y	(500.0f)


#define ENEMY_BULLET_ATK		(5.0f)
#define ENEMY_TOUCH_ATK			(3.0f)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct 
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			shoutpos_plue;
	D3DXVECTOR3			HitcheckPosA;				
	D3DXVECTOR3			HitcheckPosB;
	D3DXVECTOR3			HitcheckPosC;
	D3DXVECTOR3			HitcheckPosA_size;
	D3DXVECTOR3			HitcheckPosB_size;
	D3DXVECTOR3			HitcheckPosC_size;
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h

	DX11_MODEL			model;				// ���f�����

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	bool				use;

	//float				radius;				//�����蔻��p�̔��a

	float				time;				//���`��Ԃ̎��Ԍv�Z�p
	float				Hit_CDtime;				//���`��Ԃ̎��Ԍv�Z�p
	float				Hit_CDtime_plue;

	float				Rebirthtime;
	float				Rebirthtime_plue;
	float				HP;
	float				Atk;
	bool				beHit;
	float				shout_time;
	float				shout_time_plue;
	float				move_time;
	float				move_time_plue;
	float				move_rot;
	float				move_rot_plue;
	bool				touchnow;
	bool				typeCmoveY;
	float				jumpup_time;
	float				jumpup_time_plue;
	float				jumpdown_time;
	float				jumpdown_time_plue;
	float				shock_time;
	float				shock_time_plue;

	bool		drawcheck;
	D3DXVECTOR3 left_pos;			// �ʒu
	D3DXVECTOR3 right_pos;			// �ʒu
	D3DXVECTOR3 front_pos;			// �ʒu
	D3DXVECTOR3 back_pos;			// �ʒu
	D3DXVECTOR3 top_pos;			// �ʒu
	D3DXVECTOR3 down_pos;			// �ʒu
}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemyTypeA(void);
ENEMY *GetEnemyTypeB(void);
ENEMY *GetEnemyTypeC(void);
ENEMY *GetEnemyTypeD(void);
ENEMY *GetEnemyBOSS(void);

void EnemyDrawCheck(void);
bool enemy_player(D3DXVECTOR3 playerpos, D3DXVECTOR3 enemypos, int enemytype, D3DXVECTOR3 rot);
