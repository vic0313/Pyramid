//=============================================================================
//
// 地図の処理 [meshMap.cpp]
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
#include "Map.h"
#include "MapTransparent.h"
#include "meshMapReverse.h"
#include "particle.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FIELD_MAX			(1)				// テクスチャの数
#define TEXTURE_WALL_MAX			(2)						// テクスチャの数
#define TEXTURE_CEILING_MAX 		(1)				// テクスチャの数
#define TEXTURE_SLOPE_MAX			(1)				// テクスチャの数
#define TEXTURE_SLOPEWALL_MAX		(1)		// テクスチャの数
#define TEXTURE_BOSSFIELD_MAX		(1)		// テクスチャの数
#define TEXTURE_RANDOM_FIELD_MAX	(2)		// テクスチャの数
#define TEXTURE_TRIANGLE_MAX	(1)
#define	VALUE_MOVE_FIELD		(5.0f)					// 移動速度
#define	VALUE_ROTATE_FIELD	(D3DX_PI * 0.001f)		// 回転速度
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER  *player = GetPlayer();
static ENEMY   *enemytypeA = GetEnemyTypeA();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
static OBJECT  *bosskey = GetKeyObject();
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_FIELD_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;	// テクスチャ番号
static ID3D11ShaderResourceView		*g_Texture_Wall[TEXTURE_WALL_MAX] = { NULL };	// テクスチャ情報
static int							g_TexWallNo;		// テクスチャ番号
static ID3D11ShaderResourceView		*g_Texture_Ceiling[TEXTURE_CEILING_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo_Ceiling;	// テクスチャ番号
static ID3D11ShaderResourceView		*g_Texture_Slope[TEXTURE_SLOPE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo_Slope;	// テクスチャ番号
static ID3D11ShaderResourceView	*g_Texture_SlopeWall[TEXTURE_SLOPEWALL_MAX] = { NULL };	// テクスチャ情報
static int						g_TexNo_SlopeWall;	// テクスチャ番号
static ID3D11ShaderResourceView	*g_Texture_BOSSField[TEXTURE_BOSSFIELD_MAX] = { NULL };	// テクスチャ情報
static int						g_TexNo_BOSSField;	// テクスチャ番号
static ID3D11ShaderResourceView	*g_Texture_RandomField[TEXTURE_RANDOM_FIELD_MAX] = { NULL };	// テクスチャ情報
static int						g_TexNo_RandomField;	// テクスチャ番号
static ID3D11ShaderResourceView	*g_Texture_TriangleWall[TEXTURE_TRIANGLE_MAX] = { NULL };	// テクスチャ情報
static int						g_TexNo_TriangleWall;	// テクスチャ番号



static MESH_MAP g_aMeshField[MAX_MESH_FIELD];		// メッシュ地面ワーク
static int g_nNumMeshField = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshWall[MAX_MESH_WALL];		// メッシュ壁ワーク
static int g_nNumMeshWall = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldCeiling[MAX_MESH_FIELD_CEILING];		// メッシュ地面ワーク
static int g_nNumMeshCeiling = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldSlope[MAX_MESH_FIELD_SLOPE];		// メッシュ地面ワーク
static int g_nNumMeshSlope = 0;						// メッシュ壁の数
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// メッシュ壁の数
static MESH_MAP g_BOSSField[MAX_MESH_BOSS_FIELD];
static int g_nNumBOSSField = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshRandomField[MAX_MESH_RANDOM_FIELD];		// メッシュ地面ワーク
static int g_nNumMeshRandomField = 0;						// メッシュ壁の数
static MESH_MAP g_Trianglewall[MAX_MESH_TRIANGLE_WALL];
static int g_nNumMeshTriangleWall = 0;						// メッシュ壁の数


static char* g_TextureName[] = {
	"data/TEXTURE/wall004.jpg",
};
static char* g_TextureWallName[] = {
	"data/TEXTURE/wall000.jpg",
	"data/TEXTURE/wall005.jpg",
};
static char* g_TextureCeilingName[] = {
	"data/TEXTURE/wall004.jpg",
};
static char* g_TextureSlopeName[] = {
	"data/TEXTURE/field004.jpg",
};
static char *g_TexturSlopeWallName[] = {
	"data/TEXTURE/030.jpg",
};
static char *g_TexturBOSSFieldName[] = {
	"data/TEXTURE/field006.jpg",
};
static char *g_TexturRandomFieldName[] = {
	"data/TEXTURE/field007.jpg",
	"data/TEXTURE/field008.jpg",
};
static char* g_TextureTriangleWallName[] = {
	"data/TEXTURE/ttaerhtecx.jpg",
};

// BOSS エリアの波の処理
static VERTEX_3D	*g_Vertex = NULL;

static D3DXVECTOR3	g_Center;					// 波の発生場所
static float		g_Time = 0.0f;				// 波の経過時間
static float		g_wave_frequency = 3.0f;	// 波の周波数
static float		g_wave_correction = 0.02f;	// 波の距離補正
static float		g_wave_amplitude = BOSS_WAVE_AMPLITUDE;	// 波の振幅
//static bool			namidause=false;

static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static MAP_POINT	*g_ceilingpoint = GetCeilingPoint();
static MAP_POINT	*g_wallpoint = GetWallPoint();
static MAP_POINT *g_slopewallpoint = GetSlopeWallPoint();
static MAP_POINT	*g_fieldslopepoint = GetFieldSlopePoint();

//カメラの注視点の線形移動用の移動座標テーブル
//static D3DXVECTOR3 g_Transparent_pos[MAX_Transparent];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY)
{
	MESH_MAP *pMesh_Field;

	if (g_nNumMeshField >= MAX_MESH_FIELD)
	{
		return E_FAIL;
	}


	// テクスチャ生成
	for (int i = 0; i < TEXTURE_FIELD_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	pMesh_Field = &g_aMeshField[g_nNumMeshField];
	g_nNumMeshField++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_Field->material, sizeof(pMesh_Field->material));
	pMesh_Field->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_Field->pos = pos;

	pMesh_Field->rot = rot;


	// ブロック数の設定
	pMesh_Field->nNumBlockX = nNumBlockX;
	pMesh_Field->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_Field->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_Field->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_Field->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_Field->fBlockSizeX = fBlockSizeX;
	pMesh_Field->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Field->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Field->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Field->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Field->indexBuffer);


	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Field->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_Field->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Field->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_Field->fBlockSizeX; //+(pMesh_Field->nNumBlockX) * pMesh_Field->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.y = startY;
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_Field->fBlockSizeZ; //+pMesh_Field->nNumBlockZ * pMesh_Field->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Field->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Field->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_Field->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_Field->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Field->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_Field->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_Field->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + pMesh_Field->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Field->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ)
{
	MESH_MAP *pMesh_Wall;

	if (g_nNumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_WALL_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureWallName[i],
			NULL,
			NULL,
			&g_Texture_Wall[i],
			NULL);
	}
	g_TexWallNo = 0;
	
	pMesh_Wall = &g_aMeshWall[g_nNumMeshWall];

	g_nNumMeshWall++;

	pMesh_Wall->use = true;
	// マテリアル情報の初期化
	ZeroMemory(&pMesh_Wall->material, sizeof(pMesh_Wall->material));
	pMesh_Wall->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_Wall->pos = pos;

	pMesh_Wall->rot = rot;

	// ブロック数の設定
	pMesh_Wall->nNumBlockX = nNumBlockX;
	pMesh_Wall->nNumBlockY = nNumBlockY;

	// 頂点数の設定
	pMesh_Wall->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// インデックス数の設定
	pMesh_Wall->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// ポリゴン数の設定
	pMesh_Wall->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// ブロックサイズの設定
	pMesh_Wall->fBlockSizeX = fBlockSizeX;
	pMesh_Wall->fBlockSizeY = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Wall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Wall->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Wall->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Wall->indexBuffer);

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Wall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxY = 0; nCntVtxY < (pMesh_Wall->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Wall->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh_Wall->nNumBlockX / 2.0f) * pMesh_Wall->fBlockSizeX + nCntVtxX * pMesh_Wall->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh_Wall->nNumBlockY * pMesh_Wall->fBlockSizeY - nCntVtxY * pMesh_Wall->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// 法線の設定
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Wall->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Wall->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh_Wall->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh_Wall->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Wall->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxY < (pMesh_Wall->nNumBlockY - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + pMesh_Wall->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Wall->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshFieldCeiling(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ)
{
	MESH_MAP *pMesh_FieldCeiling;

	if (g_nNumMeshCeiling >= MAX_MESH_FIELD_CEILING)
	{
		return E_FAIL;
	}


	// テクスチャ生成
	for (int i = 0; i < TEXTURE_CEILING_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureCeilingName[i],
			NULL,
			NULL,
			&g_Texture_Ceiling[i],
			NULL);
	}

	g_TexNo_Ceiling = 0;

	pMesh_FieldCeiling = &g_aMeshFieldCeiling[g_nNumMeshCeiling];
	g_nNumMeshCeiling++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_FieldCeiling->material, sizeof(pMesh_FieldCeiling->material));
	pMesh_FieldCeiling->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_FieldCeiling->pos = pos;

	pMesh_FieldCeiling->rot = rot;


	// ブロック数の設定
	pMesh_FieldCeiling->nNumBlockX = nNumBlockX;
	pMesh_FieldCeiling->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_FieldCeiling->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_FieldCeiling->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_FieldCeiling->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_FieldCeiling->fBlockSizeX = fBlockSizeX;
	pMesh_FieldCeiling->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldCeiling->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeiling->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldCeiling->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeiling->indexBuffer);


	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeiling->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldCeiling->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeiling->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.x = startX - nCntVtxX * pMesh_FieldCeiling->fBlockSizeX; //+(pMesh_FieldCeiling->nNumBlockX) * pMesh_FieldCeiling->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldCeiling->fBlockSizeZ; //+pMesh_FieldCeiling->nNumBlockZ * pMesh_FieldCeiling->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeiling->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeiling->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldCeiling->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldCeiling->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeiling->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_FieldCeiling->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + pMesh_FieldCeiling->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeiling->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshSlope(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ,
	int startX, int startZ, int startY, int endY, int slopecheck)
{

	MESH_MAP *pMesh_Slope;

	if (g_nNumMeshSlope >= MAX_MESH_FIELD_SLOPE)
	{
		return E_FAIL;
	}


	// テクスチャ生成
	for (int i = 0; i < TEXTURE_SLOPE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureSlopeName[i],
			NULL,
			NULL,
			&g_Texture_Slope[i],
			NULL);
	}

	g_TexNo_Slope = 0;

	pMesh_Slope = &g_aMeshFieldSlope[g_nNumMeshSlope];
	g_nNumMeshSlope++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_Slope->material, sizeof(pMesh_Slope->material));
	pMesh_Slope->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_Slope->pos = pos;

	pMesh_Slope->rot = rot;

	pMesh_Slope->yblock = endY - startY;
	pMesh_Slope->fBlockSizeY_X = float((pMesh_Slope->yblock + 1.0) / nNumBlockX);
	pMesh_Slope->fBlockSizeY_Z = float((pMesh_Slope->yblock + 1.0) / nNumBlockZ);

	// ブロック数の設定
	pMesh_Slope->nNumBlockX = nNumBlockX;
	pMesh_Slope->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_Slope->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_Slope->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_Slope->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_Slope->fBlockSizeX = fBlockSizeX;
	pMesh_Slope->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Slope->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Slope->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Slope->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Slope->indexBuffer);


	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Slope->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;


		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_Slope->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Slope->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_Slope->fBlockSizeX; //+(pMesh_Slope->nNumBlockX) * pMesh_Slope->fBlockSizeX
					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
				switch (int check = slopecheck)
				{
				case 1:
					pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.y = startY + nCntVtxX * pMesh_Slope->fBlockSizeY_X ;
					break;
				case 2:
					pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.y = endY - nCntVtxX * pMesh_Slope->fBlockSizeY_X ;
					break;
				case 3:
					pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.y = endY - nCntVtxZ * pMesh_Slope->fBlockSizeY_X ;
					break;
				case 4:
					pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.y = startY + nCntVtxZ * pMesh_Slope->fBlockSizeY_X ;
					break;
				default:
					break;
				}

				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_Slope->fBlockSizeZ; //+pMesh_Slope->nNumBlockZ * pMesh_Slope->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Slope->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Slope->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_Slope->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_Slope->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Slope->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_Slope->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + pMesh_Slope->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Slope->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitSlopeWall(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	MESH_MAP *pMesh_SlopeWall;

	if (g_nNumMeshSlopeWall >= MAX_MESH_SLOPE_WALL)
	{
		return E_FAIL;
	}

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_SLOPEWALL_MAX; i++)
	{
		g_Texture_SlopeWall[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturSlopeWallName[i],
			NULL,
			NULL,
			&g_Texture_SlopeWall[i],
			NULL);
	}
	g_TexNo_SlopeWall = 0;

	pMesh_SlopeWall = &g_slopewall[g_nNumMeshSlopeWall];
	g_nNumMeshSlopeWall++;

	// ポリゴン表示位置の中心座標を設定
	pMesh_SlopeWall->pos = possize;

	pMesh_SlopeWall->rot = rot;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_SlopeWall->material, sizeof(pMesh_SlopeWall->material));
	pMesh_SlopeWall->material.Diffuse = col;

	// ブロック数の設定
	pMesh_SlopeWall->nNumBlockX = 1;
	pMesh_SlopeWall->nNumBlockZ = 1;

	// 頂点数の設定
	pMesh_SlopeWall->nNumVertex = 4;

	// インデックス数の設定
	pMesh_SlopeWall->nNumVertexIndex = 4;

	// ポリゴン数の設定
	pMesh_SlopeWall->nNumPolygon = 2;



	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeWall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeWall->vertexBuffer);
	//// インデックスバッファ生成
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	//bd.ByteWidth = sizeof(unsigned short) * g_slopewall[g_nNumMeshSlopeWall].nNumVertexIndex;
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//GetDevice()->CreateBuffer(&bd, NULL, &g_slopewall[g_nNumMeshSlopeWall].indexBuffer);

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif
		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_SlopeWall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// 頂点座標の設定
		// 頂点０番（左上の頂点）
		pVtx[0].Position = pos0 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点１番（右上の頂点）
		pVtx[1].Position = pos1 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点２番（左下の頂点）
		pVtx[2].Position = pos2 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点３番（右下の頂点）
		pVtx[3].Position = pos3 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		GetDeviceContext()->Unmap(pMesh_SlopeWall->vertexBuffer, 0);

	}

	return S_OK;
}

