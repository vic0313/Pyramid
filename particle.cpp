//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "rope.h"
#include "bullet.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数
#define TEXTURE_BULLET_MAX			(1)			// テクスチャの数
#define TEXTURE_TWOJUMP_MAX			(1)			// テクスチャの数

#define	PARTICLE_SIZE_X		(20.0f)		// 頂点サイズ
#define	PARTICLE_SIZE_Y		(20.0f)		// 頂点サイズ
#define	PARTICLE_BULLET_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_BULLET_SIZE_Y		(40.0f)		// 頂点サイズ
#define	PARTICLE_TWOJUMP_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_TWOJUMP_SIZE_Y		(40.0f)		// 頂点サイズ

#define	PARTICLE_DIED_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_DIED_SIZE_Y		(40.0f)		// 頂点サイズ
#define	VALUE_MOVE_PARTICLE	(5.0f)		// 移動速度



#define	DISP_SHADOW						// 影の表示

#define ROPE_PARTICLE_LIFE	(20)
#define BULLET_PARTICLE_LIFE	(20)
#define TOWJUMP_PARTICLE_LIFE	(5.0f)
//#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeRopeVertexParticle(void);
HRESULT MakeBulletVertexParticle(void);
HRESULT MakeTwojumpVertexParticle(void);
HRESULT MakeDiedVertexParticle(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ROPE			*g_rope = GetRope();
static PLAYER *pPlayer = GetPlayer();
static BULLET *g_bullet = GetBullet();
static GAMEDATA *gamedata = GetData();
static PARTICLE					g_Rope_Particle[MAX_PARTICLE_ROPE];		// パーティクルワーク
static PARTICLE					g_Bullet_Particle[MAX_PARTICLE_BULLET];		// パーティクルワーク
static PARTICLE					g_Twojump_Particle[MAX_PARTICLE_BULLET];		// パーティクルワーク
static PARTICLE					died_Particle[MAX_PARTICLE_DIED];		// パーティクルワーク

static ID3D11Buffer					*g_rope_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_rope_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11Buffer					*g_bullet_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_bullet_Texture[TEXTURE_BULLET_MAX] = { NULL };	// テクスチャ情報
static ID3D11Buffer					*g_twojump_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_twojump_Texture[TEXTURE_TWOJUMP_MAX] = { NULL };	// テクスチャ情報
static ID3D11Buffer					*died_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*died_Texture[TEXTURE_TWOJUMP_MAX] = { NULL };	// テクスチャ情報
//static D3DXMATRIX				g_mtxWorldParticle;				// ワールドマトリックス


static float					g_fWidthBase = 5.0f;			// 基準の幅
//static float					g_fHeightBase = 10.0f;			// 基準の高さ
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード

static int					usedparticle;
static int					life;
static bool					ok;
//static int					twojumpcount = 0;
static float				twojumppos=0.0f;
//static D3DXVECTOR3 pPOS= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//static D3DXVECTOR3  move= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//static D3DXVECTOR3	twojumppos= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
static char *g_rope_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};
static char *g_bullet_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};
static char *g_twojump_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};
static char *died_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	{
		// 頂点情報の作成
		MakeRopeVertexParticle();

		// テクスチャ生成
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_rope_TextureName[i],
				NULL,
				NULL,
				&g_rope_Texture[i],
				NULL);
		}



		// パーティクルワークの初期化
		for (int i = 0; i < MAX_PARTICLE_ROPE; i++)
		{
			g_Rope_Particle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Rope_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Rope_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Rope_Particle[i].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Rope_Particle[i].g_TexNo = 0;
			ZeroMemory(&g_Rope_Particle[i].material, sizeof(g_Rope_Particle[i].material));
			g_Rope_Particle[i].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_Rope_Particle[i].fSizeX = PARTICLE_SIZE_X;
			g_Rope_Particle[i].fSizeY = PARTICLE_SIZE_Y;
			g_Rope_Particle[i].nIdxShadow = -1;
			g_Rope_Particle[i].nLife = 0;
			g_Rope_Particle[i].bUse = false;
			g_Rope_Particle[i].time = 0.0f;
			g_Rope_Particle[i].timeplus = 0.01f;
		}

		usedparticle = 0;
		g_roty = 0.0f;
		g_spd = 0.0f;
		life = ROPE_PARTICLE_LIFE;
		ok = false;

		//return S_OK;
	}

	{
		// 頂点情報の作成
		MakeBulletVertexParticle();

		// テクスチャ生成
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_bullet_TextureName[i],
				NULL,
				NULL,
				&g_bullet_Texture[i],
				NULL);
		}



		// パーティクルワークの初期化
		for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
		{
			g_Bullet_Particle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet_Particle[i].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet_Particle[i].g_TexNo = 0;
			ZeroMemory(&g_Bullet_Particle[i].material, sizeof(g_Bullet_Particle[i].material));
			g_Bullet_Particle[i].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_Bullet_Particle[i].fSizeX = PARTICLE_BULLET_SIZE_X;
			g_Bullet_Particle[i].fSizeY = PARTICLE_BULLET_SIZE_Y;
			g_Bullet_Particle[i].nIdxShadow = -1;
			g_Bullet_Particle[i].nLife = 0;
			g_Bullet_Particle[i].bUse = false;
			g_Bullet_Particle[i].time = 0.0f;
			g_Bullet_Particle[i].timeplus = 0.01f;

		}

		
		//return S_OK;
	}


	{
		// 頂点情報の作成
		MakeTwojumpVertexParticle();

		// テクスチャ生成
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_twojump_TextureName[i],
				NULL,
				NULL,
				&g_twojump_Texture[i],
				NULL);
		}



		// パーティクルワークの初期化
		for (int i = 0; i < MAX_PARTICLE_TWOJUMP; i++)
		{
			g_Twojump_Particle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Twojump_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Twojump_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Twojump_Particle[i].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Twojump_Particle[i].g_TexNo = 0;
			ZeroMemory(&g_Twojump_Particle[i].material, sizeof(g_Twojump_Particle[i].material));
			g_Twojump_Particle[i].material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_Twojump_Particle[i].fSizeX = PARTICLE_TWOJUMP_SIZE_X;
			g_Twojump_Particle[i].fSizeY = PARTICLE_TWOJUMP_SIZE_Y;
			g_Twojump_Particle[i].nIdxShadow = -1;
			g_Twojump_Particle[i].nLife = 0;
			g_Twojump_Particle[i].bUse = false;
			g_Twojump_Particle[i].time = 0.0f;
			g_Twojump_Particle[i].timeplus = 0.1f;
			g_Twojump_Particle[i].moveway= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		
		
		

		//return S_OK;
	}


	{
		// 頂点情報の作成
		MakeDiedVertexParticle();

		// テクスチャ生成
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				died_TextureName[i],
				NULL,
				NULL,
				&died_Texture[i],
				NULL);
		}

		// パーティクルワークの初期化
		for (int i = 0; i < MAX_PARTICLE_DIED; i++)
		{
			died_Particle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			died_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			died_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			died_Particle[i].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			died_Particle[i].g_TexNo = 0;
			ZeroMemory(&died_Particle[i].material, sizeof(died_Particle[i].material));
			died_Particle[i].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			died_Particle[i].fSizeX = PARTICLE_DIED_SIZE_X;
			died_Particle[i].fSizeY = PARTICLE_DIED_SIZE_Y;
			died_Particle[i].nIdxShadow = -1;
			died_Particle[i].nLife = 0;
			died_Particle[i].bUse = false;
			died_Particle[i].time = 0.0f;
			died_Particle[i].timeplus = 0.01f;
		}

	}
	return S_OK;
}


