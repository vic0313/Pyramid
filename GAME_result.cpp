//=============================================================================
//
// �v�Z��ʂ̏��� [GAME_result.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "timer.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "shadow.h"
#include "rope.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "particle.h"
#include "bullet.h"
#include "fade.h"
#include "UI.h"
#include "other_objects.h"
#include "MapTransparent.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static PLAYER  *player = GetPlayer();
static ENEMY   *enemyboss = GetEnemyBOSS();
static CAMERA *camera = GetCamera();
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGameResult(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;
	
	InitCamera();
	InitPlayer();
	InitResultUI();
	InitShadow();
	
	// ���C�g��L����
	SetLightEnable(true);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);

	
	if (enemyboss[0].use == true)
	{

		PlaySound(SOUND_LABEL_BGM_sample004);
	}
	else
	{
		PlaySound(SOUND_LABEL_BGM_sample005);
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitGameResult(void)
{
	UninitCamera();
	UninitPause();
	UninitBattleUI();
	UninitResultUI();
	UninitBuying();
	UninitOtherobjects();
	UninitMap();
	UninitPlayer();
	UninitShadow();
	UninitEnemy();
	UninitTransparent();
	UninitParticle();
	UninitRope();
	UninitBullet();
	UninitTime();
	UninitTittle();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameResult(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

#endif
	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_TITLE);
	}
	
	
	// �v���C���[�̍X�V����
	//UpdatePlayer();
	
	// �J�����X�V
	UpdateCamera();
	// �G�l�~�[�̍X�V����
	
	
	UpdateMap();
	
	// �e�̍X�V����
	//UpdateShadow();
	
	//UpdateBattleUI();
	UpdateResultUI();
	
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameResult_True(void)
{
	// �e�̕`�揈��
	//DrawShadow();
	DrawPlayer();
	DrawMap();
	//DrawBattleUI();
	
	//DrawPause();
	//DrawTime();
}

void DrawGameResult(void)
{
	
//#ifdef _DEBUG
//	// �f�o�b�O�\��
//	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);
//
//#endif
	
	//DrawGameResult_True();
	
	switch (g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGameResult_True();
		break;
	}
}


