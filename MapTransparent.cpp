//=============================================================================
//
// プレイヤー前の透明画像の処理 [MapTransparent.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "meshMap.h"
#include "input.h"
#include "renderer.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"
#include "MapTouch.h"
#include "other_objects.h"
#include "MapTransparent.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_TRIANGLE_MAX	(1)

#define	TREE_WIDTH			(80.0f)			// 頂点サイズ
#define	TREE_HEIGHT			(80.0f)			// 頂点サイズ
#define	RADIUS			(10.0f)			// 頂点サイズ

#define TEXTURE_MAX					(1)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			nIdxShadow;		// 影ID
	bool		bUse;			// 使用しているかどうか
	ID3D11Buffer		*g_VertexBuffer;

}Transparent;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertex(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER  *player = GetPlayer();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
//static ID3D11Buffer					*g_Transparent[i].g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_TextureTransparent[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static Transparent					g_Transparent[MAX_Transparent];	// 木ワーク
static int					g_TexTransparentNo;			// テクスチャ番号
static bool					g_bAlpaTest;		// アルファテストON/OFF

static char *g_TextureTransparentName[] =
{
	"data/TEXTURE/fade_black.png",
};
//透明化用



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTransparent(void)
{
	float rot = 0.0f;
	//MakeVertex();
	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureTransparentName[i],
			NULL,
			NULL,
			&g_TextureTransparent[i],
			NULL);

	}

	g_TexTransparentNo = 0;
	for (int i = 0; i < (MAX_Transparent); i++)
	{
		float fWidth = 0.0f;
		float fHeight = 0.0f;
		//for (int i = 0; i < MAX_Transparent; i++)
		//{
			//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot += D3DX_PI / (MAX_Transparent);
		fWidth -= sinf(rot)*RADIUS;
		fHeight -= (cosf(rot)*RADIUS);
		fWidth = (float)fabs(fWidth);
		fHeight = (float)fabs(fHeight);
		//}


		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_Transparent[i].g_VertexBuffer);

		// 頂点バッファに値をセットする
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_Transparent[i].g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;



		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
		vertex[1].Position = D3DXVECTOR3(fWidth, fHeight, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		vertex[3].Position = D3DXVECTOR3(fWidth, -fHeight, 0.0f);

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

		GetDeviceContext()->Unmap(g_Transparent[i].g_VertexBuffer, 0);
		

		ZeroMemory(&g_Transparent[i].material, sizeof(g_Transparent[i].material));
		g_Transparent[i].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		g_Transparent[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Transparent[i].scl = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		g_Transparent[i].fWidth = TREE_WIDTH;
		g_Transparent[i].fHeight = TREE_HEIGHT;
		g_Transparent[i].bUse = true;
		g_bAlpaTest = false;	// true;

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTransparent(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_TextureTransparent[nCntTex] != NULL)
		{// テクスチャの解放
			g_TextureTransparent[nCntTex]->Release();
			g_TextureTransparent[nCntTex] = NULL;
		}
	}

	for (int i = 0; i < MAX_Transparent; i++)
	{
		if (g_Transparent[i].g_VertexBuffer != NULL)
		{// 頂点バッファの解放
			g_Transparent[i].g_VertexBuffer->Release();
			g_Transparent[i].g_VertexBuffer = NULL;
		}
	}
	
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTransparent(void)
{
	D3DXVECTOR3 ppos = player[0].pos;
	ppos.y += PLAYER_TALL / 2;
	D3DXVECTOR3 pos = ppos - camera->pos;
	D3DXVec3Normalize(&pos, &pos);
	for (int i = 0; i < MAX_Transparent; i++)
	{
		g_Transparent[i].pos = camera->pos + pos * (Transparent_pos);
		if (gamedata->dying == true)
		{
			g_Transparent[i].bUse = false;
		}if (gamedata->dying == false)
		{
			g_Transparent[i].bUse = true;
		}
	}


	// アルファテストON/OFF
	if (GetKeyboardTrigger(DIK_F1))
	{
		//三項演算子
		g_bAlpaTest = g_bAlpaTest ? false : true;

		/*	if (g_bAlpaTest == true)
			{
				g_bAlpaTest == false;
			}
			else
			{
				g_bAlpaTest == true;
			}*/
	}
	
}

//=============================================================================
// 描画処理
//=============================================================================

void DrawTransparent(void)
{
	if (camera->shoutchange == false) { return; }
	// αテスト設定
	//if (g_bAlpaTest == true)
	//{
	//	// αテストを有効に
	//	SetAlphaTestEnable(true);
	//}
	//// Z比較なし　// ★Zバッファー
	//SetDepthEnable(false);


	// ライティングを無効
	SetLightEnable(false);
	for (int i = 0; i < MAX_Transparent; i++)
	{
		if (g_Transparent[i].bUse)
		{
			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_Transparent[i].g_VertexBuffer, &stride, &offset);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			{
				if (g_Transparent[i].bUse)
				{
					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_Transparent[i].mtxWorld);

					// ビューマトリックスを取得
					//CAMERA *cam = GetCamera();

					//myxViewにカメラ行列を取り出す
					mtxView = camera->mtxView;

					// ポリゴンを正面に向ける
#if 0
			// 逆行列を計算で求める

			//便利な関数なのですが、劇遅
					D3DXMatrixInverse(&g_Transparent[i].mtxWorld, NULL, &mtxView);

					//馬鹿正直に計算しているので本来不必要な平行移動の値まで入れちゃ
					g_Transparent[i].mtxWorld._41 = 0.0f;	//移動Tx値をクリア
					g_Transparent[i].mtxWorld._42 = 0.0f;	//移動Ty値をクリア
					g_Transparent[i].mtxWorld._43 = 0.0f;	//移動Tz値をクリア
#else 
			//正方行列→行と列が同じ要素の行列(例4*4の行列とか)
			//DirectXのカメラ行列は正方行列になっています
			//正方行列には性質がある
			//正方行列の転置行列は逆行列になる
			//転置行列→行列の左上から右下へ向かって斜めの線を引き
			//その線を対象として左右上下入れ替えた行列

			//このやり方は超早い！！
					g_Transparent[i].mtxWorld._11 = mtxView._11;
					g_Transparent[i].mtxWorld._12 = mtxView._21;
					g_Transparent[i].mtxWorld._13 = mtxView._31;
					g_Transparent[i].mtxWorld._21 = mtxView._12;
					g_Transparent[i].mtxWorld._22 = mtxView._22;
					g_Transparent[i].mtxWorld._23 = mtxView._32;
					g_Transparent[i].mtxWorld._31 = mtxView._13;
					g_Transparent[i].mtxWorld._32 = mtxView._23;
					g_Transparent[i].mtxWorld._33 = mtxView._33;
#endif

					// スケールを反映
					D3DXMatrixScaling(&mtxScale, g_Transparent[i].scl.x,
						g_Transparent[i].scl.y,
						g_Transparent[i].scl.z);
					D3DXMatrixMultiply(&g_Transparent[i].mtxWorld, &g_Transparent[i].mtxWorld, &mtxScale);

					// 移動を反映
					D3DXMatrixTranslation(&mtxTranslate, g_Transparent[i].pos.x,
						g_Transparent[i].pos.y,
						g_Transparent[i].pos.z);
					D3DXMatrixMultiply(&g_Transparent[i].mtxWorld, &g_Transparent[i].mtxWorld, &mtxTranslate);


					// ワールドマトリックスの設定
					SetWorldMatrix(&g_Transparent[i].mtxWorld);

					// マテリアル設定
					SetMaterial(g_Transparent[i].material);

					// テクスチャ設定
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureTransparent[TEXTURE_MAX]);

					// ポリゴンの描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}

	}


	// ライティングを有効に
	SetLightEnable(true);

	// αテストを無効に
	//SetAlphaTestEnable(false);


	// //Z比較あり
	//SetDepthEnable(true);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertex(void)
{
	for (int i = 0; i < MAX_Transparent; i++)
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_Transparent[i].g_VertexBuffer);

		// 頂点バッファに値をセットする
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_Transparent[i].g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float fWidth = 100.0f;
		float fHeight = 100.0f;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-fWidth / 2.0f, fHeight / 2.0f, 0.0f);
		vertex[1].Position = D3DXVECTOR3(fWidth / 2.0f, fHeight / 2.0f, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-fWidth / 2.0f, -fHeight / 2.0f, 0.0f);
		vertex[3].Position = D3DXVECTOR3(fWidth / 2.0f, -fHeight / 2.0f, 0.0f);

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

		GetDeviceContext()->Unmap(g_Transparent[i].g_VertexBuffer, 0);

	}
	
	return S_OK;
}