//HRESULT InitRopeParticle(void)
//HRESULT InitBulletParticle(void)

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	{
		//テクスチャの解放
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (g_rope_Texture[nCntTex] != NULL)
			{
				g_rope_Texture[nCntTex]->Release();
				g_rope_Texture[nCntTex] = NULL;
			}
		}

		// 頂点バッファの解放
		if (g_rope_VertexBuffer != NULL)
		{
			g_rope_VertexBuffer->Release();
			g_rope_VertexBuffer = NULL;
		}
	}


	{
		//テクスチャの解放
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_bullet_Texture[i] != NULL)
			{
				g_bullet_Texture[i]->Release();
				g_bullet_Texture[i] = NULL;
			}
		}

		// 頂点バッファの解放
		if (g_bullet_VertexBuffer != NULL)
		{
			g_bullet_VertexBuffer->Release();
			g_bullet_VertexBuffer = NULL;
		}
	}

	{
		//テクスチャの解放
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (g_twojump_Texture[nCntTex] != NULL)
			{
				g_twojump_Texture[nCntTex]->Release();
				g_twojump_Texture[nCntTex] = NULL;
			}
		}

		// 頂点バッファの解放
		if (g_twojump_VertexBuffer != NULL)
		{
			g_twojump_VertexBuffer->Release();
			g_twojump_VertexBuffer = NULL;
		}
	}

	{
		//テクスチャの解放
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (died_Texture[nCntTex] != NULL)
			{
				died_Texture[nCntTex]->Release();
				died_Texture[nCntTex] = NULL;
			}
		}

		// 頂点バッファの解放
		if (died_VertexBuffer != NULL)
		{
			died_VertexBuffer->Release();
			died_VertexBuffer = NULL;
		}
	}
}
//void UninitRopeParticle(void)
//void UninitBulletParticle(void)

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	if (pPlayer[0].use == true)
	{

		//rope
		{

			for (int i = 0; i < MAX_ROPE; i++)
			{
				if (g_rope[i].use == false) {
					ok = false;
					life = ROPE_PARTICLE_LIFE;
					usedparticle = 0;
				}
				else if (g_rope[i].use == true)
				{
					if (usedparticle >= ROPE_LONG_PART) {
						usedparticle = 0;
					}
					D3DXVECTOR3	pos = pPlayer[0].pos;
					pos.y += PLAYER_TALL / 2;
					D3DXVECTOR3 poss = g_rope[i].end_pos - pos;
					life = ROPE_PARTICLE_LIFE;
					for (int count = 0; count < ROPE_LONG_PART; count++)
					{
						g_Rope_Particle[count].nLife = life;
						g_Rope_Particle[count].pos = pPlayer[0].pos + (float)count*(poss / ROPE_LONG_PART);
						g_Rope_Particle[count].pos.y += PLAYER_TALL / 2;
						life--;
					}
					if (g_Rope_Particle[usedparticle].bUse != false) {
						g_Rope_Particle[usedparticle].bUse = false;
					}
					g_Rope_Particle[usedparticle].bUse = true;
					usedparticle++;

					ok = true;
				}
			}

			for (int i = 0; i < ROPE_LONG_PART; i++)
			{
				if (ok == false) {
					g_Rope_Particle[i].nLife--;
				}
				if (g_Rope_Particle[i].nLife <= 0)
				{
					g_Rope_Particle[i].bUse = false;
					//ReleaseShadow(g_Rope_Particle[i].nIdxShadow);
					//g_Rope_Particle[i].nIdxShadow = -1;
				}
				else
				{
					if (g_Rope_Particle[i].nLife <= 80)
					{
						//g_Rope_Particle[i].material.Diffuse.r = 0.8f - (float)(80 - g_Rope_Particle[i].nLife) / 80 * 0.8f;
						//g_Rope_Particle[i].material.Diffuse.g = 0.7f - (float)(80 - g_Rope_Particle[i].nLife) / 80 * 0.7f;
						//g_Rope_Particle[i].material.Diffuse.b = 0.2f - (float)(80 - g_Rope_Particle[i].nLife) / 80 * 0.2f;
						g_Rope_Particle[i].material.Diffuse.r = 0.8f;
						g_Rope_Particle[i].material.Diffuse.g = 0.7f;
						g_Rope_Particle[i].material.Diffuse.b = 0.2f;
					}

					//if (g_Rope_Particle[i].nLife <= 20)
					//{
					//	// α値設定
					//	g_Rope_Particle[i].material.Diffuse.a -= 0.05f;
					//	if (g_Rope_Particle[i].material.Diffuse.a < 0.0f)
					//	{
					//		g_Rope_Particle[i].material.Diffuse.a = 0.0f;
					//	}
					//}
				}
			}


			//	
			//	{
			//		for(int i = 0; i < MAX_PARTICLE; i++)
			//		{
			//			if(g_Rope_Particle[i].bUse)
			//			{// 使用中
			//				g_Rope_Particle[i].pos.x += g_Rope_Particle[i].move.x;
			//				g_Rope_Particle[i].pos.z += g_Rope_Particle[i].move.z;
			//				g_Rope_Particle[i].pos.y += g_Rope_Particle[i].move.y;
			//
			//#ifdef DISP_SHADOW
			//				if(g_Rope_Particle[i].nIdxShadow != -1)
			//				{// 影使用中
			//					float colA;
			//
			//					// 影の位置設定
			//					SetPositionShadow(g_Rope_Particle[i].nIdxShadow, D3DXVECTOR3(g_Rope_Particle[i].pos.x, 0.1f, g_Rope_Particle[i].pos.z));
			//
			//					// 影の色の設定
			//					colA = g_Rope_Particle[i].material.Diffuse.a;
			//					SetColorShadow(g_Rope_Particle[i].nIdxShadow, D3DXCOLOR(0.5f, 0.5f, 0.5f, colA));
			//				}
			//#endif
			//}
			//}
			// パーティクル発生
			//		if((rand() % 2) == 0)



		}

		//bullet
		{
			for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
			{
				if (g_Bullet_Particle[i].bUse)
				{// 使用中
					g_Bullet_Particle[i].pos.x += g_Bullet_Particle[i].move.x;
					g_Bullet_Particle[i].pos.z += g_Bullet_Particle[i].move.z;
					g_Bullet_Particle[i].pos.y += g_Bullet_Particle[i].move.y;
					//if (g_Bullet_Particle[i].pos.y <= g_Bullet_Particle[i].fSizeY / 2)
					//{// 着地した
					//	g_Bullet_Particle[i].pos.y = g_Bullet_Particle[i].fSizeY / 2;
					//	g_Bullet_Particle[i].move.y = -g_Bullet_Particle[i].move.y * 0.75f;
					//}

					g_Bullet_Particle[i].move.x += (0.0f - g_Bullet_Particle[i].move.x) * 0.015f;
					//g_Bullet_Particle[i].move.y -= 0.25f;
					g_Bullet_Particle[i].move.z += (0.0f - g_Bullet_Particle[i].move.z) * 0.015f;


					g_Bullet_Particle[i].nLife--;
					if (g_Bullet_Particle[i].nLife <= 0)
					{
						g_Bullet_Particle[i].bUse = false;
					}
					else
					{
						if (g_Bullet_Particle[i].nLife <= 80)
						{
							g_Bullet_Particle[i].material.Diffuse.r = 0.8f - (float)(80 - g_Bullet_Particle[i].nLife) / 80 * 0.8f;
							g_Bullet_Particle[i].material.Diffuse.g = 0.7f - (float)(80 - g_Bullet_Particle[i].nLife) / 80 * 0.7f;
							g_Bullet_Particle[i].material.Diffuse.b = 0.2f - (float)(80 - g_Bullet_Particle[i].nLife) / 80 * 0.2f;
						}

						if (g_Bullet_Particle[i].nLife <= 20)
						{
							// α値設定
							g_Bullet_Particle[i].material.Diffuse.a -= 0.05f;
							if (g_Bullet_Particle[i].material.Diffuse.a < 0.0f)
							{
								g_Bullet_Particle[i].material.Diffuse.a = 0.0f;
							}
						}
					}
				}

			}
		}

		//twojump
		{

			if (gamedata->bigjumpset == true)
			{
				twojumppos = 0.0f;
				for (int j = 0; j < MAX_PARTICLE_TWOJUMP; j++)
				{

					/*pPOS = pPlayer[0].pos;*/
					g_Twojump_Particle[j].bUse = true;
					//g_Twojump_Particle[i].nLife = TOWJUMP_PARTICLE_LIFE;
					g_Twojump_Particle[j].pos = pPlayer[0].pos;
					g_Twojump_Particle[j].pos.y += PLAYER_TALL;
					twojumppos += float(D3DX_PI / (MAX_PARTICLE_TWOJUMP / 2));
					if (twojumppos == D3DX_PI)
					{
						twojumppos = -D3DX_PI;
					}

					g_Twojump_Particle[j].pos.x -= sinf(twojumppos)*TWOJUMP_RADIUS;
					g_Twojump_Particle[j].pos.z -= cosf(twojumppos)*TWOJUMP_RADIUS;

					D3DXVECTOR3 pPOS = g_Twojump_Particle[j].pos - pPlayer[0].pos;
					D3DXVec3Normalize(&pPOS, &pPOS);
					g_Twojump_Particle[j].moveway = pPOS;

				}
				gamedata->bigjumpset = false;
			}


			for (int j = 0; j < MAX_PARTICLE_TWOJUMP; j++)
			{
				if (g_Twojump_Particle[j].bUse == true)
				{
					g_Twojump_Particle[j].pos += g_Twojump_Particle[j].moveway;
					g_Twojump_Particle[j].pos.y += TWOJUMP_YPLUE;
					g_Twojump_Particle[j].time += g_Twojump_Particle[j].timeplus;
				}
				if (g_Twojump_Particle[j].time >= TOWJUMP_PARTICLE_LIFE)
				{
					g_Twojump_Particle[j].time = 0.0f;
					g_Twojump_Particle[j].bUse = false;
				}

			}

		}
	}


	//died
	{

		if (gamedata->dying == true)
		{
			for (int i = 0; i < MAX_PARTICLE_DIED; i++)
			{
				if (died_Particle[i].bUse == true)
				{// 使用中
					died_Particle[i].pos.x += died_Particle[i].move.x;
					died_Particle[i].pos.z += died_Particle[i].move.z;
					died_Particle[i].pos.y += died_Particle[i].move.y;

					died_Particle[i].move.x += (0.0f - died_Particle[i].move.x) * 0.015f;
					//died_Particle[i].move.y -= 0.25f;
					died_Particle[i].move.z += (0.0f - died_Particle[i].move.z) * 0.015f;


					died_Particle[i].nLife--;
					if (died_Particle[i].nLife <= 0)
					{
						died_Particle[i].bUse = false;
					}
					else
					{
						if (died_Particle[i].nLife <= 80)
						{
							died_Particle[i].material.Diffuse.r = 0.8f - (float)(80 - died_Particle[i].nLife) / 80 * 0.8f;
							died_Particle[i].material.Diffuse.g = 0.7f - (float)(80 - died_Particle[i].nLife) / 80 * 0.7f;
							died_Particle[i].material.Diffuse.b = 0.2f - (float)(80 - died_Particle[i].nLife) / 80 * 0.2f;
						}

						if (died_Particle[i].nLife <= 20)
						{
							// α値設定
							died_Particle[i].material.Diffuse.a -= 0.05f;
							if (died_Particle[i].material.Diffuse.a < 0.0f)
							{
								died_Particle[i].material.Diffuse.a = 0.0f;
							}
						}
					}
				}
			}

			int check = 0;
			for (int i = 0; i < MAX_PARTICLE_DIED; i++)
			{
				if (died_Particle[i].bUse == true)
				{
					check++;
				}
			}
			if (check == 0)
			{
				gamedata->dying = false;
				if (gamedata->gameover != true)
				{
					pPlayer[0].use = true;
				}
				pPlayer[0].pos = D3DXVECTOR3(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y, PLAYER_FIRST_POS_Z);
			}

		}
	}
}
//void UpdateRopeParticle(void)


