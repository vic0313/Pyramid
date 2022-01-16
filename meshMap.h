//=============================================================================
//
// 地図の処理 [meshMap.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"

#include "input.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MESH_FIELD				(50)					// 壁の総数
#define	MAX_MESH_WALL				(200)					// 壁の総数
#define	MAX_MESH_FIELD_CEILING		(22)					// 壁の総数
#define	MAX_MESH_FIELD_SLOPE		(22)					// 壁の総数
#define MAX_MESH_SLOPE_WALL			(14)
#define MAX_MESH_TRIANGLE_WALL		(4)
#define MAX_MESH_BOSS_FIELD			(1)	
#define	MAX_MESH_RANDOM_FIELD		(6)					// 壁の総数
//
//#define TEXTURE_MAX					(1)
//#define MAX_Transparent				(20)
//#define Transparent_pos				(50.0f)
//#define	TREE_WIDTH			(80.0f)			// 頂点サイズ
//#define	TREE_HEIGHT			(80.0f)			// 頂点サイズ
//#define	RADIUS			(1000.0f)			// 頂点サイズ

#define BOSS_WAVE_AMPLITUDE			(20.0f)	
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	ID3D11Buffer	*vertexBuffer;	// 頂点バッファ
	ID3D11Buffer	*indexBuffer;	// インデックスバッファ
	int				texnum;
	D3DXMATRIX		mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3		pos;						// ポリゴン表示位置の中心座標
	D3DXVECTOR3		rot;						// ポリゴンの回転角
	MATERIAL		material;					// マテリアル
	int				nNumBlockX, nNumBlockZ,nNumBlockY;		// ブロック数
	int				nNumVertex;					// 総頂点数	
	int				nNumVertexIndex;			// 総インデックス数
	int				nNumPolygon;				// 総ポリゴン数
	float			fBlockSizeX, fBlockSizeZ,fBlockSizeY;	// ブロックサイズ
	int				yblock;
	float			fBlockSizeY_X;				//右左向けのSLOPE
	float			fBlockSizeY_Z;				//上下向けのSLOPE
	//VERTEX_3D		*g_Vertex ;
	bool			use;
	bool			cameracheck;	//trueならば、PLAYERの前です
	bool			DrawCheck;		//TRUEならば、カメラの範囲外
} MESH_MAP;

//typedef struct
//{
//	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
//	D3DXVECTOR3 pos;			// 位置
//	D3DXVECTOR3 scl;			// スケール
//	MATERIAL	material;		// マテリアル
//	float		fWidth;			// 幅
//	float		fHeight;		// 高さ
//	int			nIdxShadow;		// 影ID
//	bool		bUse;			// 使用しているかどうか
//
//}Transparent;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ, float startY);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void DrawMeshField_Behind(void);
bool RayHitField(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal);


HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void DrawMeshWall_Behind(void);

HRESULT InitMeshFieldCeiling(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ);
void UninitMeshFieldCeiling(void);
void UpdateMeshFieldCeiling(void);
void DrawMeshFieldCeiling(void);
void DrawMeshFieldCeiling_Behind(void);

bool RayHitFieldCeiling(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal);

HRESULT InitMeshSlope(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int startX, int startZ, int startY, int endY, int slopecheck);
void UninitMeshSlope(void);
void UpdateMeshSlope(void);
void DrawMeshSlope(void);
void DrawMeshSlope_Behind(void);

bool RayHitFieldSlope(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal);

HRESULT InitSlopeWall(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col);
void UninitSlopeWall(void);
void UpdateSlopeWall(void);
void DrawSlopeWall(void);
void DrawSlopeWall_Behind(void);

HRESULT InitBOSSField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ);
void UninitBOSSField(void);
void UpdateBOSSField(void);
//void DrawBOSSField(void);

HRESULT InitMeshField_Random(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY, int texnum);
void UninitMeshField_Random(void);
void UpdateMeshField_Random(void);
//void DrawMeshField_Random(void);

HRESULT InitTriangleWall(D3DXVECTOR3 possize, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 rot, D3DXCOLOR col);
void UninitTriangleWall(void);
void UpdateTriangleWall(void);
//void DrawTriangleWall(void);

bool RayHitBOSSField(D3DXVECTOR3 pos, D3DXVECTOR3 *HitPosition, D3DXVECTOR3 *Normal);


void DrawMeshMapSize(void);
void DrawMeshMapRestarea(void);
void DrawMeshMapBattlearea(void);
//MESH_MAP *GetBOSSField(void);

MESH_MAP *Getwall(void);


void MapTransparentCheck(void);
bool RayHitTransparent(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3);



