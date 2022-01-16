//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "enemy.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;

static GAMEDATA *gamedata = GetData();
static ENEMY	*enemyBOSS = GetEnemyBOSS();
//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Light[i].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		g_Light[i].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		g_Light[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	//���s�����̐ݒ�(���E���Ƃ��炷��)
	g_Light[0].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);		//���̌���
	g_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���̐F
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					//���s����
	g_Light[0].Enable = true;									//���̃��C�g��ON
	SetLight(0, &g_Light[0]);									//����Őݒ肵�Ă���



	// �t�H�O�̏�����(���̌���)
	g_Fog.FogStart = 30.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd = 100.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(true);

}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{
	switch (gamedata->modenow)
	{
	case MODE_TITLE:
	{
		// �t�H�O�̏�����(���̌���)
		g_Fog.FogStart = 500.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
		g_Fog.FogEnd = 1000.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�t�H�O�̐F
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;
	case MODE_RESTAREA:
	{
		// �t�H�O�̏�����(���̌���)
		g_Fog.FogStart = 500.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
		g_Fog.FogEnd = 1000.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�t�H�O�̐F
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;
	case MODE_BATTLEAREA:
	{
		if (enemyBOSS[0].use == true) 
		{
			// �t�H�O�̏�����(���̌���)
			g_Fog.FogStart = 5000.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
			g_Fog.FogEnd = 5000.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
			g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�t�H�O�̐F
			SetFog(&g_Fog);
			SetFogEnable(true);
		}
		else
		{
			// �t�H�O�̏�����(���̌���)
			g_Fog.FogStart = 300.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
			g_Fog.FogEnd =800.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
			g_Fog.FogColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//�t�H�O�̐F
			SetFog(&g_Fog);
			SetFogEnable(true);
		}
		
	}
		break;
	case MODE_RESULT:
	{
		// �t�H�O�̏�����(���̌���)
		g_Fog.FogStart = 30.0f;								//���_���炱�̋��������ƃt�H�O��������n�߂�
		g_Fog.FogEnd = 300.0f;									//�����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�t�H�O�̐F
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;

	default:
		break;
	}


}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}