//void UpdateBulletParticle(void)

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void) 
{
	// ライティングを無効に
	SetLightEnable(false);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// αテストを有効に
	//SetAlphaTestEnable(true);

	// Z比較無し
	SetDepthEnable(false);

	// フォグ無効
	//SetFogEnable(false);
	CAMERA *cam = GetCamera();
	//void DrawRopeParticle(void)
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		CAMERA *cam = GetCamera();

		

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_rope_VertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_ROPE; i++)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_rope_Texture[g_Rope_Particle[i].g_TexNo]);
			if (g_Rope_Particle[i].bUse)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Rope_Particle[i].g_mtxWorldParticle);

				// ビューマトリックスを取得
				mtxView = cam->mtxView;

				g_Rope_Particle[i].g_mtxWorldParticle._11 = mtxView._11;
				g_Rope_Particle[i].g_mtxWorldParticle._12 = mtxView._21;
				g_Rope_Particle[i].g_mtxWorldParticle._13 = mtxView._31;
				g_Rope_Particle[i].g_mtxWorldParticle._21 = mtxView._12;
				g_Rope_Particle[i].g_mtxWorldParticle._22 = mtxView._22;
				g_Rope_Particle[i].g_mtxWorldParticle._23 = mtxView._32;
				g_Rope_Particle[i].g_mtxWorldParticle._31 = mtxView._13;
				g_Rope_Particle[i].g_mtxWorldParticle._32 = mtxView._23;
				g_Rope_Particle[i].g_mtxWorldParticle._33 = mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, g_Rope_Particle[i].scale.x, g_Rope_Particle[i].scale.y, g_Rope_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Rope_Particle[i].g_mtxWorldParticle, &g_Rope_Particle[i].g_mtxWorldParticle, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_Rope_Particle[i].pos.x, g_Rope_Particle[i].pos.y, g_Rope_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Rope_Particle[i].g_mtxWorldParticle, &g_Rope_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ワールドマトリックスの設定
				SetWorldMatrix(&g_Rope_Particle[i].g_mtxWorldParticle);

				// マテリアル設定
				SetMaterial(g_Rope_Particle[i].material);

				// ポリゴンの描画
				GetDeviceContext()->Draw(4, 0);
			}
		}

		//// ライティングを有効に
		//SetLightEnable(true);

		//// 通常ブレンドに戻す
		//SetBlendState(BLEND_MODE_ALPHABLEND);

		//// Z比較有効
		//SetDepthEnable(true);

		//// フォグ有効
		////SetFogEnable(true);

	}

	//void DrawBulletParticle(void)
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		

		//// ライティングを無効に
		//SetLightEnable(false);

		//// 加算合成に設定
		//SetBlendState(BLEND_MODE_ADD);

		//// Z比較無し
		//SetDepthEnable(false);

		// フォグ無効
		//SetFogEnable(false);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_bullet_VertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_bullet_Texture[g_Bullet_Particle[i].g_TexNo]);
			if (g_Bullet_Particle[i].bUse)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Bullet_Particle[i].g_mtxWorldParticle);

				// ビューマトリックスを取得
				mtxView = cam->mtxView;

				g_Bullet_Particle[i].g_mtxWorldParticle._11 = mtxView._11;
				g_Bullet_Particle[i].g_mtxWorldParticle._12 = mtxView._21;
				g_Bullet_Particle[i].g_mtxWorldParticle._13 = mtxView._31;
				g_Bullet_Particle[i].g_mtxWorldParticle._21 = mtxView._12;
				g_Bullet_Particle[i].g_mtxWorldParticle._22 = mtxView._22;
				g_Bullet_Particle[i].g_mtxWorldParticle._23 = mtxView._32;
				g_Bullet_Particle[i].g_mtxWorldParticle._31 = mtxView._13;
				g_Bullet_Particle[i].g_mtxWorldParticle._32 = mtxView._23;
				g_Bullet_Particle[i].g_mtxWorldParticle._33 = mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, g_Bullet_Particle[i].scale.x, g_Bullet_Particle[i].scale.y, g_Bullet_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Bullet_Particle[i].g_mtxWorldParticle, &g_Bullet_Particle[i].g_mtxWorldParticle, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_Bullet_Particle[i].pos.x, g_Bullet_Particle[i].pos.y, g_Bullet_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Bullet_Particle[i].g_mtxWorldParticle, &g_Bullet_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ワールドマトリックスの設定
				SetWorldMatrix(&g_Bullet_Particle[i].g_mtxWorldParticle);

				// マテリアル設定
				SetMaterial(g_Bullet_Particle[i].material);

				// ポリゴンの描画
				GetDeviceContext()->Draw(4, 0);
			}
		}

	}


	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		CAMERA *cam = GetCamera();

		//// ライティングを無効に
		//SetLightEnable(false);

		//// 加算合成に設定
		//SetBlendState(BLEND_MODE_ADD);

		//// Z比較無し
		//SetDepthEnable(false);

		//// フォグ無効
		////SetFogEnable(false);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_twojump_VertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


		for (int i = 0; i < MAX_PARTICLE_TWOJUMP; i++)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_twojump_Texture[g_Twojump_Particle[i].g_TexNo]);
			if (g_Twojump_Particle[i].bUse==true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Twojump_Particle[i].g_mtxWorldParticle);

				// ビューマトリックスを取得
				mtxView = cam->mtxView;

				g_Twojump_Particle[i].g_mtxWorldParticle._11 = mtxView._11;
				g_Twojump_Particle[i].g_mtxWorldParticle._12 = mtxView._21;
				g_Twojump_Particle[i].g_mtxWorldParticle._13 = mtxView._31;
				g_Twojump_Particle[i].g_mtxWorldParticle._21 = mtxView._12;
				g_Twojump_Particle[i].g_mtxWorldParticle._22 = mtxView._22;
				g_Twojump_Particle[i].g_mtxWorldParticle._23 = mtxView._32;
				g_Twojump_Particle[i].g_mtxWorldParticle._31 = mtxView._13;
				g_Twojump_Particle[i].g_mtxWorldParticle._32 = mtxView._23;
				g_Twojump_Particle[i].g_mtxWorldParticle._33 = mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, g_Twojump_Particle[i].scale.x, g_Twojump_Particle[i].scale.y, g_Twojump_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Twojump_Particle[i].g_mtxWorldParticle, &g_Twojump_Particle[i].g_mtxWorldParticle, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_Twojump_Particle[i].pos.x, g_Twojump_Particle[i].pos.y, g_Twojump_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Twojump_Particle[i].g_mtxWorldParticle, &g_Twojump_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ワールドマトリックスの設定
				SetWorldMatrix(&g_Twojump_Particle[i].g_mtxWorldParticle);

				// マテリアル設定
				SetMaterial(g_Twojump_Particle[i].material);

				// ポリゴンの描画
				GetDeviceContext()->Draw(4, 0);
			}
		}

		//// ライティングを有効に
		//SetLightEnable(true);

		//// 通常ブレンドに戻す
		//SetBlendState(BLEND_MODE_ALPHABLEND);

		//// Z比較有効
		//SetDepthEnable(true);

		//// フォグ有効
		////SetFogEnable(true);

	}

	//死
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;


		//// ライティングを無効に
		//SetLightEnable(false);

		//// 加算合成に設定
		//SetBlendState(BLEND_MODE_ADD);

		//// Z比較無し
		//SetDepthEnable(false);

		// フォグ無効
		//SetFogEnable(false);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &died_VertexBuffer, &stride, &offset);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_DIED; i++)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &died_Texture[died_Particle[i].g_TexNo]);
			if (died_Particle[i].bUse)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&died_Particle[i].g_mtxWorldParticle);

				// ビューマトリックスを取得
				mtxView = cam->mtxView;

				died_Particle[i].g_mtxWorldParticle._11 = mtxView._11;
				died_Particle[i].g_mtxWorldParticle._12 = mtxView._21;
				died_Particle[i].g_mtxWorldParticle._13 = mtxView._31;
				died_Particle[i].g_mtxWorldParticle._21 = mtxView._12;
				died_Particle[i].g_mtxWorldParticle._22 = mtxView._22;
				died_Particle[i].g_mtxWorldParticle._23 = mtxView._32;
				died_Particle[i].g_mtxWorldParticle._31 = mtxView._13;
				died_Particle[i].g_mtxWorldParticle._32 = mtxView._23;
				died_Particle[i].g_mtxWorldParticle._33 = mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, died_Particle[i].scale.x, died_Particle[i].scale.y, died_Particle[i].scale.z);
				D3DXMatrixMultiply(&died_Particle[i].g_mtxWorldParticle, &died_Particle[i].g_mtxWorldParticle, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, died_Particle[i].pos.x, died_Particle[i].pos.y, died_Particle[i].pos.z);
				D3DXMatrixMultiply(&died_Particle[i].g_mtxWorldParticle, &died_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ワールドマトリックスの設定
				SetWorldMatrix(&died_Particle[i].g_mtxWorldParticle);

				// マテリアル設定
				SetMaterial(died_Particle[i].material);

				// ポリゴンの描画
				GetDeviceContext()->Draw(4, 0);
			}
		}

	}

	// ライティングを有効に
	SetLightEnable(true);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(true);

	// αテストを無効に
	//SetAlphaTestEnable(false);

	// フォグ有効
	SetFogEnable(true);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeRopeVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_rope_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_rope_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_rope_VertexBuffer, 0);
	}

	return S_OK;
}

