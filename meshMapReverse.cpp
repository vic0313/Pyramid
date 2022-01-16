//=============================================================================
//
// 半透明地図の処理 [meshMapReverse.cpp]
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
#include "Map.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_FIELD_MAX		(1)				// テクスチャの数
#define TEXTURE_WALL_MAX			(1)						// テクスチャの数
#define TEXTURE_CEILING_MAX 		(1)				// テクスチャの数
#define TEXTURE_SLOPE_MAX		(1)				// テクスチャの数
#define TEXTURE_SLOPEWALL_MAX					(1)		// テクスチャの数

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

static MAP_POINT	*g_fieldpointReverse = GetFieldPointReverse();
static MAP_POINT	*g_ceilingpointReverse = GetCeilingPointReverse();
static MAP_POINT	*g_wallpointReverse = GetWallPointReverse();
static MAP_POINT	*g_slopewallpointReverse = GetSlopeWallPointReverse();


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

static MESH_MAP g_aMeshFieldReverse[MAX_MESH_FIELD];		// メッシュ地面ワーク
static int g_nNumMeshField = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshWallReverse[MAX_MESH_WALL];		// メッシュ壁ワーク
static int g_nNumMeshWall = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldReverseCeiling[MAX_MESH_FIELD_CEILING];		// メッシュ地面ワーク
static int g_nNumMeshCeiling = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldReverseSlope[MAX_MESH_FIELD_SLOPE];		// メッシュ地面ワーク
static int g_nNumMeshSlope = 0;						// メッシュ壁の数
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// メッシュ壁の数

