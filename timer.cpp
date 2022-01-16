//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : GP11A132　43　林尚頡
//
//=============================================================================
#include <time.h>
#include "main.h"
#include "timer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// キャラサイズ
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(4)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static GAMEDATA					*gamedata = GetData();
static char *g_TexturName[] = {
	"data/TEXTURE/number16x32.png",			//数字素材
	"data/TEXTURE/yellow.jpg",				//フレーム
	"data/TEXTURE/timersozai1.jpg",
	"data/TEXTURE/timersozai2.jpg",
};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static D3DXVECTOR3				g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号


static double					g_Time;					   // 0から計算の時間
static int					g_Time_real;			   // TIME_MAXから0まで計算の時間
static int					g_Time_minute;
static int					g_Time_second;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTime(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	g_Use   = true;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = D3DXVECTOR3(SCREEN_WIDTH-40, 40.0f, 0.0f);
	g_TexNo = 0;

	g_Time = 0;				// タイマーの初期化

	gamedata->start = clock();		//ゲーム開始初期化の時、タイマースタート

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	
	

	//今は経つの時間
	gamedata->end = clock();
	

	g_Time = (gamedata->end- gamedata->start)/1000.0+ gamedata->time;			//単位:秒  end-startの単位はミリ秒　　例:1234ミリ秒=1.234秒
	//g_Time_real = Time_MAX - g_Time;		//単位:秒
	g_Time_real = (int)g_Time;
	g_Time_minute = (g_Time_real / 60);		//単位:分
	g_Time_second = (g_Time_real % 60);		//単位:秒


	if (g_Time > Time_MAX)
	{
		g_Time = Time_MAX;
	}


	
#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	if (gamedata->DescriptionFirst == true)
	{
		// Z比較なし　// ★Zバッファー
		SetDepthEnable(false);

		// ライティングを無効
		SetLightEnable(false);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// マトリクス設定
		SetWorldViewProjection2D();

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		//フレームの描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			// タイマーの位置やテクスチャー座標を反映
			float px = g_Pos.x - g_w * 5;	// フレームの表示位置X
			float py = g_Pos.y;				// フレームの表示位置Y
			float pw = g_w * 14;				// フレームの表示幅
			float ph = g_h * 2;				// フレームの表示高さ

			float tw = 1.0f;		// テクスチャの幅
			float th = 1.0f;		// テクスチャの高さ
			float tx = tw;				// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		//フレームの描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
			// タイマーの位置やテクスチャー座標を反映
			float px = g_Pos.x - (g_w * 5);	// フレームの表示位置X
			float py = g_Pos.y;				// フレームの表示位置Y
			float pw = g_w;				// フレームの表示幅
			float ph = g_h;				// フレームの表示高さ

			float tw = 1.0f;		// テクスチャの幅
			float th = 1.0f;		// テクスチャの高さ
			float tx = tw;				// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		//タイマー数字の外枠の描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// 桁数分処理する
			int number1 = g_Time_minute;
			int number2 = g_Time_second;

			for (int i = 0; i < TIME_DIGIT; i++)
			{
				if (i < 2)
				{
					//秒数字の外枠の描画

					// 今回表示する桁の数字
					float x = (float)(number2 % 10);

					// タイマーの位置やテクスチャー座標を反映
					float px = g_Pos.x - (g_w * i * 3);	// タイマーの秒の表示位置X
					float py = g_Pos.y;			// タイマーの表示位置Y
					float pw = g_h + 10;				// タイマーの表示幅
					float ph = g_h + 10;				// タイマーの表示高さ

					float tw = 1.0f;		// テクスチャの幅
					float th = 1.0f;		// テクスチャの高さ
					float tx = x * tw;			// テクスチャの左上X座標
					float ty = 0.0f;			// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

					// 次の桁へ
					number2 /= 10;
				}
				else
				{
					//分数字の外枠の描画

					// 今回表示する桁の数字
					float x = (float)(number1 % 10);

					// タイマーの位置やテクスチャー座標を反映
					float px = g_Pos.x - TEXTURE_WIDTH - (g_w * i * 3);	// タイマーの分の表示位置X
					float py = g_Pos.y;			// タイマーの表示位置Y
					float pw = g_h + 10;				// タイマーの表示幅
					float ph = g_h + 10;				// タイマーの表示高さ

					float tw = 1.0f;		// テクスチャの幅
					float th = 1.0f;		// テクスチャの高さ
					float tx = x * tw;			// テクスチャの左上X座標
					float ty = 0.0f;			// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

					// 次の桁へ
					number1 /= 10;
				}

			}
		}


		//数字の描画
		{

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// 桁数分処理する
			int number1 = g_Time_minute;
			int number2 = g_Time_second;

			for (int i = 0; i < TIME_DIGIT; i++)
			{
				if (i < 2)
				{
					//秒の数字の描画

					// 今回表示する桁の数字
					float x = (float)(number2 % 10);

					// タイマーの位置やテクスチャー座標を反映
					float px = g_Pos.x - (g_w * 3 * i);	// タイマーの秒の表示位置X
					float py = g_Pos.y;			// タイマーの表示位置Y
					float pw = g_w;				// タイマーの表示幅
					float ph = g_h;				// タイマーの表示高さ

					float tw = 1.0f / 10;		// テクスチャの幅
					float th = 1.0f / 1;		// テクスチャの高さ
					float tx = x * tw;			// テクスチャの左上X座標
					float ty = 0.0f;			// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f));

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

					// 次の桁へ
					number2 /= 10;
				}
				else
				{
					//分の数字の描画

					// 今回表示する桁の数字
					float x = (float)(number1 % 10);

					// タイマーの位置やテクスチャー座標を反映
					float px = g_Pos.x - TEXTURE_WIDTH - (g_w * 3 * i);	// タイマーの分の表示位置X
					float py = g_Pos.y;			// タイマーの表示位置Y
					float pw = g_w;				// タイマーの表示幅
					float ph = g_h;				// タイマーの表示高さ

					float tw = 1.0f / 10;		// テクスチャの幅
					float th = 1.0f / 1;		// テクスチャの高さ
					float tx = x * tw;			// テクスチャの左上X座標
					float ty = 0.0f;			// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f));

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

					// 次の桁へ
					number1 /= 10;
				}

			}
		}


		// ライティングを有効に
		SetLightEnable(true);

		// Z比較あり
		SetDepthEnable(true);

	}
	
}




//=============================================================================
// タイマー構造体の先頭アドレスを取得
//=============================================================================
double GetTime(void)
{
	return g_Time;
}

int GetMinute(void)
{
	return g_Time_minute;
}

int GetSecond(void)
{
	return g_Time_second;
}


clock_t *getstart(void)
{
	return &start;
}

clock_t *getend(void)
{
	return &end;
}