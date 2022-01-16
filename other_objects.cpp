//=============================================================================
//
// ���̃��f������ [other_objects.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "other_objects.h"
#include "shadow.h"
#include "renderer.h"
#include "bullet.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_KEY		"data/MODEL/key.obj"		// �ǂݍ��ރ��f����
#define	MODEL_SHOP		"data/MODEL/shop.obj"		// �ǂݍ��ރ��f����
#define	MODEL_SHOPER	"data/MODEL/shoperobj.obj"		// �ǂݍ��ރ��f����

#define KEY_SHADOW_SIZE	(0.4f)						// �e�̑傫��



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static OBJECT			key_object[MAX_KEY];				// �G�l�~�[
static OBJECT			shop_object[2];				// �G�l�~�[
static GAMEDATA *gamedata = GetData();


static DX11_MODEL			key_model;
static DX11_MODEL			shop_model;
static DX11_MODEL			shoper_model;

static D3DXVECTOR3 key_pos[] = {
	D3DXVECTOR3(-1000.0f,50.0f,1400.0f),
	D3DXVECTOR3(900.0f,100.0f,-700.0f),
	D3DXVECTOR3(0.0f,580.0f,-600.0f),
	D3DXVECTOR3(1050.0f,330.0f,1400.0f),
	D3DXVECTOR3(-1450.0f,450.0f,-1450.0f),
};


//static D3DXVECTOR3 g_MoveTbl[] = {
//	D3DXVECTOR3(0.0f,7.0f,0.0f),
//	D3DXVECTOR3(-200.0f,7.0f,0.0f),
//	D3DXVECTOR3(-200.0f,7.0f,200.0f),
//};
//
//static float	g_MoveSpd[] = {
//	0.01f,
//	0.1f,
//	0.005f,
//};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitOtherobjects(void)
{
	//if (gamedata->modenow == MODE_TITLE)
	
		LoadModel(MODEL_KEY, &key_model);
		LoadModel(MODEL_SHOP, &shop_model);
		LoadModel(MODEL_SHOPER, &shoper_model);
	
	for (int i = 0; i < MAX_KEY ; i++)
	{

		key_object[i].pos = key_pos[i];
		key_object[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		key_object[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		key_object[i].use = true;		//�g�p��
		//key_object[i].radius = 5.0f;		//�����蔻��p�̔��a���Z�b�g
		key_object[i].touchsize = D3DXVECTOR3(KEY_RADIUS, KEY_RADIUS, KEY_RADIUS);

		key_object[i].time = 0.0f;
		key_object[i].time_plue = 0.0f;
		

		D3DXVECTOR3 pos = key_object[i].pos;
		pos.y = 0.0f;
		//key_object[i].shadowIdx = CreateShadow(pos, KEY_SHADOW_SIZE, KEY_SHADOW_SIZE);
	}
	for (int i = 0; i < 2; i++)
	{

		
		shop_object[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		shop_object[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		shop_object[i].use = true;		//�g�p��
	}
	shop_object[0].pos =D3DXVECTOR3(-600.0f, 0.0f, -7800.0f);
	shop_object[1].pos =D3DXVECTOR3(-590.0f, 40.0f, -7800.0f);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitOtherobjects(void)
{
	// ���f���̉������
	/*for (int i = 0; i < MAX_ENEMY_TYPEA; i++)
	{
		UnloadModel(&key_object[i].model);
	}*/
	UnloadModel(&key_model);
	UnloadModel(&shop_model);
	UnloadModel(&shoper_model);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateOtherobjects(void)
{
	//typeA�̏���
	for (int i = 0; i < MAX_KEY; i++)
	{
		
		if (key_object[i].use == false)continue;
		
		key_object[i].rot.y += (D3DX_PI / 20);
		if (key_object[i].rot.y == D3DX_PI)
		{
			key_object[i].rot.y = -D3DX_PI;
		}
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawOtherobjects(void)
{
	if (gamedata->modenow == MODE_BATTLEAREA)
	{
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// �J�����O����
		SetCullingMode(CULL_MODE_NONE);

		for (int i = 0; i < MAX_KEY; i++)
		{
			if (key_object[i].use == false) continue;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&key_object[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, key_object[i].scl.x, key_object[i].scl.y, key_object[i].scl.z);
			D3DXMatrixMultiply(&key_object[i].mtxWorld, &key_object[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, key_object[i].rot.y, key_object[i].rot.x, key_object[i].rot.z);
			D3DXMatrixMultiply(&key_object[i].mtxWorld, &key_object[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, key_object[i].pos.x, key_object[i].pos.y, key_object[i].pos.z);
			D3DXMatrixMultiply(&key_object[i].mtxWorld, &key_object[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&key_object[i].mtxWorld);

			// ���f���`��
			DrawModel(&key_model);

		}

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);
	}
	else if (gamedata->modenow == MODE_RESTAREA)
	{
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// �J�����O����
		SetCullingMode(CULL_MODE_NONE);

		for (int i = 0; i < 2; i++)
		{
			if (shop_object[i].use == false) continue;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&shop_object[i].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, shop_object[i].scl.x, shop_object[i].scl.y, shop_object[i].scl.z);
			D3DXMatrixMultiply(&shop_object[i].mtxWorld, &shop_object[i].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, shop_object[i].rot.y, shop_object[i].rot.x, shop_object[i].rot.z);
			D3DXMatrixMultiply(&shop_object[i].mtxWorld, &shop_object[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, shop_object[i].pos.x, shop_object[i].pos.y, shop_object[i].pos.z);
			D3DXMatrixMultiply(&shop_object[i].mtxWorld, &shop_object[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&shop_object[i].mtxWorld);

			if (i == 0)
			{
				// ���f���`��
				DrawModel(&shop_model);
			}
			else
			{
				// ���f���`��
				DrawModel(&shoper_model);
			}
			

		}

		// �J�����O�ݒ��߂�
		SetCullingMode(CULL_MODE_BACK);
	}
	
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
OBJECT *GetKeyObject()
{
	return &(key_object[0]);
}
