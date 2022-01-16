//=============================================================================
//
// UI処理 [UI.cpp]
// Author : GP11A132　43　林尚頡
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "UI.h"
#include "sprite.h"
#include "enemy.h"
#include "other_objects.h"
#include "GAME_result.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// キャラサイズ
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_WIDTH_FULLSIZE				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT_FULLSIZE				(SCREEN_HEIGHT)	// 
#define TEXTURE_PAUSE_MAX					(5)		// テクスチャの数
#define TEXTURE_TITLE_MAX					(2)		// テクスチャの数
#define TEXTURE_BUYING_MAX					(5)		// テクスチャの数
#define TEXTURE_BATTLEUI_MAX				(12)		// テクスチャの数
#define TEXTURE_RESULTUI_MAX				(4)		// テクスチャの数
#define MAX_PAUSE_UI				(23)
#define MAX_RESULTUI				(23)
#define MAX_BATTLEUI				(25)
#define MAX_BUYING				(7)
#define MAX_TITLEUI				(2)

#define LIFE_W				(400.0f)
#define LIFE_H				(30.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PAUSE					pause[MAX_PAUSE_UI];
static PAUSE					Tittle[MAX_TITLEUI];
static PAUSE					battleUI[MAX_BATTLEUI];
static PAUSE					ResultUI[MAX_RESULTUI];
static PAUSE					BuyingUI[MAX_BUYING];
static ENEMY					*enemyBOSS = GetEnemyBOSS();
static PLAYER					*player = GetPlayer();
static CAMERA					*camera = GetCamera();
//static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static GAMEDATA *gamedata = GetData();