static char* g_TextureName[] = {
	"data/TEXTURE/wall004.jpg",
};
static char* g_TextureWallName[] = {
	"data/TEXTURE/wall000.jpg",
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

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshFieldReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY)
{
	MESH_MAP *pMesh_FieldReverse;

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

	pMesh_FieldReverse = &g_aMeshFieldReverse[g_nNumMeshField];
	g_nNumMeshField++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_FieldReverse->material, sizeof(pMesh_FieldReverse->material));
	pMesh_FieldReverse->material.Diffuse = col;

	// ポリゴン表示位置の中心座標を設定
	pMesh_FieldReverse->pos = pos;

	pMesh_FieldReverse->rot = rot;

	pMesh_FieldReverse->cameracheck = false;
	// ブロック数の設定
	pMesh_FieldReverse->nNumBlockX = nNumBlockX;
	pMesh_FieldReverse->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_FieldReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_FieldReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_FieldReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_FieldReverse->fBlockSizeX = fBlockSizeX;
	pMesh_FieldReverse->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldReverse->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldReverse->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_FieldReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldReverse->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_FieldReverse->fBlockSizeX; //+(pMesh_FieldReverse->nNumBlockX) * pMesh_FieldReverse->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.y = startY;
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldReverse->fBlockSizeZ; //+pMesh_FieldReverse->nNumBlockZ * pMesh_FieldReverse->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldReverse->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldReverse->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldReverse->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_FieldReverse->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + pMesh_FieldReverse->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldReverse->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshWallReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ)
{
	MESH_MAP *pMesh_WallReverse;

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

	pMesh_WallReverse = &g_aMeshWallReverse[g_nNumMeshWall];

	g_nNumMeshWall++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_WallReverse->material, sizeof(pMesh_WallReverse->material));
	pMesh_WallReverse->material.Diffuse = col;

	pMesh_WallReverse->cameracheck = false;
	// ポリゴン表示位置の中心座標を設定
	pMesh_WallReverse->pos = pos;

	pMesh_WallReverse->rot = rot;

	// ブロック数の設定
	pMesh_WallReverse->nNumBlockX = nNumBlockX;
	pMesh_WallReverse->nNumBlockY = nNumBlockY;

	// 頂点数の設定
	pMesh_WallReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// インデックス数の設定
	pMesh_WallReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// ポリゴン数の設定
	pMesh_WallReverse->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// ブロックサイズの設定
	pMesh_WallReverse->fBlockSizeX = fBlockSizeX;
	pMesh_WallReverse->fBlockSizeY = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_WallReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_WallReverse->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_WallReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_WallReverse->indexBuffer);

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
		GetDeviceContext()->Map(pMesh_WallReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxY = 0; nCntVtxY < (pMesh_WallReverse->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_WallReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh_WallReverse->nNumBlockX / 2.0f) * pMesh_WallReverse->fBlockSizeX + nCntVtxX * pMesh_WallReverse->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh_WallReverse->nNumBlockY * pMesh_WallReverse->fBlockSizeY - nCntVtxY * pMesh_WallReverse->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// 法線の設定
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh_WallReverse->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_WallReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh_WallReverse->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh_WallReverse->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_WallReverse->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxY < (pMesh_WallReverse->nNumBlockY - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + pMesh_WallReverse->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_WallReverse->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshFieldCeilingReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ)
{
	MESH_MAP *pMesh_FieldCeilingReverse;

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

	pMesh_FieldCeilingReverse = &g_aMeshFieldReverseCeiling[g_nNumMeshCeiling];
	g_nNumMeshCeiling++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_FieldCeilingReverse->material, sizeof(pMesh_FieldCeilingReverse->material));
	pMesh_FieldCeilingReverse->material.Diffuse = col;

	pMesh_FieldCeilingReverse->cameracheck = false;
	// ポリゴン表示位置の中心座標を設定
	pMesh_FieldCeilingReverse->pos = pos;

	pMesh_FieldCeilingReverse->rot = rot;


	// ブロック数の設定
	pMesh_FieldCeilingReverse->nNumBlockX = nNumBlockX;
	pMesh_FieldCeilingReverse->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_FieldCeilingReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_FieldCeilingReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_FieldCeilingReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_FieldCeilingReverse->fBlockSizeX = fBlockSizeX;
	pMesh_FieldCeilingReverse->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldCeilingReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeilingReverse->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldCeilingReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeilingReverse->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_FieldCeilingReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldCeilingReverse->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeilingReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_FieldCeilingReverse->fBlockSizeX; //+(pMesh_FieldCeilingReverse->nNumBlockX) * pMesh_FieldCeilingReverse->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldCeilingReverse->fBlockSizeZ; //+pMesh_FieldCeilingReverse->nNumBlockZ * pMesh_FieldCeilingReverse->fBlockSizeZ

				// 法線の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeilingReverse->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeilingReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldCeilingReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldCeilingReverse->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeilingReverse->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_FieldCeilingReverse->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + pMesh_FieldCeilingReverse->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeilingReverse->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitMeshSlopeReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ,
	int startX, int startZ, int startY, int endY, int slopecheck)
{

	MESH_MAP *pMesh_SlopeReverse;

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

	pMesh_SlopeReverse = &g_aMeshFieldReverseSlope[g_nNumMeshSlope];
	g_nNumMeshSlope++;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_SlopeReverse->material, sizeof(pMesh_SlopeReverse->material));
	pMesh_SlopeReverse->material.Diffuse = col;

	pMesh_SlopeReverse->cameracheck = false;
	// ポリゴン表示位置の中心座標を設定
	pMesh_SlopeReverse->pos = pos;

	pMesh_SlopeReverse->rot = rot;

	pMesh_SlopeReverse->yblock = endY - startY;
	pMesh_SlopeReverse->fBlockSizeY_X = float((pMesh_SlopeReverse->yblock + 1.0) / nNumBlockX);
	pMesh_SlopeReverse->fBlockSizeY_Z = float((pMesh_SlopeReverse->yblock + 1.0) / nNumBlockZ);

	// ブロック数の設定
	pMesh_SlopeReverse->nNumBlockX = nNumBlockX;
	pMesh_SlopeReverse->nNumBlockZ = nNumBlockZ;

	// 頂点数の設定
	pMesh_SlopeReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	pMesh_SlopeReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	pMesh_SlopeReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	pMesh_SlopeReverse->fBlockSizeX = fBlockSizeX;
	pMesh_SlopeReverse->fBlockSizeZ = fBlockSizeZ;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeReverse->vertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_SlopeReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeReverse->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_SlopeReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		D3DXVECTOR3		nor,p0,p1;
		//if slopecheck=1 ->左下から右上の坂道
		//if slopecheck=2 ->右下から左上の坂道
		//if slopecheck=3 ->上上から下下の坂道
		//if slopecheck=4 ->下上から上下の坂道
		switch (int check = slopecheck)
		{
		case 1:
			p0 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)- D3DXVECTOR3((float)(startX+ fBlockSizeX), (float)(startY + pMesh_SlopeReverse->fBlockSizeY_X), (float)startZ) ;
			p1 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)- D3DXVECTOR3((float)startX, (float)startY, (float)(startZ- fBlockSizeZ)) ;
			
			break;
		case 2:
			 p0 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)-D3DXVECTOR3((float)(startX + fBlockSizeX), (float)(startY - pMesh_SlopeReverse->fBlockSizeY_X), (float)startZ) ;
			 p1 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)-D3DXVECTOR3((float)startX, (float)startY, (float)(startZ - fBlockSizeZ)) ;
			
			break;
		case 3:
			p0 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)-D3DXVECTOR3((float)(startX + fBlockSizeX), (float)startY , (float)startZ) ;
			p1 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)- D3DXVECTOR3((float)startX, (float)(startY - pMesh_SlopeReverse->fBlockSizeY_Z), (float)(startZ - fBlockSizeZ)) ;
			
			break;
		case 4:
			p0 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)-D3DXVECTOR3((float)(startX + fBlockSizeX), (float)startY , (float)startZ) ;
			p1 = D3DXVECTOR3((float)startX, (float)startY, (float)startZ)-D3DXVECTOR3((float)startX, (float)(startY + pMesh_SlopeReverse->fBlockSizeY_Z), (float)(startZ - fBlockSizeZ)) ;
			
			break;
		default:
			break;
		}
		crossProduct(&nor, &p0, &p1);


		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_SlopeReverse->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_SlopeReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_SlopeReverse->fBlockSizeX; //+(pMesh_SlopeReverse->nNumBlockX) * pMesh_SlopeReverse->fBlockSizeX
					//if slopecheck=1 ->左下から右上の坂道
					//if slopecheck=2 ->右下から左上の坂道
					//if slopecheck=3 ->上上から下下の坂道
					//if slopecheck=4 ->下上から上下の坂道
				switch (int check = slopecheck)
				{
				case 1:
					pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.y = startY + nCntVtxX * pMesh_SlopeReverse->fBlockSizeY_X - 1.0f;
					
					break;
				case 2:
					pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.y = endY - nCntVtxX * pMesh_SlopeReverse->fBlockSizeY_X - 1.0f;
					break;
				case 3:
					pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.y = endY - nCntVtxZ * pMesh_SlopeReverse->fBlockSizeY_X - 1.0f;
					break;
				case 4:
					pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.y = startY + nCntVtxZ * pMesh_SlopeReverse->fBlockSizeY_X - 1.0f;
					break;
				default:
					break;
				}

				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_SlopeReverse->fBlockSizeZ; //+pMesh_SlopeReverse->nNumBlockZ * pMesh_SlopeReverse->fBlockSizeZ

				// 法線の設定
				//pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Normal = nor;

				// 拡散光の設定
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_SlopeReverse->vertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める
		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_SlopeReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_SlopeReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_SlopeReverse->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh_SlopeReverse->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (pMesh_SlopeReverse->nNumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + pMesh_SlopeReverse->nNumBlockX;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(pMesh_SlopeReverse->indexBuffer, 0);
	}

	return S_OK;
}

