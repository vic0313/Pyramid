//=============================================================================
//
// ライト処理 [light.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;

static GAMEDATA *gamedata = GetData();
static ENEMY	*enemyBOSS = GetEnemyBOSS();
//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{

	//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Light[i].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		g_Light[i].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		g_Light[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Light[i].Attenuation = 100.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	//並行光源の設定(世界を照えらす光)
	g_Light[0].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);		//光の向き
	g_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//光の色
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					//並行光源
	g_Light[0].Enable = true;									//このライトをON
	SetLight(0, &g_Light[0]);									//これで設定している



	// フォグの初期化(霧の効果)
	g_Fog.FogStart = 30.0f;								//視点からこの距離離れるとフォグがかかり始める
	g_Fog.FogEnd = 100.0f;									//ここまで離れるとフォグの色で見えなくなる
	g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//フォグの色
	SetFog(&g_Fog);
	SetFogEnable(true);

}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{
	switch (gamedata->modenow)
	{
	case MODE_TITLE:
	{
		// フォグの初期化(霧の効果)
		g_Fog.FogStart = 500.0f;								//視点からこの距離離れるとフォグがかかり始める
		g_Fog.FogEnd = 1000.0f;									//ここまで離れるとフォグの色で見えなくなる
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//フォグの色
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;
	case MODE_RESTAREA:
	{
		// フォグの初期化(霧の効果)
		g_Fog.FogStart = 500.0f;								//視点からこの距離離れるとフォグがかかり始める
		g_Fog.FogEnd = 1000.0f;									//ここまで離れるとフォグの色で見えなくなる
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//フォグの色
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;
	case MODE_BATTLEAREA:
	{
		if (enemyBOSS[0].use == true) 
		{
			// フォグの初期化(霧の効果)
			g_Fog.FogStart = 5000.0f;								//視点からこの距離離れるとフォグがかかり始める
			g_Fog.FogEnd = 5000.0f;									//ここまで離れるとフォグの色で見えなくなる
			g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//フォグの色
			SetFog(&g_Fog);
			SetFogEnable(true);
		}
		else
		{
			// フォグの初期化(霧の効果)
			g_Fog.FogStart = 300.0f;								//視点からこの距離離れるとフォグがかかり始める
			g_Fog.FogEnd =800.0f;									//ここまで離れるとフォグの色で見えなくなる
			g_Fog.FogColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//フォグの色
			SetFog(&g_Fog);
			SetFogEnable(true);
		}
		
	}
		break;
	case MODE_RESULT:
	{
		// フォグの初期化(霧の効果)
		g_Fog.FogStart = 30.0f;								//視点からこの距離離れるとフォグがかかり始める
		g_Fog.FogEnd = 300.0f;									//ここまで離れるとフォグの色で見えなくなる
		g_Fog.FogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//フォグの色
		SetFog(&g_Fog);
		SetFogEnable(true);
	}
		break;

	default:
		break;
	}


}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
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
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}



