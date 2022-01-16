//=============================================================================
//
// メイン処理 [main.cpp]
// Author : GP11A132 43 林尚頡
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
// マクロ定義
//*****************************************************************************

#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"ピラミッド冒険"	// ウインドウのキャプション名


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//static bool	g_bPause ;	// ポーズON/OFF

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
static GAMEDATA	gamedata;

long g_MouseX = 0;
long g_MouseY = 0;

#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif
//MODE g_Mode = MODE_RESTAREA;					// 起動時の画面を設定
MODE g_Mode = MODE_TITLE;
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);
	
	InitLight();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);
	// サウンド処理の初期化 
	InitSound(hWnd);
	// フェードの初期化
	InitFade();

	

	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);
	
	
	
	// 最初のモードをセット
	SetMode(g_Mode);	// ここはSetModeのままで！
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 終了のモードをセット
	SetMode(MODE_MAX);


	// サウンド終了処理
	UninitSound();

	//入力の終了処理
	UninitInput();

	UninitFade();

	// レンダラー
	UninitRenderer();

	
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
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
	
	// ライトの更新処理
	UpdateLight();

	// フェード処理の更新
	UpdateFade();

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
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
	
	
	// フェード描画
	DrawFade();

	// バックバッファ、フロントバッファ入れ替え
	Present();
}
//= ============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	// モードを変える前に全部メモリを解放しちゃう
	switch (g_Mode)
	{
	case MODE_TITLE:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// タイトル画面の終了処理
		UninitGameTitle();
		break;

	case MODE_RESTAREA:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// ゲーム画面の終了処理
		UninitGameRestarea();
		break;

	case MODE_BATTLEAREA:
		if (mode == MODE_MAX)
		{
			UninitGameResult();
		}
		// ゲーム画面の終了処理
		UninitGameBattlearea();
		break;

	case MODE_RESULT:
		// リザルト画面の終了処理
		if (mode != MODE_TITLE)
		{
			UninitGameResult();
		}

		break;
	}
	
	MODE modePrevious = g_Mode;
	g_Mode = mode;	// 次のモードをセットしている
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
		// タイトル画面の初期化
		InitGameTitle();
	}break;
	case MODE_RESTAREA:
	{
		// ゲーム画面の初期化
		InitGameRestarea();
		gamedata.pausecheck = false;
	}break;
	case MODE_BATTLEAREA:
	{
		// リザルト画面の初期化
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
		// ゲーム終了時の処理
		//UninitGameResult();
	}break;
	}
}


//=============================================================================
// モードの取得
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