HRESULT MakeBulletVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_bullet_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_bullet_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_BULLET_SIZE_X / 2, PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_BULLET_SIZE_X / 2, PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_BULLET_SIZE_X / 2, -PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_BULLET_SIZE_X / 2, -PARTICLE_BULLET_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_bullet_VertexBuffer, 0);
	}

	return S_OK;
}

HRESULT MakeTwojumpVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_twojump_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_twojump_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_TWOJUMP_SIZE_X / 2, PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_TWOJUMP_SIZE_X / 2, PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_TWOJUMP_SIZE_X / 2, -PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_TWOJUMP_SIZE_X / 2, -PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_twojump_VertexBuffer, 0);
	}

	return S_OK;
}

HRESULT MakeDiedVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &died_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(died_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_DIED_SIZE_X / 2, PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_DIED_SIZE_X / 2, PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_DIED_SIZE_X / 2, -PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_DIED_SIZE_X / 2, -PARTICLE_DIED_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(died_VertexBuffer, 0);
	}

	return S_OK;
}
//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	g_Rope_Particle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetBulletParticle(D3DXVECTOR3 bulletpos)
{
	int nIdxParticle = -1;
	
	for(int i = 0; i < MAX_PARTICLE_BULLET; i++)
	{
		if(!g_Bullet_Particle[i].bUse)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;
			pos = bulletpos;
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.x = sinf(fAngle) * fLength;

			move.z = cosf(fAngle) * fLength;
			nLife = rand() % 50;
			fSize = (float)(rand() % 30 + 20);
			if (i & 1) {

				move.y = rand() % 300 / 100.0f;
			}
			else {
				move.y = -(rand() % 300 / 100.0f);
			}
			//pos.y = fSize / 2;

			g_Bullet_Particle[i].pos = pos;
			g_Bullet_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet_Particle[i].move = move;
			g_Bullet_Particle[i].material.Diffuse = D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f);
			g_Bullet_Particle[i].nLife = nLife;
			g_Bullet_Particle[i].bUse = true;

			nIdxParticle = i;

			break;
		}
	}

	return nIdxParticle;
}