HRESULT InitBOSSField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	MESH_MAP *pg_BOSSField;

	if (g_nNumBOSSField >= MAX_MESH_BOSS_FIELD)
	{
		return E_FAIL;
	}
	

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_BOSSFIELD_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturBOSSFieldName[i],
			NULL,
			NULL,
			&g_Texture_BOSSField[i],
			NULL);
	}

	g_TexNo_BOSSField = 0;

	pg_BOSSField = &g_BOSSField[g_nNumBOSSField];
	g_nNumBOSSField++;

	// マテリアル情報の初期化
	ZeroMemory(&pg_BOSSField->material, sizeof(pg_BOSSField->material));
	pg_BOSSField->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pg_BOSSField->pos = pos;

	pg_BOSSField->rot = rot;
	
	// ブロック数の設定
	pg_BOSSField->nNumBlockX = nNumBlockX;
	pg_BOSSField->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pg_BOSSField->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pg_BOSSField->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pg_BOSSField->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pg_BOSSField->fBlockSizeX = fBlockSizeX;
	pg_BOSSField->fBlockSizeZ = fBlockSizeZ;
	
	

	// 頂点情報をメモリに作っておく（波の為）
	// 波の処理
	//  　波の高さ　= sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
	g_Vertex = new VERTEX_3D[pg_BOSSField->nNumVertex];
	g_Center = D3DXVECTOR3(0.0f, 0.0f, 900.0f);	// 波の発生場所
	g_Time = 0.0f;								// 波の経過時間(+と-とで内側外側になる)
	g_wave_frequency = 2.0f;					// 波の周波数(上下運動の速さ)
	g_wave_correction = 0.02f;					// 波の距離補正(変えなくても良いと思う)
	g_wave_amplitude = BOSS_WAVE_AMPLITUDE;					// 波の振幅(波の高さ)

	for (int nCntVtxZ = 0; nCntVtxZ < (pg_BOSSField->nNumBlockZ + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (pg_BOSSField->nNumBlockX + 1); nCntVtxX++)
		{
			// 頂点座標の設定
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.x = -(pg_BOSSField->nNumBlockX / 2.0f) * pg_BOSSField->fBlockSizeX + nCntVtxX * pg_BOSSField->fBlockSizeX;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.z = 900.0f+(pg_BOSSField->nNumBlockZ / 2.0f) * pg_BOSSField->fBlockSizeZ - nCntVtxZ * pg_BOSSField->fBlockSizeZ;

			float dx = g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.x - g_Center.x;
			float dz = g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.z - g_Center.z;

			// 波紋の中心点からの距離を得る
			float len = (float)sqrt(dx * dx + dz * dz);

			// 波の高さを、sin関数で得る
		//  　波の高さ　= sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
			//g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
			// 法線の設定
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 拡散光の設定
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			float texSizeX = 1.0f;
			float texSizeZ = 1.0f;
			// テクスチャ座標の設定
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
		}
	}

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif



		

		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * pg_BOSSField->nNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &pg_BOSSField->vertexBuffer);

		// インデックスバッファ生成
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(unsigned short) * pg_BOSSField->nNumVertexIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &pg_BOSSField->indexBuffer);


		{
			// 頂点バッファへのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			GetDeviceContext()->Map(pg_BOSSField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*pg_BOSSField->nNumVertex);
			GetDeviceContext()->Unmap(pg_BOSSField->vertexBuffer, 0);
		}

		{//インデックスバッファの中身を埋める
			// インデックスバッファのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			GetDeviceContext()->Map(pg_BOSSField->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			unsigned short *pIdx = (unsigned short*)msr.pData;

			int nCntIdx = 0;
			for (int nCntVtxZ = 0; nCntVtxZ < pg_BOSSField->nNumBlockZ; nCntVtxZ++)
			{
				if (nCntVtxZ > 0)
				{// 縮退ポリゴンのためのダブりの設定
					pIdx[nCntIdx] = (nCntVtxZ + 1) * (pg_BOSSField->nNumBlockX + 1);
					nCntIdx++;
				}

				for (int nCntVtxX = 0; nCntVtxX < (pg_BOSSField->nNumBlockX + 1); nCntVtxX++)
				{
					pIdx[nCntIdx] = (nCntVtxZ + 1) * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX;
					nCntIdx++;
					pIdx[nCntIdx] = nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX;
					nCntIdx++;
				}

				if (nCntVtxZ < (pg_BOSSField->nNumBlockZ - 1))
				{// 縮退ポリゴンのためのダブりの設定
					pIdx[nCntIdx] = nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + pg_BOSSField->nNumBlockX;
					nCntIdx++;
				}
			}

			GetDeviceContext()->Unmap(pg_BOSSField->indexBuffer, 0);
		}
	}
	return S_OK;
}