HRESULT InitSlopeWallReverse(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	MESH_MAP *pMesh_SlopeWallReverse;

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

	pMesh_SlopeWallReverse = &g_slopewall[g_nNumMeshSlopeWall];
	g_nNumMeshSlopeWall++;

	pMesh_SlopeWallReverse->cameracheck = false;
	// ポリゴン表示位置の中心座標を設定
	pMesh_SlopeWallReverse->pos = possize;

	pMesh_SlopeWallReverse->rot = rot;

	// マテリアル情報の初期化
	ZeroMemory(&pMesh_SlopeWallReverse->material, sizeof(pMesh_SlopeWallReverse->material));
	pMesh_SlopeWallReverse->material.Diffuse = col;

	// ブロック数の設定
	pMesh_SlopeWallReverse->nNumBlockX = 1;
	pMesh_SlopeWallReverse->nNumBlockZ = 1;

	// 頂点数の設定
	pMesh_SlopeWallReverse->nNumVertex = 4;

	// インデックス数の設定
	pMesh_SlopeWallReverse->nNumVertexIndex = 4;

	// ポリゴン数の設定
	pMesh_SlopeWallReverse->nNumPolygon = 2;



	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeWallReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeWallReverse->vertexBuffer);
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
		GetDeviceContext()->Map(pMesh_SlopeWallReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// 頂点座標の設定
		// 頂点０番（左上の頂点）
		pVtx[0].Position = pos0 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点１番（右上の頂点）
		pVtx[1].Position = pos1 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点２番（左下の頂点）
		pVtx[2].Position = pos2 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点３番（右下の頂点）
		pVtx[3].Position = pos3 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		GetDeviceContext()->Unmap(pMesh_SlopeWallReverse->vertexBuffer, 0);

	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshFieldReverse(void)
{

	MESH_MAP *pMesh_FieldReverse;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh_FieldReverse = &g_aMeshFieldReverse[nCntMeshField];

		if (pMesh_FieldReverse->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_FieldReverse->vertexBuffer->Release();
			pMesh_FieldReverse->vertexBuffer = NULL;
		}

		if (pMesh_FieldReverse->indexBuffer)
		{// インデックスバッファの解放
			pMesh_FieldReverse->indexBuffer->Release();
			pMesh_FieldReverse->indexBuffer = NULL;
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

void UninitMeshWallReverse(void)
{
	MESH_MAP *pMesh_WallReverse;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh_WallReverse = &g_aMeshWallReverse[nCntMeshField];

		if (pMesh_WallReverse->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_WallReverse->vertexBuffer->Release();
			pMesh_WallReverse->vertexBuffer = NULL;
		}

		if (pMesh_WallReverse->indexBuffer)
		{// インデックスバッファの解放
			pMesh_WallReverse->indexBuffer->Release();
			pMesh_WallReverse->indexBuffer = NULL;
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

void UninitMeshFieldCeilingReverse(void)
{

	MESH_MAP *pMesh_FieldCeilingReverse;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshCeiling; nCntMeshField++)
	{
		pMesh_FieldCeilingReverse = &g_aMeshFieldReverseCeiling[nCntMeshField];

		if (pMesh_FieldCeilingReverse->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_FieldCeilingReverse->vertexBuffer->Release();
			pMesh_FieldCeilingReverse->vertexBuffer = NULL;
		}

		if (pMesh_FieldCeilingReverse->indexBuffer)
		{// インデックスバッファの解放
			pMesh_FieldCeilingReverse->indexBuffer->Release();
			pMesh_FieldCeilingReverse->indexBuffer = NULL;
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

void UninitMeshSlopeReverse(void)
{

	MESH_MAP *pMesh_SlopeReverse;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshSlope; nCntMeshField++)
	{
		pMesh_SlopeReverse = &g_aMeshFieldReverseSlope[nCntMeshField];

		if (pMesh_SlopeReverse->vertexBuffer)
		{// 頂点バッファの解放
			pMesh_SlopeReverse->vertexBuffer->Release();
			pMesh_SlopeReverse->vertexBuffer = NULL;
		}

		if (pMesh_SlopeReverse->indexBuffer)
		{// インデックスバッファの解放
			pMesh_SlopeReverse->indexBuffer->Release();
			pMesh_SlopeReverse->indexBuffer = NULL;
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

void UninitSlopeWallReverse(void)
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
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshFieldReverse(void)
{
	
}

void UpdateMeshWallReverse(void)
{
}

void UpdateMeshFieldCeilingReverse(void)
{
}

void UpdateMeshSlopeReverse(void)
{
}

void UpdateSlopeWallReverse(void)
{


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshFieldReverse(void)
{
	MESH_MAP *pMesh_FieldReverse;
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshField; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverse[i].cameracheck == false) { continue; }
		}
		pMesh_FieldReverse = &g_aMeshFieldReverse[i];

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldReverse->vertexBuffer, &stride, &offset);

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(pMesh_FieldReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// マテリアル設定
		SetMaterial(pMesh_FieldReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh_FieldReverse->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldReverse->rot.y, pMesh_FieldReverse->rot.x, pMesh_FieldReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_FieldReverse->mtxWorld, &pMesh_FieldReverse->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldReverse->pos.x, pMesh_FieldReverse->pos.y, pMesh_FieldReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_FieldReverse->mtxWorld, &pMesh_FieldReverse->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&pMesh_FieldReverse->mtxWorld);

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(pMesh_FieldReverse->nNumVertexIndex, 0, 0);
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshWallReverse(void)
{
	MESH_MAP *pMesh_WallReverse;
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshWallReverse[i].cameracheck == false) { continue; }
		}
		pMesh_WallReverse = &g_aMeshWallReverse[i];

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_WallReverse->vertexBuffer, &stride, &offset);

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(pMesh_WallReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

		// マテリアル設定
		SetMaterial(pMesh_WallReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh_WallReverse->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_WallReverse->rot.y, pMesh_WallReverse->rot.x, pMesh_WallReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_WallReverse->mtxWorld, &pMesh_WallReverse->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh_WallReverse->pos.x, pMesh_WallReverse->pos.y, pMesh_WallReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_WallReverse->mtxWorld, &pMesh_WallReverse->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&pMesh_WallReverse->mtxWorld);

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(pMesh_WallReverse->nNumVertexIndex, 0, 0);
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshFieldCeilingReverse(void)
{
	MESH_MAP *pMesh_FieldCeilingReverse;
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshCeiling; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverseCeiling[i].cameracheck == false) { continue; }
		}
		pMesh_FieldCeilingReverse = &g_aMeshFieldReverseCeiling[i];

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeilingReverse->vertexBuffer, &stride, &offset);

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeilingReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

		// マテリアル設定
		SetMaterial(pMesh_FieldCeilingReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh_FieldCeilingReverse->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeilingReverse->rot.y, pMesh_FieldCeilingReverse->rot.x, pMesh_FieldCeilingReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_FieldCeilingReverse->mtxWorld, &pMesh_FieldCeilingReverse->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeilingReverse->pos.x, pMesh_FieldCeilingReverse->pos.y, pMesh_FieldCeilingReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_FieldCeilingReverse->mtxWorld, &pMesh_FieldCeilingReverse->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&pMesh_FieldCeilingReverse->mtxWorld);

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(pMesh_FieldCeilingReverse->nNumVertexIndex, 0, 0);
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawMeshSlopeReverse(void)
{
	MESH_MAP *pMesh_SlopeReverse;
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshSlope; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverseSlope[i].cameracheck == false) { continue; }
		}
		pMesh_SlopeReverse = &g_aMeshFieldReverseSlope[i];

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_SlopeReverse->vertexBuffer, &stride, &offset);

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(pMesh_SlopeReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

		// マテリアル設定
		SetMaterial(pMesh_SlopeReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh_SlopeReverse->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_SlopeReverse->rot.y, pMesh_SlopeReverse->rot.x, pMesh_SlopeReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_SlopeReverse->mtxWorld, &pMesh_SlopeReverse->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh_SlopeReverse->pos.x, pMesh_SlopeReverse->pos.y, pMesh_SlopeReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_SlopeReverse->mtxWorld, &pMesh_SlopeReverse->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&pMesh_SlopeReverse->mtxWorld);

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(pMesh_SlopeReverse->nNumVertexIndex, 0, 0);
	}

	// ライティングオン
	SetLightEnable(true);
}

void DrawSlopeWallReverse(void)
{
	int i;

	// ライティングオフ
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].cameracheck == false) { continue; }
		}

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

	// ライティングオン
	SetLightEnable(true);
}

