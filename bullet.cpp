//=============================================================================
//
// �e���� [bullet.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "sound.h"
#include "MapTouch.h"
#include "particle.h"
#include "meshMap.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET					"data/MODEL/bigbullet.obj"
#define	BULLET_SPEED		(20.0f)			// �e�̈ړ��X�s�[�h

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA						*camera = GetCamera();
static GAMEDATA						*gamedata = GetData();
static MESH_MAP						*g_wall = Getwall();
static PLAYER						*player = GetPlayer();
static BULLET						g_Bullet[MAX_BULLET];	// �؃��[�N
static BULLET						enemy_bullet1[MAX_ENEMY_BULLET_1];	// �؃��[�N
static BULLET						enemy_bullet2[MAX_ENEMY_BULLET_2];	// �؃��[�N
static BULLET						enemy_boss[MAX_ENEMY_BOSS];	// �؃��[�N

static int							g_TexNo;				// �e�N�X�`���ԍ�
static ID3D11Buffer					*g_bullet_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_bullet_Texture[MAX_BULLET] = { NULL };	// �e�N�X�`�����

static DX11_MODEL	bulletmodel;
static DX11_MODEL	enemybulletmodel;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	if (gamedata->modenow == MODE_TITLE)
	{
		LoadModel(MODEL_BULLET, &bulletmodel);
		LoadModel(MODEL_BULLET, &enemybulletmodel);
	}
	
	for(int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].particlepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].oneshoutmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].Hitcheck_size = D3DXVECTOR3(BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS);
		g_Bullet[i].use = false;

		g_Bullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].spd = BULLET_SPEED;
		g_Bullet[i].touchcheck = false;
		g_Bullet[i].touchparticle = false;
		g_Bullet[i].oneshoutcheck = false;
		g_Bullet[i].drawcheck=true;
		g_Bullet[i].left_pos= g_Bullet[i].pos;
		g_Bullet[i].right_pos = g_Bullet[i].pos;
		g_Bullet[i].front_pos = g_Bullet[i].pos;
		g_Bullet[i].back_pos = g_Bullet[i].pos;
		g_Bullet[i].top_pos = g_Bullet[i].pos;
		g_Bullet[i].down_pos = g_Bullet[i].pos;
	}

	for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
	{
		enemy_bullet1[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet1[i].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet1[i].particlepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet1[i].oneshoutmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet1[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemy_bullet1[i].Hitcheck_size = D3DXVECTOR3(BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS);
		enemy_bullet1[i].use = false;
		
		enemy_bullet1[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet1[i].spd = BULLET_SPEED;
		enemy_bullet1[i].touchcheck = false;
		enemy_bullet1[i].touchparticle = false;

		enemy_bullet1[i].drawcheck = true;
		enemy_bullet1[i].left_pos = g_Bullet[i].pos;
		enemy_bullet1[i].right_pos = g_Bullet[i].pos;
		enemy_bullet1[i].front_pos = g_Bullet[i].pos;
		enemy_bullet1[i].back_pos = g_Bullet[i].pos;
		enemy_bullet1[i].top_pos = g_Bullet[i].pos;
		enemy_bullet1[i].down_pos = g_Bullet[i].pos;
	}
	for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
	{
		enemy_bullet2[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet2[i].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet2[i].particlepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet2[i].oneshoutmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet2[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemy_bullet2[i].Hitcheck_size = D3DXVECTOR3(BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS);
		enemy_bullet2[i].use = false;
					
		enemy_bullet2[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_bullet2[i].spd = BULLET_SPEED;
		enemy_bullet2[i].touchcheck = false;
		enemy_bullet2[i].touchparticle = false;

		enemy_bullet2[i].time_down=0.0f;
		enemy_bullet2[i].time_downplus=0.01f;
		enemy_bullet2[i].time_up=0.0f;
		enemy_bullet2[i].timeplus_up=0.01f;

		enemy_bullet2[i].drawcheck = true;
		enemy_bullet2[i].left_pos = g_Bullet[i].pos;
		enemy_bullet2[i].right_pos = g_Bullet[i].pos;
		enemy_bullet2[i].front_pos = g_Bullet[i].pos;
		enemy_bullet2[i].back_pos = g_Bullet[i].pos;
		enemy_bullet2[i].top_pos = g_Bullet[i].pos;
		enemy_bullet2[i].down_pos = g_Bullet[i].pos;
	}
	for (int i = 0; i < MAX_ENEMY_BOSS; i++)
	{
		enemy_boss[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss[i].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss[i].particlepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss[i].oneshoutmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemy_boss[i].Hitcheck_size = D3DXVECTOR3(BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS, BULLET_CHECK_RADIUS);
		enemy_boss[i].use = false;
		
		enemy_boss[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_boss[i].spd = BULLET_SPEED;
		enemy_boss[i].touchcheck = false;
		enemy_boss[i].touchparticle = false;
		enemy_boss[i].oneshoutcheck = false;
		
		enemy_boss[i].time_down = 0.0f;
		enemy_boss[i].time_downplus = 0.01f;
		enemy_boss[i].time_up = 0.0f;
		enemy_boss[i].timeplus_up = 0.01f;
		enemy_boss[i].time_Split = 0.0f;
		enemy_boss[i].timeplus_Split = 0.1f;

		enemy_boss[i].drawcheck = true;
		enemy_boss[i].left_pos = g_Bullet[i].pos;
		enemy_boss[i].right_pos = g_Bullet[i].pos;
		enemy_boss[i].front_pos = g_Bullet[i].pos;
		enemy_boss[i].back_pos = g_Bullet[i].pos;
		enemy_boss[i].top_pos = g_Bullet[i].pos;
		enemy_boss[i].down_pos = g_Bullet[i].pos;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	UnloadModel(&bulletmodel);
	UnloadModel(&enemybulletmodel);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	BulletDrawCheck();
	//�v���C���[�̒e
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == false) { continue; }
		int pariclenum = 0;
		g_Bullet[i].touchcheck = false;

		// �e�̈ړ�����
		if (g_Bullet[i].oneshoutcheck == false) {
			g_Bullet[i].pos.x -= sinf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
			g_Bullet[i].pos.z -= cosf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
		}
		else {
			g_Bullet[i].pos += g_Bullet[i].oneshoutmove * g_Bullet[i].spd;
		}
		//�J����check�p��pos��ݒ�
		g_Bullet[i].left_pos = g_Bullet[i].pos;
		g_Bullet[i].left_pos.x -= BULLET_WIDTH;
		g_Bullet[i].right_pos = g_Bullet[i].pos;
		g_Bullet[i].right_pos.x += BULLET_WIDTH;
		g_Bullet[i].front_pos = g_Bullet[i].pos;
		g_Bullet[i].front_pos.z -= BULLET_WIDTH;
		g_Bullet[i].back_pos = g_Bullet[i].pos;
		g_Bullet[i].back_pos.z += BULLET_WIDTH;
		g_Bullet[i].top_pos = g_Bullet[i].pos;
		g_Bullet[i].top_pos.y += BULLET_WIDTH;
		g_Bullet[i].down_pos = g_Bullet[i].pos;
		g_Bullet[i].down_pos.y -= BULLET_WIDTH;

		//�ǂƒn�ʓ����蔻��
		MeshfieldTouch(3);
		MeshceilingTouch(3);
		MeshwallTouch(3);
		SlopewallTouch(3);
		if (g_Bullet[i].touchcheck == true) {
			while (pariclenum < 15)
			{
				SetBulletParticle(g_Bullet[i].particlepos);
				pariclenum++;
			}
			g_Bullet[i].use = false;
		}

		//�Q�[���͈͊O�����锻��
		if (gamedata->modenow == MODE_BATTLEAREA)
		{
			if (g_Bullet[i].pos.y > 800.0f || g_Bullet[i].pos.y < -100.0f
				|| g_Bullet[i].pos.x<-1600.0f || g_Bullet[i].pos.x > 1600.0f
				|| g_Bullet[i].pos.z<-1800.0f || g_Bullet[i].pos.z > 1600.0f)
			{
				g_Bullet[i].use = false;
			}
			else if(g_wall[152].use==true)
			{
				if (g_Bullet[i].pos.x<=300.0f && g_Bullet[i].pos.x >= -300.0f
					&& g_Bullet[i].pos.z > 300.0f)
				{
					g_Bullet[i].use = false;
				}
			}
		}
		else if (gamedata->modenow == MODE_RESTAREA)
		{
			if (g_Bullet[i].pos.y > 800.0f || g_Bullet[i].pos.y < -100.0f
				|| g_Bullet[i].pos.x<-1600.0f || g_Bullet[i].pos.x > 1600.0f
				|| g_Bullet[i].pos.z<-10000.0f || g_Bullet[i].pos.z > -7000.0f)
			{
				g_Bullet[i].use = false;
			}
		}
	}
	//�G�l�~�[�̒e1��
	for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
	{
		if (enemy_bullet1[i].use == false) { continue; }
		int pariclenum = 0;
		enemy_bullet1[i].touchcheck = false;

		// �e�̈ړ�����
		enemy_bullet1[i].pos.x -= sinf(enemy_bullet1[i].rot.y) * enemy_bullet1[i].spd;
		enemy_bullet1[i].pos.z -= cosf(enemy_bullet1[i].rot.y) * enemy_bullet1[i].spd;
		enemy_bullet1[i].pos.y -= (enemy_bullet1[i].oneshoutmove.y) * enemy_bullet1[i].spd;
		
		//�J����check�p��pos��ݒ�
		enemy_bullet1[i].left_pos = g_Bullet[i].pos;
		enemy_bullet1[i].left_pos.x -= BULLET_WIDTH;
		enemy_bullet1[i].right_pos = g_Bullet[i].pos;
		enemy_bullet1[i].right_pos.x += BULLET_WIDTH;
		enemy_bullet1[i].front_pos = g_Bullet[i].pos;
		enemy_bullet1[i].front_pos.z -= BULLET_WIDTH;
		enemy_bullet1[i].back_pos = g_Bullet[i].pos;
		enemy_bullet1[i].back_pos.z += BULLET_WIDTH;
		enemy_bullet1[i].top_pos = g_Bullet[i].pos;
		enemy_bullet1[i].top_pos.y += BULLET_WIDTH;
		enemy_bullet1[i].down_pos = g_Bullet[i].pos;
		enemy_bullet1[i].down_pos.y -= BULLET_WIDTH;
		//�ǂƒn�ʓ����蔻��
		MeshfieldTouch(3);
		MeshceilingTouch(3);
		MeshwallTouch(3);
		SlopewallTouch(3);
		if (enemy_bullet1[i].touchcheck == true) {
			enemy_bullet1[i].use = false;
		}
		//�Q�[���͈͊O�����锻��
		if (enemy_bullet1[i].pos.y > 800.0f || enemy_bullet1[i].pos.y < -100.0f
			|| enemy_bullet1[i].pos.x<-1600.0f || enemy_bullet1[i].pos.x > 1600.0f
			|| enemy_bullet1[i].pos.z<-1800.0f || enemy_bullet1[i].pos.z > 1600.0f)
		{
			enemy_bullet1[i].use = false;
		}
	}
	//�G�l�~�[�̒e2��
	for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
	{
		if (enemy_bullet2[i].use == false) { continue; }
		int pariclenum = 0;
		enemy_bullet2[i].touchcheck = false;

		// �e�̈ړ�����
		if (enemy_bullet2[i].pos.x <= enemy_bullet2[i].pos2.x + 1.0f&&enemy_bullet2[i].pos.x >= enemy_bullet2[i].pos2.x - 1.0f
			&&enemy_bullet2[i].pos.z <= enemy_bullet2[i].pos2.z + 1.0f&&enemy_bullet2[i].pos.z >= enemy_bullet2[i].pos2.z - 1.0f)
		{
			enemy_bullet2[i].pos.x = enemy_bullet2[i].pos2.x;
			enemy_bullet2[i].pos.z = enemy_bullet2[i].pos2.z;
		}
		else
		{
			enemy_bullet2[i].pos.x += enemy_bullet2[i].oneshoutmove.x/80.0f;
			enemy_bullet2[i].pos.z += enemy_bullet2[i].oneshoutmove.z/80.0f;
		}
		//�e�̏㏸�Ɖ�����
		if(enemy_bullet2[i].time_up<1.0f)
		{
			float	jumopos;
			if (enemy_bullet2[i].pos.y >= 450.0f)
			{
				jumopos = 2.0f;							
			}
			else
			{
				jumopos = 6.0f;							
			}
			int nowNo = (int)enemy_bullet2[i].time_up;				// �������ł���e�[�u���ԍ������o���Ă���
			float nowTime = enemy_bullet2[i].time_up - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
			float jumopos2 = jumopos;
			jumopos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			jumopos2 -= jumopos;
			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			enemy_bullet2[i].pos.y += jumopos2;
			enemy_bullet2[i].timeplus_up += 0.001f;
			enemy_bullet2[i].time_up += enemy_bullet2[i].timeplus_up;		// ���Ԃ�i�߂Ă���
			if (enemy_bullet2[i].time_up >= 1) {

				enemy_bullet2[i].time_up = 1;
				enemy_bullet2[i].timeplus_up = 0.01f;

			}
		}
		else
		{
			float	downpos = 7.0f;							
			float nowTime = enemy_bullet2[i].time_down;			// ���ԕ����ł��鏭�������o���Ă���
			downpos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			enemy_bullet2[i].pos.y -= downpos;
			enemy_bullet2[i].time_downplus += 0.001f;
			enemy_bullet2[i].time_down += enemy_bullet2[i].time_downplus;	// ���Ԃ�i�߂Ă���
		}
		//�J����check�p��pos��ݒ�
		enemy_bullet2[i].left_pos = g_Bullet[i].pos;
		enemy_bullet2[i].left_pos.x -= BULLET_WIDTH;
		enemy_bullet2[i].right_pos = g_Bullet[i].pos;
		enemy_bullet2[i].right_pos.x += BULLET_WIDTH;
		enemy_bullet2[i].front_pos = g_Bullet[i].pos;
		enemy_bullet2[i].front_pos.z -= BULLET_WIDTH;
		enemy_bullet2[i].back_pos = g_Bullet[i].pos;
		enemy_bullet2[i].back_pos.z += BULLET_WIDTH;
		enemy_bullet2[i].top_pos = g_Bullet[i].pos;
		enemy_bullet2[i].top_pos.y += BULLET_WIDTH;
		enemy_bullet2[i].down_pos = g_Bullet[i].pos;
		enemy_bullet2[i].down_pos.y -= BULLET_WIDTH;

		//�ǂƒn�ʓ����蔻��
		MeshfieldTouch(3);
		MeshwallTouch(3);
		SlopewallTouch(3);
		if (enemy_bullet2[i].touchcheck == true) {
			enemy_bullet2[i].use = false;
		}
		//�Q�[���͈͊O�����锻��
		if (enemy_bullet2[i].pos.y > 639.0f) { enemy_bullet2[i].pos.y = 639.0f; }
		if (enemy_bullet2[i].pos.y > 800.0f || enemy_bullet2[i].pos.y < -100.0f
			|| enemy_bullet2[i].pos.x<-1600.0f || enemy_bullet2[i].pos.x > 1600.0f
			|| enemy_bullet2[i].pos.z<-1800.0f || enemy_bullet2[i].pos.z > 1600.0f)
		{
			enemy_bullet2[i].use = false;
		}
	}
	//BOSS�̒e
	for (int i = 0; i < MAX_ENEMY_BOSS; i++)
	{
		if (enemy_boss[i].use == false) { continue; }
		int pariclenum = 0;
		enemy_boss[i].touchcheck = false;
			// �e�̈ړ�����
		if (enemy_boss[i].time_Split<1.0f)
		{
			enemy_boss[i].time_Split += enemy_boss[i].timeplus_Split;
			enemy_boss[i].pos.x += enemy_boss[i].oneshoutmove.x / 80.0f;
			enemy_boss[i].pos.z += enemy_boss[i].oneshoutmove.z / 80.0f;
				
		}
		else
		{
			if (i%BOSS_BULLET_ONENUM == 0)
			{
				if (enemy_boss[i].pos.x >= enemy_boss[i].pos2.x - 1.0f&&enemy_boss[i].pos.x <= enemy_boss[i].pos2.x + 1.0f
					&&enemy_boss[i].pos.z >= enemy_boss[i].pos2.z - 1.0f&&enemy_boss[i].pos.z <= enemy_boss[i].pos2.z + 1.0f)
				{
					enemy_boss[i].pos.x= enemy_boss[i].pos2.x ;
					enemy_boss[i].pos.z= enemy_boss[i].pos2.z ;
				}
				else
				{
					enemy_boss[i].pos.x += enemy_boss[i].oneshoutmove.x / 80.0f;
					enemy_boss[i].pos.z += enemy_boss[i].oneshoutmove.z / 80.0f;
				}
			}else if (i%BOSS_BULLET_ONENUM == 1)
			{
				enemy_boss[i].pos.x += (sinf(D3DX_PI/5)*(enemy_boss[i].oneshoutmove.x)) / 80.0f;
				enemy_boss[i].pos.z += (cosf(D3DX_PI/5)*(enemy_boss[i].oneshoutmove.z)) / 80.0f;
			}
			else if (i%BOSS_BULLET_ONENUM == 2)
			{
				enemy_boss[i].pos.x += (sinf(D3DX_PI / 3)*(enemy_boss[i].oneshoutmove.x)) / 80.0f;
				enemy_boss[i].pos.z += (cosf(D3DX_PI / 3)*(enemy_boss[i].oneshoutmove.z)) / 80.0f;
			}
			else if (i%BOSS_BULLET_ONENUM == 3)
			{
				enemy_boss[i].pos.x += (sinf(-D3DX_PI  / 5)*(enemy_boss[i].oneshoutmove.x)) / 80.0f;
				enemy_boss[i].pos.z += (cosf(-D3DX_PI  / 5)*(enemy_boss[i].oneshoutmove.z)) / 80.0f;
			}
			else if (i%BOSS_BULLET_ONENUM == 4)
			{
				enemy_boss[i].pos.x += (sinf(-D3DX_PI / 3)*(enemy_boss[i].oneshoutmove.x)) / 80.0f;
				enemy_boss[i].pos.z += (cosf(-D3DX_PI / 3)*(enemy_boss[i].oneshoutmove.z)) / 80.0f;
			}
		}
		if (enemy_boss[i].time_up < 1.0f)
		{
			float	jumopos;
			if (enemy_boss[i].pos.y >= 450.0f)
			{
				jumopos = 3.0f;							
			}
			else
			{
				jumopos = 8.0f;							
			}

			int nowNo = (int)enemy_boss[i].time_up;				// �������ł���e�[�u���ԍ������o���Ă���
			float nowTime = enemy_boss[i].time_up - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
			float jumopos2 = jumopos;
			jumopos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			jumopos2 -= jumopos;
			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			enemy_boss[i].pos.y += jumopos2;
			enemy_boss[i].timeplus_up += 0.001f;
			enemy_boss[i].time_up += enemy_boss[i].timeplus_up;		// ���Ԃ�i�߂Ă���
			if (enemy_boss[i].time_up >= 1) {

				enemy_boss[i].time_up = 1;
				enemy_boss[i].timeplus_up = 0.01f;

			}
		}
		else
		{
			float	downpos = 7.0f;							
			float nowTime = enemy_boss[i].time_down;			// ���ԕ����ł��鏭�������o���Ă���
			downpos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			enemy_boss[i].pos.y -= downpos;
			enemy_boss[i].time_downplus += 0.001f;
			enemy_boss[i].time_down += enemy_boss[i].time_downplus;	// ���Ԃ�i�߂Ă���
		}
		//�J����check�p��pos��ݒ�
		enemy_boss[i].left_pos = g_Bullet[i].pos;
		enemy_boss[i].left_pos.x -= BULLET_WIDTH;
		enemy_boss[i].right_pos = g_Bullet[i].pos;
		enemy_boss[i].right_pos.x += BULLET_WIDTH;
		enemy_boss[i].front_pos = g_Bullet[i].pos;
		enemy_boss[i].front_pos.z -= BULLET_WIDTH;
		enemy_boss[i].back_pos = g_Bullet[i].pos;
		enemy_boss[i].back_pos.z += BULLET_WIDTH;
		enemy_boss[i].top_pos = g_Bullet[i].pos;
		enemy_boss[i].top_pos.y += BULLET_WIDTH;
		enemy_boss[i].down_pos = g_Bullet[i].pos;
		enemy_boss[i].down_pos.y -= BULLET_WIDTH;
		//�ǂƒn�ʓ����蔻��
		MeshfieldTouch(3);
		if (enemy_boss[i].touchcheck == true) {
			enemy_boss[i].use = false;
		}
		//�Q�[���͈͊O�����锻��
		if (enemy_boss[i].pos.y > 639.0f) { enemy_boss[i].pos.y = 639.0f; }
		if (enemy_boss[i].pos.y > 800.0f || enemy_boss[i].pos.y < -100.0f
			|| enemy_boss[i].pos.x<-1600.0f || enemy_boss[i].pos.x > 1600.0f
			|| enemy_boss[i].pos.z<-1800.0f || enemy_boss[i].pos.z > 1600.0f)
		{
			enemy_boss[i].use = false;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i <MAX_BULLET; i++) 
	{
		if (g_Bullet[i].use == true)
		{
			if (gamedata->drawopen == false)
			{
				if (g_Bullet[i].drawcheck == false) { continue; }
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Bullet[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_Bullet[i].scl.x, g_Bullet[i].scl.y, g_Bullet[i].scl.z);
			D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Bullet[i].rot.y , g_Bullet[i].rot.x, g_Bullet[i].rot.z);
			D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].pos.z);
			D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&g_Bullet[i].mtxWorld);

			// ���f���`��
			//DrawModel(&g_Bullet[i].model);
			DrawModel(&bulletmodel);
			
		}
	}
	for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
	{
		if (enemy_bullet1[i].use == true)
		{
			if (gamedata->drawopen == false)
			{
				if (enemy_bullet1[i].drawcheck == false) { continue; }
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemy_bullet1[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, enemy_bullet1[i].scl.x, enemy_bullet1[i].scl.y, enemy_bullet1[i].scl.z);
			D3DXMatrixMultiply(&enemy_bullet1[i].mtxWorld, &enemy_bullet1[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_bullet1[i].rot.y, enemy_bullet1[i].rot.x, enemy_bullet1[i].rot.z);
			D3DXMatrixMultiply(&enemy_bullet1[i].mtxWorld, &enemy_bullet1[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, enemy_bullet1[i].pos.x, enemy_bullet1[i].pos.y, enemy_bullet1[i].pos.z);
			D3DXMatrixMultiply(&enemy_bullet1[i].mtxWorld, &enemy_bullet1[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&enemy_bullet1[i].mtxWorld);

			// ���f���`��
			//DrawModel(&enemy_bullet1[i].model);
			DrawModel(&enemybulletmodel);

		}
	}
	for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
	{
		if (enemy_bullet2[i].use == true)
		{
			if (gamedata->drawopen == false)
			{
				if (enemy_bullet2[i].drawcheck == false) { continue; }
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemy_bullet2[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, enemy_bullet2[i].scl.x, enemy_bullet2[i].scl.y, enemy_bullet2[i].scl.z);
			D3DXMatrixMultiply(&enemy_bullet2[i].mtxWorld, &enemy_bullet2[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_bullet2[i].rot.y, enemy_bullet2[i].rot.x, enemy_bullet2[i].rot.z);
			D3DXMatrixMultiply(&enemy_bullet2[i].mtxWorld, &enemy_bullet2[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, enemy_bullet2[i].pos.x, enemy_bullet2[i].pos.y, enemy_bullet2[i].pos.z);
			D3DXMatrixMultiply(&enemy_bullet2[i].mtxWorld, &enemy_bullet2[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&enemy_bullet2[i].mtxWorld);

			// ���f���`��
			//DrawModel(&enemy_bullet2[i].model);
			DrawModel(&enemybulletmodel);

		}
	}
	for (int i = 0; i < MAX_ENEMY_BOSS; i++)
	{
		if (enemy_boss[i].use == true)
		{
			if (gamedata->drawopen == false)
			{
				if (enemy_boss[i].drawcheck == false) { continue; }
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemy_boss[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, enemy_boss[i].scl.x, enemy_boss[i].scl.y, enemy_boss[i].scl.z);
			D3DXMatrixMultiply(&enemy_boss[i].mtxWorld, &enemy_boss[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_boss[i].rot.y, enemy_boss[i].rot.x, enemy_boss[i].rot.z);
			D3DXMatrixMultiply(&enemy_boss[i].mtxWorld, &enemy_boss[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, enemy_boss[i].pos.x, enemy_boss[i].pos.y, enemy_boss[i].pos.z);
			D3DXMatrixMultiply(&enemy_boss[i].mtxWorld, &enemy_boss[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&enemy_boss[i].mtxWorld);

			// ���f���`��
			//DrawModel(&enemy_boss[i].model);
			DrawModel(&enemybulletmodel);

		}
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nIdxBullet = -1;

	for(int i = 0; i < MAX_BULLET; i++)
	{
		if(!g_Bullet[i].use)
		{
			g_Bullet[i].pos = pos;
			g_Bullet[i].rot = rot;
			g_Bullet[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet[i].use = true;
			D3DXVECTOR3		nor;
			nor = camera->at - camera->pos;
			D3DXVec3Normalize(&nor, &nor);
			g_Bullet[i].oneshoutmove= nor;
			if (camera->shoutchange == true) {
				g_Bullet[i].oneshoutcheck = false;
			}
			else
			{
				g_Bullet[i].oneshoutcheck = true;
			}
			nIdxBullet = i;
			PlaySound(SOUND_LABEL_SE_shot000);

			break;
		}
	}

	return nIdxBullet;
}

int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int type)
{
	int nIdxBullet = -1;
	switch (type)
	{
	case 1:
	{
		for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
		{
			if (!enemy_bullet1[i].use)
			{
				enemy_bullet1[i].pos = pos;
				enemy_bullet1[i].rot = rot;
				//enemy_bullet1[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				enemy_bullet1[i].use = true;
				enemy_bullet1[i].oneshoutmove = pos;
				enemy_bullet1[i].oneshoutmove.y = 0.0f;
				nIdxBullet = i;

				PlaySound(SOUND_LABEL_SE_shot000);

				break;
			}
		}

	}break;
	case 2:
	{
		for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
		{
			if (!enemy_bullet1[i].use)
			{
				enemy_bullet1[i].pos = pos;
				enemy_bullet1[i].rot = rot;
				enemy_bullet1[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				enemy_bullet1[i].use = true;
				D3DXVECTOR3		nor;
				nor = pos - player[0].pos;
				D3DXVec3Normalize(&nor, &nor);
				enemy_bullet1[i].oneshoutmove = nor;

				
				nIdxBullet = i;

				PlaySound(SOUND_LABEL_SE_shot000);

				break;
			}
		}

		
	}break;
	case 3:
	{
		for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
		{
			if (!enemy_bullet2[i].use)
			{
				enemy_bullet2[i].pos = pos;
				enemy_bullet2[i].pos2 = player[0].pos;
				enemy_bullet2[i].rot = rot;
				enemy_bullet2[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				enemy_bullet2[i].use = true;
				enemy_bullet2[i].oneshoutmove = enemy_bullet2[i].pos2-pos;
				enemy_bullet2[i].time_down = 0.0f;
				enemy_bullet2[i].time_downplus = 0.01f;
				enemy_bullet2[i].time_up = 0.0f;
				enemy_bullet2[i].timeplus_up = 0.01f;
				
				nIdxBullet = i;

				PlaySound(SOUND_LABEL_SE_shot000);

				break;
			}
		}


	}break;
	case 4:
	{
		int bulletnum = 0;
		for (int i = 0; i < MAX_ENEMY_BOSS; i++)
		{
			if (!enemy_boss[i].use)
			{
				enemy_boss[i].pos = pos;
				enemy_boss[i].pos2 = player[0].pos;
				enemy_boss[i].rot = rot;
				enemy_boss[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				enemy_boss[i].use = true;
				enemy_boss[i].oneshoutmove = player[0].pos - pos;
				enemy_boss[i].time_down = 0.0f;
				enemy_boss[i].time_downplus = 0.01f;
				enemy_boss[i].time_up = 0.0f;
				enemy_boss[i].timeplus_up = 0.01f;
				enemy_boss[i].time_Split = 0.0f;
				
				nIdxBullet = i;
				
				if (bulletnum >= BOSS_BULLET_ONENUM)
				{
					PlaySound(SOUND_LABEL_SE_shot000);
					break;
				}
				else
				{
					bulletnum++;
				}
			}
		}
	}break;
	default:
		break;
	}
	return nIdxBullet;
}

//=============================================================================
// �J������CHECK
//=============================================================================
void BulletDrawCheck(void)
{

	D3DXVECTOR3 plong = camera->at - camera->pos;
	float	kyori = (float)sqrt((plong.x*plong.x) + (plong.y*plong.y) + (plong.z*plong.z));
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == false){continue;}
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = g_Bullet[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = g_Bullet[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = g_Bullet[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = g_Bullet[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = g_Bullet[i].top_pos  -camera->pos;
		D3DXVECTOR3 p5 = g_Bullet[i].down_pos - camera->pos;

		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans5 <= camera->xyrot_MAX || dans6 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			g_Bullet[i].drawcheck = true;
		}
		else
		{
			g_Bullet[i].drawcheck = false;
		}
	}

	for (int i = 0; i < MAX_ENEMY_BULLET_1; i++)
	{
		if (enemy_bullet1[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = enemy_bullet1[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = enemy_bullet1[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = enemy_bullet1[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = enemy_bullet1[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = enemy_bullet1[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = enemy_bullet1[i].down_pos - camera->pos;

		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans5 <= camera->xyrot_MAX || dans6 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			enemy_bullet1[i].drawcheck = true;
		}
		else
		{
			enemy_bullet1[i].drawcheck = false;
		}
	}

	for (int i = 0; i < MAX_ENEMY_BULLET_2; i++)
	{
		if (enemy_bullet2[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = enemy_bullet2[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 = enemy_bullet2[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 = enemy_bullet2[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 = enemy_bullet2[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 = enemy_bullet2[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 = enemy_bullet2[i].down_pos - camera->pos;

		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans5 <= camera->xyrot_MAX || dans6 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			enemy_bullet2[i].drawcheck = true;
		}
		else
		{
			enemy_bullet2[i].drawcheck = false;
		}
	}

	for (int i = 0; i < MAX_ENEMY_BOSS; i++)
	{
		if (enemy_boss[i].use == false) { continue; }
		bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

		D3DXVECTOR3 p0 = enemy_boss[i].left_pos - camera->pos;
		D3DXVECTOR3 p1 =enemy_boss[i].right_pos - camera->pos;
		D3DXVECTOR3 p2 =enemy_boss[i].front_pos - camera->pos;
		D3DXVECTOR3 p3 =enemy_boss[i].back_pos - camera->pos;
		D3DXVECTOR3 p4 =enemy_boss[i].top_pos - camera->pos;
		D3DXVECTOR3 p5 =enemy_boss[i].down_pos - camera->pos;

		float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
		float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
		float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
		float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
		float dans5 = acosf(D3DXVec3Dot(&plong, &p4) / (((float)sqrt((p4.x*p4.x) + (p4.y*p4.y) + (p4.z*p4.z)))*kyori));
		float dans6 = acosf(D3DXVec3Dot(&plong, &p5) / (((float)sqrt((p5.x*p5.x) + (p5.y*p5.y) + (p5.z*p5.z)))*kyori));
		if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
			|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX
			|| dans5 <= camera->xyrot_MAX || dans6 <= camera->xyrot_MAX)
		{
			drawcheck = true;
		}
		if (drawcheck == true)
		{
			enemy_boss[i].drawcheck = true;
		}
		else
		{
			enemy_boss[i].drawcheck = false;
		}
	}
}
//=============================================================================
// �e�̎擾
//=============================================================================
BULLET *GetBullet(void)
{
	return &(g_Bullet[0]);
}

BULLET *GetEnemyBullet1(void)
{
	return &(enemy_bullet1[0]);
}

BULLET *GetEnemyBullet2(void)
{
	return &(enemy_bullet2[0]);
}
BULLET *GetEnemyBulletBoss(void)
{
	return &(enemy_boss[0]);
}
