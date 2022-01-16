//=============================================================================
//
// �������n�}�̏��� [meshMapReverse.cpp]
// Author : GP11A132 43 �я���
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_FIELD_MAX		(1)				// �e�N�X�`���̐�
#define TEXTURE_WALL_MAX			(1)						// �e�N�X�`���̐�
#define TEXTURE_CEILING_MAX 		(1)				// �e�N�X�`���̐�
#define TEXTURE_SLOPE_MAX		(1)				// �e�N�X�`���̐�
#define TEXTURE_SLOPEWALL_MAX					(1)		// �e�N�X�`���̐�

#define	VALUE_MOVE_FIELD		(5.0f)					// �ړ����x
#define	VALUE_ROTATE_FIELD	(D3DX_PI * 0.001f)		// ��]���x
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER  *player = GetPlayer();
static ENEMY   *enemytypeA = GetEnemyTypeA();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();

static MAP_POINT	*g_fieldpointReverse = GetFieldPointReverse();
static MAP_POINT	*g_ceilingpointReverse = GetCeilingPointReverse();
static MAP_POINT	*g_wallpointReverse = GetWallPointReverse();
static MAP_POINT	*g_slopewallpointReverse = GetSlopeWallPointReverse();


static ID3D11ShaderResourceView		*g_Texture[TEXTURE_FIELD_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;	// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView		*g_Texture_Wall[TEXTURE_WALL_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexWallNo;		// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView		*g_Texture_Ceiling[TEXTURE_CEILING_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo_Ceiling;	// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView		*g_Texture_Slope[TEXTURE_SLOPE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo_Slope;	// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView	*g_Texture_SlopeWall[TEXTURE_SLOPEWALL_MAX] = { NULL };	// �e�N�X�`�����
static int						g_TexNo_SlopeWall;	// �e�N�X�`���ԍ�

static MESH_MAP g_aMeshFieldReverse[MAX_MESH_FIELD];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshField = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshWallReverse[MAX_MESH_WALL];		// ���b�V���ǃ��[�N
static int g_nNumMeshWall = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldReverseCeiling[MAX_MESH_FIELD_CEILING];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshCeiling = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldReverseSlope[MAX_MESH_FIELD_SLOPE];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshSlope = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// ���b�V���ǂ̐�

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
// ����������
//=============================================================================
HRESULT InitMeshFieldReverse(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY)
{
	MESH_MAP *pMesh_FieldReverse;

	if (g_nNumMeshField >= MAX_MESH_FIELD)
	{
		return E_FAIL;
	}


	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_FieldReverse->material, sizeof(pMesh_FieldReverse->material));
	pMesh_FieldReverse->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_FieldReverse->pos = pos;

	pMesh_FieldReverse->rot = rot;

	pMesh_FieldReverse->cameracheck = false;
	// �u���b�N���̐ݒ�
	pMesh_FieldReverse->nNumBlockX = nNumBlockX;
	pMesh_FieldReverse->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_FieldReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_FieldReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_FieldReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_FieldReverse->fBlockSizeX = fBlockSizeX;
	pMesh_FieldReverse->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldReverse->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldReverse->indexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldReverse->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_FieldReverse->fBlockSizeX; //+(pMesh_FieldReverse->nNumBlockX) * pMesh_FieldReverse->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.y = startY;
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldReverse->fBlockSizeZ; //+pMesh_FieldReverse->nNumBlockZ * pMesh_FieldReverse->fBlockSizeZ

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldReverse->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_WallReverse->material, sizeof(pMesh_WallReverse->material));
	pMesh_WallReverse->material.Diffuse = col;

	pMesh_WallReverse->cameracheck = false;
	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_WallReverse->pos = pos;

	pMesh_WallReverse->rot = rot;

	// �u���b�N���̐ݒ�
	pMesh_WallReverse->nNumBlockX = nNumBlockX;
	pMesh_WallReverse->nNumBlockY = nNumBlockY;

	// ���_���̐ݒ�
	pMesh_WallReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_WallReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_WallReverse->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_WallReverse->fBlockSizeX = fBlockSizeX;
	pMesh_WallReverse->fBlockSizeY = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_WallReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_WallReverse->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_WallReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_WallReverse->indexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_WallReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxY = 0; nCntVtxY < (pMesh_WallReverse->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_WallReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh_WallReverse->nNumBlockX / 2.0f) * pMesh_WallReverse->fBlockSizeX + nCntVtxX * pMesh_WallReverse->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh_WallReverse->nNumBlockY * pMesh_WallReverse->fBlockSizeY - nCntVtxY * pMesh_WallReverse->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// �@���̐ݒ�
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh_WallReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh_WallReverse->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_WallReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh_WallReverse->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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


	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_FieldCeilingReverse->material, sizeof(pMesh_FieldCeilingReverse->material));
	pMesh_FieldCeilingReverse->material.Diffuse = col;

	pMesh_FieldCeilingReverse->cameracheck = false;
	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_FieldCeilingReverse->pos = pos;

	pMesh_FieldCeilingReverse->rot = rot;


	// �u���b�N���̐ݒ�
	pMesh_FieldCeilingReverse->nNumBlockX = nNumBlockX;
	pMesh_FieldCeilingReverse->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_FieldCeilingReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_FieldCeilingReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_FieldCeilingReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_FieldCeilingReverse->fBlockSizeX = fBlockSizeX;
	pMesh_FieldCeilingReverse->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldCeilingReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeilingReverse->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldCeilingReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeilingReverse->indexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeilingReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldCeilingReverse->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeilingReverse->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_FieldCeilingReverse->fBlockSizeX; //+(pMesh_FieldCeilingReverse->nNumBlockX) * pMesh_FieldCeilingReverse->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldCeilingReverse->fBlockSizeZ; //+pMesh_FieldCeilingReverse->nNumBlockZ * pMesh_FieldCeilingReverse->fBlockSizeZ

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldCeilingReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeilingReverse->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeilingReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldCeilingReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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


	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_SlopeReverse->material, sizeof(pMesh_SlopeReverse->material));
	pMesh_SlopeReverse->material.Diffuse = col;

	pMesh_SlopeReverse->cameracheck = false;
	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_SlopeReverse->pos = pos;

	pMesh_SlopeReverse->rot = rot;

	pMesh_SlopeReverse->yblock = endY - startY;
	pMesh_SlopeReverse->fBlockSizeY_X = float((pMesh_SlopeReverse->yblock + 1.0) / nNumBlockX);
	pMesh_SlopeReverse->fBlockSizeY_Z = float((pMesh_SlopeReverse->yblock + 1.0) / nNumBlockZ);

	// �u���b�N���̐ݒ�
	pMesh_SlopeReverse->nNumBlockX = nNumBlockX;
	pMesh_SlopeReverse->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_SlopeReverse->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_SlopeReverse->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_SlopeReverse->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_SlopeReverse->fBlockSizeX = fBlockSizeX;
	pMesh_SlopeReverse->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeReverse->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_SlopeReverse->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeReverse->indexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_SlopeReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		D3DXVECTOR3		nor,p0,p1;
		//if slopecheck=1 ->��������E��̍⓹
		//if slopecheck=2 ->�E�����獶��̍⓹
		//if slopecheck=3 ->��ォ�牺���̍⓹
		//if slopecheck=4 ->���ォ��㉺�̍⓹
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
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_SlopeReverse->fBlockSizeX; //+(pMesh_SlopeReverse->nNumBlockX) * pMesh_SlopeReverse->fBlockSizeX
					//if slopecheck=1 ->��������E��̍⓹
					//if slopecheck=2 ->�E�����獶��̍⓹
					//if slopecheck=3 ->��ォ�牺���̍⓹
					//if slopecheck=4 ->���ォ��㉺�̍⓹
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

				// �@���̐ݒ�
				//pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Normal = nor;

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_SlopeReverse->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_SlopeReverse->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_SlopeReverse->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_SlopeReverse->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	//�e�N�X�`������
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
	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_SlopeWallReverse->pos = possize;

	pMesh_SlopeWallReverse->rot = rot;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_SlopeWallReverse->material, sizeof(pMesh_SlopeWallReverse->material));
	pMesh_SlopeWallReverse->material.Diffuse = col;

	// �u���b�N���̐ݒ�
	pMesh_SlopeWallReverse->nNumBlockX = 1;
	pMesh_SlopeWallReverse->nNumBlockZ = 1;

	// ���_���̐ݒ�
	pMesh_SlopeWallReverse->nNumVertex = 4;

	// �C���f�b�N�X���̐ݒ�
	pMesh_SlopeWallReverse->nNumVertexIndex = 4;

	// �|���S�����̐ݒ�
	pMesh_SlopeWallReverse->nNumPolygon = 2;



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeWallReverse->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeWallReverse->vertexBuffer);
	//// �C���f�b�N�X�o�b�t�@����
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	//bd.ByteWidth = sizeof(unsigned short) * g_slopewall[g_nNumMeshSlopeWall].nNumVertexIndex;
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//GetDevice()->CreateBuffer(&bd, NULL, &g_slopewall[g_nNumMeshSlopeWall].indexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif
		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_SlopeWallReverse->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// ���_���W�̐ݒ�
		// ���_�O�ԁi����̒��_�j
		pVtx[0].Position = pos0 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�P�ԁi�E��̒��_�j
		pVtx[1].Position = pos1 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�Q�ԁi�����̒��_�j
		pVtx[2].Position = pos2 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�R�ԁi�E���̒��_�j
		pVtx[3].Position = pos3 - D3DXVECTOR3(1.0f, 1.0f, 1.0f);;
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		GetDeviceContext()->Unmap(pMesh_SlopeWallReverse->vertexBuffer, 0);

	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitMeshFieldReverse(void)
{

	MESH_MAP *pMesh_FieldReverse;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh_FieldReverse = &g_aMeshFieldReverse[nCntMeshField];

		if (pMesh_FieldReverse->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh_FieldReverse->vertexBuffer->Release();
			pMesh_FieldReverse->vertexBuffer = NULL;
		}

		if (pMesh_FieldReverse->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_FieldReverse->indexBuffer->Release();
			pMesh_FieldReverse->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_FIELD_MAX; i++)
	{
		if (g_Texture[i])	g_Texture[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
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
		{// ���_�o�b�t�@�̉��
			pMesh_WallReverse->vertexBuffer->Release();
			pMesh_WallReverse->vertexBuffer = NULL;
		}

		if (pMesh_WallReverse->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_WallReverse->indexBuffer->Release();
			pMesh_WallReverse->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_WALL_MAX; i++)
	{
		if (g_Texture_Wall[i])	g_Texture_Wall[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
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
		{// ���_�o�b�t�@�̉��
			pMesh_FieldCeilingReverse->vertexBuffer->Release();
			pMesh_FieldCeilingReverse->vertexBuffer = NULL;
		}

		if (pMesh_FieldCeilingReverse->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_FieldCeilingReverse->indexBuffer->Release();
			pMesh_FieldCeilingReverse->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_CEILING_MAX; i++)
	{
		if (g_Texture_Ceiling[i])	g_Texture_Ceiling[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
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
		{// ���_�o�b�t�@�̉��
			pMesh_SlopeReverse->vertexBuffer->Release();
			pMesh_SlopeReverse->vertexBuffer = NULL;
		}

		if (pMesh_SlopeReverse->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_SlopeReverse->indexBuffer->Release();
			pMesh_SlopeReverse->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_SLOPE_MAX; i++)
	{
		if (g_Texture_Slope[i])	g_Texture_Slope[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
	g_nNumMeshSlope = 0;
}

void UninitSlopeWallReverse(void)
{

	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshSlopeWall; nCntMeshField++)
	{
		g_slopewall[nCntMeshField];

		if (g_slopewall[nCntMeshField].vertexBuffer)
		{// ���_�o�b�t�@�̉��
			g_slopewall[nCntMeshField].vertexBuffer->Release();
			g_slopewall[nCntMeshField].vertexBuffer = NULL;
		}

		//if (g_slopewall[nCntMeshField].indexBuffer)
		//{// �C���f�b�N�X�o�b�t�@�̉��
		//	g_slopewall[nCntMeshField].indexBuffer->Release();
		//	g_slopewall[nCntMeshField].indexBuffer = NULL;
		//}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_SLOPEWALL_MAX; i++)
	{
		if (g_Texture_SlopeWall[i])	g_Texture_SlopeWall[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
	g_nNumMeshSlopeWall = 0;
}
//=============================================================================
// �X�V����
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


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshFieldReverse(void)
{
	MESH_MAP *pMesh_FieldReverse;
	int i;

	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshField; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverse[i].cameracheck == false) { continue; }
		}
		pMesh_FieldReverse = &g_aMeshFieldReverse[i];

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldReverse->vertexBuffer, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(pMesh_FieldReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		// �}�e���A���ݒ�
		SetMaterial(pMesh_FieldReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh_FieldReverse->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldReverse->rot.y, pMesh_FieldReverse->rot.x, pMesh_FieldReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_FieldReverse->mtxWorld, &pMesh_FieldReverse->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldReverse->pos.x, pMesh_FieldReverse->pos.y, pMesh_FieldReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_FieldReverse->mtxWorld, &pMesh_FieldReverse->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&pMesh_FieldReverse->mtxWorld);

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(pMesh_FieldReverse->nNumVertexIndex, 0, 0);
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshWallReverse(void)
{
	MESH_MAP *pMesh_WallReverse;
	int i;

	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshWallReverse[i].cameracheck == false) { continue; }
		}
		pMesh_WallReverse = &g_aMeshWallReverse[i];

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_WallReverse->vertexBuffer, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(pMesh_WallReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

		// �}�e���A���ݒ�
		SetMaterial(pMesh_WallReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh_WallReverse->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_WallReverse->rot.y, pMesh_WallReverse->rot.x, pMesh_WallReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_WallReverse->mtxWorld, &pMesh_WallReverse->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh_WallReverse->pos.x, pMesh_WallReverse->pos.y, pMesh_WallReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_WallReverse->mtxWorld, &pMesh_WallReverse->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&pMesh_WallReverse->mtxWorld);

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(pMesh_WallReverse->nNumVertexIndex, 0, 0);
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshFieldCeilingReverse(void)
{
	MESH_MAP *pMesh_FieldCeilingReverse;
	int i;

	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshCeiling; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverseCeiling[i].cameracheck == false) { continue; }
		}
		pMesh_FieldCeilingReverse = &g_aMeshFieldReverseCeiling[i];

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeilingReverse->vertexBuffer, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeilingReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

		// �}�e���A���ݒ�
		SetMaterial(pMesh_FieldCeilingReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh_FieldCeilingReverse->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeilingReverse->rot.y, pMesh_FieldCeilingReverse->rot.x, pMesh_FieldCeilingReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_FieldCeilingReverse->mtxWorld, &pMesh_FieldCeilingReverse->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeilingReverse->pos.x, pMesh_FieldCeilingReverse->pos.y, pMesh_FieldCeilingReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_FieldCeilingReverse->mtxWorld, &pMesh_FieldCeilingReverse->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&pMesh_FieldCeilingReverse->mtxWorld);

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(pMesh_FieldCeilingReverse->nNumVertexIndex, 0, 0);
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshSlopeReverse(void)
{
	MESH_MAP *pMesh_SlopeReverse;
	int i;

	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshSlope; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_aMeshFieldReverseSlope[i].cameracheck == false) { continue; }
		}
		pMesh_SlopeReverse = &g_aMeshFieldReverseSlope[i];

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_SlopeReverse->vertexBuffer, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(pMesh_SlopeReverse->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

		// �}�e���A���ݒ�
		SetMaterial(pMesh_SlopeReverse->material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh_SlopeReverse->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_SlopeReverse->rot.y, pMesh_SlopeReverse->rot.x, pMesh_SlopeReverse->rot.z);
		D3DXMatrixMultiply(&pMesh_SlopeReverse->mtxWorld, &pMesh_SlopeReverse->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh_SlopeReverse->pos.x, pMesh_SlopeReverse->pos.y, pMesh_SlopeReverse->pos.z);
		D3DXMatrixMultiply(&pMesh_SlopeReverse->mtxWorld, &pMesh_SlopeReverse->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&pMesh_SlopeReverse->mtxWorld);

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(pMesh_SlopeReverse->nNumVertexIndex, 0, 0);
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawSlopeWallReverse(void)
{
	int i;

	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].cameracheck == false) { continue; }
		}

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_slopewall[i].vertexBuffer, &stride, &offset);

		//// �C���f�b�N�X�o�b�t�@�ݒ�
		//GetDeviceContext()->IASetIndexBuffer(g_slopewall[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_SlopeWall[g_TexNo_SlopeWall]);

		// �}�e���A���ݒ�
		SetMaterial(g_slopewall[i].material);

		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_slopewall[i].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_slopewall[i].rot.y, g_slopewall[i].rot.x, g_slopewall[i].rot.z);
		D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_slopewall[i].pos.x, g_slopewall[i].pos.y, g_slopewall[i].pos.z);
		D3DXMatrixMultiply(&g_slopewall[i].mtxWorld, &g_slopewall[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_slopewall[i].mtxWorld);

		// �|���S���̕`��
		GetDeviceContext()->Draw(g_slopewall[i].nNumVertex, 0);
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

/*
g_fieldpointReverse = GetFieldPointReverse();
g_ceilingpointReverse = GetCeilingPointReverse();
g_wallpointReverse = GetWallPointReverse();
g_slopewallpointReverse = GetSlopeWallPointReverse();

static MESH_MAP g_aMeshFieldReverse[MAX_MESH_FIELD];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshField = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshWallReverse[MAX_MESH_WALL];		// ���b�V���ǃ��[�N
static int g_nNumMeshWall = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldReverseCeiling[MAX_MESH_FIELD_CEILING];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshCeiling = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldReverseSlope[MAX_MESH_FIELD_SLOPE];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshSlope = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// ���b�V���ǂ̐�

*/
//=============================================================================
// �J������CHECK
//=============================================================================
void MapTransparentCheckReverse(void)
{
	for(int i=0;i< g_nNumMeshField;i++)
	{
		bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�

		{

			D3DXVECTOR3 pp0 = g_fieldpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_fieldpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_fieldpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_fieldpointReverse[i].pos3;
			//float result = (float)atan2(kyori, kyori2); //�J�������_�ƃJ���������_�̌��ʂ̊p�x
			bool ans1 = RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
			bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1, pp2, pp3);
			bool ans3 = RayHitTransparent(camera->pos, player[0].top_pos, pp0, pp1, pp2, pp3);
			bool ans4 = RayHitTransparent(camera->pos, player[0].down_pos, pp0, pp1, pp2, pp3);

			if (ans1 == true || ans2 == true || ans3 == true || ans4 == true)
			{
				frontcheck = true;
			}
		}
		//�O�㔻��
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
		bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�
		{
			D3DXVECTOR3 pp0 = g_wallpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_wallpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_wallpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_wallpointReverse[i].pos3;
			//float result = (float)atan2(kyori, kyori2); //�J�������_�ƃJ���������_�̌��ʂ̊p�x
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
		bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�

		{

			D3DXVECTOR3 pp0 = g_ceilingpointReverse[i].pos0;
			D3DXVECTOR3 pp1 = g_ceilingpointReverse[i].pos1;
			D3DXVECTOR3 pp2 = g_ceilingpointReverse[i].pos2;
			D3DXVECTOR3 pp3 = g_ceilingpointReverse[i].pos3;

			//float result = (float)atan2(kyori, kyori2); //�J�������_�ƃJ���������_�̌��ʂ̊p�x
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
		bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�

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