static ID3D11ShaderResourceView	*g_Texture_pause[TEXTURE_PAUSE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView	*g_Texture_Tittle[TEXTURE_TITLE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView	*g_Texture_battleUI[TEXTURE_BATTLEUI_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView	*g_Texture_ResultUI[TEXTURE_RESULTUI_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView	*g_Texture_BuyingUI[TEXTURE_BUYING_MAX] = { NULL };	// テクスチャ情報


static float shoppos1[] = {200.0f,235.0f,270.0f,352.0f};


static char *g_TexturName_Tittle[] = {
	"data/TEXTURE/TITLE.png",
	"data/TEXTURE/Press_ENTER.png",
};

static char *g_TexturName_Buying[] = {
	"data/TEXTURE/shopUI1.png",
	"data/TEXTURE/shopUI2.png",
	"data/TEXTURE/shopUI3.png",
	"data/TEXTURE/shopUI4.png",
	"data/TEXTURE/shopUI5.png",
};

static char *g_TexturName_Pause[] = {
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/pause menu.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/Description.png",
	"data/TEXTURE/pause menu2.png",
	
};

static char *g_TexturName_BattleUI[] = {
	"data/TEXTURE/life_back.png",
	"data/TEXTURE/life.png",
	"data/TEXTURE/playermark.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/KEY_BACK.png",
	"data/TEXTURE/KEY_GET.png",
	"data/TEXTURE/GAMEOVER.png",
	"data/TEXTURE/bossmark.png",
	"data/TEXTURE/bossmarkdead.png",
	"data/TEXTURE/moneyUI.png",
	"data/TEXTURE/shouting1.png",
	"data/TEXTURE/bossdead.png",
};

static char *g_TexturName_ResultUI[] = {
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/result3.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/result_2.png",
	

};

static float animeTime = 0.0f;
static float animeTime_plue = 0.05f;
bool		animechange=false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTittle(void)
{
	//ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_TITLE_MAX; i++)
	{
		g_Texture_Tittle[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName_Tittle[i],
			NULL,
			NULL,
			&g_Texture_Tittle[i],
			NULL);
	}

	for (int i = 0; i < MAX_TITLEUI; i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &Tittle[i].g_VertexBuffer);
		Tittle[i].g_Use = true;
		Tittle[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//i=0 pauseの黒
	// プレイヤーの初期化

	Tittle[0].g_w = TEXTURE_WIDTH_FULLSIZE;
	Tittle[0].g_h = TEXTURE_HEIGHT_FULLSIZE;
	Tittle[0].g_Pos = D3DXVECTOR3(500.0f, 20.0f, 0.0f);
	Tittle[0].g_TexNo = 0;
	//i=1 MENUの画面
	Tittle[1].g_w = TEXTURE_WIDTH_FULLSIZE;
	Tittle[1].g_h = TEXTURE_HEIGHT_FULLSIZE;
	Tittle[1].g_Pos = D3DXVECTOR3(500.0f, 20.0f, 0.0f);
	Tittle[1].g_TexNo = 1;
	


	return S_OK;
}

HRESULT InitBuying(void)
{
	//ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_BUYING; i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &BuyingUI[i].g_VertexBuffer);
		BuyingUI[i].g_Use = false;
		BuyingUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_BUYING_MAX; i++)
	{
		g_Texture_BuyingUI[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName_Buying[i],
			NULL,
			NULL,
			&g_Texture_BuyingUI[i],
			NULL);
	}

	
	{
		// 頂点バッファに値をセットする
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(BuyingUI[6].g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float fWidth = 50.0f;
		float fHeight = 50.0f;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-fWidth , fHeight, 0.0f);
		vertex[1].Position = D3DXVECTOR3(fWidth , fHeight, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-fWidth , 0.0f, 0.0f);
		vertex[3].Position = D3DXVECTOR3(fWidth , 0.0f, 0.0f);

		// 拡散光の設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(BuyingUI[6].g_VertexBuffer, 0);

		ZeroMemory(&BuyingUI[6].material, sizeof(BuyingUI[6].material));
		BuyingUI[6].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		BuyingUI[6].g_Pos = D3DXVECTOR3(0.0f, 0.0, 0.0f);
		BuyingUI[6].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		BuyingUI[6].g_TexNo = 4;


	}

	//i=0 SHOPUI1
	BuyingUI[0].g_w = TEXTURE_WIDTH_FULLSIZE;
	BuyingUI[0].g_h = TEXTURE_HEIGHT_FULLSIZE;
	BuyingUI[0].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE/2, TEXTURE_HEIGHT_FULLSIZE/2, 0.0f);
	BuyingUI[0].g_TexNo = 0;
	//i=1 指標
	BuyingUI[1].g_w = 30.0f;
	BuyingUI[1].g_h = 30.0f;
	BuyingUI[1].g_Pos = D3DXVECTOR3(680.0f, shoppos1[0], 0.0f);
	BuyingUI[1].g_TexNo = 2;
	//i=2 金ないの時のUI
	BuyingUI[2].g_w = 300.0f;
	BuyingUI[2].g_h = 40.0f;
	BuyingUI[2].g_Pos = D3DXVECTOR3(550.0f, shoppos1[0], 0.0f);
	BuyingUI[2].g_TexNo = 1;
	//i=3~5 上限到達のUI
	for (int i = 3; i < 6; i++)
	{
		BuyingUI[i].g_w = 220.0f;
		BuyingUI[i].g_h = 10.0f;
		BuyingUI[i].g_TexNo = 3;
	}
	BuyingUI[3].g_Pos = D3DXVECTOR3(820.0f, shoppos1[1], 0.0f);
	BuyingUI[4].g_Pos = D3DXVECTOR3(820.0f, shoppos1[2], 0.0f);
	BuyingUI[5].g_Pos = D3DXVECTOR3(820.0f, shoppos1[3], 0.0f);

	
	return S_OK;
}

HRESULT InitPause(void)
{
	//ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_PAUSE_MAX; i++)
	{
		g_Texture_pause[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName_Pause[i],
			NULL,
			NULL,
			&g_Texture_pause[i],
			NULL);
	}

	for(int i=0;i<MAX_PAUSE_UI;i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &pause[i].g_VertexBuffer);
		pause[i].g_Use = false;
		pause[i].color= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	//i=0 pauseの黒
	// プレイヤーの初期化
	
	pause[0].g_w = TEXTURE_WIDTH_FULLSIZE;
	pause[0].g_h = TEXTURE_HEIGHT_FULLSIZE;
	pause[0].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE/2, TEXTURE_HEIGHT_FULLSIZE/2, 0.0f);
	pause[0].g_TexNo = 0;
	pause[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
	//i=1 MENUの画面
	pause[1].g_w = TEXTURE_WIDTH_FULLSIZE ;
	pause[1].g_h = TEXTURE_HEIGHT_FULLSIZE ;
	pause[1].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE / 2, TEXTURE_HEIGHT_FULLSIZE / 2, 0.0f);
	pause[1].g_TexNo = 1;
	pause[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//i=2,3,4  HP MAX
	for (int i = 2; i < 5; i++)
	{
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(400.0f- (pause[i].g_w*(i-2)), 180.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	//i=5,6,7  HP NOW
	for (int i = 5; i < 8; i++)
	{
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(400.0f - (pause[i].g_w*(i -5)), 230.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	//i=8,9,10  ATK
	for (int i = 8; i < 11; i++)
	{
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(750.0f - (pause[i].g_w*(i - 8)), 180.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	//i=11		もらった鍵
	pause[11].g_w = TEXTURE_WIDTH;
	pause[11].g_h = TEXTURE_HEIGHT;
	pause[11].g_Pos = D3DXVECTOR3(400.0f, 280.0f, 0.0f);
	pause[11].g_TexNo = 2;
	//i=12		殘鍵
	pause[12].g_w = TEXTURE_WIDTH;
	pause[12].g_h = TEXTURE_HEIGHT;
	pause[12].g_Pos = D3DXVECTOR3(750.0f, 280.0f, 0.0f);
	pause[12].g_TexNo = 2;
	//i=13		殘機
	pause[13].g_w = TEXTURE_WIDTH;
	pause[13].g_h = TEXTURE_HEIGHT;
	pause[13].g_Pos = D3DXVECTOR3(750.0f, 230.0f, 0.0f);
	pause[13].g_TexNo = 2;
	
	for (int i = 14; i < 16; i++)
	{
		//i=14		時間　分
		//i=15		時間　分
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(360.0f- (pause[i].g_w*(i - 14)), 390.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	for (int i = 16; i < 18; i++)
	{
		//i=14		時間　分
		//i=15		時間　分
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(450.0f- (pause[i].g_w*(i - 16)), 390.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	
	
	//i=18		TWOJUMPのもらう
	pause[18].g_w = 250.0f;
	pause[18].g_h = 40.0f;
	pause[18].g_Pos = D3DXVECTOR3(470.0f, 335.0f, 0.0f);
	pause[18].g_TexNo = 4;
	pause[18].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//i=19 20 21 money
	for (int i = 19; i < 22; i++)
	{
		pause[i].g_w = TEXTURE_WIDTH;
		pause[i].g_h = TEXTURE_HEIGHT;
		pause[i].g_Pos = D3DXVECTOR3(200.0f - (pause[i].g_w*(i - 19)), 105.0f, 0.0f);
		pause[i].g_TexNo = 2;
	}
	//i=23		説明用画面
	pause[22].g_w = TEXTURE_WIDTH_FULLSIZE;
	pause[22].g_h = TEXTURE_HEIGHT_FULLSIZE;
	pause[22].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE / 2, TEXTURE_HEIGHT_FULLSIZE / 2, 0.0f);
	pause[22].g_TexNo = 3;
	pause[22].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return S_OK;
}

HRESULT InitBattleUI(void)
{
	//ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_BATTLEUI_MAX; i++)
	{
		g_Texture_battleUI[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName_BattleUI[i],
			NULL,
			NULL,
			&g_Texture_battleUI[i],
			NULL);
	}

	for (int i = 0; i < MAX_BATTLEUI; i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &battleUI[i].g_VertexBuffer);
		battleUI[i].color= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
	}

	//i=0 player life back
	battleUI[0].g_Use = true;
	battleUI[0].g_w = LIFE_W;
	battleUI[0].g_h = LIFE_H;
	battleUI[0].g_Pos = D3DXVECTOR3(325.0f, 480.0f, 0.0f);
	battleUI[0].g_TexNo = 0;
	//i=1 player life
	battleUI[1].g_Use = true;
	battleUI[1].g_w = LIFE_W;
	battleUI[1].g_h = LIFE_H;
	battleUI[1].g_Pos = D3DXVECTOR3(325.0f, 480.0f, 0.0f);
	battleUI[1].g_TexNo = 1;
	//i=2 playermark
	battleUI[2].g_Use = true;
	battleUI[2].g_w = 100.0f;
	battleUI[2].g_h = 100.0f;
	battleUI[2].g_Pos = D3DXVECTOR3(80.0f, 480.0f, 0.0f);
	battleUI[2].g_TexNo = 2;
	//i=3 player殘機
	battleUI[3].g_Use = true;
	battleUI[3].g_w = TEXTURE_WIDTH;
	battleUI[3].g_h = TEXTURE_HEIGHT;
	battleUI[3].g_Pos = D3DXVECTOR3(110.0f, 470.0f, 0.0f);
	battleUI[3].g_TexNo = 3;
	battleUI[3].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	//i=4,5,6,7,8  KEY_BACK
	for (int i = 4; i < 9; i++)
	{
		battleUI[i].g_Use = true;
		battleUI[i].g_w = 50.0f;
		battleUI[i].g_h = 50.0f;
		battleUI[i].g_Pos = D3DXVECTOR3((160.0f+(i-4)*battleUI[i].g_w), 435.0f, 0.0f);
		battleUI[i].g_TexNo = 4;
	}
	//i=9,10,11,12,13 KEY_GET
	for (int i = 9; i < 14; i++)
	{
		battleUI[i].g_Use = false;
		battleUI[i].g_w = 50.0f;
		battleUI[i].g_h = 50.0f;
		battleUI[i].g_Pos = battleUI[i-5].g_Pos;
		battleUI[i].g_TexNo = 5;
	}
	//i=14		GAMEOVER画面
	battleUI[14].g_w = TEXTURE_WIDTH_FULLSIZE;
	battleUI[14].g_h = TEXTURE_HEIGHT_FULLSIZE;
	battleUI[14].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE/2, TEXTURE_HEIGHT_FULLSIZE/2, 0.0f);
	battleUI[14].g_TexNo = 6;
	//
	//i=15 boss life back
	battleUI[15].g_Use = false;
	battleUI[15].g_w = LIFE_W;
	battleUI[15].g_h = LIFE_H;
	battleUI[15].g_Pos = D3DXVECTOR3(325.0f, 70.0f, 0.0f);
	battleUI[15].g_TexNo = 0;
	//i=16 boss life
	battleUI[16].g_Use = false;
	battleUI[16].g_w = LIFE_W;
	battleUI[16].g_h = LIFE_H;
	battleUI[16].g_Pos = D3DXVECTOR3(325.0f, 70.0f, 0.0f);
	battleUI[16].g_TexNo = 1;
	//i=17 bossmark
	battleUI[17].g_Use = false;
	battleUI[17].g_w = 100.0f;
	battleUI[17].g_h = 100.0f;
	battleUI[17].g_Pos = D3DXVECTOR3(80.0f, 70.0f, 0.0f);
	battleUI[17].g_TexNo = 7;
	//i=18 bossmarkdead
	battleUI[18].g_Use = false;
	battleUI[18].g_w = 100.0f;
	battleUI[18].g_h = 100.0f;
	battleUI[18].g_Pos = D3DXVECTOR3(80.0f, 70.0f, 0.0f);
	battleUI[18].g_TexNo = 8;
	//i=19	moneyUI
	battleUI[19].g_Use = true;
	battleUI[19].g_w = 230.0f;
	battleUI[19].g_h = 40.0f;
	battleUI[19].g_Pos = D3DXVECTOR3(250.0f, 517.0f, 0.0f);
	battleUI[19].g_TexNo = 9;
	//i=20,21,22 money
	for (int i = 20; i < 23; i++)
	{
		battleUI[i].g_Use = true;
		battleUI[i].g_w = 25.0f;
		battleUI[i].g_h = 30.0f;
		battleUI[i].g_Pos = D3DXVECTOR3((300.0f - (i - 20)*battleUI[i].g_w), 517.0f, 0.0f);
		battleUI[i].g_TexNo = 3;
		battleUI[i].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	//i=23		shout画面
	battleUI[23].g_Use = false;
	battleUI[23].g_w = TEXTURE_WIDTH_FULLSIZE;
	battleUI[23].g_h = TEXTURE_HEIGHT_FULLSIZE;
	battleUI[23].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE / 2, TEXTURE_HEIGHT_FULLSIZE / 2, 0.0f);
	battleUI[23].g_TexNo = 10;
	//i=24		BOSS 死んだ
	battleUI[24].g_Use = false;
	battleUI[24].g_w = 800.0f;
	battleUI[24].g_h = 150.0f;
	battleUI[24].g_Pos = D3DXVECTOR3(TEXTURE_WIDTH_FULLSIZE / 2, TEXTURE_HEIGHT_FULLSIZE / 3, 0.0f);
	battleUI[24].g_TexNo = 11;
	return S_OK;
}

HRESULT InitResultUI(void)
{
	//ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_RESULTUI_MAX; i++)
	{
		g_Texture_ResultUI[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName_ResultUI[i],
			NULL,
			NULL,
			&g_Texture_ResultUI[i],
			NULL);
	}

	for (int i = 0; i < MAX_RESULTUI; i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &ResultUI[i].g_VertexBuffer);
		ResultUI[i].g_Use = true;
	}
	//i=0 pauseの黒
	// プレイヤーの初期化

	ResultUI[0].g_w = TEXTURE_WIDTH_FULLSIZE;
	ResultUI[0].g_h = TEXTURE_HEIGHT_FULLSIZE;
	ResultUI[0].g_Pos = D3DXVECTOR3(500.0f, 20.0f, 0.0f);
	ResultUI[0].g_TexNo = 0;
	ResultUI[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
	//i=1 resultの画面
	ResultUI[1].g_w = TEXTURE_WIDTH_FULLSIZE;
	ResultUI[1].g_h = TEXTURE_HEIGHT_FULLSIZE;
	ResultUI[1].g_Pos = D3DXVECTOR3(500.0f, 20.0f, 0.0f);
	ResultUI[1].g_TexNo = 1;
	ResultUI[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//i=2,3,4  HP MAX
	for (int i = 2; i < 5; i++)
	{
		ResultUI[i].g_w = TEXTURE_WIDTH;
		ResultUI[i].g_h = TEXTURE_HEIGHT;
		ResultUI[i].g_Pos = D3DXVECTOR3(300.0f - (ResultUI[i].g_w*(i - 2)), 115.0f, 0.0f);
		ResultUI[i].g_TexNo = 2;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	//i=5		もらった鍵
	ResultUI[5].g_w = TEXTURE_WIDTH;
	ResultUI[5].g_h = TEXTURE_HEIGHT;
	ResultUI[5].g_Pos = D3DXVECTOR3(300.0f, 165.0f, 0.0f);
	ResultUI[5].g_TexNo = 2;
	ResultUI[5].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//i=6		追加能力数
	ResultUI[6].g_w = TEXTURE_WIDTH;
	ResultUI[6].g_h = TEXTURE_HEIGHT;
	ResultUI[6].g_Pos = D3DXVECTOR3(300.0f, 215.0f, 0.0f);
	ResultUI[6].g_TexNo = 2;
	ResultUI[6].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//i=7,8,9  ATK
	for (int i = 7; i < 10; i++)
	{
		ResultUI[i].g_w = TEXTURE_WIDTH;
		ResultUI[i].g_h = TEXTURE_HEIGHT;
		ResultUI[i].g_Pos = D3DXVECTOR3(300.0f - (ResultUI[i].g_w*(i - 8)), 275.0f, 0.0f);
		ResultUI[i].g_TexNo = 2;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	//i=10		殘機
	ResultUI[10].g_w = TEXTURE_WIDTH;
	ResultUI[10].g_h = TEXTURE_HEIGHT;
	ResultUI[10].g_Pos = D3DXVECTOR3(300.0f, 325.0f, 0.0f);
	ResultUI[10].g_TexNo = 2;
	ResultUI[10].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//i=11		BOSS討伐
	ResultUI[11].g_w = TEXTURE_WIDTH;
	ResultUI[11].g_h = TEXTURE_HEIGHT;
	ResultUI[11].g_Pos = D3DXVECTOR3(300.0f, 375.0f, 0.0f);
	ResultUI[11].g_TexNo = 2;
	ResultUI[11].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	for (int i = 12; i < 14; i++)
	{
		//i=12		時間　分
		//i=13		時間　分
		ResultUI[i].g_w = TEXTURE_WIDTH;
		ResultUI[i].g_h = TEXTURE_HEIGHT;
		ResultUI[i].g_Pos = D3DXVECTOR3(250.0f - (ResultUI[i].g_w*(i - 14)), 420.0f, 0.0f);
		ResultUI[i].g_TexNo = 2;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	for (int i = 14; i < 16; i++)
	{
		//i=14		時間　秒
		//i=15		時間　秒
		ResultUI[i].g_w = TEXTURE_WIDTH;
		ResultUI[i].g_h = TEXTURE_HEIGHT;
		ResultUI[i].g_Pos = D3DXVECTOR3(355.0f - (ResultUI[i].g_w*(i - 16)), 420.0f, 0.0f);
		ResultUI[i].g_TexNo = 2;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	for (int i = 16; i < 20; i++)
	{
		//i=16~18	得点
		ResultUI[i].g_w = TEXTURE_WIDTH;
		ResultUI[i].g_h = TEXTURE_HEIGHT;
		ResultUI[i].g_Pos = D3DXVECTOR3(760.0f - (ResultUI[i].g_w*(i - 16)), 180.0f, 0.0f);
		ResultUI[i].g_TexNo = 2;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	for (int i = 20; i < 23; i++)
	{
		//i=19~21　評論
		ResultUI[i].g_w = 200.0f;
		ResultUI[i].g_h = 200.0f;
		ResultUI[i].g_Pos = D3DXVECTOR3(750.0f, 350.0f, 0.0f);
		ResultUI[i].g_TexNo = 3;
		ResultUI[i].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	animeTime = 0.0f;
	animeTime_plue = 0.05f;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTittle(void)
{
	for (int i = 0; i < MAX_TITLEUI; i++)
	{
		if (Tittle[i].g_VertexBuffer)
		{
			Tittle[i].g_VertexBuffer->Release();
			Tittle[i].g_VertexBuffer = NULL;
		}
	}


	for (int i = 0; i < TEXTURE_TITLE_MAX; i++)
	{
		if (g_Texture_Tittle[i])
		{
			g_Texture_Tittle[i]->Release();
			g_Texture_Tittle[i] = NULL;
		}
	}

}

void UninitBuying(void)
{
	for (int i = 0; i < MAX_BUYING; i++)
	{
		if (BuyingUI[i].g_VertexBuffer)
		{
			BuyingUI[i].g_VertexBuffer->Release();
			BuyingUI[i].g_VertexBuffer = NULL;
		}
	}


	for (int i = 0; i < TEXTURE_BUYING_MAX; i++)
	{
		if (g_Texture_BuyingUI[i])
		{
			g_Texture_BuyingUI[i]->Release();
			g_Texture_BuyingUI[i] = NULL;
		}
	}

}

void UninitPause(void)
{
	for (int i = 0; i < MAX_PAUSE_UI; i++)
	{
		if (pause[i].g_VertexBuffer)
		{
			pause[i].g_VertexBuffer->Release();
			pause[i].g_VertexBuffer = NULL;
		}
	}
	

	for (int i = 0; i < TEXTURE_PAUSE_MAX; i++)
	{
		if (g_Texture_pause[i])
		{
			g_Texture_pause[i]->Release();
			g_Texture_pause[i] = NULL;
		}
	}

}

void UninitBattleUI(void)
{
	for (int i = 0; i < MAX_BATTLEUI; i++)
	{
		if (battleUI[i].g_VertexBuffer)
		{
			battleUI[i].g_VertexBuffer->Release();
			battleUI[i].g_VertexBuffer = NULL;
		}
	}


	for (int i = 0; i < TEXTURE_BATTLEUI_MAX; i++)
	{
		if (g_Texture_battleUI[i])
		{
			g_Texture_battleUI[i]->Release();
			g_Texture_battleUI[i] = NULL;
		}
	}

}

void UninitResultUI(void)
{
	for (int i = 0; i < MAX_RESULTUI; i++)
	{
		if (ResultUI[i].g_VertexBuffer)
		{
			ResultUI[i].g_VertexBuffer->Release();
			ResultUI[i].g_VertexBuffer = NULL;
		}
	}


	for (int i = 0; i < TEXTURE_RESULTUI_MAX; i++)
	{
		if (g_Texture_ResultUI[i])
		{
			g_Texture_ResultUI[i]->Release();
			g_Texture_ResultUI[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTittle(void)
{
	if (gamedata->modenow == MODE_RESTAREA)
	{
		if (gamedata->DescriptionFirst == false)
		{
			if (animechange == false)
			{
				Tittle[1].color.a -= 0.05f;
				if (Tittle[1].color.a <= 0.0f)
				{
					Tittle[1].color.a = 0.0f;
					animechange = true;
				}
			}
			else
			{

				Tittle[1].color.a += 0.05f;
				if (Tittle[1].color.a >= 1.0f)
				{
					Tittle[1].color.a = 1.0f;
					animechange = false;
				}
			}
		}
	}
	else if (gamedata->modenow == MODE_TITLE)
	{
		
			if (animechange == false)
			{
				Tittle[1].color.a -= 0.05f;
				if (Tittle[1].color.a <= 0.0f)
				{
					Tittle[1].color.a = 0.0f;
					animechange = true;
				}
			}
			else
			{

				Tittle[1].color.a += 0.05f;
				if (Tittle[1].color.a >= 1.0f)
				{
					Tittle[1].color.a = 1.0f;
					animechange = false;
				}
			}
		
	}
	

	/*for (int i = 0; i < MAX_PAUSE_UI; i++)
	{

	}*/

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

void UpdateBuying(void)
{
	if (player[0].pos.z >= -7900.0f&&player[0].pos.z <= -7700.0f&&player[0].pos.x <= -400.0f)
	{
		if (gamedata->buying == false)
		{
			BuyingUI[6].g_Use = true;
		}
		else
		{
			BuyingUI[6].g_Use = false;
		}
		
	}
	else
	{
		BuyingUI[6].g_Use = false;
	}
	BuyingUI[6].g_Pos = player[0].pos;
	BuyingUI[6].g_Pos.y += PLAYER_TALL ;
	if (gamedata->buying == true && gamedata->buying_cameramove == 1)
	{
		BuyingUI[0].g_Use = true;
		BuyingUI[1].g_Use = true;
		
		if (gamedata->playerMAXHP >= PLAYER_MAXHP_MAX)
		{
			BuyingUI[3].g_Use = true;
		}
		if (gamedata->playerAtk >= PLAYER_ATK_MAX)
		{
			BuyingUI[4].g_Use = true;
		}
		if (gamedata->twojump == true)
		{
			BuyingUI[5].g_Use = true;
		}
		if (GetKeyboardTrigger(DIK_S))
		{
			if (BuyingUI[1].g_Pos.y == shoppos1[0])
			{
				if (BuyingUI[3].g_Use == true)
				{
					if (BuyingUI[4].g_Use == true)
					{
						if (BuyingUI[5].g_Use != true)
						{
							BuyingUI[1].g_Pos.y = shoppos1[3];
							PlaySound(SOUND_LABEL_SE_lockon000);
						}
					}
					else
					{
						BuyingUI[1].g_Pos.y = shoppos1[2];
						PlaySound(SOUND_LABEL_SE_lockon000);
					}
				}
				else
				{
					BuyingUI[1].g_Pos.y = shoppos1[1];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}
			}
			else if (BuyingUI[1].g_Pos.y == shoppos1[1])
			{

				if (BuyingUI[4].g_Use == true)
				{
					if (BuyingUI[5].g_Use != true)
					{
						BuyingUI[1].g_Pos.y = shoppos1[3];
						PlaySound(SOUND_LABEL_SE_lockon000);
					}
				}
				else
				{
					BuyingUI[1].g_Pos.y = shoppos1[2];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}

			}
			else if (BuyingUI[1].g_Pos.y == shoppos1[2])
			{
				if (BuyingUI[5].g_Use != true)
				{
					BuyingUI[1].g_Pos.y = shoppos1[3];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}
			}
		}
		if (GetKeyboardTrigger(DIK_W))
		{
			if (BuyingUI[1].g_Pos.y == shoppos1[3])
			{
				
				if (BuyingUI[4].g_Use == true)
				{
					if (BuyingUI[3].g_Use == true)
					{
						BuyingUI[1].g_Pos.y = shoppos1[0];
						PlaySound(SOUND_LABEL_SE_lockon000);
					}
					else
					{
						BuyingUI[1].g_Pos.y = shoppos1[1];
						PlaySound(SOUND_LABEL_SE_lockon000);
					}
				}
				else
				{
					BuyingUI[1].g_Pos.y = shoppos1[2];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}
				
			}
			else if (BuyingUI[1].g_Pos.y == shoppos1[2])
			{

				if (BuyingUI[3].g_Use == true)
				{
					BuyingUI[1].g_Pos.y = shoppos1[0];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}
				else
				{
					BuyingUI[1].g_Pos.y = shoppos1[1];
					PlaySound(SOUND_LABEL_SE_lockon000);
				}

			}
			else if (BuyingUI[1].g_Pos.y == shoppos1[1])
			{
				
				BuyingUI[1].g_Pos.y = shoppos1[0];
				PlaySound(SOUND_LABEL_SE_lockon000);
				
			}
		}
		BuyingUI[2].g_Pos.y = BuyingUI[1].g_Pos.y;

		if (BuyingUI[1].g_Pos.y == shoppos1[0] && GetKeyboardTrigger(DIK_RETURN))
		{
			gamedata->playerHPnow = gamedata->playerMAXHP;
			BuyingUI[2].color.a = 0.0f;
			PlaySound(SOUND_LABEL_SE_defend001);
		}
		else if (BuyingUI[1].g_Pos.y == shoppos1[1] && GetKeyboardTrigger(DIK_RETURN))
		{
			if (gamedata->playerMAXHP < PLAYER_MAXHP_MAX&&gamedata->money>= HP_PRICE)
			{
				gamedata->playerMAXHP += 10;
				gamedata->playerHPnow += 10;
				gamedata->money -= HP_PRICE;
				PlaySound(SOUND_LABEL_SE_defend001);
			}
			else if(gamedata->playerMAXHP < PLAYER_MAXHP_MAX&&gamedata->money < HP_PRICE)
			{
				BuyingUI[2].g_Use = true;
				BuyingUI[2].color.a = 1.0f;
			}
		}
		else if (BuyingUI[1].g_Pos.y == shoppos1[2] && GetKeyboardTrigger(DIK_RETURN))
		{
			if (gamedata->playerAtk< PLAYER_ATK_MAX&&gamedata->money >= ATK_PRICE)
			{
				gamedata->playerAtk += 10;
				gamedata->money -= ATK_PRICE;
				PlaySound(SOUND_LABEL_SE_defend001);
			}
			else if (gamedata->playerAtk < PLAYER_ATK_MAX&&gamedata->money < ATK_PRICE)
			{
				BuyingUI[2].g_Use = true;
				BuyingUI[2].color.a = 1.0f;
			}
		}
		else if (BuyingUI[1].g_Pos.y == shoppos1[3] && GetKeyboardTrigger(DIK_RETURN))
		{
			if (gamedata->twojump == false && gamedata->money >= TWOJUMP_PRICE)
			{
				
				gamedata->twojump = true;
				gamedata->money -= TWOJUMP_PRICE;
				PlaySound(SOUND_LABEL_SE_defend001);
			}
			else if (gamedata->twojump == false && gamedata->money < TWOJUMP_PRICE)
			{
				BuyingUI[2].g_Use = true;
				BuyingUI[2].color.a = 1.0f;

			}
		}


		if (BuyingUI[2].g_Use == true)
		{
			BuyingUI[2].color.a -= 0.01f;
			if (BuyingUI[2].color.a <= 0.0f)
			{
				BuyingUI[2].g_Use = false;
				BuyingUI[2].color.a = 1.0f;
			}
		}



	}
	else
	{
		for (int i = 0; i < MAX_BUYING; i++)
		{
			if (i == 6)continue;
			BuyingUI[i].g_Use = false;
		}
		BuyingUI[1].g_Pos.y= shoppos1[0];
	}


	/*for (int i = 0; i < MAX_PAUSE_UI; i++)
	{

	}*/

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

void UpdatePause(void)
{
	if (gamedata->gameover != true)
	{
		if (gamedata->pausecheck == true)
		{
			for (int i = 0; i < MAX_PAUSE_UI; i++)
			{
				pause[i].g_Use = true;
			}
			if (gamedata->twojump == false)
			{
				pause[18].g_Use = false;
			}
		}
		else {
			for (int i = 0; i < MAX_PAUSE_UI; i++)
			{
				pause[i].g_Use = false;
			}
		}
	}
	
	/*for (int i = 0; i < MAX_PAUSE_UI; i++)
	{

	}*/

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

void UpdateBattleUI(void)
{
	if (player[0].pos.x > -600.0f&&player[0].pos.x <600.0f&&player[0].pos.z >250.0f)
	{
		battleUI[15].g_Use = true;
		battleUI[16].g_Use = true;
		battleUI[17].g_Use = true;
	}
	else
	{
		battleUI[15].g_Use = false;
		battleUI[16].g_Use = false;
		battleUI[17].g_Use = false;
	}
	//i=0 player life back
	battleUI[0].g_w = (LIFE_W*gamedata->playerMAXHP) / PLAYER_MAXHP_INITIAL;
	battleUI[0].g_Pos.x = (325.0f) + ((LIFE_W*(gamedata->playerMAXHP - PLAYER_MAXHP_INITIAL)) / PLAYER_MAXHP_INITIAL)/2;
	//LIFE_W:PLAYER_MAXHP_INITIAL=?:(gamedata->playerMAXHP-PLAYER_MAXHP_INITIAL)
	//i=1 player life
							//LIFE_W:PLAYER_MAXHP_INITIAL=?:gamedata->playerHPnow
	battleUI[1].g_w = battleUI[0].g_w*(gamedata->playerHPnow / gamedata->playerMAXHP);
	//LIFE_W:PLAYER_MAXHP_INITIAL=?:(gamedata->playerMAXHP-gamedata->playerHPnow)
	battleUI[1].g_Pos.x = battleUI[0].g_Pos.x - (battleUI[0].g_w - battleUI[1].g_w) / 2;
	//i=2 playermark

	//i=3 player殘機

	//i=4,5,6,7,8  KEY_BACK
	//i=9,10,11,12,13 KEY_GET
	switch (gamedata->getkey)
	{
	case 1:
		battleUI[4].g_Use = false;
		battleUI[9].g_Use = true;
		break;
	case 2:
		battleUI[5].g_Use = false;
		battleUI[10].g_Use = true;
		break;
	case 3:
		battleUI[6].g_Use = false;
		battleUI[11].g_Use = true;
		break;
	case 4:
		battleUI[7].g_Use = false;
		battleUI[12].g_Use = true;
		break;
	case 5:
		battleUI[8].g_Use = false;
		battleUI[13].g_Use = true;
		break;
	default:
		break;
	}
	battleUI[16].g_w = battleUI[15].g_w*(enemyBOSS[0].HP / ENEMY_BOSS_MAXHP);
	//LIFE_W:PLAYER_MAXHP_INITIAL=?:(gamedata->playerMAXHP-gamedata->playerHPnow)
	battleUI[16].g_Pos.x = battleUI[15].g_Pos.x - (battleUI[15].g_w - battleUI[16].g_w) / 2;
	if (enemyBOSS[0].use == false)
	{
		battleUI[16].g_Use = false;
		battleUI[17].g_Use = false;
		battleUI[18].g_Use = true;
	}
	else
	{
		battleUI[18].g_Use = false;
	}

	if (camera->shoutchange == false)
	{
		battleUI[23].g_Use = true;
	}
	else
	{
		battleUI[23].g_Use = false;
	}

	if (enemyBOSS[0].use == false)
	{
		battleUI[24].g_Use = true;
		battleUI[24].color.a -= 0.01f;
		if (battleUI[24].color.a <= 0.0f)
		{
			battleUI[24].color.a = 0.0f;
			battleUI[24].g_Use = false;
		}
	}
	else
	{
		battleUI[24].g_Use =false;
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

void UpdateResultUI(void)
{

	animeTime += animeTime_plue;
	if (animeTime >= 1.0f)
	{
		for (int i = 2; i < 5; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 1.5f)
	{
		for (int i = 5; i < 6; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 2.0f)
	{
		for (int i = 6; i < 7; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 2.5f)
	{
		for (int i = 7; i < 10; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 3.0f)
	{
		for (int i = 10; i < 11; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 3.5f)
	{
		for (int i = 11; i < 12; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 4.0f)
	{
		for (int i = 12; i < 16; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 6.0f)
	{
		for (int i = 16; i < 20; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
	if (animeTime >= 9.0f)
	{
		for (int i = 20; i < 23; i++)
		{
			if (ResultUI[i].color.a >= 1.0f)
			{
				ResultUI[i].color.a = 1.0f;
				break;
			}
			else
			{
				ResultUI[i].color.a += 0.1f;
			}
		}
	}
		

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTittle(void)
{

	// 2Dの描画処理
		// ライトを無効化する
	SetLightEnable(false);

	// Z比較なし
	SetDepthEnable(false);
	if(gamedata->modenow==MODE_TITLE)
	{

		for (int i = 0; i < MAX_TITLEUI; i++)
		{
			
				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &Tittle[i].g_VertexBuffer, &stride, &offset);

				// マトリクス設定
				SetWorldViewProjection2D();

				// プリミティブトポロジ設定
				GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// マテリアル設定
				MATERIAL material;
				ZeroMemory(&material, sizeof(material));
				material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SetMaterial(material);

				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Tittle[Tittle[i].g_TexNo]);

				float px, py, pw, ph, tw, th, tx, ty;
				px = Tittle[i].g_w / 2;	// スコアの表示位置X
				py = Tittle[i].g_h / 2;			// スコアの表示位置Y
				pw = Tittle[i].g_w;				// スコアの表示幅
				ph = Tittle[i].g_h;				// スコアの表示高さ

				tw = 1.0f;		// テクスチャの幅
				th = 1.0f;		// テクスチャの高さ
				tx = 0.0f;			// テクスチャの左上X座標
				ty = 0.0f;			// テクスチャの左上Y座標
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				// ポリゴン描画
				SetSpriteColor(Tittle[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					Tittle[i].color);


				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
		}

	}
	else if (gamedata->modenow == MODE_RESTAREA)
	{
		if (gamedata->DescriptionFirst == false) 
		{
			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &Tittle[1].g_VertexBuffer, &stride, &offset);

			// マトリクス設定
			SetWorldViewProjection2D();

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Tittle[Tittle[1].g_TexNo]);

			float px, py, pw, ph, tw, th, tx, ty;
			px = Tittle[1].g_w / 2;	// スコアの表示位置X
			py = Tittle[1].g_h / 2;			// スコアの表示位置Y
			pw = Tittle[1].g_w;				// スコアの表示幅
			ph = Tittle[1].g_h;				// スコアの表示高さ

			tw = 1.0f;		// テクスチャの幅
			th = 1.0f;		// テクスチャの高さ
			tx = 0.0f;			// テクスチャの左上X座標
			ty = 0.0f;			// テクスチャの左上Y座標
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			// ポリゴン描画
			SetSpriteColor(Tittle[1].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				Tittle[1].color);


			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライトを有効にする
	SetLightEnable(true);

	// Z比較有効
	SetDepthEnable(true);

}

void DrawBuyingMark(void)
{
	if (BuyingUI[6].g_Use == true && gamedata->buying == false)
	{
		// αテストを有効に
		SetAlphaTestEnable(true);

		// ライティングを無効
		SetLightEnable(false);

		D3DXMATRIX mtxView, mtxScale, mtxTranslate;

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &BuyingUI[6].g_VertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&BuyingUI[6].mtxWorld);

		// ビューマトリックスを取得
		CAMERA *cam = GetCamera();

		//myxViewにカメラ行列を取り出す
		mtxView = cam->mtxView;

		BuyingUI[6].mtxWorld._11 = mtxView._11;
		BuyingUI[6].mtxWorld._12 = mtxView._21;
		BuyingUI[6].mtxWorld._13 = mtxView._31;
		BuyingUI[6].mtxWorld._21 = mtxView._12;
		BuyingUI[6].mtxWorld._22 = mtxView._22;
		BuyingUI[6].mtxWorld._23 = mtxView._32;
		BuyingUI[6].mtxWorld._31 = mtxView._13;
		BuyingUI[6].mtxWorld._32 = mtxView._23;
		BuyingUI[6].mtxWorld._33 = mtxView._33;

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, BuyingUI[6].scl.x,
			BuyingUI[6].scl.y,
			BuyingUI[6].scl.z);
		D3DXMatrixMultiply(&BuyingUI[6].mtxWorld, &BuyingUI[6].mtxWorld, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, BuyingUI[6].g_Pos.x,
			BuyingUI[6].g_Pos.y,
			BuyingUI[6].g_Pos.z);
		D3DXMatrixMultiply(&BuyingUI[6].mtxWorld, &BuyingUI[6].mtxWorld, &mtxTranslate);


		// ワールドマトリックスの設定
		SetWorldMatrix(&BuyingUI[6].mtxWorld);

		// マテリアル設定
		SetMaterial(BuyingUI[6].material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_BuyingUI[BuyingUI[6].g_TexNo]);

		// ポリゴンの描画
		GetDeviceContext()->Draw(4, 0);

		// ライティングを有効に
		SetLightEnable(true);

		// αテストを無効に
		SetAlphaTestEnable(false);
	}
}

void DrawBuying(void)
{
	if (gamedata->buying == true && gamedata->buying_cameramove == 1)
	{
		// 2Dの描画処理
		// ライトを無効化する
		SetLightEnable(false);

		// Z比較なし
		SetDepthEnable(false);

		for (int i = 0; i < MAX_BUYING; i++)
		{
			if(i==6) { continue; }
			if (BuyingUI[i].g_Use == false) { continue; }
			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &BuyingUI[i].g_VertexBuffer, &stride, &offset);

			// マトリクス設定
			SetWorldViewProjection2D();

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_BuyingUI[BuyingUI[i].g_TexNo]);

			float px, py, pw, ph, tw, th, tx, ty;
			px = BuyingUI[i].g_Pos.x;	// スコアの表示位置X
			py = BuyingUI[i].g_Pos.y;	// スコアの表示位置Y
			pw = BuyingUI[i].g_w;		// スコアの表示幅
			ph = BuyingUI[i].g_h;		// スコアの表示高さ

			tw = 1.0f;		// テクスチャの幅
			th = 1.0f;		// テクスチャの高さ
			tx = 0.0f;			// テクスチャの左上X座標
			ty = 0.0f;			// テクスチャの左上Y座標
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			// ポリゴン描画
			SetSpriteColor(BuyingUI[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				BuyingUI[i].color);


			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// ライトを有効にする
		SetLightEnable(true);

		// Z比較有効
		SetDepthEnable(true);
	}

	

}

void DrawPause(void)
{
	if (gamedata->pausecheck == false)return;
	// 2Dの描画処理
		// ライトを無効化する
	SetLightEnable(false);

	// Z比較なし
	SetDepthEnable(false);
	
	{
		
		if (gamedata->DescriptionFirst == true)
		{
			int MAXHP = (int)gamedata->playerMAXHP;
			int NOWHP = (int)gamedata->playerHPnow;
			int ATK = (int)gamedata->playerAtk;
			int KEY = (int)gamedata->getkey;
			int time_minute = (int)(gamedata->time / 60);
			int time_secend = ((int)(gamedata->time)) % 60;
			int money = (int)gamedata->money;
			for (int i = 0; i < (MAX_PAUSE_UI); i++)
			{
				if (pause[i].g_Use == true)
				{
					// 頂点バッファ設定
					UINT stride = sizeof(VERTEX_3D);
					UINT offset = 0;
					GetDeviceContext()->IASetVertexBuffers(0, 1, &pause[i].g_VertexBuffer, &stride, &offset);

					// マトリクス設定
					SetWorldViewProjection2D();

					// プリミティブトポロジ設定
					GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

					// マテリアル設定
					MATERIAL material;
					ZeroMemory(&material, sizeof(material));
					material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					SetMaterial(material);

					// テクスチャ設定
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_pause[pause[i].g_TexNo]);

					float px, py, pw, ph, tw, th, tx, ty;

					{
						// スコアの位置やテクスチャー座標を反映
						px = pause[i].g_Pos.x;	// プレイヤーの表示位置X
						py = pause[i].g_Pos.y;			// プレイヤーの表示位置Y
						pw = pause[i].g_w;				// プレイヤーの表示幅
						ph = pause[i].g_h;				// プレイヤーの表示高さ
						
						th = 1.0f / 1;		// テクスチャの高さ
						ty = 0.0f;			// テクスチャの左上Y座標
						if (i >= 2 && i <= 4)
						{
							//player MAX HP
							float x = (float)(MAXHP % 10);
							// numberを次の桁へ
							MAXHP /= 10;
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i >= 5 && i <= 7)
						{
							//player hp now
							float x = (float)(NOWHP % 10);
							// numberを次の桁へ
							NOWHP /= 10;
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i >= 8 && i <= 10)
						{
							//player ATK
							float x = (float)(ATK % 10);
							// numberを次の桁へ
							ATK /= 10;
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 11)
						{
							//もらった鍵
							float x = (float)KEY;
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 12)
						{
							//殘鍵
							float x = (float)(MAX_KEY - KEY);
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 13)
						{
							//殘機
							float x = (float)gamedata->playernum;
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 14)
						{
							//時間 分-1
							float x = (float)(time_minute % 10);
							time_minute /= 10;
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 15)
						{
							//時間 分-2

							float x = (float)(time_minute % 10);
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 16)
						{
							//時間 秒-1
							float x = (float)(time_secend % 10);
							time_secend /= 10;
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 17)
						{
							//時間 秒-2

							float x = (float)(time_secend % 10);
							// numberを次の桁へ
							tw = 1.0f / 10;		// テクスチャの幅v
							tx = tw * x;			// テクスチャの左上X座標
							// １枚のポリゴンの頂点とテクスチャ座標を設定
							//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 19)
						{
						//money1
						float x = (float)(money % 10);
						money /= 10;
						// numberを次の桁へ
						tw = 1.0f / 10;		// テクスチャの幅
						tx = tw * x;			// テクスチャの左上X座標
						// １枚のポリゴンの頂点とテクスチャ座標を設定
						//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
						//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 20)
						{
						//money2
						float x = (float)(money % 10);
						money /= 10;
						// numberを次の桁へ
						tw = 1.0f / 10;		// テクスチャの幅
						tx = tw * x;			// テクスチャの左上X座標
						// １枚のポリゴンの頂点とテクスチャ座標を設定
						//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
						//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else if (i == 21)
						{
						//money3
						float x = (float)(money % 10);
						money /= 10;
						// numberを次の桁へ
						tw = 1.0f / 10;		// テクスチャの幅
						tx = tw * x;			// テクスチャの左上X座標
						// １枚のポリゴンの頂点とテクスチャ座標を設定
						//SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
						//	, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
						}
						else 
						{
						// numberを次の桁へ
						tw = 1.0f ;		// テクスチャの幅
						tx = 0.0f;			// テクスチャの左上X座標
						// １枚のポリゴンの頂点とテクスチャ座標を設定
						
						}
						SetSpriteColor(pause[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							, pause[i].color);
					}
					// ポリゴン描画

					if (i == 22)
					{
						if (gamedata->Description == true)
						{
							GetDeviceContext()->Draw(4, 0);
						}
					}
					else {
						GetDeviceContext()->Draw(4, 0);
					}

				}
			}


		}
		else
		{
			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pause[22].g_VertexBuffer, &stride, &offset);

			// マトリクス設定
			SetWorldViewProjection2D();

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_pause[pause[22].g_TexNo]);
			float px, py, pw, ph, tw, th, tx, ty;
			px = pause[22].g_w / 2;	// スコアの表示位置X
			py = pause[22].g_h / 2;			// スコアの表示位置Y
			pw = pause[22].g_w;				// スコアの表示幅
			ph = pause[22].g_h;				// スコアの表示高さ

			tw = 1.0f;		// テクスチャの幅
			th = 1.0f;		// テクスチャの高さ
			tx = 0.0f;			// テクスチャの左上X座標
			ty = 0.0f;			// テクスチャの左上Y座標
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(pause[22].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	
	
	// ライトを有効にする
	SetLightEnable(true);

	// Z比較有効
	SetDepthEnable(true);
	
	
}

void DrawBattleUI(void)
{
	// 2Dの描画処理
		// ライトを無効化する
	SetLightEnable(false);

	// Z比較なし
	SetDepthEnable(false);
	

	if (gamedata->DescriptionFirst == true)
	{
		
		for (int i = 0; i < (MAX_BATTLEUI); i++)
		{
			if (i == 14)continue;
			if (battleUI[i].g_Use == true)
			{
				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &battleUI[i].g_VertexBuffer, &stride, &offset);

				// マトリクス設定
				SetWorldViewProjection2D();

				// プリミティブトポロジ設定
				GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// マテリアル設定
				MATERIAL material;
				ZeroMemory(&material, sizeof(material));
				material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SetMaterial(material);

				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_battleUI[battleUI[i].g_TexNo]);

				float px, py, pw, ph, tw, th, tx, ty;

				//if (gamedata->pausecheck == false)
				{
					int money = gamedata->money;
					for (int i = 0; i < MAX_BATTLEUI; i++)
					{
						// スコアの位置やテクスチャー座標を反映
						px = battleUI[i].g_Pos.x;	// プレイヤーの表示位置X
						py = battleUI[i].g_Pos.y;			// プレイヤーの表示位置Y
						pw = battleUI[i].g_w;				// プレイヤーの表示幅
						ph = battleUI[i].g_h;				// プレイヤーの表示高さ
						tw = 1.0f / 1;		// テクスチャの幅
						th = 1.0f / 1;		// テクスチャの高さ
						tx = 0.0f;			// テクスチャの左上X座標
						ty = 0.0f;			// テクスチャの左上Y座標
						
						if(i==3)
						{
							float x = (float)(gamedata->playernum % 10);
							// スコアの位置やテクスチャー座標を反映
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
						}else if(i==20)
						{
							float x = (float)(money % 10);
							money /= 10;
							// スコアの位置やテクスチャー座標を反映
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
						}
						else if (i == 21)
						{
							float x = (float)(money % 10);
							money /= 10;
							// スコアの位置やテクスチャー座標を反映
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
						}
						else if (i == 22)
						{
							float x = (float)(money % 10);
							// スコアの位置やテクスチャー座標を反映
							tw = 1.0f / 10;		// テクスチャの幅
							tx = tw * x;			// テクスチャの左上X座標
						}
						SetSpriteColor(battleUI[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th
							, battleUI[i].color);
						// ポリゴン描画
						GetDeviceContext()->Draw(4, 0);
					}

				}/*
				else {

				}*/

			}
		}

	}

	if (gamedata->gameover == true)
	{
		if (gamedata->dying == false)
		{
			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &battleUI[14].g_VertexBuffer, &stride, &offset);

			// マトリクス設定
			SetWorldViewProjection2D();

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			SetMaterial(material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_battleUI[battleUI[14].g_TexNo]);

			float px, py, pw, ph, tw, th, tx, ty;

			px = battleUI[14].g_w / 2;	// スコアの表示位置X
			py = battleUI[14].g_h / 2;			// スコアの表示位置Y
			pw = battleUI[14].g_w;				// スコアの表示幅
			ph = battleUI[14].g_h;				// スコアの表示高さ

			tw = 1.0f;		// テクスチャの幅
			th = 1.0f;		// テクスチャの高さ
			tx = 0.0f;			// テクスチャの左上X座標
			ty = 0.0f;			// テクスチャの左上Y座標
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(battleUI[14].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	// ライトを有効にする
	SetLightEnable(true);

	// Z比較有効
	SetDepthEnable(true);


}

void DrawResultUI(void)
{

	// 2Dの描画処理
		// ライトを無効化する
	SetLightEnable(false);

	// Z比較なし
	SetDepthEnable(false);

	
		int MAXHP = (int)gamedata->playerMAXHP;
		int ATK = (int)gamedata->playerAtk;
		int KEY = (int)gamedata->getkey;
		int time_minute = (int)(gamedata->time / 60);
		int time_secend = ((int)(gamedata->time)) % 60;
		int SKILL, BOSS,check;
		SKILL = 1;
		if (enemyBOSS->use == true)
		{
			BOSS = 0;
		}
		else
		{
			BOSS = 1;
		}
		int score =(int)(( MAXHP * MAXHP_SCORE)+ (KEY* KEY_SCORE)+(SKILL*SKILL_SCORE)+(BOSS*BOSS_SCORE)+(gamedata->playernum*PLAYERNUM_SCORE)+(TIME_SCORE- gamedata->time));
		if (score < FINAL_SCORE_CHECK_1)
		{
			check = 1;
		}else if (score >= FINAL_SCORE_CHECK_1 && score < FINAL_SCORE_CHECK_2)
		{
			check = 2;
		}
		else if (score >= FINAL_SCORE_CHECK_2)
		{
			check = 3;
		}
		if (BOSS == 0)
		{
			check = 1;
		}

		for (int i = 0; i < MAX_RESULTUI; i++)
		{
			if (i == 20 && check != 1)
			{
				continue;
			}
			if (i == 21 && check != 2)
			{
				continue;
			}
			if (i == 22 && check != 3)
			{
				continue;
			}
				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &ResultUI[i].g_VertexBuffer, &stride, &offset);

				// マトリクス設定
				SetWorldViewProjection2D();

				// プリミティブトポロジ設定
				GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// マテリアル設定
				MATERIAL material;
				ZeroMemory(&material, sizeof(material));
				material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SetMaterial(material);

				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_ResultUI[ResultUI[i].g_TexNo]);

				float px, py, pw, ph, tw, th, tx, ty;

				if (i == 0)
				{
					px = ResultUI[i].g_w / 2;	// スコアの表示位置X
					py = ResultUI[i].g_h / 2;			// スコアの表示位置Y
					pw = ResultUI[i].g_w;				// スコアの表示幅
					ph = ResultUI[i].g_h;				// スコアの表示高さ

					tw = 1.0f;		// テクスチャの幅
					th = 1.0f;		// テクスチャの高さ
					tx = 0.0f;			// テクスチャの左上X座標
					ty = 0.0f;			// テクスチャの左上Y座標
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					// ポリゴン描画
					SetSpriteColor(ResultUI[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						ResultUI[i].color);


					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
				else if (i == 1)
				{

					px = ResultUI[i].g_w / 2;	// スコアの表示位置X
					py = ResultUI[i].g_h / 2;			// スコアの表示位置Y
					pw = ResultUI[i].g_w;				// スコアの表示幅
					ph = ResultUI[i].g_h;				// スコアの表示高さ

					tw = 1.0f;		// テクスチャの幅
					th = 1.0f;		// テクスチャの高さ
					tx = 0.0f;			// テクスチャの左上X座標
					ty = 0.0f;			// テクスチャの左上Y座標
					// ポリゴン描画
					SetSpriteColor(ResultUI[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						ResultUI[i].color);


					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
				else
				{
					// スコアの位置やテクスチャー座標を反映
					px = ResultUI[i].g_Pos.x;	// プレイヤーの表示位置X
					py = ResultUI[i].g_Pos.y;			// プレイヤーの表示位置Y
					pw = ResultUI[i].g_w;				// プレイヤーの表示幅
					ph = ResultUI[i].g_h;				// プレイヤーの表示高さ
					tw = 1.0f / 10;		// テクスチャの幅
					th = 1.0f / 1;		// テクスチャの高さ
					ty = 0.0f;			// テクスチャの左上Y座標
					if (i >= 2 && i <= 4)
					{
						//i=2,3,4  HP MAX
						float x = (float)(MAXHP % 10);
						// numberを次の桁へ
						MAXHP /= 10;
						
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 5 )
					{
						//i=5		もらった鍵
						float x = (float)(KEY);
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 6 )
					{
						//i=6		追加能力数
						float x = (float)(SKILL);
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i >= 7&&i<=9)
					{
						//player ATK
						float x = (float)(ATK % 10);
						// numberを次の桁へ
						ATK /= 10;
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 10)
					{
						//殘機
						float x = (float)(gamedata->playernum);
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 11)
					{
						//BOSS討伐
						float x = (float)(BOSS);
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 12)
					{
						//時間 分-1
						float x = (float)(time_minute % 10);
						time_minute /= 10;
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 13)
					{
						//時間　分-2

						float x = (float)(time_minute % 10);
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 14)
					{
						//時間 秒-1
						float x = (float)(time_secend % 10);
						time_secend /= 10;
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 15)
					{
						//時間 秒-2

						float x = (float)(time_secend % 10);
						// numberを次の桁へ
						tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i >= 16&& i <= 19)
					{
					//得点

					float x = (float)(score % 10);
					score /= 10;
					// numberを次の桁へ
					tx = tw * x;			// テクスチャの左上X座標
					}
					else if (i == 20)
					{
					//評論1
						if (check == 1)
						{
							tw = 2.0f / 5;		// テクスチャの幅
							// numberを次の桁へ
							tx = 0.0f;			// テクスチャの左上X座標
						}
					}
					else if (i == 21)
					{
					//評論2
						if (check == 2)
						{
							tw = 1.0f / 5;		// テクスチャの幅
							// numberを次の桁へ
							tx = 0.0f+ (tw*2);			// テクスチャの左上X座標
						}
					}
					else if (i == 22)
					{
					//評論2
						if (check == 3)
						{
							tw = 2.0f / 5;		// テクスチャの幅
							// numberを次の桁へ
							tx = 0.0f+(3.0f/5);			// テクスチャの左上X座標
						}
					}
					// ポリゴン描画
					SetSpriteColor(ResultUI[i].g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						ResultUI[i].color);
					

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
		}

	// ライトを有効にする
	SetLightEnable(true);

	// Z比較有効
	SetDepthEnable(true);


}