/*
g_fieldpointReverse = GetFieldPointReverse();
g_ceilingpointReverse = GetCeilingPointReverse();
g_wallpointReverse = GetWallPointReverse();
g_slopewallpointReverse = GetSlopeWallPointReverse();

static MESH_MAP g_aMeshFieldReverse[MAX_MESH_FIELD];		// メッシュ地面ワーク
static int g_nNumMeshField = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshWallReverse[MAX_MESH_WALL];		// メッシュ壁ワーク
static int g_nNumMeshWall = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldReverseCeiling[MAX_MESH_FIELD_CEILING];		// メッシュ地面ワーク
static int g_nNumMeshCeiling = 0;						// メッシュ壁の数
static MESH_MAP g_aMeshFieldReverseSlope[MAX_MESH_FIELD_SLOPE];		// メッシュ地面ワーク
static int g_nNumMeshSlope = 0;						// メッシュ壁の数
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// メッシュ壁の数

*/
//=============================================================================
// カメラのCHECK
//=============================================================================
void MapTransparentCheckReverse(void)
{
	for(int i=0;i< g_nNumMeshField;i++)
	{
		bool frontcheck = false;//trueならば、PLAYERの前です

		{

			D3DXVECTOR3 pp0 = g_fieldpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_fieldpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_fieldpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_fieldpointReverse[i].pos3;
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
		//前後判定
		if (frontcheck == true)
		{
			g_aMeshFieldReverse[i].cameracheck = true;
		}
		else
		{
			g_aMeshFieldReverse[i].cameracheck = false;
		}
	}
	
	for (int i = 0; i < g_nNumMeshWall; i++)
	{
		bool frontcheck = false;//trueならば、PLAYERの前です
		{
			D3DXVECTOR3 pp0 = g_wallpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_wallpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_wallpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_wallpointReverse[i].pos3;
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
			g_aMeshWallReverse[i].cameracheck = true;
		}
		else
		{
			g_aMeshWallReverse[i].cameracheck = false;
		}
	}

	for (int i = 0; i < g_nNumMeshCeiling; i++)
	{
		bool frontcheck = false;//trueならば、PLAYERの前です

		{

			D3DXVECTOR3 pp0 = g_ceilingpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_ceilingpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_ceilingpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_ceilingpointReverse[i].pos3;

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
			g_aMeshFieldReverseCeiling[i].cameracheck = true;
		}
		else
		{
			g_aMeshFieldReverseCeiling[i].cameracheck = false;
		}
	}

	for (int i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		bool frontcheck = false;//trueならば、PLAYERの前です

		{

			D3DXVECTOR3 pp0 = g_slopewallpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_slopewallpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_slopewallpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_slopewallpointReverse[i].pos3;

			D3DXVECTOR3 HitPosition;
			D3DXVECTOR3 Normal;

			switch (g_slopewallpointReverse[i].check1)
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
