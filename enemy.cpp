//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "shadow.h"
#include "renderer.h"
#include "bullet.h"
#include "player.h"
#include <time.h>
#include "MapTouch.h"
#include "collision.h"
#include "Map.h"
#include "meshMap.h"
#include "camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY_TYPEA		"data/MODEL/enemytypeA.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPEB		"data/MODEL/enemytypeB.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPEC		"data/MODEL/enemytypeC.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPED		"data/MODEL/enemytypeD.obj"		// �ǂݍ��ރ��f����
#define	MODEL_BOSS				"data/MODEL/enemyBOSS.obj"		// �ǂݍ��ރ��f����

#define	MODEL_ENEMY_TYPEA_BEHIT		"data/MODEL/enemytypeAbehit.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPEB_BEHIT		"data/MODEL/enemytypeBbehit.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPEC_BEHIT		"data/MODEL/enemytypeCbehit.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TYPED_BEHIT		"data/MODEL/enemytypeDbehit.obj"		// �ǂݍ��ރ��f����
#define	MODEL_BOSS_BEHIT			"data/MODEL/enemyBOSSbehit.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define ENEMY_TYPEA_SHADOW_SIZE	(2.0f)						// �e�̑傫��

#define ENEMY_TYPEC_SHADOW_SIZE	(1.5f)						// �e�̑傫��
#define ENEMY_TYPEC_SHADOW_SIZE_MIN	(0.1f)						// �e�̑傫��
#define ENEMY_TYPED_SHADOW_SIZE	(3.0f)						// �e�̑傫��
#define ENEMY_TYPED_SHADOW_SIZE_MIN	(0.1f)						// �e�̑傫��
#define ENEMY_BOSS_SHADOW_SIZE	(20.0f)						// �e�̑傫��
#define ENEMY_BOSS_SHADOW_SIZE_MIN	(10.0f)						// �e�̑傫��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy_typeA[MAX_ENEMY_TYPEA];				// �G�l�~�[
static ENEMY			g_Enemy_typeB[MAX_ENEMY_TYPEB];				// �G�l�~�[
static ENEMY			g_Enemy_typeC[MAX_ENEMY_TYPEC];				// �G�l�~�[
static ENEMY			g_Enemy_typeD[MAX_ENEMY_TYPED];				// �G�l�~�[
static ENEMY			g_Enemy_BOSS[MAX_BOSS];				// �G�l�~�[
static GAMEDATA *gamedata = GetData();
static PLAYER *player = GetPlayer();
static CAMERA *camera = GetCamera();
static MAP_POINT	*g_fieldpoint = GetFieldPoint();

static DX11_MODEL			enemytypeA_model;
static DX11_MODEL			enemytypeB_model;
static DX11_MODEL			enemytypeC_model;
static DX11_MODEL			enemytypeD_model;
static DX11_MODEL			enemyBOSS_model;
static DX11_MODEL			enemytypeA_beHit_model;
static DX11_MODEL			enemytypeB_beHit_model;
static DX11_MODEL			enemytypeC_beHit_model;
static DX11_MODEL			enemytypeD_beHit_model;
static DX11_MODEL			enemyBOSS_beHit_model;


static D3DXVECTOR3 typeA_Rebirth_pos[] = {
	D3DXVECTOR3(0.0f,50.0f,-700.0f),
	D3DXVECTOR3(1300.0f,250.0f,-1000.0f),
	D3DXVECTOR3(-1000.0f,300.0f,-150.0f),
	D3DXVECTOR3(1000.0f,300.0f,-150.0f),
	D3DXVECTOR3(0.0f,250.0f,-250.0f),
	D3DXVECTOR3(-200.0f,0.0f,0.0f),
	D3DXVECTOR3(200.0f,0.0f,0.0f),
};
//OK
static D3DXVECTOR3 typeB_Rebirth_pos[] = {
	D3DXVECTOR3(-1400.0f,500.0f,-250.0f),
	D3DXVECTOR3(-350.0f,500.0f,150.0f),
	D3DXVECTOR3(350.0f,500.0f,150.0f),
	D3DXVECTOR3(1400.0f,500.0f,-250.0f),
	D3DXVECTOR3(0.0f,500.0f,-600.0f),
	D3DXVECTOR3(1375.0f,300.0f,-1375.0f),
	D3DXVECTOR3(1400.0f,460.0f,1400.0f),
	D3DXVECTOR3(-1400.0f,0.0f,1400.0f),
};
static D3DXVECTOR3 typeC_Rebirth_pos[] = {
	D3DXVECTOR3(0.0f,350.0f,0.0f),
	D3DXVECTOR3(-1400.0f,350.0f,150.0f),
	D3DXVECTOR3(1400.0f,350.0f,150.0f),
};
static D3DXVECTOR3 typeD_Rebirth_pos[] = {
	D3DXVECTOR3(-1400.0f,0.0f,0.0f),
	D3DXVECTOR3(1400.0f,0.0f,0.0f),
	D3DXVECTOR3(0.0f,250.0f,-350.0f),
};
//OK
static D3DXVECTOR3 BOSS_Rebirth_pos[] = {
	D3DXVECTOR3(0.0f,0.0f,900.0f),
};

static D3DXVECTOR3 g_MoveTbl[] = {
	D3DXVECTOR3(0.0f,7.0f,0.0f),
	D3DXVECTOR3(-200.0f,7.0f,0.0f),
	D3DXVECTOR3(-200.0f,7.0f,200.0f),
};

static float	g_MoveSpd[] = {
	0.01f,
	0.1f,
	0.005f,
};