int SetDieParticle(D3DXVECTOR3 diedpos)
{
	int nIdxParticle = -1;

	for (int i = 0; i < MAX_PARTICLE_DIED; i++)
	{
		if (!died_Particle[i].bUse)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;
			pos = diedpos;
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.x = sinf(fAngle) * fLength;

			move.z = cosf(fAngle) * fLength;
			nLife = rand() % 300;
			fSize = (float)(rand() % 30 + 20);
			if (i & 1) {

				move.y = rand() % 300 / 100.0f;
			}
			else {
				move.y = -(rand() % 300 / 100.0f);
			}
			//pos.y = fSize / 2;

			died_Particle[i].pos = pos;
			died_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			died_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			died_Particle[i].move = move;
			died_Particle[i].material.Diffuse = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
			died_Particle[i].nLife = nLife;
			died_Particle[i].bUse = true;

			nIdxParticle = i;

			break;
		}
	}

	return nIdxParticle;
}

//int SetBulletParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
//{
//	int nIdxParticle = -1;
//
//	for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
//	{
//		if (!g_Bullet_Particle[i].bUse)
//		{
//			g_Bullet_Particle[i].pos = pos;
//			g_Bullet_Particle[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_Bullet_Particle[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//			g_Bullet_Particle[i].move = move;
//			g_Bullet_Particle[i].material.Diffuse = col;
//			g_Bullet_Particle[i].fSizeX = fSizeX;
//			g_Bullet_Particle[i].fSizeY = fSizeY;
//			g_Bullet_Particle[i].nLife = nLife;
//			g_Bullet_Particle[i].bUse = true;
//
//			nIdxParticle = i;
//
//#ifdef DISP_SHADOW
//			// 影の設定
//			g_Bullet_Particle[i].nIdxShadow = CreateShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
//			if (g_Bullet_Particle[i].nIdxShadow != -1)
//			{
//				SetColorShadow(g_Bullet_Particle[i].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
//			}
//#endif
//
//			break;
//		}
//	}
//
//	return nIdxParticle;
//}


//=============================================================================
// 弾の取得
//=============================================================================
PARTICLE *GetTwojumpParticle(void)
{
	return &(g_Twojump_Particle[0]);
}
					