//=============================================================================
//
// ���C������ [main.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(960)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)			// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define FIELD_W			(3000.0f)			// �t�B�[���h�̍L��
#define FIELD_H			(3000.0f)			// �t�B�[���h�̍L��
#define FIELD_TOP		( FIELD_H/2)		// �t�B�[���h�̏�
#define FIELD_DOWN		(-FIELD_H/2)		// �t�B�[���h�̉�
#define FIELD_LEFT		(-FIELD_W/2)		// �t�B�[���h�̍�
#define FIELD_RIGHT		( FIELD_W/2)		// �t�B�[���h�̉E
#define PLAYERNUM		(3)

#define PLAYER_MAXHP_INITIAL	(100)
#define PLAYER_MAXHP_MAX	(150)
#define PLAYER_ATK_INITIAL	(10)
#define PLAYER_ATK_MAX	(50)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,							// �^�C�g�����
	MODE_RESTAREA,							// �Q�[���������
	MODE_BATTLEAREA,								// �Q�[�����
	MODE_RESULT,							// ���U���g���
	MODE_MAX
} MODE;

// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
	D3DXVECTOR3 pos;		// ���_���W
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scl;		// �g��k��
	float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};

struct GAMEDATA 
{
	float	playerMAXHP;
	float	playerHPnow;
	float	playerAtk;
	bool	pausecheck;	//true=pause��ԁG
	bool	gameover;
	int		nowplayernum;
	MODE	modenow;
	bool	bigjumpset;
	bool	dying;
	int		getkey;
	int		playernum;
	bool	Mapdraw;
	bool	DescriptionFirst;
	bool	Description;
	float	time;
	long 	start;
	long 	end;
	bool	drawopen;
	bool	buying;
	int		buying_cameramove;
	bool	twojump;
	int		money;
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char* GetDebugStr(void);
MODE GetMode(void);
//MODE *GetMode(void);

void SetMode(MODE mode);

GAMEDATA *GetData(void);