HRESULT InitMeshField_Random(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY,int texnum)
{
	MESH_MAP *pMesh_RandomField;

	if (g_nNumMeshRandomField >= MAX_MESH_RANDOM_FIELD)
	{
		return E_FAIL;
	}


	// テクスチャ生成
	for (int i = 0; i < TEXTURE_RANDOM_FIELD_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturRandomFieldName[i],
			NULL,
			NULL,
			&g_Texture_RandomField[i],
			NULL);
	}

	g_TexNo_RandomField = 0;

	pMesh_RandomField = &g_aMeshRandomField[g_nNumMeshRandomField];
	g_nNumMeshRandomField++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_RandomField->material, sizeof(pMesh_RandomField->material));
	pMesh_RandomField->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_RandomField->pos = pos;

	pMesh_RandomField->rot = rot;

	pMesh_RandomField->texnum = texnum;
	// ブロック数の設定
	pMesh_RandomField->nNumBlockX = nNumBlockX;
	pMesh_RandomField->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_RandomField->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_RandomField->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_RandomField->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_RandomField->fBlockSizeX = fBlockSizeX;
	pMesh_RandomField->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_RandomField->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_RandomField->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_RandomField->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_RandomField->indexBuffer);


	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_RandomField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_RandomField->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_RandomField->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_RandomField->fBlockSizeX; //+(pMesh_RandomField->nNumBlockX) * pMesh_RandomField->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.y = startY+ (rand() % 200) / 100.0f * 30.0f;
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_RandomField->fBlockSizeZ; //+pMesh_RandomField->nNumBlockZ * pMesh_RandomField->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_RandomField->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_RandomField->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_RandomField->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_RandomField->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_RandomField->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_RandomField->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + pMesh_RandomField->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_RandomField->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitTriangleWall(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	MESH_MAP *pMesh_TriangleWall;

	if (g_nNumMeshTriangleWall >= MAX_MESH_TRIANGLE_WALL)
	{
		return E_FAIL;
	}

	//テクスチャ生成
	for (int i = 0; i < 1; i++)
	{
		g_Texture_TriangleWall[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureTriangleWallName[i],
			NULL,
			NULL,
			&g_Texture_TriangleWall[i],
			NULL);
	}
	g_TexNo_TriangleWall = 0;

	pMesh_TriangleWall = &g_Trianglewall[g_nNumMeshTriangleWall];
	g_nNumMeshTriangleWall++;

	// ポリゴン表示位置の中心座標を設定
	pMesh_TriangleWall->pos = possize;

	pMesh_TriangleWall->rot = rot;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_TriangleWall->material, sizeof(pMesh_TriangleWall->material));
	pMesh_TriangleWall->material.Diffuse = col;

	// ブロック数の設定
	pMesh_TriangleWall->nNumBlockX = 1;
	pMesh_TriangleWall->nNumBlockZ = 1;

	// 頂点数の設定
	pMesh_TriangleWall->nNumVertex = 4;

	// インデックス数の設定
	pMesh_TriangleWall->nNumVertexIndex = 4;

	// ポリゴン数の設定
	pMesh_TriangleWall->nNumPolygon = 2;



	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_TriangleWall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_TriangleWall->vertexBuffer);
	//// インデックスバッファ生成
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	//bd.ByteWidth = sizeof(unsigned short) * g_slopewall[g_nNumMeshSlopeWall].nNumVertexIndex;
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//GetDevice()->CreateBuffer(&bd, NULL, &g_slopewall[g_nNumMeshSlopeWall].indexBuffer);

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif
		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_TriangleWall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// 頂点座標の設定
		// 頂点０番（左上の頂点）
		pVtx[0].Position = pos0 ;
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点１番（右上の頂点）
		pVtx[1].Position = pos1 ;
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点２番（左下の頂点）
		pVtx[2].Position = pos2;
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点３番（右下の頂点）
		pVtx[3].Position = pos3 ;
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		GetDeviceContext()->Unmap(pMesh_TriangleWall->vertexBuffer, 0);

	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{

	MESH_MAP *pMesh_Field;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh_Field = &g_aMeshField[nCntMeshField];

		if (pMesh_Field->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_Field->vertexBuffer->Release();
			pMesh_Field->vertexBuffer = NULL;
		}

		if (pMesh_Field->indexBuffer)
		{// インデックスバッファの解放
			pMesh_Field->indexBuffer->Release();
			pMesh_Field->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_FIELD_MAX; i++)
	{
		if (g_Texture[i])	g_Texture[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshField = 0;
}

void UninitMeshWall(void)
{
	MESH_MAP *pMesh_Wall;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh_Wall = &g_aMeshWall[nCntMeshField];

		if (pMesh_Wall->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_Wall->vertexBuffer->Release();
			pMesh_Wall->vertexBuffer = NULL;
		}

		if (pMesh_Wall->indexBuffer)
		{// インデックスバッファの解放
			pMesh_Wall->indexBuffer->Release();
			pMesh_Wall->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_WALL_MAX; i++)
	{
		if (g_Texture_Wall[i])	g_Texture_Wall[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshWall = 0;
}

void UninitMeshFieldCeiling(void)
{

	MESH_MAP *pMesh_FieldCeiling;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshCeiling; nCntMeshField++)
	{
		pMesh_FieldCeiling = &g_aMeshFieldCeiling[nCntMeshField];

		if (pMesh_FieldCeiling->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_FieldCeiling->vertexBuffer->Release();
			pMesh_FieldCeiling->vertexBuffer = NULL;
		}

		if (pMesh_FieldCeiling->indexBuffer)
		{// インデックスバッファの解放
			pMesh_FieldCeiling->indexBuffer->Release();
			pMesh_FieldCeiling->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_CEILING_MAX; i++)
	{
		if (g_Texture_Ceiling[i])	g_Texture_Ceiling[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshCeiling = 0;
}

void UninitMeshSlope(void)
{

	MESH_MAP *pMesh_Slope;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshSlope; nCntMeshField++)
	{
		pMesh_Slope = &g_aMeshFieldSlope[nCntMeshField];

		if (pMesh_Slope->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_Slope->vertexBuffer->Release();
			pMesh_Slope->vertexBuffer = NULL;
		}

		if (pMesh_Slope->indexBuffer)
		{// インデックスバッファの解放
			pMesh_Slope->indexBuffer->Release();
			pMesh_Slope->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_SLOPE_MAX; i++)
	{
		if (g_Texture_Slope[i])	g_Texture_Slope[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshSlope = 0;
}

void UninitSlopeWall(void)
{

	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshSlopeWall; nCntMeshField++)
	{
		g_slopewall[nCntMeshField];

		if (g_slopewall[nCntMeshField].vertexBuffer)
		{// 頂点バッファの解放
			g_slopewall[nCntMeshField].vertexBuffer->Release();
			g_slopewall[nCntMeshField].vertexBuffer = NULL;
		}

		//if (g_slopewall[nCntMeshField].indexBuffer)
		//{// インデックスバッファの解放
		//	g_slopewall[nCntMeshField].indexBuffer->Release();
		//	g_slopewall[nCntMeshField].indexBuffer = NULL;
		//}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_SLOPEWALL_MAX; i++)
	{
		if (g_Texture_SlopeWall[i])	g_Texture_SlopeWall[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshSlopeWall = 0;
}

void UninitBOSSField(void)
{
	MESH_MAP *pg_BOSSField;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumBOSSField; nCntMeshField++)
	{
		pg_BOSSField = &g_BOSSField[nCntMeshField];

		if (pg_BOSSField->vertexBuffer)
		{// 頂点バッファの解放
			pg_BOSSField->vertexBuffer->Release();
			pg_BOSSField->vertexBuffer = NULL;
		}

		if (pg_BOSSField->indexBuffer)
		{// インデックスバッファの解放
			pg_BOSSField->indexBuffer->Release();
			pg_BOSSField->indexBuffer = NULL;
		}
		if (g_Vertex)
		{
			delete[] g_Vertex;
			g_Vertex = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_BOSSFIELD_MAX; i++)
	{
		if (g_Texture_BOSSField[i])	g_Texture_BOSSField[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumBOSSField = 0;
	
	

}

void UninitMeshField_Random(void)
{

	MESH_MAP *pMesh_RandomField;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshRandomField; nCntMeshField++)
	{
		pMesh_RandomField = &g_aMeshRandomField[nCntMeshField];

		if (pMesh_RandomField->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_RandomField->vertexBuffer->Release();
			pMesh_RandomField->vertexBuffer = NULL;
		}

		if (pMesh_RandomField->indexBuffer)
		{// インデックスバッファの解放
			pMesh_RandomField->indexBuffer->Release();
			pMesh_RandomField->indexBuffer = NULL;
		}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_RANDOM_FIELD_MAX; i++)
	{
		if (g_Texture_RandomField[i])	g_Texture_RandomField[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshRandomField = 0;
}

void UninitTriangleWall(void)
{

	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshTriangleWall; nCntMeshField++)
	{
		g_Trianglewall[g_nNumMeshTriangleWall];

		if (g_Trianglewall[nCntMeshField].vertexBuffer)
		{// 頂点バッファの解放
			g_Trianglewall[nCntMeshField].vertexBuffer->Release();
			g_Trianglewall[nCntMeshField].vertexBuffer = NULL;
		}

		//if (g_slopewall[nCntMeshField].indexBuffer)
		//{// インデックスバッファの解放
		//	g_slopewall[nCntMeshField].indexBuffer->Release();
		//	g_slopewall[nCntMeshField].indexBuffer = NULL;
		//}
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_SLOPEWALL_MAX; i++)
	{
		if (g_Texture_TriangleWall[i])	g_Texture_TriangleWall[i]->Release();
	}

	//読み込み数をリセットする
	g_nNumMeshTriangleWall = 0;
}


//=============================================================================
// 更新処理
//=============================================================================

void UpdateMeshField(void)
{
	//static MODE mode = GetMode();
	/*switch (mode)
	{
	case MODE_TITLE:
	{

	}
	break;
	case MODE_RESTAREA:
	{

	}
	break;
	case MODE_BATTLEAREA:
	{

	}
	break;
	case MODE_RESULT:
	{

	}
	break;*/
	//if (gamedata->modenow == MODE_BATTLEAREA)
	//{
	//	for (int j = 0; j < MAX_MESH_FIELD; j++)
	//	{
	//		D3DXVECTOR3 HitPosition;		//交点
	//		D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//		D3DXVECTOR3 playerpos;
	//		D3DXVECTOR3 camerapos = camera->pos;
	//		int num = j;
	//		for (int i = 0; i < MAX_PLAYER; i++) {
	//			playerpos = player[i].pos;
	//		}
	//		bool ans = CameraRayTouch_Field(playerpos, camerapos, &HitPosition, &Normal, &num);
	//		if (ans) {
	//			g_aMeshField[num].material.Diffuse.a = 0.25f;
	//		}
	//		else
	//		{
	//			g_aMeshField[num].material.Diffuse.a = 1.0f;
	//		}
	//	}
	//}
	
}
void UpdateMeshWall(void)
{
	//static MODE mode = GetMode();
	if (gamedata->modenow == MODE_BATTLEAREA)
	{
		if(gamedata->getkey==MAX_KEY)
		{
			g_aMeshWall[152].use = false;
		}

		//for (int j = 19; j < MAX_MESH_WALL; j++)
		//{
		//	if (j == 152)break;
		//	D3DXVECTOR3 HitPosition;		//交点
		//	D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
		//	D3DXVECTOR3 playerpos;
		//	D3DXVECTOR3 camerapos = camera->pos;
		//	int numw = j;
		//	for (int i = 0; i < MAX_PLAYER; i++) {
		//		playerpos = player[i].pos;
		//	}
		//	bool ans = CameraRayTouch_Wall(playerpos, camerapos, &HitPosition, &Normal, &numw);
		//	if (ans) {
		//		g_aMeshWall[numw].material.Diffuse.a = 0.25f;
		//	}
		//	else
		//	{
		//		g_aMeshWall[numw].material.Diffuse.a = 1.0f;
		//	}
		//}
	}
}
void UpdateMeshFieldCeiling(void)
{
	////static MODE mode = GetMode();
	//if (gamedata->modenow == MODE_BATTLEAREA)
	//{
	//	for (int j = 0; j < MAX_MESH_FIELD_CEILING; j++)
	//	{
	//		D3DXVECTOR3 HitPosition;		//交点
	//		D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//		D3DXVECTOR3 playerpos;
	//		D3DXVECTOR3 camerapos = camera->pos;
	//		int numc = j;
	//		for (int i = 0; i < MAX_PLAYER; i++) {
	//			playerpos = player[i].pos;
	//		}
	//		bool ans = CameraRayTouch_Ceiling(playerpos, camerapos, &HitPosition, &Normal, &numc);
	//		if (ans) {
	//			g_aMeshFieldCeiling[numc].material.Diffuse.a = 0.25f;
	//		}
	//		else
	//		{
	//			g_aMeshFieldCeiling[numc].material.Diffuse.a = 1.0f;
	//		}
	//	}
	//}
}

void UpdateMeshSlope(void)
{
	////static MODE mode = GetMode();
	//if (gamedata->modenow == MODE_BATTLEAREA)
	//{
	//	for (int j = 0; j < MAX_MESH_FIELD_SLOPE; j++)
	//	{
	//		D3DXVECTOR3 HitPosition;		//交点
	//		D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//		D3DXVECTOR3 playerpos;
	//		D3DXVECTOR3 camerapos = camera->pos;
	//		int nums = j;
	//		for (int i = 0; i < MAX_PLAYER; i++) {
	//			playerpos = player[i].pos;
	//		}
	//		bool ans = CameraRayTouch_Slope(playerpos, camerapos, &HitPosition, &Normal, &nums);
	//		if (ans) {
	//			g_aMeshFieldSlope[nums].material.Diffuse.a = 0.25f;
	//		}
	//		else
	//		{
	//			g_aMeshFieldSlope[nums].material.Diffuse.a = 1.0f;
	//		}
	//	}
	//}
}

void UpdateSlopeWall(void)
{
	////static MODE mode = GetMode();
	//if (gamedata->modenow == MODE_BATTLEAREA)
	//{
	//	for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
	//	{
	//		D3DXVECTOR3 HitPosition;		//交点
	//		D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//		D3DXVECTOR3 playerpos;
	//		D3DXVECTOR3 camerapos = camera->pos;
	//		int numsw = j;
	//		for (int i = 0; i < MAX_PLAYER; i++) {
	//			playerpos = player[i].pos;
	//		}
	//		bool ans = CameraRayTouch_SlopeWall(playerpos, camerapos, &HitPosition, &Normal, &numsw);
	//		if (ans) {
	//			g_slopewall[numsw].material.Diffuse.a = 0.25f;
	//		}
	//		else
	//		{
	//			g_slopewall[numsw].material.Diffuse.a = 1.0f;
	//		}
	//	}
	//}

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

void UpdateBOSSField(void)
{
	//static MODE mode = GetMode();
	if (gamedata->modenow == MODE_BATTLEAREA)
	{
		for (int j = 0; j < MAX_PLAYER; j++)
		{
			if (player[j].use == false) { continue; }
			//if (GetKeyboardTrigger(DIK_T))
			//{
			//	player[j].namidause = player[j].namidause ? false : true;
			//	/*if (player[j].namidause == true) {
			//		player[j].namidause = false;
			//	}
			//	else
			//	{
			//		player[j].namidause = true;
			//	}*/
			//}
			for (int i = 0; i < MAX_MESH_BOSS_FIELD; i++)
			{
				MESH_MAP *pg_BOSSField;
				pg_BOSSField = &g_BOSSField[i];
				// 波の処理
				float dt = 0.05f;

				for (int z = 0; z < pg_BOSSField->nNumBlockZ; z++)
				{
					for (int x = 0; x < pg_BOSSField->nNumBlockX; x++)
					{
						float dx = g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.x - g_Center.x;
						float dz = g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.z - g_Center.z;

						// 波紋の中心点からの距離を得る
						float len = (float)sqrt(dx * dx + dz * dz);

						if (player[j].namidause == true) {
							// 波の高さを、sin関数で得る
						//  　波の高さ　= sin( -経過時間 * 周波数 + 距離 * 距離補正 ) * 振幅
							g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
						}
						else {
							g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.y = 0.0f;
						}



					}

				}
				g_Time += dt;


				// 頂点バッファに値をセットする
				D3D11_MAPPED_SUBRESOURCE msr;
				GetDeviceContext()->Map(pg_BOSSField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

				// 全頂点情報を毎回上書きしているのはDX11ではこの方が早いからです
				memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*pg_BOSSField->nNumVertex);

				GetDeviceContext()->Unmap(pg_BOSSField->vertexBuffer, 0);
			}
		}

	}

}

void UpdateMeshField_Random(void)
{
	//for (int j = 0; j < MAX_MESH_RANDOM_FIELD; j++)
	//{
	//	D3DXVECTOR3 HitPosition;		//交点
	//	D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//	D3DXVECTOR3 playerpos;
	//	D3DXVECTOR3 camerapos = camera->pos;
	//	int num = j;
	//	for (int i = 0; i < MAX_PLAYER; i++) {
	//		playerpos = player[i].pos;
	//	}
	//	bool ans = CameraRayTouch_Field(playerpos, camerapos, &HitPosition, &Normal, &num);
	//	if (ans) {
	//		g_aMeshRandomField[num].material.Diffuse.a = 0.25f;
	//	}
	//	else
	//	{
	//		g_aMeshRandomField[num].material.Diffuse.a = 1.0f;
	//	}
	//}
}
			
void UpdateTriangleWall(void)
{
	//for (int j = 0; j < MAX_MESH_SLOPE_WALL; j++)
	//{
	//	D3DXVECTOR3 HitPosition;		//交点
	//	D3DXVECTOR3 Normal;				//ぶつかったポリゴンの法線ベクトル(向き)
	//	D3DXVECTOR3 playerpos;
	//	D3DXVECTOR3 camerapos = camera->pos;
	//	int numsw = j;
	//	for (int i = 0; i < MAX_PLAYER; i++) {
	//		playerpos = player[i].pos;
	//	}
	//	bool ans = CameraRayTouch_SlopeWall(playerpos, camerapos, &HitPosition, &Normal, &numsw);
	//	if (ans) {
	//		g_slopewall[numsw].material.Diffuse.a = 0.25f;
	//	}
	//	else
	//	{
	//		g_slopewall[numsw].material.Diffuse.a = 1.0f;
	//	}
	//}

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}



//=============================================================================
// 描画処理
//=============================================================================
//絶対消えない、バトルエリアの壁と地面 とタイトル區
void DrawMeshMapRestarea(void) {
	// ライティングオフ
	SetLightEnable(false);
	//地面
	{
		MESH_MAP *pMesh_Field;
		

		for (int i = 0; i < 2; i++)
		{
			pMesh_Field = &g_aMeshField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// マテリアル設定
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Field->mtxWorld);


			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);

		}
		
	}
	//壁
	{
		MESH_MAP *pMesh_Wall;

		for (int i = 0; i < 10; i++)
		{
			pMesh_Wall = &g_aMeshWall[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

			// マテリアル設定
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}

		
	}
	//天井
	{
		MESH_MAP *pMesh_FieldCeiling;



		for (int i = 0; i < 1; i++)
		{
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// マテリアル設定
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}

		
	}
	//三角形
	{


		for (int i = 0; i < 4; i++)
		{


			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_Trianglewall[i].vertexBuffer, &stride, &offset);

			//// インデックスバッファ設定
			//GetDeviceContext()->IASetIndexBuffer(g_Trianglewall[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_TriangleWall[g_TexNo_TriangleWall]);

			// マテリアル設定
			SetMaterial(g_Trianglewall[i].material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Trianglewall[i].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Trianglewall[i].rot.y, g_Trianglewall[i].rot.x, g_Trianglewall[i].rot.z);
			D3DXMatrixMultiply(&g_Trianglewall[i].mtxWorld, &g_Trianglewall[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Trianglewall[i].pos.x, g_Trianglewall[i].pos.y, g_Trianglewall[i].pos.z);
			D3DXMatrixMultiply(&g_Trianglewall[i].mtxWorld, &g_Trianglewall[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&g_Trianglewall[i].mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->Draw(g_Trianglewall[i].nNumVertex, 0);
		}

		
	}
	//
	{

		MESH_MAP *pMesh_RandomField;
		
		for (int i = 0; i < 1; i++)
		{
			pMesh_RandomField = &g_aMeshRandomField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_RandomField->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_RandomField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_RandomField[pMesh_RandomField->texnum]);

			// マテリアル設定
			SetMaterial(pMesh_RandomField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_RandomField->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_RandomField->rot.y, pMesh_RandomField->rot.x, pMesh_RandomField->rot.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_RandomField->pos.x, pMesh_RandomField->pos.y, pMesh_RandomField->pos.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_RandomField->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_RandomField->nNumVertexIndex, 0, 0);
		}
	}
	// ライティングオン
	SetLightEnable(true);
}
void DrawMeshMapBattlearea(void) 
{
	DrawMeshMapSize();
	
	// 地面の描画処理
	{
		
		DrawMeshField_Behind();

		// 地面の描画処理
		DrawMeshSlope_Behind();

		DrawSlopeWall_Behind();

		// 壁の描画処理
		DrawMeshWall_Behind();

		//天井の描画処理
		DrawMeshFieldCeiling_Behind();

	}
	DrawShadow();
	DrawParticle();
	// 影の描画処理
	
	DrawTransparent();
	{
		DrawMeshField();
		// 地面の描画処理
		DrawMeshSlope();
		DrawSlopeWall();
		// 壁の描画処理
		DrawMeshWall();

		

		//天井の描画処理
		DrawMeshFieldCeiling();

	}
	{
		DrawSlopeWallReverse();
		// 地面の描画処理
		DrawMeshSlopeReverse();
		// 壁の描画処理
		DrawMeshWallReverse();
		// 地面の描画処理
		DrawMeshFieldReverse();
		//天井の描画処理
		DrawMeshFieldCeilingReverse();
		// 壁の描画処理
		DrawMeshWallReverse();

	}
}

void DrawMeshMapSize(void) {
	// ライティングオフ
	SetLightEnable(false);
	//field
	{
		static MESH_MAP *pMesh_Field;
		
		{
			for (int i = 2; i < 10; i++)
			{
				if (gamedata->drawopen == false)
				{
					if (g_aMeshField[i].DrawCheck == true) { continue; }
				}
				pMesh_Field = &g_aMeshField[i];

				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

				// インデックスバッファ設定
				GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

				// プリミティブトポロジ設定
				GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

				// マテリアル設定
				SetMaterial(pMesh_Field->material);

				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
				D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
				D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				SetWorldMatrix(&pMesh_Field->mtxWorld);


				// ポリゴンの描画
				GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);

			}
		}
		//// ライティングオン
		//SetLightEnable(true);
	}
	//wall
	{
		MESH_MAP *pMesh_Wall;

		
		for (int i = 10; i < 42; i++)
		{
			if (gamedata->drawopen == false)
			{
				if (g_aMeshWall[i].DrawCheck == true) { continue; }
			}
			pMesh_Wall = &g_aMeshWall[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

			// マテリアル設定
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}

		//// ライティングオン
		//SetLightEnable(true);
	}
	//ceiling
	{
		MESH_MAP *pMesh_FieldCeiling;
		

		//// ライティングオフ
		//SetLightEnable(false);

		for (int i = 1; i < 4; i++)
		{
			if (gamedata->drawopen == false)
			{
				if (g_aMeshFieldCeiling[i].DrawCheck == true) { continue; }
			}
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// マテリアル設定
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}

		//// ライティングオン
		//SetLightEnable(true);

		
	}
	//落穴
	{
		MESH_MAP *pMesh_RandomField;
		int i;

		// ライティングオフ
		SetLightEnable(false);

		for (i = 1; i < g_nNumMeshRandomField; i++)
		{
			/*if (gamedata->Mapdraw == true)
			{
				if (g_aMeshRandomField[i].DrawCheck == true) { continue; }
			}*/
			pMesh_RandomField = &g_aMeshRandomField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_RandomField->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_RandomField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_RandomField[pMesh_RandomField->texnum]);

			// マテリアル設定
			SetMaterial(pMesh_RandomField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_RandomField->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_RandomField->rot.y, pMesh_RandomField->rot.x, pMesh_RandomField->rot.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_RandomField->pos.x, pMesh_RandomField->pos.y, pMesh_RandomField->pos.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_RandomField->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_RandomField->nNumVertexIndex, 0, 0);
		}

		
	}
	//Bossエリアの地面
	{
		MESH_MAP *pg_BOSSField;


		// ライティングオフ
		SetLightEnable(false);

		for (int i = 0; i < g_nNumBOSSField; i++)
		{
			/*if (gamedata->Mapdraw == true)
			{
				if (g_BOSSField[i].DrawCheck == true) { continue; }
			}*/
			pg_BOSSField = &g_BOSSField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pg_BOSSField->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pg_BOSSField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_BOSSField[g_TexNo_BOSSField]);

			// マテリアル設定
			SetMaterial(pg_BOSSField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pg_BOSSField->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pg_BOSSField->rot.y, pg_BOSSField->rot.x, pg_BOSSField->rot.z);
			D3DXMatrixMultiply(&pg_BOSSField->mtxWorld, &pg_BOSSField->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pg_BOSSField->pos.x, pg_BOSSField->pos.y, pg_BOSSField->pos.z);
			D3DXMatrixMultiply(&pg_BOSSField->mtxWorld, &pg_BOSSField->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pg_BOSSField->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pg_BOSSField->nNumVertexIndex, 0, 0);
		}
	}
	// ライティングオン
	SetLightEnable(true);
}

//player前
void DrawMeshField(void)
{
	MESH_MAP *pMesh_Field;
	

	// ライティングオフ
	SetLightEnable(false);

	for (int i = 10; i < g_nNumMeshField; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshField[i].DrawCheck == true) { continue; }
		}

		if (g_aMeshField[i].cameracheck == true)
		{
			pMesh_Field = &g_aMeshField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// マテリアル設定
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Field->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshWall(void)
{
	MESH_MAP *pMesh_Wall;
	
	// ライティングオフ
	SetLightEnable(false);

	for (int i = 42; i < g_nNumMeshWall; i++)
	{
		if (g_aMeshWall[i].use == false)continue;
		
		if (gamedata->drawopen == false)
		{
			if (g_aMeshWall[i].DrawCheck == true) { continue; }
		}


		if (g_aMeshWall[i].cameracheck == true)
		{
			pMesh_Wall = &g_aMeshWall[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			if (i == 152) {
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[1]);
			}
			else
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);
			}


			// マテリアル設定
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshFieldCeiling(void)
{
	MESH_MAP *pMesh_FieldCeiling;
	
	// ライティングオフ
	SetLightEnable(false);

	for (int i = 4; i < g_nNumMeshCeiling; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldCeiling[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshFieldCeiling[i].cameracheck == true)
		{
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// マテリアル設定
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}
		
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshSlope(void)
{
	MESH_MAP *pMesh_Slope;
	

	// ライティングオフ
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlope; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldSlope[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshFieldSlope[i].cameracheck == true)
		{
			pMesh_Slope = &g_aMeshFieldSlope[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Slope->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Slope->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

			// マテリアル設定
			SetMaterial(pMesh_Slope->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Slope->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Slope->rot.y, pMesh_Slope->rot.x, pMesh_Slope->rot.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Slope->pos.x, pMesh_Slope->pos.y, pMesh_Slope->pos.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Slope->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Slope->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawSlopeWall(void)
{
	// ライティングオフ
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].DrawCheck == true) { continue; }
		}
		if (g_slopewall[i].cameracheck == true)
		{

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_slopewall[i].vertexBuffer, &stride, &offset);

			//// インデックスバッファ設定
			//GetDeviceContext()->IASetIndexBuffer(g_slopewall[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_SlopeWall[g_TexNo_SlopeWall]);

			// マテリアル設定
			SetMaterial(g_slopewall[i].material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_slopewall[i].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_slopewall[i].rot.y, g_slopewall[i].rot.x, g_slopewall[i].rot.z);
			D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_slopewall[i].pos.x, g_slopewall[i].pos.y, g_slopewall[i].pos.z);
			D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&g_slopewall[i].mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->Draw(g_slopewall[i].nNumVertex, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}
//player後
void DrawMeshField_Behind(void)
{
	MESH_MAP *pMesh_Field;
	

	// ライティングオフ
	SetLightEnable(false);

	for (int i = 10; i < g_nNumMeshField; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshField[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshField[i].cameracheck == false)
		{
			
			pMesh_Field = &g_aMeshField[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// マテリアル設定
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Field->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshWall_Behind(void)
{
	MESH_MAP *pMesh_Wall;
	

	// ライティングオフ
	SetLightEnable(false);

	for (int i = 42; i < g_nNumMeshWall; i++)
	{
		if (g_aMeshWall[i].use == false)continue;
		if (gamedata->drawopen == false)
		{
			if (g_aMeshWall[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshWall[i].cameracheck == false)
		{
			pMesh_Wall = &g_aMeshWall[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			if (i == 152) {
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[1]);
			}
			else
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);
			}


			// マテリアル設定
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshFieldCeiling_Behind(void)
{
	MESH_MAP *pMesh_FieldCeiling;
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 4; i < g_nNumMeshCeiling; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldCeiling[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshFieldCeiling[i].cameracheck == false)
		{
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// マテリアル設定
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshSlope_Behind(void)
{
	MESH_MAP *pMesh_Slope;

	// ライティングオフ
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlope; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldSlope[i].DrawCheck == true) { continue; }
		}
		if (g_aMeshFieldSlope[i].cameracheck == false)
		{
			pMesh_Slope = &g_aMeshFieldSlope[i];

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Slope->vertexBuffer, &stride, &offset);

			// インデックスバッファ設定
			GetDeviceContext()->IASetIndexBuffer(pMesh_Slope->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

			// マテリアル設定
			SetMaterial(pMesh_Slope->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pMesh_Slope->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Slope->rot.y, pMesh_Slope->rot.x, pMesh_Slope->rot.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Slope->pos.x, pMesh_Slope->pos.y, pMesh_Slope->pos.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&pMesh_Slope->mtxWorld);

			// ポリゴンの描画
			GetDeviceContext()->DrawIndexed(pMesh_Slope->nNumVertexIndex, 0, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawSlopeWall_Behind(void)
{
	// ライティングオフ
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].DrawCheck == true) { continue; }
		}
		if (g_slopewall[i].cameracheck == false)
		{

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_slopewall[i].vertexBuffer, &stride, &offset);

		//// インデックスバッファ設定
		//GetDeviceContext()->IASetIndexBuffer(g_slopewall[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_SlopeWall[g_TexNo_SlopeWall]);

		// マテリアル設定
		SetMaterial(g_slopewall[i].material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_slopewall[i].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_slopewall[i].rot.y, g_slopewall[i].rot.x, g_slopewall[i].rot.z);
		D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_slopewall[i].pos.x, g_slopewall[i].pos.y, g_slopewall[i].pos.z);
		D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&g_slopewall[i].mtxWorld);

		// ポリゴンの描画
		GetDeviceContext()->Draw(g_slopewall[i].nNumVertex, 0);
		}
	}

	// ライティングオン
	SetLightEnable(true);
}



//=============================================================================
// カメラのCHECK
//=============================================================================
void MapTransparentCheck(void)
{
	
	//D3DXVECTOR3 pPos = player[0].pos- camera->pos;
	D3DXVECTOR3 plong = camera->at- camera->pos ;//カメラ→視点向量上、カメラ→playerの距離
	D3DXVECTOR3 ppos = player[0].pos;
	ppos.y += PLAYER_TALL / 2;
	float	kyori = (float)sqrt((plong.x*plong.x) + (plong.y*plong.y) + (plong.z*plong.z));
	//float	camera_Y_UP = (VIEW_ANGLE / 2);
	//float	camera_Y_DOWN = -(VIEW_ANGLE / 2);
	//float	camera_X_RIGHT = (float)atan( ((SCREEN_WIDTH / 2) / (SCREEN_HEIGHT / 2)) * (tanf(VIEW_ANGLE / 2)) );
	//float	camera_X_LEFT = -(float)atan( ((SCREEN_WIDTH / 2) / (SCREEN_HEIGHT / 2)) * (tanf(VIEW_ANGLE / 2)) );
	//float  camera_radius_XZ = tan(camera_X_RIGHT)*kyori;//camera.pos->camera.atの視錐台の幅MAX
	
	//field
	{
		//カメラの範囲内判定
		for (int i = 2; i < g_nNumMeshField; i++)
		{
			bool drawcheck = false;	//TRUEならば、カメラの範囲内
			
			D3DXVECTOR3 p0 = g_fieldpoint[i].pos0 - camera->pos;
			D3DXVECTOR3 p1 = g_fieldpoint[i].pos1 - camera->pos;
			D3DXVECTOR3 p2 = g_fieldpoint[i].pos2 - camera->pos;
			D3DXVECTOR3 p3 = g_fieldpoint[i].pos3 - camera->pos;

			float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
			float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
			float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
			float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
			if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
				|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX)
			{
				drawcheck = true;
			}
			else if (dans1 > camera->xyrot_MAX && dans2 > camera->xyrot_MAX
				&& dans3 > camera->xyrot_MAX && dans4 > camera->xyrot_MAX)
			{
				//MESH_MAP *pMesh_Field;
				//pMesh_Field = &g_aMeshField[i];
				//// 頂点バッファへのポインタを取得
				//D3D11_MAPPED_SUBRESOURCE msr;
				//GetDeviceContext()->Map(pMesh_Field->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				//VERTEX_3D * pVtx = (VERTEX_3D*)msr.pData;
				D3DXVECTOR3 width = g_fieldpoint[i].pos1 - g_fieldpoint[i].pos0;
				D3DXVECTOR3 length = g_fieldpoint[i].pos2 - g_fieldpoint[i].pos0;
				D3DXVECTOR3 point = g_fieldpoint[i].pos0;
				for (int j = 0; j< g_aMeshField[i].nNumBlockX+1; j++)
				{
					if (j != 0)
					{
						point.x += width.x / g_aMeshField[i].nNumBlockX;
						point.y += width.y / g_aMeshField[i].nNumBlockX;
						point.z += width.z / g_aMeshField[i].nNumBlockX;
					}
					point.z= g_fieldpoint[i].pos0.z;
					for (int k = 0; k < g_aMeshField[i].nNumBlockZ+1 ; k++)
					{
						if (j == 0 && k == 0)continue;
						if(j== g_aMeshField[i].nNumBlockX&&k== g_aMeshField[i].nNumBlockZ)continue;

						
						
						if (k != 0)
						{
							point.z += length.z / g_aMeshField[i].nNumBlockZ;
						}
						D3DXVECTOR3 pp0 = point - camera->pos;
						float ddans1 = acosf(D3DXVec3Dot(&plong, &pp0) / (((float)sqrt((pp0.x*pp0.x) + (pp0.y*pp0.y) + (pp0.z*pp0.z)))*kyori));
						if (ddans1 <= camera->xyrot_MAX)
						{
							drawcheck = true;
							break;
						}

					}
					if (drawcheck == true)
					{
						break;
					}
				}
				/*bool checkans = camera_check(g_fieldpoint[i].pos0, g_fieldpoint[i].pos1, g_fieldpoint[i].pos2, g_fieldpoint[i].pos3);
				if (checkans == true)
				{
					drawcheck = true;
				}*/
			}

			//カメラ中に判定
			if (drawcheck == true)
			{
				g_aMeshField[i].DrawCheck = false;
				//break;
			}
			else
			{
				g_aMeshField[i].DrawCheck = true;
			}
		}
		//PLAYERの前後判定
		for (int i = 10; i < g_nNumMeshField; i++)
		{
			bool frontcheck = false;//trueならば、PLAYERの前です
			
			{
				
				D3DXVECTOR3 pp0 = g_fieldpoint[i].pos0;
				D3DXVECTOR3 pp1 = g_fieldpoint[i].pos1;
				D3DXVECTOR3 pp2 = g_fieldpoint[i].pos2;
				D3DXVECTOR3 pp3 = g_fieldpoint[i].pos3;
				//float result = (float)atan2(kyori, kyori2); //カメラ→点とカメラ→視点の向量の角度
				bool ans1=RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
				bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1,pp2, pp3);
				bool ans3 = RayHitTransparent(camera->pos, player[0].top_pos, pp0, pp1, pp2, pp3);
				bool ans4 = RayHitTransparent(camera->pos, player[0].down_pos, pp0, pp1, pp2, pp3);

				if (ans1==true || ans2 == true || ans3 == true || ans4 == true)
				{
					frontcheck = true;
				}
			}
			//前後判定
			if (frontcheck == true)
			{
				g_aMeshField[i].cameracheck = true;
			}
			else
			{
				g_aMeshField[i].cameracheck = false;
			}

		}

	}
	//Wall
	{
		//カメラの範囲内判定
		for (int i = 10; i < g_nNumMeshWall; i++)
		{
			bool drawcheck = false;	//TRUEならば、カメラの範囲内
			
			//
			//MESH_MAP *pMesh_Wall;
			//pMesh_Wall = &g_aMeshWall[i];
			//// 頂点バッファへのポインタを取得
			//D3D11_MAPPED_SUBRESOURCE msr;
			//GetDeviceContext()->Map(pMesh_Wall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			//
			//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			//
			//D3DXVECTOR3 p0 = pVtx[0].Position - camera->pos;
			//D3DXVECTOR3 p1 = pVtx[1].Position - camera->pos;
			//D3DXVECTOR3 p2 = pVtx[2].Position - camera->pos;
			//D3DXVECTOR3 p3 = pVtx[3].Position - camera->pos;

			D3DXVECTOR3 p0 = g_wallpoint[i].pos0 - camera->pos;
			D3DXVECTOR3 p1 = g_wallpoint[i].pos1 - camera->pos;
			D3DXVECTOR3 p2 = g_wallpoint[i].pos2 - camera->pos;
			D3DXVECTOR3 p3 = g_wallpoint[i].pos3 - camera->pos;

			float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
			float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
			float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
			float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
			if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
				|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX)
			{
				drawcheck = true;
			}
			else if (dans1 > camera->xyrot_MAX && dans2 > camera->xyrot_MAX
				&& dans3 > camera->xyrot_MAX && dans4 > camera->xyrot_MAX)
			{
				
				D3DXVECTOR3 width = g_wallpoint[i].pos1 - g_wallpoint[i].pos0;
				D3DXVECTOR3 length = g_wallpoint[i].pos2 - g_wallpoint[i].pos0;
				D3DXVECTOR3 point = g_wallpoint[i].pos0;
				for (int j = 0; j < g_aMeshWall[i].nNumBlockX + 1; j++)
				{
					if (j != 0)
					{
						point.x += width.x / g_aMeshWall[i].nNumBlockX;
						point.y += width.y / g_aMeshWall[i].nNumBlockX;
						point.z += width.z / g_aMeshWall[i].nNumBlockX;
					}
					point.y = g_wallpoint[i].pos0.y;
					for (int k = 0; k < g_aMeshWall[i].nNumBlockY + 1; k++)
					{
						if (j == 0 && k == 0)continue;
						if (j == g_aMeshWall[i].nNumBlockX&&k == g_aMeshWall[i].nNumBlockY)continue;

						
						if (k != 0)
						{
							point.y += length.y / g_aMeshWall[i].nNumBlockY;
						}
						
						D3DXVECTOR3 pp0 = point - camera->pos;
						float ddans1 = acosf(D3DXVec3Dot(&plong, &pp0) / (((float)sqrt((pp0.x*pp0.x) + (pp0.y*pp0.y) + (pp0.z*pp0.z)))*kyori));
						if (ddans1 <= camera->xyrot_MAX)
						{
							drawcheck = true;
							break;
						}

					}
					if (drawcheck == true)
					{
						break;
					}
				}
				//bool checkans = camera_check(g_wallpoint[i].pos0, g_wallpoint[i].pos1, g_wallpoint[i].pos2, g_wallpoint[i].pos3);
				//if (checkans == true)
				//{
				//	drawcheck = true;
				//}
			}
			//カメラ中に判定
			if (drawcheck == true)
			{
				g_aMeshWall[i].DrawCheck = false;
				//break;
			}
			else
			{
				g_aMeshWall[i].DrawCheck = true;
			}
		}
		//PLAYERの前後判定
		for (int i = 42; i < g_nNumMeshWall; i++)
		{
			bool frontcheck = false;//trueならば、PLAYERの前です
			{
				D3DXVECTOR3 pp0 = g_wallpoint[i].pos0;
				D3DXVECTOR3 pp1 = g_wallpoint[i].pos1;
				D3DXVECTOR3 pp2 = g_wallpoint[i].pos2;
				D3DXVECTOR3 pp3 = g_wallpoint[i].pos3;
				//float result = (float)atan2(kyori, kyori2); //カメラ→点とカメラ→視点の向量の角度
				bool ans1 = RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
				bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1, pp2, pp3);
				bool ans3 = RayHitTransparent(camera->pos, player[0].top_pos, pp0, pp1, pp2, pp3);
				bool ans4 = RayHitTransparent(camera->pos, player[0].down_pos, pp0, pp1, pp2, pp3);

			
				if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
				{
					frontcheck = true;
				}
			}

			if (frontcheck == true)
			{
				g_aMeshWall[i].cameracheck = true;
			}
			else
			{
				g_aMeshWall[i].cameracheck = false;
			}

		}
	}
	//Ceilling
	{
		
		//カメラの範囲内判定
		for (int i = 1; i < g_nNumMeshCeiling; i++)
		{
			bool drawcheck = false;	//TRUEならば、カメラの範囲内

			//MESH_MAP *pMesh_FieldCeiling;
			//pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];
			//// 頂点バッファへのポインタを取得
			//D3D11_MAPPED_SUBRESOURCE msr;
			//GetDeviceContext()->Map(pMesh_FieldCeiling->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			//
			//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			//
			//D3DXVECTOR3 p0 = pVtx[0].Position - camera->pos;
			//D3DXVECTOR3 p1 = pVtx[1].Position - camera->pos;
			//D3DXVECTOR3 p2 = pVtx[2].Position - camera->pos;
			//D3DXVECTOR3 p3 = pVtx[3].Position - camera->pos;

			D3DXVECTOR3 p0 = g_ceilingpoint[i].pos0 - camera->pos;
			D3DXVECTOR3 p1 = g_ceilingpoint[i].pos1 - camera->pos;
			D3DXVECTOR3 p2 = g_ceilingpoint[i].pos2 - camera->pos;
			D3DXVECTOR3 p3 = g_ceilingpoint[i].pos3 - camera->pos;

			float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
			float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
			float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
			float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
			if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
				|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX)
			{
				drawcheck = true;
			}
			else if (dans1 > camera->xyrot_MAX && dans2 > camera->xyrot_MAX
				&& dans3 > camera->xyrot_MAX && dans4 > camera->xyrot_MAX)
			{
				D3DXVECTOR3 width = g_ceilingpoint[i].pos1 - g_ceilingpoint[i].pos0;
				D3DXVECTOR3 length = g_ceilingpoint[i].pos2 - g_ceilingpoint[i].pos0;
				D3DXVECTOR3 point = g_ceilingpoint[i].pos0;
				for (int j = 0; j < g_aMeshFieldCeiling[i].nNumBlockX + 1; j++)
				{
					if (j != 0)
					{
						point.x += width.x / g_aMeshFieldCeiling[i].nNumBlockX;
						point.z += width.z / g_aMeshFieldCeiling[i].nNumBlockX;
					}
					point.z = g_ceilingpoint[i].pos0.z;
					for (int k = 0; k < g_aMeshFieldCeiling[i].nNumBlockZ + 1; k++)
					{
						if (j == 0 && k == 0)continue;
						if (j == g_aMeshFieldCeiling[i].nNumBlockX&&k == g_aMeshFieldCeiling[i].nNumBlockZ)continue;



						if (k != 0)
						{
							point.z += length.z / g_aMeshField[i].nNumBlockZ;
						}
						D3DXVECTOR3 pp0 = point - camera->pos;
						float ddans1 = acosf(D3DXVec3Dot(&plong, &pp0) / (((float)sqrt((pp0.x*pp0.x) + (pp0.y*pp0.y) + (pp0.z*pp0.z)))*kyori));
						if (ddans1 <= camera->xyrot_MAX)
						{
							drawcheck = true;
							break;
						}

					}
					if (drawcheck == true)
					{
						break;
					}
				}
				//bool checkans = camera_check(g_ceilingpoint[i].pos0, g_ceilingpoint[i].pos1, g_ceilingpoint[i].pos2, g_ceilingpoint[i].pos3);
				//if (checkans == true)
				//{
				//	drawcheck = true;
				//}
			}
			//カメラ中に判定
			if (drawcheck == true)
			{
				g_aMeshFieldCeiling[i].DrawCheck = false;
				//break;
			}
			else
			{
				g_aMeshFieldCeiling[i].DrawCheck = true;
			}
		}
		//PLAYERの前後判定
		for (int i = 0; i < g_nNumMeshCeiling; i++)
		{
			bool frontcheck = false;//trueならば、PLAYERの前です

			{
				
				D3DXVECTOR3 pp0 = g_ceilingpoint[i].pos0 ;
				D3DXVECTOR3 pp1 = g_ceilingpoint[i].pos1 ;
				D3DXVECTOR3 pp2 = g_ceilingpoint[i].pos2 ;
				D3DXVECTOR3 pp3 = g_ceilingpoint[i].pos3 ;
				
				//float result = (float)atan2(kyori, kyori2); //カメラ→点とカメラ→視点の向量の角度
				bool ans1 = RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
				bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1, pp2, pp3);
				bool ans3 = RayHitTransparent(camera->pos, player[0].center_pos, pp0, pp1, pp2, pp3);
				bool ans4 = RayHitTransparent(camera->pos, player[0].down_pos, pp0, pp1, pp2, pp3);

				if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
				{
					frontcheck = true;
				}
			}
			if (frontcheck == true)
			{
				g_aMeshFieldCeiling[i].cameracheck = true;
			}
			else
			{
				g_aMeshFieldCeiling[i].cameracheck = false;
			}

		}
	}
	//Slope
	{
		//カメラの範囲内判定
		for (int i = 0; i < g_nNumMeshSlope; i++)
		{
			bool drawcheck = false;	//TRUEならば、カメラの範囲内

			//MESH_MAP *pMesh_Slope;
			//pMesh_Slope = &g_aMeshFieldSlope[i];
			//// 頂点バッファへのポインタを取得
			//D3D11_MAPPED_SUBRESOURCE msr;
			//GetDeviceContext()->Map(pMesh_Slope->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			//
			//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			//
			//D3DXVECTOR3 p0 = pVtx[0].Position - camera->pos;
			//D3DXVECTOR3 p1 = pVtx[1].Position - camera->pos;
			//D3DXVECTOR3 p2 = pVtx[2].Position - camera->pos;
			//D3DXVECTOR3 p3 = pVtx[3].Position - camera->pos;

			D3DXVECTOR3 p0 = g_fieldslopepoint[i].pos0 - camera->pos;
			D3DXVECTOR3 p1 = g_fieldslopepoint[i].pos1 - camera->pos;
			D3DXVECTOR3 p2 = g_fieldslopepoint[i].pos2 - camera->pos;
			D3DXVECTOR3 p3 = g_fieldslopepoint[i].pos3 - camera->pos;

			float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
			float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
			float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
			float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
			if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
				|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX)
			{
				drawcheck = true;
			}
			else if (dans1 > camera->xyrot_MAX && dans2 > camera->xyrot_MAX
				&& dans3 > camera->xyrot_MAX && dans4 > camera->xyrot_MAX)
			{
				D3DXVECTOR3 width = g_fieldslopepoint[i].pos1 - g_fieldslopepoint[i].pos0;
				D3DXVECTOR3 length = g_fieldslopepoint[i].pos2 - g_fieldslopepoint[i].pos0;
				D3DXVECTOR3 point = g_fieldslopepoint[i].pos0;
				for (int j = 0; j < g_aMeshFieldSlope[i].nNumBlockX + 1; j++)
				{
					if (j != 0)
					{
						point.x += width.x / g_aMeshFieldSlope[i].nNumBlockX;
						point.y += width.y / g_aMeshFieldSlope[i].nNumBlockX;
						point.z += width.z / g_aMeshFieldSlope[i].nNumBlockX;
					}
					point.y = g_fieldslopepoint[i].pos0.y;
					for (int k = 0; k < g_aMeshFieldSlope[i].nNumBlockY + 1; k++)
					{
						if (j == 0 && k == 0)continue;
						if (j == g_aMeshFieldSlope[i].nNumBlockX&&k == g_aMeshFieldSlope[i].nNumBlockY)continue;


						if (k != 0)
						{
							point.y += length.y / g_aMeshFieldSlope[i].nNumBlockY;
						}

						D3DXVECTOR3 pp0 = point - camera->pos;
						float ddans1 = acosf(D3DXVec3Dot(&plong, &pp0) / (((float)sqrt((pp0.x*pp0.x) + (pp0.y*pp0.y) + (pp0.z*pp0.z)))*kyori));
						if (ddans1 <= camera->xyrot_MAX)
						{
							drawcheck = true;
							break;
						}

					}
					if (drawcheck == true)
					{
						break;
					}
				}
				//bool checkans = camera_check(g_fieldslopepoint[i].pos0, g_fieldslopepoint[i].pos1, g_fieldslopepoint[i].pos2, g_fieldslopepoint[i].pos3);
				//if (checkans == true)
				//{
				//	drawcheck = true;
				//}
			}
			//カメラ中に判定
			if (drawcheck == true)
			{
				g_aMeshFieldSlope[i].DrawCheck = false;
				//break;
			}
			else
			{
				g_aMeshFieldSlope[i].DrawCheck = true;
			}
		}
		//PLAYERの前後判定
		for (int i = 0; i < g_nNumMeshSlope; i++)
		{
			bool frontcheck = false;//trueならば、PLAYERの前です

			{
				
				D3DXVECTOR3 pp0 = g_fieldslopepoint[i].pos0 ;
				D3DXVECTOR3 pp1 = g_fieldslopepoint[i].pos1 ;
				D3DXVECTOR3 pp2 = g_fieldslopepoint[i].pos2 ;
				D3DXVECTOR3 pp3 = g_fieldslopepoint[i].pos3 ;

				//float result = (float)atan2(kyori, kyori2); //カメラ→点とカメラ→視点の向量の角度
				bool ans1 = RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
				bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1, pp2, pp3);
				bool ans3 = RayHitTransparent(camera->pos, player[0].top_pos, pp0, pp1, pp2, pp3);
				bool ans4 = RayHitTransparent(camera->pos, player[0].center_pos, pp0, pp1, pp2, pp3);

				
				if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
				{
					frontcheck = true;
				}
			}
			if (frontcheck == true)
			{
				g_aMeshFieldSlope[i].cameracheck = true;
			}
			else
			{
				g_aMeshFieldSlope[i].cameracheck = false;
			}
		}
	}
	//SlopeWall
	{
		//
		//カメラの範囲内判定
		for (int i = 0; i < g_nNumMeshSlopeWall; i++)
		{
			bool drawcheck = false;	//TRUEならば、カメラの範囲内

			//MESH_MAP *pMesh_SlopeWall;
			//pMesh_SlopeWall = &g_slopewall[i];
			//// 頂点バッファへのポインタを取得
			//D3D11_MAPPED_SUBRESOURCE msr;
			//GetDeviceContext()->Map(pMesh_SlopeWall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			//
			//VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			//
			//
			//D3DXVECTOR3 p0 = pVtx[0].Position - camera->pos;
			//D3DXVECTOR3 p1 = pVtx[1].Position - camera->pos;
			//D3DXVECTOR3 p2 = pVtx[2].Position - camera->pos;
			//D3DXVECTOR3 p3 = pVtx[3].Position - camera->pos;

			D3DXVECTOR3 p0 = g_slopewallpoint[i].pos0 - camera->pos;
			D3DXVECTOR3 p1 = g_slopewallpoint[i].pos1 - camera->pos;
			D3DXVECTOR3 p2 = g_slopewallpoint[i].pos2 - camera->pos;
			D3DXVECTOR3 p3 = g_slopewallpoint[i].pos3 - camera->pos;

			float dans1 = acosf(D3DXVec3Dot(&plong, &p0) / (((float)sqrt((p0.x*p0.x) + (p0.y*p0.y) + (p0.z*p0.z)))*kyori));
			float dans2 = acosf(D3DXVec3Dot(&plong, &p1) / (((float)sqrt((p1.x*p1.x) + (p1.y*p1.y) + (p1.z*p1.z)))*kyori));
			float dans3 = acosf(D3DXVec3Dot(&plong, &p2) / (((float)sqrt((p2.x*p2.x) + (p2.y*p2.y) + (p2.z*p2.z)))*kyori));
			float dans4 = acosf(D3DXVec3Dot(&plong, &p3) / (((float)sqrt((p3.x*p3.x) + (p3.y*p3.y) + (p3.z*p3.z)))*kyori));
			if (dans1 <= camera->xyrot_MAX || dans2 <= camera->xyrot_MAX
				|| dans3 <= camera->xyrot_MAX || dans4 <= camera->xyrot_MAX)
			{
				drawcheck = true;
			}
			else if (dans1 > camera->xyrot_MAX && dans2 > camera->xyrot_MAX
				&& dans3 > camera->xyrot_MAX && dans4 > camera->xyrot_MAX)
			{
				D3DXVECTOR3 width = g_slopewallpoint[i].pos1 - g_slopewallpoint[i].pos0;
				D3DXVECTOR3 length;
				if (g_slopewallpoint[i].pos2.y != g_slopewallpoint[i].pos0.y)
				{
					 length = g_slopewallpoint[i].pos2 - g_slopewallpoint[i].pos0;
				}
				else
				{
					length = g_slopewallpoint[i].pos1 - g_slopewallpoint[i].pos0;
				}
				D3DXVECTOR3 point = g_slopewallpoint[i].pos0;
				for (int j = 0; j < 11; j++)
				{
					if (j != 0)
					{
						point.x += (float)(width.x / 10);
						point.z += (float)(width.z / 10);
					}
					point.y = g_slopewallpoint[i].pos0.y;
					for (int k = 0; k < 11; k++)
					{
						if (j == 0 && k == 0)continue;
						if (j == 10&&k == 10)continue;


						if (k != 0)
						{
							point.y += (float)(length.y / 10);
						}

						D3DXVECTOR3 pp0 = point - camera->pos;
						float ddans1 = acosf(D3DXVec3Dot(&plong, &pp0) / (((float)sqrt((pp0.x*pp0.x) + (pp0.y*pp0.y) + (pp0.z*pp0.z)))*kyori));
						if (ddans1 <= camera->xyrot_MAX)
						{
							drawcheck = true;
							break;
						}

					}
					if (drawcheck == true)
					{
						break;
					}
				}
				//bool checkans = camera_check(g_slopewallpoint[i].pos0, g_slopewallpoint[i].pos1, g_slopewallpoint[i].pos2, g_slopewallpoint[i].pos3);
				//if (checkans == true)
				//{
				//	drawcheck = true;
				//}
			}
			//カメラ中に判定
			if (drawcheck == true)
			{
				g_slopewall[i].DrawCheck = false;
				//break;
			}
			else
			{
				g_slopewall[i].DrawCheck = true;
			}
		}
		//PLAYERの前後判定
		for (int i = 0; i < g_nNumMeshSlopeWall; i++)
		{
			bool frontcheck = false;//trueならば、PLAYERの前です
			
			{
				
				D3DXVECTOR3 pp0 = g_slopewallpoint[i].pos0 ;
				D3DXVECTOR3 pp1 = g_slopewallpoint[i].pos1 ;
				D3DXVECTOR3 pp2 = g_slopewallpoint[i].pos2 ;
				D3DXVECTOR3 pp3 = g_slopewallpoint[i].pos3 ;
				
				D3DXVECTOR3 HitPosition;
				D3DXVECTOR3 Normal;
				
				switch (g_slopewallpoint[i].check1)
				{
				case 1:
				{
					bool ans1 = RayCast(pp0, pp2, pp1, camera->pos, player[0].left_pos, &HitPosition, &Normal);
					bool ans2 = RayCast(pp0, pp2, pp1, camera->pos, player[0].right_pos, &HitPosition, &Normal);
					bool ans3 = RayCast(pp0, pp2, pp1, camera->pos, player[0].top_pos, &HitPosition, &Normal);
					bool ans4 = RayCast(pp0, pp2, pp1, camera->pos, player[0].pos, &HitPosition, &Normal);
					
					if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
					{
						frontcheck = true;
					}
				}
					break;
				case 2:
				{
					bool ans1 = RayCast(pp0, pp3, pp1, camera->pos, player[0].left_pos, &HitPosition, &Normal);
					bool ans2 = RayCast(pp0, pp3, pp1, camera->pos, player[0].right_pos, &HitPosition, &Normal);
					bool ans3 = RayCast(pp0, pp3, pp1, camera->pos, player[0].top_pos, &HitPosition, &Normal);
					bool ans4 = RayCast(pp0, pp3, pp1, camera->pos, player[0].pos, &HitPosition, &Normal);
					
					if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
					{
						frontcheck = true;
					}
				}
					break;
				default:
					break;
				}

				
			}
			if (frontcheck == true)
			{
				g_slopewall[i].cameracheck = true;
			}
			else
			{
				g_slopewall[i].cameracheck = false;
			}

		}
	}

}

//=============================================================================
// RAY判定
//=============================================================================

bool RayHitField(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal)
{

	for (int i = 1; i < g_nNumMeshField; i++)
	{
		D3D11_MAPPED_SUBRESOURCE msrrr;
		GetDeviceContext()->Map(g_aMeshField[i].vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrrr);

		VERTEX_3D* pVtxxx = (VERTEX_3D*)msrrr.pData;

		D3DXVECTOR3 start = pos;
		D3DXVECTOR3 end = pos;

		// 少し上から、ズドーンと下へレイを飛ばす
		start.y += 100.0f;
		end.y -= 1000.0f;


		for (int z = 0; z < g_aMeshField[i].nNumBlockZ; z++)
		{
			for (int x = 0; x < g_aMeshField[i].nNumBlockX; x++)
			{
				D3DXVECTOR3 p0 = pVtxxx[z * (g_aMeshField[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p1 = pVtxxx[z * (g_aMeshField[i].nNumBlockX + 1) + (x + 1)].Position;
				D3DXVECTOR3 p2 = pVtxxx[(z + 1) * (g_aMeshField[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p3 = pVtxxx[(z + 1) * (g_aMeshField[i].nNumBlockX + 1) + (x + 1)].Position;

				bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);

				if (ans)
				{
					return true;
				}

				ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
				if (ans)
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool RayHitBOSSField(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal)
{
	D3DXVECTOR3 start = pos;
	D3DXVECTOR3 end = pos;

	
		// 少し上から、ズドーンと下へレイを飛ばす
		start.y += 100.0f;
		end.y -= 1000.0f;

		for (int z = 0; z < g_BOSSField[0].nNumBlockZ; z++)
		{
			for (int x = 0; x < g_BOSSField[0].nNumBlockX; x++)
			{
					D3DXVECTOR3 p0 = g_Vertex[z * (g_BOSSField[0].nNumBlockX + 1) + x].Position;
					D3DXVECTOR3 p1 = g_Vertex[z * (g_BOSSField[0].nNumBlockX + 1) + (x + 1)].Position;
					D3DXVECTOR3 p2 = g_Vertex[(z + 1) * (g_BOSSField[0].nNumBlockX + 1) + x].Position;
					D3DXVECTOR3 p3 = g_Vertex[(z + 1) * (g_BOSSField[0].nNumBlockX + 1) + (x + 1)].Position;
					if (start.x >= p0.x&&start.x <= p1.x&&start.z >= p2.z&&start.z <= p0.z)
					{
						bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
						if (ans)
						{
							return true;
						}
						ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
						if (ans)
						{
							return true;
						}
					}
					else
					{
						continue;
					}
					
			}
		}
	return false;
}

bool RayHitFieldCeiling(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal)
{

	for (int i = 1; i < g_nNumMeshCeiling; i++)
	{
		D3D11_MAPPED_SUBRESOURCE msrrr;
		GetDeviceContext()->Map(g_aMeshFieldCeiling[i].vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrrr);

		VERTEX_3D* pVtxxx = (VERTEX_3D*)msrrr.pData;

		D3DXVECTOR3 start = pos;
		D3DXVECTOR3 end = pos;

		// 少し上から、ズドーンと下へレイを飛ばす
		start.y += 100.0f;
		end.y -= 1000.0f;


		for (int z = 0; z < g_aMeshFieldCeiling[i].nNumBlockZ; z++)
		{
			for (int x = 0; x < g_aMeshFieldCeiling[i].nNumBlockX; x++)
			{
				D3DXVECTOR3 p0 = pVtxxx[z * (g_aMeshFieldCeiling[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p1 = pVtxxx[z * (g_aMeshFieldCeiling[i].nNumBlockX + 1) + (x + 1)].Position;
				D3DXVECTOR3 p2 = pVtxxx[(z + 1) * (g_aMeshFieldCeiling[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p3 = pVtxxx[(z + 1) * (g_aMeshFieldCeiling[i].nNumBlockX + 1) + (x + 1)].Position;

				bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
				if (ans)
				{
					return true;
				}

				ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
				if (ans)
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool RayHitFieldSlope(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal)
{

	for (int i = 0; i < g_nNumMeshSlope; i++)
	{
		D3D11_MAPPED_SUBRESOURCE msrr;
		GetDeviceContext()->Map(g_aMeshFieldSlope[i].vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msrr);

		VERTEX_3D* pVtxx = (VERTEX_3D*)msrr.pData;

		D3DXVECTOR3 start = pos;
		D3DXVECTOR3 end = pos;

		// 少し上から、ズドーンと下へレイを飛ばす
		start.y += 100.0f;
		end.y -= 1000.0f;


		for (int z = 0; z < g_aMeshFieldSlope[i].nNumBlockZ; z++)
		{
			for (int x = 0; x < g_aMeshFieldSlope[i].nNumBlockX; x++)
			{
				D3DXVECTOR3 p0 = pVtxx[z * (g_aMeshFieldSlope[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p1 = pVtxx[z * (g_aMeshFieldSlope[i].nNumBlockX + 1) + (x + 1)].Position;
				D3DXVECTOR3 p2 = pVtxx[(z + 1) * (g_aMeshFieldSlope[i].nNumBlockX + 1) + x].Position;
				D3DXVECTOR3 p3 = pVtxx[(z + 1) * (g_aMeshFieldSlope[i].nNumBlockX + 1) + (x + 1)].Position;

				bool ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
				D3DXVECTOR3 OK = *HitPosition;
				if (ans)
				{
					if (pos.y >= OK.y && pos.y <= (OK.y + 2.0f)) {
						*HitPosition = OK;

						return true;
					}

				}

				ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
				if (ans)
				{
					if (pos.y >= OK.y && pos.y <= (OK.y + 2.0f)) {
						*HitPosition = OK;

						return true;
					}
				}
			}

		}
	}
	return false;
}

bool RayHitTransparent(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3)
{
	D3DXVECTOR3 HitPosition;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 start = pos0;
	D3DXVECTOR3 end = pos1;
	{
		bool ans = RayCast(p0, p2, p1, start, end, &HitPosition, &Normal);
		if (ans)
		{
			return true;
		}
		ans = RayCast(p1, p2, p3, start, end, &HitPosition, &Normal);
		if (ans)
		{
			return true;
		}
	}
	
	
	return false;
}



MESH_MAP *Getwall()
{
	return (&g_aMeshWall[0]);
}