static int move_A = 0;
static int move_D = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	if (gamedata->modenow == MODE_TITLE)
	{
		LoadModel(MODEL_ENEMY_TYPEA, &enemytypeA_model);
		LoadModel(MODEL_ENEMY_TYPEB, &enemytypeB_model);
		LoadModel(MODEL_ENEMY_TYPEC, &enemytypeC_model);
		LoadModel(MODEL_ENEMY_TYPED, &enemytypeD_model);
		LoadModel(MODEL_BOSS, &enemyBOSS_model);
		LoadModel(MODEL_ENEMY_TYPEA_BEHIT, &enemytypeA_beHit_model);
		LoadModel(MODEL_ENEMY_TYPEB_BEHIT, &enemytypeB_beHit_model);
		LoadModel(MODEL_ENEMY_TYPEC_BEHIT, &enemytypeC_beHit_model);
		LoadModel(MODEL_ENEMY_TYPED_BEHIT, &enemytypeD_beHit_model);
		LoadModel(MODEL_BOSS_BEHIT, &enemyBOSS_beHit_model);

	}
	
	
	for (int i = 0; i < MAX_ENEMY_TYPEA ; i++)
	{

		g_Enemy_typeA[i].pos = typeA_Rebirth_pos[i];
		g_Enemy_typeA[i].shoutpos_plue= D3DXVECTOR3(ENEMY_TYPEA_SHOUTPOS_X, ENEMY_TYPEA_SHOUTPOS_Y, ENEMY_TYPEA_SHOUTPOS_Z);
		g_Enemy_typeA[i].HitcheckPosA = typeA_Rebirth_pos[i];
		g_Enemy_typeA[i].HitcheckPosA_size = D3DXVECTOR3(ENEMY_TYPEA_POSA_X, ENEMY_TYPEA_POSA_Y, ENEMY_TYPEA_POSA_Z);
		g_Enemy_typeA[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_typeA[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		g_Enemy_typeA[i].spd = 3.0f;		// �ړ��X�s�[�h�N���A
		g_Enemy_typeA[i].Atk = ENEMY_TYPEA_ATK;
		g_Enemy_typeA[i].HP = ENEMY_TYPEA_MAXHP;
		g_Enemy_typeA[i].use = true;		//�g�p��

		g_Enemy_typeA[i].time = 0.0f;

		g_Enemy_typeA[i].beHit = false;
		g_Enemy_typeA[i].Hit_CDtime = HIT_CDTIME;
		g_Enemy_typeA[i].Hit_CDtime_plue = HIT_CDTIME_PLUE;
		g_Enemy_typeA[i].Rebirthtime = REBIRTH_TYPEA_TIME;
		g_Enemy_typeA[i].Rebirthtime_plue = REBIRTH_TIME_PLUE;

		g_Enemy_typeA[i].shout_time=0.0f;
		g_Enemy_typeA[i].shout_time_plue=0.02f;
		g_Enemy_typeA[i].move_time = 2.0f;
		g_Enemy_typeA[i].move_time_plue = 0.01f;

		g_Enemy_typeA[i].move_rot = 0.0f;
		g_Enemy_typeA[i].move_rot_plue = 0.01f;
		if (gamedata->modenow == MODE_TITLE)
		{
			D3DXVECTOR3 pos = g_Enemy_typeA[i].pos;
			pos.y = 1.0f;
			g_Enemy_typeA[i].shadowIdx = CreateShadow(pos, ENEMY_TYPEA_SHADOW_SIZE, ENEMY_TYPEA_SHADOW_SIZE);
		}
		g_Enemy_typeA[i].touchnow = false;
		g_Enemy_typeA[i].drawcheck = true;

		g_Enemy_typeA[i].left_pos= g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].right_pos= g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].front_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].back_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].top_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].down_pos = g_Enemy_typeA[i].pos;
	}

	for (int i = 0; i < MAX_ENEMY_TYPEB; i++)
	{

		g_Enemy_typeB[i].pos = typeB_Rebirth_pos[i];
		g_Enemy_typeB[i].shoutpos_plue = D3DXVECTOR3(ENEMY_TYPEB_SHOUTPOS_X, ENEMY_TYPEB_SHOUTPOS_Y, ENEMY_TYPEB_SHOUTPOS_Z);
		g_Enemy_typeB[i].HitcheckPosA = typeB_Rebirth_pos[i];
		g_Enemy_typeB[i].HitcheckPosA_size = D3DXVECTOR3(ENEMY_TYPEB_POSA_X, ENEMY_TYPEB_POSA_Y, ENEMY_TYPEB_POSA_Z);
		g_Enemy_typeB[i].HitcheckPosB = typeB_Rebirth_pos[i];
		g_Enemy_typeB[i].HitcheckPosB_size = D3DXVECTOR3(ENEMY_TYPEB_POSB_X, ENEMY_TYPEB_POSB_Y, ENEMY_TYPEB_POSB_Z);
		g_Enemy_typeB[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_typeB[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy_typeB[i].Atk = ENEMY_TYPEB_ATK;
		g_Enemy_typeB[i].HP = ENEMY_TYPEB_MAXHP;
		g_Enemy_typeB[i].spd = 0.0f;		// �ړ��X�s�[�h�N���A
		g_Enemy_typeB[i].use = true;		//�g�p��
		g_Enemy_typeB[i].time = 0.0f;				//���`��ԗp�̎��Ԃ��N���A

		g_Enemy_typeB[i].beHit = false;
		g_Enemy_typeB[i].Hit_CDtime = HIT_CDTIME;
		g_Enemy_typeB[i].Hit_CDtime_plue = HIT_CDTIME_PLUE;
		g_Enemy_typeB[i].Rebirthtime = REBIRTH_TYPEB_TIME;
		g_Enemy_typeB[i].Rebirthtime_plue = REBIRTH_TIME_PLUE;

		g_Enemy_typeB[i].shout_time = 0.0f;
		g_Enemy_typeB[i].shout_time_plue = 0.01f;
		g_Enemy_typeB[i].move_rot = 0.0f;
		g_Enemy_typeB[i].move_rot_plue = 0.01f;
		g_Enemy_typeB[i].touchnow = false;
		g_Enemy_typeB[i].drawcheck = true;
		g_Enemy_typeB[i].left_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].right_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].front_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].back_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].top_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].down_pos = g_Enemy_typeB[i].pos;
	}
	for (int i = 0; i < MAX_ENEMY_TYPEC; i++)
	{

		g_Enemy_typeC[i].pos = typeC_Rebirth_pos[i];
		g_Enemy_typeC[i].shoutpos_plue = D3DXVECTOR3(ENEMY_TYPEC_SHOUTPOS_X, ENEMY_TYPEC_SHOUTPOS_Y, ENEMY_TYPEC_SHOUTPOS_Z);
		g_Enemy_typeC[i].HitcheckPosA = typeC_Rebirth_pos[i];
		g_Enemy_typeC[i].HitcheckPosA_size = D3DXVECTOR3(ENEMY_TYPEC_POSA_X, ENEMY_TYPEC_POSA_Y, ENEMY_TYPEC_POSA_Z);
		g_Enemy_typeC[i].HitcheckPosB = typeC_Rebirth_pos[i];
		g_Enemy_typeC[i].HitcheckPosB_size = D3DXVECTOR3(ENEMY_TYPEC_POSB_X, ENEMY_TYPEC_POSB_Y, ENEMY_TYPEC_POSB_Z);
		g_Enemy_typeC[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_typeC[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy_typeC[i].Atk = ENEMY_TYPEC_ATK;
		g_Enemy_typeC[i].HP = ENEMY_TYPEC_MAXHP;
		g_Enemy_typeC[i].spd = 0.0f;		// �ړ��X�s�[�h�N���A

		g_Enemy_typeC[i].use = true;		//�g�p��

		g_Enemy_typeC[i].time = 0.0f;				//���`��ԗp�̎��Ԃ��N���A

		g_Enemy_typeC[i].beHit = false;
		g_Enemy_typeC[i].Hit_CDtime = HIT_CDTIME;
		g_Enemy_typeC[i].Hit_CDtime_plue = HIT_CDTIME_PLUE;
		g_Enemy_typeC[i].Rebirthtime = REBIRTH_TYPEC_TIME;
		g_Enemy_typeC[i].Rebirthtime_plue = REBIRTH_TIME_PLUE;

		g_Enemy_typeC[i].shout_time = 0.0f;
		g_Enemy_typeC[i].shout_time_plue = 0.03f;
		g_Enemy_typeC[i].move_time = 0.0f;
		g_Enemy_typeC[i].move_time_plue = 0.05f;

		g_Enemy_typeC[i].move_rot = 0.0f;
		g_Enemy_typeC[i].move_rot_plue = 0.05f;
		g_Enemy_typeC[i].touchnow = false;
		g_Enemy_typeC[i].typeCmoveY = false;
		g_Enemy_typeC[i].drawcheck = true;
		if (gamedata->modenow == MODE_TITLE)
		{
			D3DXVECTOR3 pos = g_Enemy_typeC[i].pos;
			pos.y = 1.0f;
			g_Enemy_typeC[i].shadowIdx = CreateShadow(pos, ENEMY_TYPEC_SHADOW_SIZE, ENEMY_TYPEC_SHADOW_SIZE);
		}
		g_Enemy_typeC[i].left_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].right_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].front_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].back_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].top_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].down_pos = g_Enemy_typeC[i].pos;
	}
	for (int i = 0; i < MAX_ENEMY_TYPED; i++)
	{

		g_Enemy_typeD[i].pos = typeD_Rebirth_pos[i];
		g_Enemy_typeD[i].HitcheckPosA = typeD_Rebirth_pos[i];
		g_Enemy_typeD[i].HitcheckPosA_size = D3DXVECTOR3(ENEMY_TYPED_POSA_X, ENEMY_TYPED_POSA_Y, ENEMY_TYPED_POSA_Z);
		g_Enemy_typeD[i].HitcheckPosB = typeD_Rebirth_pos[i];
		g_Enemy_typeD[i].HitcheckPosB_size = D3DXVECTOR3(ENEMY_TYPED_POSB_X, ENEMY_TYPED_POSB_Y, ENEMY_TYPED_POSB_Z);
		g_Enemy_typeD[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_typeD[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy_typeD[i].Atk = ENEMY_TYPED_ATK;
		g_Enemy_typeD[i].HP = ENEMY_TYPED_MAXHP;
		g_Enemy_typeD[i].spd = 5.0f;		// �ړ��X�s�[�h�N���A

		g_Enemy_typeD[i].use = true;		//�g�p��

		g_Enemy_typeD[i].time = 0.0f;				//���`��ԗp�̎��Ԃ��N���A

		g_Enemy_typeD[i].beHit = false;
		g_Enemy_typeD[i].Hit_CDtime = HIT_CDTIME;
		g_Enemy_typeD[i].Hit_CDtime_plue = HIT_CDTIME_PLUE;
		g_Enemy_typeD[i].Rebirthtime = REBIRTH_TYPED_TIME;
		g_Enemy_typeD[i].Rebirthtime_plue = REBIRTH_TIME_PLUE;

		g_Enemy_typeD[i].move_time = 2.0f;
		g_Enemy_typeD[i].move_time_plue = 0.01f;

		g_Enemy_typeD[i].move_rot=0.0f;
		g_Enemy_typeD[i].move_rot_plue = 0.03f;

		g_Enemy_typeD[i].touchnow = false;
		g_Enemy_typeD[i].drawcheck = true;
		if (gamedata->modenow == MODE_TITLE)
		{
			D3DXVECTOR3 pos = g_Enemy_typeD[i].pos;
			pos.y = 1.0f;
			g_Enemy_typeD[i].shadowIdx = CreateShadow(pos, ENEMY_TYPED_SHADOW_SIZE, ENEMY_TYPED_SHADOW_SIZE);
		}
		g_Enemy_typeD[i].left_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].right_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].front_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].back_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].top_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].down_pos = g_Enemy_typeD[i].pos;
	}
	for (int i = 0; i < MAX_BOSS; i++)
	{

		g_Enemy_BOSS[i].pos = BOSS_Rebirth_pos[i];
		g_Enemy_BOSS[i].HitcheckPosA = BOSS_Rebirth_pos[i];
		g_Enemy_BOSS[i].HitcheckPosA_size = D3DXVECTOR3(ENEMY_BOSS_POSA_X, ENEMY_BOSS_POSA_Y, ENEMY_BOSS_POSA_Z);
		g_Enemy_BOSS[i].HitcheckPosB = BOSS_Rebirth_pos[i];
		g_Enemy_BOSS[i].HitcheckPosB_size = D3DXVECTOR3(ENEMY_BOSS_POSB_X, ENEMY_BOSS_POSB_Y, ENEMY_BOSS_POSB_Z);
		g_Enemy_BOSS[i].HitcheckPosC = BOSS_Rebirth_pos[i];
		g_Enemy_BOSS[i].HitcheckPosC_size = D3DXVECTOR3(ENEMY_BOSS_POSC_X, ENEMY_BOSS_POSC_Y, ENEMY_BOSS_POSC_Z);
		g_Enemy_BOSS[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_BOSS[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy_BOSS[i].Atk = ENEMY_BOSS_ATK;
		g_Enemy_BOSS[i].HP = ENEMY_BOSS_MAXHP;
		g_Enemy_BOSS[i].spd = 0.0f;		// �ړ��X�s�[�h�N���A

		g_Enemy_BOSS[i].use = true;		//�g�p��

		g_Enemy_BOSS[i].time = 0.0f;				//���`��ԗp�̎��Ԃ��N���A

		g_Enemy_BOSS[i].beHit = false;
		g_Enemy_BOSS[i].Hit_CDtime = HIT_CDTIME;
		g_Enemy_BOSS[i].Hit_CDtime_plue = HIT_CDTIME_PLUE;

		g_Enemy_BOSS[i].shout_time = 0.0f;
		g_Enemy_BOSS[i].shout_time_plue = 0.01f;

		g_Enemy_BOSS[i].jumpup_time = 0.0f;
		g_Enemy_BOSS[i].jumpup_time_plue = 0.1f;
		g_Enemy_BOSS[i].jumpdown_time = 0.0f;
		g_Enemy_BOSS[i].jumpdown_time_plue = 0.1f;
		g_Enemy_BOSS[i].shock_time = 0.0f;
		g_Enemy_BOSS[i].shock_time_plue = 0.1f;
		g_Enemy_BOSS[i].move_time = 0.0f;
		g_Enemy_BOSS[i].move_time_plue = 0.01f;
		g_Enemy_BOSS[i].move_rot = 0.0f;
		g_Enemy_BOSS[i].move_rot_plue = 0.03f;
		g_Enemy_BOSS[i].shoutpos_plue = D3DXVECTOR3(ENEMY_BOSS_SHOUTPOS_X, ENEMY_BOSS_SHOUTPOS_Y, ENEMY_BOSS_SHOUTPOS_Z);
			g_Enemy_BOSS[i].touchnow = false;		
		g_Enemy_BOSS[i].drawcheck = false;
		
		if (gamedata->modenow == MODE_RESTAREA)
		{
			
			D3DXVECTOR3 pos = g_Enemy_BOSS[i].pos;
			pos.y = 1.0f;

			g_Enemy_BOSS[i].shadowIdx = CreateShadow(pos, ENEMY_BOSS_SHADOW_SIZE, ENEMY_BOSS_SHADOW_SIZE);
		}
		g_Enemy_BOSS[i].left_pos = g_Enemy_BOSS[i].pos;
		g_Enemy_BOSS[i].right_pos = g_Enemy_BOSS[i].pos;
		g_Enemy_BOSS[i].front_pos = g_Enemy_BOSS[i].pos;
		g_Enemy_BOSS[i].back_pos = g_Enemy_BOSS[i].pos;
		g_Enemy_BOSS[i].top_pos = g_Enemy_BOSS[i].pos;
		g_Enemy_BOSS[i].down_pos = g_Enemy_BOSS[i].pos;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// ���f���̉������
	UnloadModel(&enemytypeA_model);
	UnloadModel(&enemytypeB_model);
	UnloadModel(&enemytypeC_model);
	UnloadModel(&enemytypeD_model);
	UnloadModel(&enemyBOSS_model);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	srand((unsigned)time(NULL));
	EnemyDrawCheck();
	//typeA�̏���
	for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
	{
		
		//����ENEMY���񂾂�A��莞�Ԍ�d������
		if (g_Enemy_typeA[i].use == false) 
		{
			if (g_Enemy_typeA[i].Rebirthtime == REBIRTH_TYPEA_TIME)
			{
				gamedata->money += 10;
			}
			ReleaseShadow(g_Enemy_typeA[i].shadowIdx);
			g_Enemy_typeA[i].Rebirthtime -= g_Enemy_typeA[i].Rebirthtime_plue;
			if (g_Enemy_typeA[i].Rebirthtime <= 0)
			{

				g_Enemy_typeA[i].Rebirthtime = REBIRTH_TYPEA_TIME;
				g_Enemy_typeA[i].pos= typeA_Rebirth_pos[i];
				g_Enemy_typeA[i].use = true;

				D3DXVECTOR3 pos = g_Enemy_typeA[i].pos;
				pos.y += 1.0f;
				g_Enemy_typeA[i].shadowIdx = CreateShadow(pos, ENEMY_TYPEA_SHADOW_SIZE, ENEMY_TYPEA_SHADOW_SIZE);
			}
			continue;
		}
		else {
			D3DXVECTOR3 shadow_pos = g_Enemy_typeA[i].pos;
			SetPositionShadow(g_Enemy_typeA[i].shadowIdx, shadow_pos, ENEMY_TYPEA_SHADOW_SIZE, ENEMY_TYPEA_SHADOW_SIZE);
		}
		//hitcheck�p������POS�𒲐�����
		{
			g_Enemy_typeA[i].HitcheckPosA = g_Enemy_typeA[i].pos;
			g_Enemy_typeA[i].HitcheckPosA.y += g_Enemy_typeA[i].HitcheckPosA_size.y;
		}
		//�����U�����󂯂���A���G���Ԃ̐ݒ�A�����Ď��ԏI�������A�܂��U�����󂯂�ł��܂�
		if (g_Enemy_typeA[i].beHit == true) 
		{
			g_Enemy_typeA[i].Hit_CDtime -= g_Enemy_typeA[i].Hit_CDtime_plue;
			if (g_Enemy_typeA[i].Hit_CDtime <= 0)
			{
				g_Enemy_typeA[i].beHit = false;
				g_Enemy_typeA[i].Hit_CDtime = HIT_CDTIME;
			}
		}
		if (g_Enemy_typeA[i].pos.y >= 250.0f)
		{

			g_Enemy_typeA[i].pos.y--;
		}
		//�J����check�p��POS�𒲐�����
		g_Enemy_typeA[i].left_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].left_pos.x -= ENEMY_TYPEA_POSA_X;
		g_Enemy_typeA[i].left_pos.y += ENEMY_TYPEA_POSA_Y/2;
		g_Enemy_typeA[i].right_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].right_pos.x += ENEMY_TYPEA_POSA_X;
		g_Enemy_typeA[i].right_pos.y += ENEMY_TYPEA_POSA_Y / 2;
		g_Enemy_typeA[i].front_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].front_pos.z -= ENEMY_TYPEA_POSA_X;
		g_Enemy_typeA[i].front_pos.y += ENEMY_TYPEA_POSA_Y / 2;
		g_Enemy_typeA[i].back_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].back_pos.z += ENEMY_TYPEA_POSA_X;
		g_Enemy_typeA[i].back_pos.y += ENEMY_TYPEA_POSA_Y / 2;
		g_Enemy_typeA[i].top_pos = g_Enemy_typeA[i].pos;
		g_Enemy_typeA[i].top_pos.y += ENEMY_TYPEA_POSA_Y;
		g_Enemy_typeA[i].down_pos = g_Enemy_typeA[i].pos;
		//�s���̐���
		{
			if (player[0].pos.x > -600.0f&&player[0].pos.x < 600.0f
				&&player[0].pos.z>250.0f)
			{
				//�ړ��̏���
				if (g_Enemy_typeA[i].move_time >= 1)
				{
					move_A = rand() % 20 + 1;
					if (move_A > 11)
					{
						g_Enemy_typeA[i].move_rot = g_Enemy_typeA[i].rot.y - (D3DX_PI) / (move_A - 11);
						g_Enemy_typeA[i].move_rot_plue = (float)fabs((g_Enemy_typeA[i].rot.y - g_Enemy_typeA[i].move_rot) / 6);
						if (g_Enemy_typeA[i].move_rot > D3DX_PI)
						{
							g_Enemy_typeA[i].move_rot -= 2 * D3DX_PI;
						}
					}
					else
					{
						g_Enemy_typeA[i].move_rot = g_Enemy_typeA[i].rot.y + (D3DX_PI) / (move_A);
						g_Enemy_typeA[i].move_rot_plue = (float)fabs((g_Enemy_typeA[i].rot.y - g_Enemy_typeA[i].move_rot) / 6);
						if (g_Enemy_typeA[i].move_rot < -D3DX_PI)
						{
							g_Enemy_typeA[i].move_rot += 2 * D3DX_PI;
						}
					}
					g_Enemy_typeA[i].move_time = 0.0f;
				}
				else
				{
					g_Enemy_typeA[i].move_time += g_Enemy_typeA[i].move_time_plue;
				}


				if (g_Enemy_typeA[i].rot.y <= (g_Enemy_typeA[i].move_rot + 0.1f) && g_Enemy_typeA[i].rot.y >= (g_Enemy_typeA[i].move_rot - 0.1f))
				{
					g_Enemy_typeA[i].rot.y = g_Enemy_typeA[i].move_rot;

					g_Enemy_typeA[i].pos.x -= sinf(g_Enemy_typeA[i].rot.y) * g_Enemy_typeA[i].spd;
					g_Enemy_typeA[i].pos.z -= cosf(g_Enemy_typeA[i].rot.y) * g_Enemy_typeA[i].spd;

				}
				else
				{
					if (g_Enemy_typeA[i].move_rot > g_Enemy_typeA[i].rot.y)
					{
						g_Enemy_typeA[i].rot.y += g_Enemy_typeA[i].move_rot_plue;

					}
					else if (g_Enemy_typeA[i].move_rot < g_Enemy_typeA[i].rot.y)
					{
						g_Enemy_typeA[i].rot.y -= g_Enemy_typeA[i].move_rot_plue;
					}


				}

			}
			else
			{
				bool ans = enemy_player(player[0].pos, g_Enemy_typeA[i].pos, 1, g_Enemy_typeA[i].rot);
				if (ans == true)
				{
					//enemy��ROT�̐���
					{
						D3DXVECTOR3 muke1 = g_Enemy_typeA[i].pos - player[0].pos;
						D3DXVECTOR3 muke2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
						float rang = acosf(D3DXVec3Dot(&muke1, &muke2) / (((float)sqrt((muke1.x*muke1.x) + (muke1.y*muke1.y) + (muke1.z*muke1.z)))));
						if (g_Enemy_typeA[i].pos.x < player[0].pos.x)
						{
							rang = -rang;
						}
						if (g_Enemy_typeA[i].rot.y <= (rang + 0.5f) && g_Enemy_typeA[i].rot.y >= (rang - 0.5f))
						{
							g_Enemy_typeA[i].rot.y = rang;
						}
						else
						{
							if (g_Enemy_typeA[i].rot.y > rang)
							{
								g_Enemy_typeA[i].rot.y -= g_Enemy_typeA[i].move_rot_plue;

							}
							else if (g_Enemy_typeA[i].rot.y < rang)
							{
								g_Enemy_typeA[i].rot.y += g_Enemy_typeA[i].move_rot_plue;
							}
						}
					}
					//�U���̏���
					if (g_Enemy_typeA[i].shout_time > 1.0f)
					{
						D3DXVECTOR3 shoutpos = g_Enemy_typeA[i].pos;
						shoutpos.x -= sinf(g_Enemy_typeA[i].rot.y)*g_Enemy_typeA[i].shoutpos_plue.z;
						shoutpos.y += g_Enemy_typeA[i].shoutpos_plue.y;
						shoutpos.z -= cosf(g_Enemy_typeA[i].rot.y)*g_Enemy_typeA[i].shoutpos_plue.z;
						SetEnemyBullet(shoutpos, g_Enemy_typeA[i].rot, 1);
						g_Enemy_typeA[i].shout_time = 0.0f;
					}
					else
					{
						g_Enemy_typeA[i].shout_time += g_Enemy_typeA[i].shout_time_plue;
					}
				}
				else
				{
					//�ړ��̏���
					if (g_Enemy_typeA[i].move_time >= 1)
					{
						move_A = rand() % 20 + 1;
						if (move_A > 11)
						{
							g_Enemy_typeA[i].move_rot = g_Enemy_typeA[i].rot.y - (D3DX_PI) / (move_A - 11);
							g_Enemy_typeA[i].move_rot_plue = (float)fabs((g_Enemy_typeA[i].rot.y - g_Enemy_typeA[i].move_rot) / 6);
							if (g_Enemy_typeA[i].move_rot > D3DX_PI)
							{
								g_Enemy_typeA[i].move_rot -= 2 * D3DX_PI;
							}
						}
						else
						{
							g_Enemy_typeA[i].move_rot = g_Enemy_typeA[i].rot.y + (D3DX_PI) / (move_A);
							g_Enemy_typeA[i].move_rot_plue = (float)fabs((g_Enemy_typeA[i].rot.y - g_Enemy_typeA[i].move_rot) / 6);
							if (g_Enemy_typeA[i].move_rot < -D3DX_PI)
							{
								g_Enemy_typeA[i].move_rot += 2 * D3DX_PI;
							}
						}
						g_Enemy_typeA[i].move_time = 0.0f;
					}
					else
					{
						g_Enemy_typeA[i].move_time += g_Enemy_typeA[i].move_time_plue;
					}


					if (g_Enemy_typeA[i].rot.y <= (g_Enemy_typeA[i].move_rot + 0.1f) && g_Enemy_typeA[i].rot.y >= (g_Enemy_typeA[i].move_rot - 0.1f))
					{
						g_Enemy_typeA[i].rot.y = g_Enemy_typeA[i].move_rot;

						g_Enemy_typeA[i].pos.x -= sinf(g_Enemy_typeA[i].rot.y) * g_Enemy_typeA[i].spd;
						g_Enemy_typeA[i].pos.z -= cosf(g_Enemy_typeA[i].rot.y) * g_Enemy_typeA[i].spd;

					}
					else
					{
						if (g_Enemy_typeA[i].move_rot > g_Enemy_typeA[i].rot.y)
						{
							g_Enemy_typeA[i].rot.y += g_Enemy_typeA[i].move_rot_plue;

						}
						else if (g_Enemy_typeA[i].move_rot < g_Enemy_typeA[i].rot.y)
						{
							g_Enemy_typeA[i].rot.y -= g_Enemy_typeA[i].move_rot_plue;
						}
					}


				}
			}
			

		}
		
		//map�̔���
		{
			//2�K
			if (g_Enemy_typeA[i].pos.y >= 250.0f)
			{
				if ((g_Enemy_typeA[i].pos.x) <= 750.0f &&
					(g_Enemy_typeA[i].pos.z) >= -750.0f && (g_Enemy_typeA[i].pos.z) <= 250.0f)
				{
					if (g_Enemy_typeA[i].pos.z - ENEMY_TYPEA_POSA_X <= -750.0f)
					{
						g_Enemy_typeA[i].pos.z = -750.0f + ENEMY_TYPEA_POSA_X;
					}
					else if (g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X >= 250.0f)
					{
						g_Enemy_typeA[i].pos.z = 250.0f - ENEMY_TYPEA_POSA_X;
					}
				}
				else if ((g_Enemy_typeA[i].pos.x) > 750.0f && (g_Enemy_typeA[i].pos.z) <= 250.0f)
				{
					if ((g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= 250.0f)
					{
						g_Enemy_typeA[i].pos.z = 250.0f - ENEMY_TYPEA_POSA_X;
					}
					if (g_Enemy_typeA[i].pos.z <= -750.0f && (g_Enemy_typeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= 750.0f)
					{
						g_Enemy_typeA[i].pos.x = 750.0f + ENEMY_TYPEA_POSA_X;
					}
				}

			}
			else if (g_Enemy_typeA[i].pos.y >= 120.0f&&g_Enemy_typeA[i].pos.y < 250.0f)
			{
				//2����
				if ((g_Enemy_typeA[i].pos.x) <= 750.0f && (g_Enemy_typeA[i].pos.x) >= 350.0f
					&& (g_Enemy_typeA[i].pos.z) <= -1000.0f)
				{
					if ((g_Enemy_typeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= 350.0f)
					{
						g_Enemy_typeA[i].pos.x = 350.0f + ENEMY_TYPEA_POSA_X;
					}
					else if ((g_Enemy_typeA[i].pos.x + ENEMY_TYPEA_POSA_X) >= 750.0f)
					{
						g_Enemy_typeA[i].pos.x = 750.0f - ENEMY_TYPEA_POSA_X;
					}

					if ((g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X) <= -1000.0f)
					{
						g_Enemy_typeA[i].pos.z = 1000.0f - ENEMY_TYPEA_POSA_X;
					}
				}
			}
			else if (g_Enemy_typeA[i].pos.y >= 50.0f&&g_Enemy_typeA[i].pos.y < 120.0f)
			{
				//2����
				if ((g_Enemy_typeA[i].pos.x) >= -200.0f && (g_Enemy_typeA[i].pos.z) <= -500.0f
					&& (g_Enemy_typeA[i].pos.z) >= -1100.0f)
				{
					if ((g_Enemy_typeA[i].pos.x - ENEMY_TYPEA_POSA_X) <= -200.0f)
					{
						g_Enemy_typeA[i].pos.x = -200.0f + ENEMY_TYPEA_POSA_X;
					}

					if ((g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X) >= -500.0f)
					{
						g_Enemy_typeA[i].pos.z = -500.0f - ENEMY_TYPEA_POSA_X;
					}
					else if ((g_Enemy_typeA[i].pos.z - ENEMY_TYPEA_POSA_X) <= -1100.0f)
					{
						g_Enemy_typeA[i].pos.z = -1100.0f + ENEMY_TYPEA_POSA_X;
					}
				}
			}
			else if (g_Enemy_typeA[i].pos.y < 50.0f&&g_Enemy_typeA[i].pos.z <= 300.0f)
			{
				//1�K
				if ((g_Enemy_typeA[i].pos.x) >= -200.0f && (g_Enemy_typeA[i].pos.z) <= -500.0f
					&& (g_Enemy_typeA[i].pos.z) >= -1100.0f)
				{
					if ((g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X) > -500.0f)
					{
						g_Enemy_typeA[i].pos.z = -500.0f - ENEMY_TYPEA_POSA_X;
					}
					else if ((g_Enemy_typeA[i].pos.z - ENEMY_TYPEA_POSA_X) < -1100.0f)
					{
						g_Enemy_typeA[i].pos.z = -1100.0f + ENEMY_TYPEA_POSA_X;
					}
				}
			}else if (g_Enemy_typeA[i].pos.z + ENEMY_TYPEA_POSA_X > 250.0f)
			{
				g_Enemy_typeA[i].pos.z = 250.0f - ENEMY_TYPEA_POSA_X;
			}
			else if (g_Enemy_typeA[i].pos.z - ENEMY_TYPEA_POSA_X < -1500.0f)
			{
				g_Enemy_typeA[i].pos.z = -1500.0f + ENEMY_TYPEA_POSA_X;
			}

			if (g_Enemy_typeA[i].pos.y < 1.0f)
			{
				g_Enemy_typeA[i].pos.y = 1.0f;
			}
			else if (g_Enemy_typeA[i].pos.y > 620.0f)
			{
				g_Enemy_typeA[i].pos.y = 620.0f;
			}
			
		}
	}
	//typeB�̏���
	for (int i = 0; i < MAX_ENEMY_TYPEB; i++)
	{
		//����ENEMY���񂾂�A��莞�Ԍ�d������
		if (g_Enemy_typeB[i].use == false)
		{
			if (g_Enemy_typeB[i].Rebirthtime == REBIRTH_TYPEB_TIME)
			{
				gamedata->money += 15;
			}
			g_Enemy_typeB[i].Rebirthtime -= g_Enemy_typeB[i].Rebirthtime_plue;
			if (g_Enemy_typeB[i].Rebirthtime <= 0)
			{
				g_Enemy_typeB[i].Rebirthtime = REBIRTH_TYPEB_TIME;
				g_Enemy_typeB[i].pos = typeB_Rebirth_pos[i];
				g_Enemy_typeB[i].use = true;
				
			}
			continue;
		}
		//�J����check�p��POS�𒲐�����
		g_Enemy_typeB[i].left_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].left_pos.x -= ENEMY_TYPEB_POSA_X;
		g_Enemy_typeB[i].left_pos.y += ENEMY_TYPEB_POSB_YPOS- ENEMY_TYPEB_POSB_Y / 2;
		g_Enemy_typeB[i].right_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].right_pos.x += ENEMY_TYPEB_POSA_X;
		g_Enemy_typeB[i].right_pos.y += ENEMY_TYPEB_POSB_YPOS - ENEMY_TYPEB_POSB_Y / 2;
		g_Enemy_typeB[i].front_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].front_pos.z -= ENEMY_TYPEB_POSA_X;
		g_Enemy_typeB[i].front_pos.y += ENEMY_TYPEB_POSB_YPOS - ENEMY_TYPEB_POSB_Y / 2;
		g_Enemy_typeB[i].back_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].back_pos.z += ENEMY_TYPEB_POSA_X;
		g_Enemy_typeB[i].back_pos.y += ENEMY_TYPEB_POSB_YPOS - ENEMY_TYPEB_POSB_Y / 2;
		g_Enemy_typeB[i].top_pos = g_Enemy_typeB[i].pos;
		g_Enemy_typeB[i].top_pos.y += ENEMY_TYPEB_POSB_YPOS + ENEMY_TYPEB_POSB_Y / 2;
		g_Enemy_typeB[i].down_pos = g_Enemy_typeB[i].pos;
		//hitcheck�p������POS�𒲐�����
		{
			g_Enemy_typeB[i].HitcheckPosA = g_Enemy_typeB[i].pos;
			g_Enemy_typeB[i].HitcheckPosA.y += g_Enemy_typeB[i].HitcheckPosA_size.y;
			g_Enemy_typeB[i].HitcheckPosB = g_Enemy_typeB[i].pos;
			g_Enemy_typeB[i].HitcheckPosB.y += ENEMY_TYPEB_POSB_YPOS;
		}
		//�J����check�p��POS�𒲐�����

		//�����U�����󂯂���A���G���Ԃ̐ݒ�A�����Ď��ԏI�������A�܂��U�����󂯂�ł��܂�
		if (g_Enemy_typeB[i].beHit == true)
		{
			g_Enemy_typeB[i].Hit_CDtime -= g_Enemy_typeB[i].Hit_CDtime_plue;
			if (g_Enemy_typeB[i].Hit_CDtime <= 0)
			{
				g_Enemy_typeB[i].beHit = false;
				g_Enemy_typeB[i].Hit_CDtime = HIT_CDTIME;
			}
		}
		//�U���̏���
		if (player[0].pos.x > -600.0f&&player[0].pos.x < 600.0f
			&&player[0].pos.z>250.0f)
		{
			continue;
		}
		else
		{
			bool ans = enemy_player(player[0].pos, g_Enemy_typeB[i].pos, 2, g_Enemy_typeB[i].rot);
			if (ans == true)
			{
				//enemy��ROT�̐���
				{
					D3DXVECTOR3 muke1 =  player[0].pos- g_Enemy_typeB[i].pos ;
					D3DXVECTOR3 muke2 = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
					float rang = acosf(D3DXVec3Dot(&muke1, &muke2) / (((float)sqrt((muke1.x*muke1.x) + (muke1.y*muke1.y) + (muke1.z*muke1.z)))));
					if (g_Enemy_typeB[i].pos.x < player[0].pos.x)
					{
						rang = -rang;
					}
					if (g_Enemy_typeB[i].rot.y <= (rang + 0.2f) && g_Enemy_typeB[i].rot.y >= (rang - 0.2f))
					{
						g_Enemy_typeB[i].rot.y = rang;
					}
					else
					{
						if (g_Enemy_typeB[i].rot.y > rang)
						{
							g_Enemy_typeB[i].rot.y -= g_Enemy_typeB[i].move_rot_plue;
							g_Enemy_typeB[i].shout_time = 0.8f;

						}
						else if (g_Enemy_typeB[i].rot.y < rang)
						{
							g_Enemy_typeB[i].rot.y += g_Enemy_typeB[i].move_rot_plue;
							g_Enemy_typeB[i].shout_time = 0.8f;
						}
					}
				}

				//�U���̏���
				if (g_Enemy_typeB[i].shout_time > 1.0f)
				{


					D3DXVECTOR3 shoutpos = g_Enemy_typeB[i].pos;
					shoutpos.x -= sinf(g_Enemy_typeB[i].rot.y)*g_Enemy_typeB[i].shoutpos_plue.z;
					shoutpos.y += g_Enemy_typeB[i].shoutpos_plue.y;
					shoutpos.z -= cosf(g_Enemy_typeB[i].rot.y)*g_Enemy_typeB[i].shoutpos_plue.z;
					SetEnemyBullet(shoutpos, g_Enemy_typeB[i].rot, 3);
					g_Enemy_typeB[i].shout_time = 0.0f;

				}
				else
				{
					g_Enemy_typeB[i].shout_time += g_Enemy_typeB[i].shout_time_plue;
				}


			}
		}
	}
	//typeC�̏���
	for (int i = 0; i < MAX_ENEMY_TYPEC; i++)
	{
		//����ENEMY���񂾂�A��莞�Ԍ�d������
		if (g_Enemy_typeC[i].use == false)
		{
			if (g_Enemy_typeC[i].Rebirthtime == REBIRTH_TYPEC_TIME)
			{
				gamedata->money += 20;
			}
			ReleaseShadow(g_Enemy_typeC[i].shadowIdx);
			g_Enemy_typeC[i].Rebirthtime -= g_Enemy_typeC[i].Rebirthtime_plue;
			if (g_Enemy_typeC[i].Rebirthtime <= 0)
			{
				g_Enemy_typeC[i].pos = typeC_Rebirth_pos[i];
				g_Enemy_typeC[i].Rebirthtime = REBIRTH_TYPEC_TIME;
				g_Enemy_typeC[i].use = true;
				g_Enemy_typeC[i].move_time = 0.0f;
				g_Enemy_typeC[i].typeCmoveY = false;
				D3DXVECTOR3 pos = g_Enemy_typeC[i].pos;
				g_Enemy_typeC[i].shadowIdx = CreateShadow(pos, ENEMY_TYPEC_SHADOW_SIZE, ENEMY_TYPEC_SHADOW_SIZE);
			}
			continue;
		}
		//�J����check�p��POS�𒲐�����
		g_Enemy_typeC[i].left_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].left_pos.x -= ENEMY_TYPEC_POSB_Z * 2 / 3;
		g_Enemy_typeC[i].left_pos.y += ENEMY_TYPEC_POSB_YPOS ;
		g_Enemy_typeC[i].right_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].right_pos.x += ENEMY_TYPEC_POSB_Z*2/3;
		g_Enemy_typeC[i].right_pos.y += ENEMY_TYPEC_POSB_YPOS ;
		g_Enemy_typeC[i].front_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].front_pos.z -= ENEMY_TYPEC_POSB_X;
		g_Enemy_typeC[i].front_pos.y += ENEMY_TYPEC_POSB_YPOS ;
		g_Enemy_typeC[i].back_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].back_pos.z += ENEMY_TYPEC_POSB_X;
		g_Enemy_typeC[i].back_pos.y += ENEMY_TYPEC_POSB_YPOS ;
		g_Enemy_typeC[i].top_pos = g_Enemy_typeC[i].pos;
		g_Enemy_typeC[i].top_pos.y += ENEMY_TYPEC_POSB_YPOS*2;
		g_Enemy_typeC[i].down_pos = g_Enemy_typeC[i].pos;
		//hitcheck�p������POS�𒲐�����
		{
			g_Enemy_typeC[i].HitcheckPosA = g_Enemy_typeC[i].pos;
			g_Enemy_typeC[i].HitcheckPosA.y += g_Enemy_typeC[i].HitcheckPosA_size.y;
			g_Enemy_typeC[i].HitcheckPosB = g_Enemy_typeC[i].pos;
			g_Enemy_typeC[i].HitcheckPosB.y += ENEMY_TYPEC_POSB_YPOS;
		}
		//�����U�����󂯂���A���G���Ԃ̐ݒ�A�����Ď��ԏI�������A�܂��U�����󂯂�ł��܂�
		if (g_Enemy_typeC[i].beHit == true)
		{
			g_Enemy_typeC[i].Hit_CDtime -= g_Enemy_typeC[i].Hit_CDtime_plue;
			if (g_Enemy_typeC[i].Hit_CDtime <= 0)
			{
				g_Enemy_typeC[i].beHit = false;
				g_Enemy_typeC[i].Hit_CDtime = HIT_CDTIME;
			}
		}
		//�ړ��̏���
		if (g_Enemy_typeC[i].typeCmoveY == false)
		{
			g_Enemy_typeC[i].pos.y++;
			g_Enemy_typeC[i].move_time += g_Enemy_typeC[i].move_time_plue;


			if (g_Enemy_typeC[i].move_time >= 4.0f)
			{
				g_Enemy_typeC[i].move_time = 0.0f;
				g_Enemy_typeC[i].typeCmoveY = true;
			}

		}
		else
		{
			g_Enemy_typeC[i].pos.y--;
			g_Enemy_typeC[i].move_time += g_Enemy_typeC[i].move_time_plue;

			if (g_Enemy_typeC[i].move_time >= 4.0f)
			{
				g_Enemy_typeC[i].move_time = 0.0f;
				g_Enemy_typeC[i].typeCmoveY = false;
			}

		}
		// �e���v���C���[�̈ʒu�ɍ��킹��
		D3DXVECTOR3 shadow_pos = g_Enemy_typeC[i].pos;
		shadow_pos.y = 251.0;
		float shadow_size = ENEMY_TYPEC_SHADOW_SIZE;
		float parts = (4.0f / g_Enemy_typeC[i].move_time_plue) + 1.0f;
		if (g_Enemy_typeC[i].pos.y >= typeC_Rebirth_pos[i].y)
		{
			shadow_size = ENEMY_TYPEC_SHADOW_SIZE - (ENEMY_TYPEC_SHADOW_SIZE - ENEMY_TYPEC_SHADOW_SIZE_MIN)*((g_Enemy_typeC[i].pos.y - typeC_Rebirth_pos[i].y) / (parts));
		}
		else
		{
			shadow_size = ENEMY_TYPEC_SHADOW_SIZE;
		}
		SetPositionShadow(g_Enemy_typeC[i].shadowIdx, shadow_pos, shadow_size, shadow_size);

		if (player[0].pos.x > -600.0f&&player[0].pos.x < 600.0f
			&&player[0].pos.z>250.0f)
		{
			continue;
		}
		else
		{
			bool ans = enemy_player(player[0].pos, g_Enemy_typeC[i].pos, 3, g_Enemy_typeC[i].rot);
			if (ans == true)
			{
				//enemy��ROT�̐���
				{
					D3DXVECTOR3 muke1 = g_Enemy_typeC[i].pos - player[0].pos;
					D3DXVECTOR3 muke2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
					float rang = acosf(D3DXVec3Dot(&muke1, &muke2) / (((float)sqrt((muke1.x*muke1.x) + (muke1.y*muke1.y) + (muke1.z*muke1.z)))));
					if (g_Enemy_typeC[i].pos.x < player[0].pos.x)
					{
						rang = -rang;
					}

					if (g_Enemy_typeC[i].rot.y <= (rang + 0.5f) && g_Enemy_typeC[i].rot.y >= (rang - 0.5f))
					{
						g_Enemy_typeC[i].rot.y = rang;
					}
					else
					{
						if (g_Enemy_typeC[i].rot.y > rang)
						{
							g_Enemy_typeC[i].rot.y -= g_Enemy_typeC[i].move_rot_plue;

						}
						else if (g_Enemy_typeC[i].rot.y < rang)
						{
							g_Enemy_typeC[i].rot.y += g_Enemy_typeC[i].move_rot_plue;
						}
					}
				}

				//�U���̏���
				if (g_Enemy_typeC[i].shout_time > 1.0f)
				{
					D3DXVECTOR3 shoutpos = g_Enemy_typeC[i].pos;
					shoutpos.x -= sinf(g_Enemy_typeC[i].rot.y)*g_Enemy_typeC[i].shoutpos_plue.z;
					shoutpos.y += g_Enemy_typeC[i].shoutpos_plue.y;
					shoutpos.z -= cosf(g_Enemy_typeC[i].rot.y)*g_Enemy_typeC[i].shoutpos_plue.z;
					SetEnemyBullet(shoutpos, g_Enemy_typeC[i].rot, 2);
					g_Enemy_typeC[i].shout_time = 0.0f;

				}
				else
				{
					g_Enemy_typeC[i].shout_time += g_Enemy_typeC[i].shout_time_plue;
				}


			}
		}
		
		
	}
	//typeD�̏���
	for (int i = 0; i < MAX_ENEMY_TYPED; i++)
	{
		g_Enemy_typeD[i].pos.y--;
		//����ENEMY���񂾂�A��莞�Ԍ�d������
		if (g_Enemy_typeD[i].use == false)
		{
			if (g_Enemy_typeD[i].Rebirthtime == REBIRTH_TYPED_TIME)
			{
				gamedata->money += 20;
			}
			ReleaseShadow(g_Enemy_typeD[i].shadowIdx);
			g_Enemy_typeD[i].Rebirthtime -= g_Enemy_typeD[i].Rebirthtime_plue;
			
			if (g_Enemy_typeD[i].Rebirthtime <= 0)
			{
				g_Enemy_typeD[i].Rebirthtime = REBIRTH_TYPED_TIME;
				g_Enemy_typeD[i].pos = typeD_Rebirth_pos[i];
				g_Enemy_typeD[i].use = true;

				D3DXVECTOR3 pos = g_Enemy_typeD[i].pos;
				pos.y = 1.0f;
				g_Enemy_typeD[i].shadowIdx = CreateShadow(pos, ENEMY_TYPED_SHADOW_SIZE, ENEMY_TYPED_SHADOW_SIZE);
			}
			continue;
		}
		//�J����check�p��POS�𒲐�����
		g_Enemy_typeD[i].left_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].left_pos.x -= ENEMY_TYPED_POSB_XPOS;
		g_Enemy_typeD[i].left_pos.y += ENEMY_TYPED_POSB_YPOS- ENEMY_TYPED_POSB_Y /2;
		g_Enemy_typeD[i].right_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].right_pos.x += ENEMY_TYPED_POSB_XPOS;
		g_Enemy_typeD[i].right_pos.y += ENEMY_TYPED_POSB_YPOS - ENEMY_TYPED_POSB_Y / 2;
		g_Enemy_typeD[i].front_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].front_pos.z -= ENEMY_TYPED_POSB_XPOS;
		g_Enemy_typeD[i].front_pos.y += ENEMY_TYPED_POSB_YPOS - ENEMY_TYPED_POSB_Y / 2;
		g_Enemy_typeD[i].back_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].back_pos.z += ENEMY_TYPED_POSB_XPOS;
		g_Enemy_typeD[i].back_pos.y += ENEMY_TYPED_POSB_YPOS - ENEMY_TYPED_POSB_Y / 2;
		g_Enemy_typeD[i].top_pos = g_Enemy_typeD[i].pos;
		g_Enemy_typeD[i].top_pos.y += ENEMY_TYPED_POSB_YPOS + ENEMY_TYPED_POSB_Y / 2;
		g_Enemy_typeD[i].down_pos = g_Enemy_typeD[i].pos;
		//hitcheck�p������POS�𒲐�����
		{
			g_Enemy_typeD[i].HitcheckPosA = g_Enemy_typeD[i].pos;
			g_Enemy_typeD[i].HitcheckPosA.y += g_Enemy_typeD[i].HitcheckPosA_size.y;
			g_Enemy_typeD[i].HitcheckPosB = g_Enemy_typeD[i].pos;
			g_Enemy_typeD[i].HitcheckPosB.y += ENEMY_TYPED_POSB_YPOS;
			g_Enemy_typeD[i].HitcheckPosB.x += (float)(sinf(g_Enemy_typeD[i].rot.y)*ENEMY_TYPED_POSB_XPOS);
			g_Enemy_typeD[i].HitcheckPosB.z -=(float)(cosf(g_Enemy_typeD[i].rot.y)*ENEMY_TYPED_POSB_XPOS);

		}
		//�����U�����󂯂���A���G���Ԃ̐ݒ�A�����Ď��ԏI�������A�܂��U�����󂯂�ł��܂�
		if (g_Enemy_typeD[i].beHit == true)
		{
			g_Enemy_typeD[i].Hit_CDtime -= g_Enemy_typeD[i].Hit_CDtime_plue;
			if (g_Enemy_typeD[i].Hit_CDtime <= 0)
			{
				g_Enemy_typeD[i].beHit = false;
				g_Enemy_typeD[i].Hit_CDtime = HIT_CDTIME;
			}
		}
		//�s���̐���
		if (player[0].pos.x > -600.0f&&player[0].pos.x < 600.0f
			&&player[0].pos.z>250.0f)
		{
			//�ړ��̏���
			if (g_Enemy_typeD[i].move_time >= 1)
			{
				move_A = rand() % 20 + 1;
				if (move_A > 11)
				{
					g_Enemy_typeD[i].move_rot = g_Enemy_typeD[i].rot.y - (D3DX_PI) / (move_A - 11);
					g_Enemy_typeD[i].move_rot_plue = (float)fabs((g_Enemy_typeD[i].rot.y - g_Enemy_typeD[i].move_rot) / 6);
					if (g_Enemy_typeD[i].move_rot > D3DX_PI)
					{
						g_Enemy_typeD[i].move_rot -= 2 * D3DX_PI;
					}
				}
				else
				{
					g_Enemy_typeD[i].move_rot = g_Enemy_typeD[i].rot.y + (D3DX_PI) / (move_A);
					g_Enemy_typeD[i].move_rot_plue = (float)fabs((g_Enemy_typeD[i].rot.y - g_Enemy_typeD[i].move_rot) / 6);
					if (g_Enemy_typeD[i].move_rot < -D3DX_PI)
					{
						g_Enemy_typeD[i].move_rot += 2 * D3DX_PI;
					}
				}
				g_Enemy_typeD[i].move_time = 0.0f;
			}
			else
			{
				g_Enemy_typeD[i].move_time += g_Enemy_typeD[i].move_time_plue;
			}


			if (g_Enemy_typeD[i].rot.y <= (g_Enemy_typeD[i].move_rot + 0.1f) && g_Enemy_typeD[i].rot.y >= (g_Enemy_typeD[i].move_rot - 0.1f))
			{
				g_Enemy_typeD[i].rot.y = g_Enemy_typeD[i].move_rot;

				g_Enemy_typeD[i].pos.x -= sinf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
				g_Enemy_typeD[i].pos.z -= cosf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;

			}
			else
			{
				if (g_Enemy_typeD[i].move_rot > g_Enemy_typeD[i].rot.y)
				{
					g_Enemy_typeD[i].rot.y += g_Enemy_typeD[i].move_rot_plue;

				}
				else if (g_Enemy_typeD[i].move_rot < g_Enemy_typeD[i].rot.y)
				{
					g_Enemy_typeD[i].rot.y -= g_Enemy_typeD[i].move_rot_plue;
				}


			}
			
		}
		else
		{
			bool ans = enemy_player(player[0].pos, g_Enemy_typeD[i].pos, 4, g_Enemy_typeD[i].rot);
			if (ans == true)
			{
				//enemy��ROT�̐���
				{
					D3DXVECTOR3 muke1 = g_Enemy_typeD[i].pos - player[0].pos;
					D3DXVECTOR3 muke2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
					float rang = acosf(D3DXVec3Dot(&muke1, &muke2) / (((float)sqrt((muke1.x*muke1.x) + (muke1.y*muke1.y) + (muke1.z*muke1.z)))));
					if (g_Enemy_typeD[i].pos.x < player[0].pos.x)
					{
						rang = -rang;
					}
					if (g_Enemy_typeD[i].rot.y <= (rang + 0.5f) && g_Enemy_typeD[i].rot.y >= (rang - 0.5f))
					{
						g_Enemy_typeD[i].rot.y = rang;
					}
					else
					{
						if (g_Enemy_typeD[i].rot.y > rang)
						{
							g_Enemy_typeD[i].rot.y -= g_Enemy_typeD[i].move_rot_plue;

						}
						else if (g_Enemy_typeD[i].rot.y < rang)
						{
							g_Enemy_typeD[i].rot.y += g_Enemy_typeD[i].move_rot_plue;
						}
					}
				}
				D3DXVECTOR3 kyori =  g_Enemy_typeD[i].pos-player[0].pos;
				float kyori1 = (float)sqrt((kyori.x*kyori.x)+(kyori.y*kyori.y)+(kyori.z*kyori.z));
				bool HitAans = CollisionBB(player[i].pos, g_Enemy_typeD[i].HitcheckPosA,
					player[i].touchsize, g_Enemy_typeD[i].HitcheckPosA_size);
				bool HitBans = CollisionBB(player[i].pos, g_Enemy_typeD[i].HitcheckPosB,
					player[i].touchsize, g_Enemy_typeD[i].HitcheckPosB_size);
				//�U���̏���
				if (HitAans==true|| HitBans==true)
				{
					g_Enemy_typeD[i].touchnow = true;

					g_Enemy_typeD[i].shout_time = 0.0f;
				}

				if (g_Enemy_typeD[i].touchnow == true)
				{
					g_Enemy_typeD[i].shout_time += 0.1f;
					g_Enemy_typeD[i].pos.x += sinf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
					g_Enemy_typeD[i].pos.z += cosf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
					if (g_Enemy_typeD[i].shout_time >= 1.0f)
					{
						g_Enemy_typeD[i].touchnow = false;
					}
				}
				else
				{
					g_Enemy_typeD[i].pos.x -= sinf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
					g_Enemy_typeD[i].pos.z -= cosf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
				}

			}
			else
			{
				//�ړ��̏���
				if (g_Enemy_typeD[i].move_time >= 1)
				{
					move_A = rand() % 20 + 1;
					if (move_A > 11)
					{
						g_Enemy_typeD[i].move_rot = g_Enemy_typeD[i].rot.y - (D3DX_PI) / (move_A - 11);
						g_Enemy_typeD[i].move_rot_plue = (float)fabs((g_Enemy_typeD[i].rot.y - g_Enemy_typeD[i].move_rot) / 6);
						if (g_Enemy_typeD[i].move_rot > D3DX_PI)
						{
							g_Enemy_typeD[i].move_rot -= 2 * D3DX_PI;
						}
					}
					else
					{
						g_Enemy_typeD[i].move_rot = g_Enemy_typeD[i].rot.y + (D3DX_PI) / (move_A);
						g_Enemy_typeD[i].move_rot_plue = (float)fabs((g_Enemy_typeD[i].rot.y - g_Enemy_typeD[i].move_rot) / 6);
						if (g_Enemy_typeD[i].move_rot < -D3DX_PI)
						{
							g_Enemy_typeD[i].move_rot += 2 * D3DX_PI;
						}
					}
					g_Enemy_typeD[i].move_time = 0.0f;
				}
				else
				{
					g_Enemy_typeD[i].move_time += g_Enemy_typeD[i].move_time_plue;
				}


				if (g_Enemy_typeD[i].rot.y <= (g_Enemy_typeD[i].move_rot + 0.1f) && g_Enemy_typeD[i].rot.y >= (g_Enemy_typeD[i].move_rot - 0.1f))
				{
					g_Enemy_typeD[i].rot.y = g_Enemy_typeD[i].move_rot;

					g_Enemy_typeD[i].pos.x -= sinf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;
					g_Enemy_typeD[i].pos.z -= cosf(g_Enemy_typeD[i].rot.y) * g_Enemy_typeD[i].spd;

				}
				else
				{
					if (g_Enemy_typeD[i].move_rot > g_Enemy_typeD[i].rot.y)
					{
						g_Enemy_typeD[i].rot.y += g_Enemy_typeD[i].move_rot_plue;

					}
					else if (g_Enemy_typeD[i].move_rot < g_Enemy_typeD[i].rot.y)
					{
						g_Enemy_typeD[i].rot.y -= g_Enemy_typeD[i].move_rot_plue;
					}


				}


			}


		}

		if ((g_Enemy_typeD[i].pos.z + ENEMY_TYPED_POSA_Z) > 300.0f)
		{
			g_Enemy_typeD[i].pos.z = 300.0f - ENEMY_TYPED_POSA_Z;
		}
		else if ((g_Enemy_typeD[i].pos.z - ENEMY_TYPED_POSA_Z) < -1500.0f)
		{
			g_Enemy_typeD[i].pos.z = -1500.0f + ENEMY_TYPED_POSA_Z;
		}

		//SHADOW
		{
			// �e���v���C���[�̈ʒu�ɍ��킹��
			D3DXVECTOR3 shadow_pos = g_Enemy_typeD[i].pos;
			shadow_pos.y = 1.0;
			float shadow_size = ENEMY_TYPED_SHADOW_SIZE;

			for (int i = 0; i < MAX_MESH_FIELD; i++)
			{
				if (g_Enemy_typeD[i].pos.x >= g_fieldpoint[i].pos0.x&&g_Enemy_typeD[i].pos.x <= g_fieldpoint[i].pos1.x
					&&g_Enemy_typeD[i].pos.z >= g_fieldpoint[i].pos2.z&&g_Enemy_typeD[i].pos.z <= g_fieldpoint[i].pos0.z
					&&g_Enemy_typeD[i].pos.y >= g_fieldpoint[i].pos0.y)
				{
					if (g_fieldpoint[i].pos0.y > shadow_pos.y)
					{
						shadow_pos.y = g_fieldpoint[i].pos0.y + 1.0f;

					}
				}
			}

			if (g_Enemy_typeD[i].pos.y - shadow_pos.y >= PLAYER_SHADOW_SIZE_YPOS_MAX)
			{
				shadow_size = ENEMY_TYPED_SHADOW_SIZE_MIN;
			}
			else
			{
				if (g_Enemy_typeD[i].pos.y != shadow_pos.y)
				{
					shadow_size = ENEMY_TYPED_SHADOW_SIZE - (ENEMY_TYPED_SHADOW_SIZE - ENEMY_TYPED_SHADOW_SIZE_MIN)*((g_Enemy_typeD[i].pos.y - shadow_pos.y) / PLAYER_SHADOW_SIZE_YPOS_MAX);
				}

			}
			SetPositionShadow(g_Enemy_typeD[i].shadowIdx, shadow_pos, shadow_size, shadow_size);
		}
	}
	//boss�̏���
	for (int i = 0; i < MAX_BOSS; i++)
	{
		
		if (g_Enemy_BOSS[i].use == false)continue;
		//hitcheck�p������POS�𒲐�����
		{
			g_Enemy_BOSS[i].HitcheckPosA = g_Enemy_BOSS[i].pos;
			g_Enemy_BOSS[i].HitcheckPosA.y += g_Enemy_BOSS[i].HitcheckPosA_size.y;
			g_Enemy_BOSS[i].HitcheckPosB = g_Enemy_BOSS[i].pos;
			g_Enemy_BOSS[i].HitcheckPosB.y += ENEMY_BOSS_POSB_YPOS;
			g_Enemy_BOSS[i].HitcheckPosC = g_Enemy_BOSS[i].pos;
			g_Enemy_BOSS[i].HitcheckPosC.y += ENEMY_BOSS_POSC_YPOS;
		}
		//�����U�����󂯂���A���G���Ԃ̐ݒ�A�����Ď��ԏI�������A�܂��U�����󂯂�ł��܂�
		
		if (g_Enemy_BOSS[i].beHit == true)
		{
			g_Enemy_BOSS[i].Hit_CDtime -= g_Enemy_BOSS[i].Hit_CDtime_plue;
			if (g_Enemy_BOSS[i].Hit_CDtime <= 0)
			{
				g_Enemy_BOSS[i].beHit = false;
				g_Enemy_BOSS[i].Hit_CDtime = HIT_CDTIME_BOSS;
			}
		}
		
		if (player[0].pos.x > -600.0f&&player[0].pos.x < 600.0f
			&&player[0].pos.z>250.0f)
		{
			//�ړ��̏���
			if (g_Enemy_BOSS[i].move_time >= 4.0f)
			{
				if (g_Enemy_BOSS[i].jumpup_time < 1.0f)
				{
					float	jumopos =50.0f;							// XYZ�ړ��ʂ��v�Z���Ă���
					int nowNo = (int)g_Enemy_BOSS[i].jumpup_time;				// �������ł���e�[�u���ԍ������o���Ă���
					float nowTime = g_Enemy_BOSS[i].jumpup_time - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
					float jumopos2 = jumopos;
					jumopos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
					jumopos2 -= jumopos;
					// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
					g_Enemy_BOSS[i].pos.y += jumopos2;
					g_Enemy_BOSS[i].jumpup_time_plue += 0.001f;
					g_Enemy_BOSS[i].jumpup_time += g_Enemy_BOSS[i].jumpup_time_plue;		// ���Ԃ�i�߂Ă���
				}
				else
				{
					float	downpos = 7.0f;							// XYZ�ړ��ʂ��v�Z���Ă���
					float nowTime = g_Enemy_BOSS[i].jumpdown_time;			// ���ԕ����ł��鏭�������o���Ă���
					downpos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
					// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
					g_Enemy_BOSS[i].pos.y -= downpos;
					g_Enemy_BOSS[i].jumpdown_time_plue += 0.001f;
					g_Enemy_BOSS[i].jumpdown_time += g_Enemy_BOSS[i].jumpdown_time_plue;	// ���Ԃ�i�߂Ă���
					if (g_Enemy_BOSS[i].pos.y < 1.0f)
					{
						g_Enemy_BOSS[i].pos.y = 1.0f;
						g_Enemy_BOSS[i].move_time = 0.0f;

						g_Enemy_BOSS[i].jumpup_time = 0.0f;
						g_Enemy_BOSS[i].jumpup_time_plue = 0.1f;
						g_Enemy_BOSS[i].jumpdown_time = 0.0f;
						g_Enemy_BOSS[i].jumpdown_time_plue = 0.1f;
						if (player[0].namidause == false)
						{
							player[0].namidause = true;
						}
					}
				}
				
				
			}
			else
			{
				g_Enemy_BOSS[i].move_time += g_Enemy_BOSS[i].move_time_plue;
			}
			//enemy��ROT�̐���
			{
				D3DXVECTOR3 muke1 = player[0].pos - g_Enemy_BOSS[i].pos;
				D3DXVECTOR3 muke2 = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				float rang = acosf(D3DXVec3Dot(&muke1, &muke2) / (((float)sqrt((muke1.x*muke1.x) + (muke1.y*muke1.y) + (muke1.z*muke1.z)))));
				if (g_Enemy_BOSS[i].pos.x < player[0].pos.x)
				{
					rang = -rang;
				}

				if (g_Enemy_BOSS[i].rot.y <= (rang + 0.2f) && g_Enemy_BOSS[i].rot.y >= (rang - 0.2f))
				{
					g_Enemy_BOSS[i].rot.y = rang;
				}
				else
				{
					if (g_Enemy_BOSS[i].rot.y > rang)
					{
						g_Enemy_BOSS[i].rot.y -= g_Enemy_BOSS[i].move_rot_plue;
						g_Enemy_BOSS[i].shout_time = 0.8f;

					}
					else if (g_Enemy_BOSS[i].rot.y < rang)
					{
						g_Enemy_BOSS[i].rot.y += g_Enemy_BOSS[i].move_rot_plue;
						g_Enemy_BOSS[i].shout_time = 0.8f;
					}
				}
			}

			//�U���̏���
			if (g_Enemy_BOSS[i].shout_time > 4.0f)
			{


				D3DXVECTOR3 shoutpos = g_Enemy_BOSS[i].pos;
				shoutpos.x -= sinf(g_Enemy_BOSS[i].rot.y)*g_Enemy_BOSS[i].shoutpos_plue.z;
				shoutpos.y += g_Enemy_BOSS[i].shoutpos_plue.y;
				shoutpos.z -= cosf(g_Enemy_BOSS[i].rot.y)*g_Enemy_BOSS[i].shoutpos_plue.z;
				SetEnemyBullet(shoutpos, g_Enemy_BOSS[i].rot, 4);
				g_Enemy_BOSS[i].shout_time = 0.0f;

			}
			else
			{
				g_Enemy_BOSS[i].shout_time += g_Enemy_BOSS[i].shout_time_plue;
			}

		}
		else
		{
			g_Enemy_BOSS[i].shout_time = 0.0f;
			g_Enemy_BOSS[i].shout_time_plue = 0.1f;

			g_Enemy_BOSS[i].jumpup_time = 0.0f;
			g_Enemy_BOSS[i].jumpup_time_plue = 0.1f;
			g_Enemy_BOSS[i].jumpdown_time = 0.0f;
			g_Enemy_BOSS[i].jumpdown_time_plue = 0.1f;
			g_Enemy_BOSS[i].move_time = 0.0f;
			g_Enemy_BOSS[i].move_time_plue = 0.01f;
			g_Enemy_BOSS[i].shock_time = 0.0f;
		}
		//SHADOW
		{
		// �e���v���C���[�̈ʒu�ɍ��킹��
		D3DXVECTOR3 shadow_pos = g_Enemy_BOSS[i].pos;
		shadow_pos.y = 1.0;
		float shadow_size = ENEMY_BOSS_SHADOW_SIZE;

		if (g_Enemy_BOSS[i].pos.y - shadow_pos.y >= PLAYER_SHADOW_SIZE_YPOS_MAX)
		{
			shadow_size = ENEMY_BOSS_SHADOW_SIZE_MIN;
		}
		else
		{
			if (g_Enemy_BOSS[i].pos.y != shadow_pos.y)
			{
				shadow_size = ENEMY_BOSS_SHADOW_SIZE - (ENEMY_BOSS_SHADOW_SIZE - ENEMY_BOSS_SHADOW_SIZE_MIN)*((g_Enemy_BOSS[i].pos.y - shadow_pos.y) / PLAYER_SHADOW_SIZE_YPOS_MAX);
			}

		}
		SetPositionShadow(g_Enemy_BOSS[i].shadowIdx, shadow_pos, shadow_size, shadow_size);
		}
		if (player[0].namidause == true)
		{
			g_Enemy_BOSS[i].shock_time += g_Enemy_BOSS[i].shock_time_plue;
			if (g_Enemy_BOSS[i].shock_time > 8.0f)
			{
				player[0].namidause = false;
				g_Enemy_BOSS[i].shock_time = 0.0f;
			}
		}
	}


	MeshfieldTouch(4);
	//MeshceilingTouch(4);
	MeshwallTouch(4);
	SlopewallTouch(4);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
	{
		if (g_Enemy_typeA[i].use == false) continue;
		if (gamedata->drawopen == false)
		{
			if (g_Enemy_typeA[i].drawcheck == false) { continue; }
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy_typeA[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Enemy_typeA[i].scl.x, g_Enemy_typeA[i].scl.y, g_Enemy_typeA[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy_typeA[i].mtxWorld, &g_Enemy_typeA[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy_typeA[i].rot.y, g_Enemy_typeA[i].rot.x, g_Enemy_typeA[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy_typeA[i].mtxWorld, &g_Enemy_typeA[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy_typeA[i].pos.x, g_Enemy_typeA[i].pos.y, g_Enemy_typeA[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy_typeA[i].mtxWorld, &g_Enemy_typeA[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Enemy_typeA[i].mtxWorld);

		if (g_Enemy_typeA[i].beHit == true)
		{
			// ���f���`��
			DrawModel(&enemytypeA_beHit_model);
		}
		else if (g_Enemy_typeA[i].beHit == false)
		{
			// ���f���`��
			DrawModel(&enemytypeA_model);
		}
		
	}
	for (int i = 0; i < MAX_ENEMY_TYPEB; i++)
	{
		if (g_Enemy_typeB[i].use == false) continue;
		if (gamedata->drawopen == false)
		{
			if (g_Enemy_typeB[i].drawcheck == false) { continue; }
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy_typeB[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Enemy_typeB[i].scl.x, g_Enemy_typeB[i].scl.y, g_Enemy_typeB[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy_typeB[i].mtxWorld, &g_Enemy_typeB[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy_typeB[i].rot.y, g_Enemy_typeB[i].rot.x, g_Enemy_typeB[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy_typeB[i].mtxWorld, &g_Enemy_typeB[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy_typeB[i].pos.x, g_Enemy_typeB[i].pos.y, g_Enemy_typeB[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy_typeB[i].mtxWorld, &g_Enemy_typeB[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Enemy_typeB[i].mtxWorld);

		if (g_Enemy_typeB[i].beHit == true)
		{
			// ���f���`��
			DrawModel(&enemytypeB_beHit_model);
		}
		else
		{
			// ���f���`��
			DrawModel(&enemytypeB_model);
		}
	}
	for (int i = 0; i < MAX_ENEMY_TYPEC; i++)
	{
		if (g_Enemy_typeC[i].use == false) continue;
		if (gamedata->drawopen == false)
		{
			if (g_Enemy_typeC[i].drawcheck == false) { continue; }
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy_typeC[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Enemy_typeC[i].scl.x, g_Enemy_typeC[i].scl.y, g_Enemy_typeC[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy_typeC[i].mtxWorld, &g_Enemy_typeC[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy_typeC[i].rot.y, g_Enemy_typeC[i].rot.x, g_Enemy_typeC[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy_typeC[i].mtxWorld, &g_Enemy_typeC[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy_typeC[i].pos.x, g_Enemy_typeC[i].pos.y, g_Enemy_typeC[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy_typeC[i].mtxWorld, &g_Enemy_typeC[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Enemy_typeC[i].mtxWorld);

		if (g_Enemy_typeC[i].beHit == true)
		{
			// ���f���`��
			DrawModel(&enemytypeC_beHit_model);
		}
		else
		{
			// ���f���`��
			DrawModel(&enemytypeC_model);
		}
	}
	for (int i = 0; i < MAX_ENEMY_TYPED; i++)
	{
		if (g_Enemy_typeD[i].use == false) continue;
		if (gamedata->drawopen == false)
		{
			if (g_Enemy_typeD[i].drawcheck == false) { continue; }
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy_typeD[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Enemy_typeD[i].scl.x, g_Enemy_typeD[i].scl.y, g_Enemy_typeD[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy_typeD[i].mtxWorld, &g_Enemy_typeD[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy_typeD[i].rot.y, g_Enemy_typeD[i].rot.x, g_Enemy_typeD[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy_typeD[i].mtxWorld, &g_Enemy_typeD[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy_typeD[i].pos.x, g_Enemy_typeD[i].pos.y, g_Enemy_typeD[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy_typeD[i].mtxWorld, &g_Enemy_typeD[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Enemy_typeD[i].mtxWorld);

		if (g_Enemy_typeD[i].beHit == true)
		{
			// ���f���`��
			DrawModel(&enemytypeD_beHit_model);
		}
		else
		{
			// ���f���`��
			DrawModel(&enemytypeD_model);
		}
	}
	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Enemy_BOSS[i].use == false) continue;
		if (gamedata->drawopen == false)
		{
			if (g_Enemy_BOSS[i].drawcheck == false) { continue; }
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy_BOSS[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Enemy_BOSS[i].scl.x, g_Enemy_BOSS[i].scl.y, g_Enemy_BOSS[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy_BOSS[i].mtxWorld, &g_Enemy_BOSS[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy_BOSS[i].rot.y, g_Enemy_BOSS[i].rot.x, g_Enemy_BOSS[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy_BOSS[i].mtxWorld, &g_Enemy_BOSS[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy_BOSS[i].pos.x, g_Enemy_BOSS[i].pos.y, g_Enemy_BOSS[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy_BOSS[i].mtxWorld, &g_Enemy_BOSS[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Enemy_BOSS[i].mtxWorld);

		if (g_Enemy_BOSS[i].beHit == true)
		{
			// ���f���`��
			DrawModel(&enemyBOSS_beHit_model);
		}
		else
		{
			// ���f���`��
			DrawModel(&enemyBOSS_model);
		}
	}
	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// ENEMY��PLAYER�̋����͈�
//=============================================================================
bool enemy_player(D3DXVECTOR3 playerpos, D3DXVECTOR3 enemypos, int enemytype, D3DXVECTOR3 rot)
{
	switch (enemytype)
	{
		case 1:
		{
			//TYPEA
			if (playerpos.x <= (enemypos.x + ENEMY_TYPEA_RANG) && playerpos.x >= (enemypos.x - ENEMY_TYPEA_RANG)
				&& playerpos.z <= (enemypos.z + ENEMY_TYPEA_RANG) && playerpos.z >= (enemypos.z - ENEMY_TYPEA_RANG)
				&& playerpos.y <= (enemypos.y + ENEMY_TYPEA_RANG_Y))
			{
				return true;
			}
		}break;
		case 2:
		{
			//TYPEB
			if (playerpos.x <= (enemypos.x + ENEMY_TYPEB_RANG) && playerpos.x >= (enemypos.x - ENEMY_TYPEB_RANG)
				&& playerpos.z <= (enemypos.z + ENEMY_TYPEB_RANG) && playerpos.z >= (enemypos.z - ENEMY_TYPEB_RANG)
				&& playerpos.y <= (enemypos.y + ENEMY_TYPEB_RANG_Y)&& playerpos.y >= (enemypos.y - ENEMY_TYPEB_RANG_Y))
			{
				return true;
			}
		}break;
		case 3:
		{
			//TYPEC
			if (playerpos.x <= (enemypos.x + ENEMY_TYPEC_RANG) && playerpos.x >= (enemypos.x - ENEMY_TYPEC_RANG)
				&& playerpos.z <= (enemypos.z + ENEMY_TYPEC_RANG) && playerpos.z >= (enemypos.z - ENEMY_TYPEC_RANG)
				&& playerpos.y <= (enemypos.y + ENEMY_TYPEC_RANG_Y) && playerpos.y >= (enemypos.y - ENEMY_TYPEC_RANG_Y))
			{
				return true;
			}
		}break;
		case 4:
		{
			//TYPED
			if (playerpos.x <= (enemypos.x + ENEMY_TYPED_RANG) && playerpos.x >= (enemypos.x - ENEMY_TYPED_RANG)
				&& playerpos.z <= (enemypos.z + ENEMY_TYPED_RANG) && playerpos.z >= (enemypos.z - ENEMY_TYPED_RANG)
				&& playerpos.y <= (enemypos.y + ENEMY_TYPED_RANG_Y) && playerpos.y >= (enemypos.y))
			{
				return true;
			}
		}break;
	default:
		break;
	}
	return false;
}

//=============================================================================
// �J������CHECK
//=============================================================================
void EnemyDrawCheck(void)
{
	D3DXVECTOR3 plong = camera->at - camera->pos;//�J���������_���ʏ�A�J������player�̋���
	float	kyori = (float)sqrt((plong.x*plong.x) + (plong.y*plong.y) + (plong.z*plong.z));
	for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
	{
		if (g_Enemy_typeA[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Enemy_typeA[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Enemy_typeA[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Enemy_typeA[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Enemy_typeA[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Enemy_typeA[i].top_pos  -camera->pos;
		D3DXVECTOR3 p5 = g_Enemy_typeA[i].down_pos - camera->pos;


		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans4 <= camera->xyrot_MAX || dans5 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Enemy_typeA[i].drawcheck = true;
		}
		else
		{
			g_Enemy_typeA[i].drawcheck = false;
		}
	}
	for (int i = 0; i < MAX_ENEMY_TYPEB; i++)
	{
		if (g_Enemy_typeB[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Enemy_typeB[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Enemy_typeB[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Enemy_typeB[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Enemy_typeB[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Enemy_typeB[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = g_Enemy_typeB[i].down_pos - camera->pos;


		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans4 <= camera->xyrot_MAX || dans5 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Enemy_typeB[i].drawcheck = true;
		}
		else
		{
			g_Enemy_typeB[i].drawcheck = false;
		}
	}
	for (int i = 0; i < MAX_ENEMY_TYPEC; i++)
	{
		if (g_Enemy_typeC[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Enemy_typeC[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Enemy_typeC[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Enemy_typeC[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Enemy_typeC[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Enemy_typeC[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = g_Enemy_typeC[i].down_pos - camera->pos;


		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans4 <= camera->xyrot_MAX || dans5 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Enemy_typeC[i].drawcheck = true;
		}
		else
		{
			g_Enemy_typeC[i].drawcheck = false;
		}
	}
	for (int i = 0; i < MAX_ENEMY_TYPED; i++)
	{
		if (g_Enemy_typeD[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Enemy_typeD[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Enemy_typeD[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Enemy_typeD[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Enemy_typeD[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Enemy_typeD[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = g_Enemy_typeD[i].down_pos - camera->pos;


		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans4 <= camera->xyrot_MAX || dans5 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Enemy_typeD[i].drawcheck = true;
		}
		else
		{
			g_Enemy_typeD[i].drawcheck = false;
		}
	}
	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Enemy_BOSS[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Enemy_BOSS[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Enemy_BOSS[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Enemy_BOSS[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Enemy_BOSS[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Enemy_BOSS[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = g_Enemy_BOSS[i].down_pos - camera->pos;


		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans4 <= camera->xyrot_MAX || dans5 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Enemy_BOSS[i].drawcheck = true;
		}
		else
		{
			g_Enemy_BOSS[i].drawcheck = false;
		}
	}
}
//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemyTypeA()
{
	return &(g_Enemy_typeA[0]);
}
ENEMY *GetEnemyTypeB()
{
	return &(g_Enemy_typeB[0]);
}

ENEMY *GetEnemyTypeC()
{
	return &(g_Enemy_typeC[0]);
}

ENEMY *GetEnemyTypeD()
{
	return &(g_Enemy_typeD[0]);
}

ENEMY *GetEnemyBOSS()
{
	return &(g_Enemy_BOSS[0]);
}
