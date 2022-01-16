//=============================================================================
//
// ���C������ [main.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "shadow.h"
#include "GAME_title.h"
#include "GAME_battlearea.h"
#include "GAME_restarea.h"
#include "GAME_result.h"
#include "sound.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "UI.h"
#include "light.h"
#include "enemy.h"
#include "timer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define CLASS_NAME		"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�s���~�b�h�`��"	// �E�C���h�E�̃L���v�V������


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//static bool	g_bPause ;	// �|�[�YON/OFF

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
static GAMEDATA	gamedata;

long g_MouseX = 0;
long g_MouseY = 0;

#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif
//MODE g_Mode = MODE_RESTAREA;					// �N�����̉�ʂ�ݒ�
MODE g_Mode = MODE_TITLE;
//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);
	
	InitLight();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);
	// �T�E���h�����̏����� 
	InitSound(hWnd);
	// �t�F�[�h�̏�����
	InitFade();

	

	SetLightEnable(true);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);
	
	
	
	// �ŏ��̃��[�h���Z�b�g
	SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �I���̃��[�h���Z�b�g
	SetMode(MODE_MAX);


	// �T�E���h�I������
	UninitSound();

	//���͂̏I������
	UninitInput();

	UninitFade();

	// �����_���[
	UninitRenderer();

	
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͂̍X�V����
	UpdateInput();
	
	if (gamedata.gameover != true)
	{
		if (gamedata.DescriptionFirst == true)
		{
			if (GetKeyboardTrigger(DIK_P)&&gamedata.buying==false)
			{
				gamedata.pausecheck = gamedata.pausecheck ? false : true;
				gamedata.Description = false;

				if (gamedata.pausecheck == true)
				{
					
					gamedata.time+= (float)((gamedata.end - gamedata.start) / 1000.0);
				}
				else
				{
					gamedata.start = clock();
				}

			}
			if (GetKeyboardTrigger(DIK_O))
			{
				if (gamedata.pausecheck == true)
				{
					gamedata.Description = gamedata.Description ? false : true;
				}
			}

			if (GetKeyboardTrigger(DIK_I))
			{
				if (gamedata.pausecheck != true)
				{
					PlaySound(SOUND_LABEL_SE_defend001);
					gamedata.drawopen = gamedata.drawopen ? false : true;
				}
			}
			
		}
		else
		{
			if (g_Mode == MODE_RESTAREA)
			{
				if (GetKeyboardTrigger(DIK_RETURN))
				{
					StopSound(SOUND_LABEL_BGM_sample003);
					PlaySound(SOUND_LABEL_BGM_sample002);
					gamedata.DescriptionFirst = true;
					gamedata.pausecheck = false;
				}
			}
		}
		

	}
	
	switch (g_Mode)
	{
	case MODE_TITLE:
		SetViewPort(TYPE_FULL_SCREEN);
		UpdateGameTitle();
		break;

	case MODE_RESTAREA:
		UpdateGameRestarea();
		break;

	case MODE_BATTLEAREA:
		UpdateGameBattlearea();
		break;

	case MODE_RESULT:
		SetViewPort(TYPE_FULL_SCREEN);
		UpdateGameResult();
		break;
	}
	
	// ���C�g�̍X�V����
	UpdateLight();

	// �t�F�[�h�����̍X�V
	UpdateFade();

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();
	
	
	switch (g_Mode)
	{
	case MODE_TITLE:
		DrawGameTitle();
		break;

	case MODE_RESTAREA:
		DrawGameRestarea();
		DrawBattleUI();
		DrawPause();
		DrawTittle();
		break;

	case MODE_BATTLEAREA:
		DrawGameBattlearea();
		DrawBattleUI();
		DrawPause();
		break;

	case MODE_RESULT:
		
		DrawGameResult();
		DrawResultUI();
		break;
	}
	
	
	// �t�F�[�h�`��
	DrawFade();

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}
//= ============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	switch (g_Mode)
	{
	case MODE_TITLE:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// �^�C�g����ʂ̏I������
		UninitGameTitle();
		break;

	case MODE_RESTAREA:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// �Q�[����ʂ̏I������
		UninitGameRestarea();
		break;

	case MODE_BATTLEAREA:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// �Q�[����ʂ̏I������
		UninitGameBattlearea();
		break;

	case MODE_RESULT:
		// ���U���g��ʂ̏I������
		if (mode != MODE_TITLE)
		{
			UninitGameResult();
		}

		break;
	}
	
	MODE modePrevious = g_Mode;
	g_Mode = mode;	// ���̃��[�h���Z�b�g���Ă���
	gamedata.modenow = g_Mode;
	
	switch (g_Mode)
	{
	case MODE_TITLE:
	{
		gamedata.pausecheck = false;

		if (modePrevious != MODE_RESULT)
		{
			InitMap();
		}
		// �^�C�g����ʂ̏�����
		InitGameTitle();
	}break;
	case MODE_RESTAREA:
	{
		// �Q�[����ʂ̏�����
		InitGameRestarea();
		gamedata.pausecheck = false;
	}break;
	case MODE_BATTLEAREA:
	{
		// ���U���g��ʂ̏�����
		InitGameBattlearea();
		gamedata.pausecheck = false;
	}break;
	case MODE_RESULT :
	{
		gamedata.pausecheck = false;
		gamedata.time += (float)((gamedata.end - gamedata.start) / 1000.0);
		InitGameResult();
		
	}break;
	case MODE_MAX:
	{
		// �Q�[���I�����̏���
		//UninitGameResult();
	}break;
	}
}


//=============================================================================
// ���[�h�̎擾
//=============================================================================
//MODE GetMode(void)
//{
//	return g_Mode;
//}
MODE GetMode(void)
{
	return g_Mode;
}



long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



GAMEDATA *GetData(void)
{
	return &gamedata;
}

