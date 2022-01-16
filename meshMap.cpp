//=============================================================================
//
// �n�}�̏��� [meshMap.cpp]
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
#include "other_objects.h"
#include "Map.h"
#include "MapTransparent.h"
#include "meshMapReverse.h"
#include "particle.h"
#include "shadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_FIELD_MAX			(1)				// �e�N�X�`���̐�
#define TEXTURE_WALL_MAX			(2)						// �e�N�X�`���̐�
#define TEXTURE_CEILING_MAX 		(1)				// �e�N�X�`���̐�
#define TEXTURE_SLOPE_MAX			(1)				// �e�N�X�`���̐�
#define TEXTURE_SLOPEWALL_MAX		(1)		// �e�N�X�`���̐�
#define TEXTURE_BOSSFIELD_MAX		(1)		// �e�N�X�`���̐�
#define TEXTURE_RANDOM_FIELD_MAX	(2)		// �e�N�X�`���̐�
#define TEXTURE_TRIANGLE_MAX	(1)
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
static OBJECT  *bosskey = GetKeyObject();
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
static ID3D11ShaderResourceView	*g_Texture_BOSSField[TEXTURE_BOSSFIELD_MAX] = { NULL };	// �e�N�X�`�����
static int						g_TexNo_BOSSField;	// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView	*g_Texture_RandomField[TEXTURE_RANDOM_FIELD_MAX] = { NULL };	// �e�N�X�`�����
static int						g_TexNo_RandomField;	// �e�N�X�`���ԍ�
static ID3D11ShaderResourceView	*g_Texture_TriangleWall[TEXTURE_TRIANGLE_MAX] = { NULL };	// �e�N�X�`�����
static int						g_TexNo_TriangleWall;	// �e�N�X�`���ԍ�



static MESH_MAP g_aMeshField[MAX_MESH_FIELD];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshField = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshWall[MAX_MESH_WALL];		// ���b�V���ǃ��[�N
static int g_nNumMeshWall = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldCeiling[MAX_MESH_FIELD_CEILING];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshCeiling = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshFieldSlope[MAX_MESH_FIELD_SLOPE];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshSlope = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_slopewall[MAX_MESH_SLOPE_WALL];
static int g_nNumMeshSlopeWall = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_BOSSField[MAX_MESH_BOSS_FIELD];
static int g_nNumBOSSField = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_aMeshRandomField[MAX_MESH_RANDOM_FIELD];		// ���b�V���n�ʃ��[�N
static int g_nNumMeshRandomField = 0;						// ���b�V���ǂ̐�
static MESH_MAP g_Trianglewall[MAX_MESH_TRIANGLE_WALL];
static int g_nNumMeshTriangleWall = 0;						// ���b�V���ǂ̐�


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

// BOSS �G���A�̔g�̏���
static VERTEX_3D	*g_Vertex = NULL;

static D3DXVECTOR3	g_Center;					// �g�̔����ꏊ
static float		g_Time = 0.0f;				// �g�̌o�ߎ���
static float		g_wave_frequency = 3.0f;	// �g�̎��g��
static float		g_wave_correction = 0.02f;	// �g�̋����␳
static float		g_wave_amplitude = BOSS_WAVE_AMPLITUDE;	// �g�̐U��
//static bool			namidause=false;

static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static MAP_POINT	*g_ceilingpoint = GetCeilingPoint();
static MAP_POINT	*g_wallpoint = GetWallPoint();
static MAP_POINT *g_slopewallpoint = GetSlopeWallPoint();
static MAP_POINT	*g_fieldslopepoint = GetFieldSlopePoint();

//�J�����̒����_�̐��`�ړ��p�̈ړ����W�e�[�u��
//static D3DXVECTOR3 g_Transparent_pos[MAX_Transparent];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int startX, int startZ, float startY)
{
	MESH_MAP *pMesh_Field;

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

	pMesh_Field = &g_aMeshField[g_nNumMeshField];
	g_nNumMeshField++;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_Field->material, sizeof(pMesh_Field->material));
	pMesh_Field->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_Field->pos = pos;

	pMesh_Field->rot = rot;


	// �u���b�N���̐ݒ�
	pMesh_Field->nNumBlockX = nNumBlockX;
	pMesh_Field->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_Field->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_Field->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_Field->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_Field->fBlockSizeX = fBlockSizeX;
	pMesh_Field->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Field->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Field->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Field->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Field->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_Field->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_Field->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Field->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_Field->fBlockSizeX; //+(pMesh_Field->nNumBlockX) * pMesh_Field->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.y = startY;
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_Field->fBlockSizeZ; //+pMesh_Field->nNumBlockZ * pMesh_Field->fBlockSizeZ

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_Field->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Field->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Field->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_Field->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
	
	pMesh_Wall = &g_aMeshWall[g_nNumMeshWall];

	g_nNumMeshWall++;

	pMesh_Wall->use = true;
	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_Wall->material, sizeof(pMesh_Wall->material));
	pMesh_Wall->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_Wall->pos = pos;

	pMesh_Wall->rot = rot;

	// �u���b�N���̐ݒ�
	pMesh_Wall->nNumBlockX = nNumBlockX;
	pMesh_Wall->nNumBlockY = nNumBlockY;

	// ���_���̐ݒ�
	pMesh_Wall->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_Wall->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_Wall->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_Wall->fBlockSizeX = fBlockSizeX;
	pMesh_Wall->fBlockSizeY = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Wall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Wall->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Wall->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Wall->indexBuffer);

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
		GetDeviceContext()->Map(pMesh_Wall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxY = 0; nCntVtxY < (pMesh_Wall->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Wall->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.x = -(pMesh_Wall->nNumBlockX / 2.0f) * pMesh_Wall->fBlockSizeX + nCntVtxX * pMesh_Wall->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.y = pMesh_Wall->nNumBlockY * pMesh_Wall->fBlockSizeY - nCntVtxY * pMesh_Wall->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Position.z = 0.0f;

				// �@���̐ݒ�
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh_Wall->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxY;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Wall->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Wall->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh_Wall->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	pMesh_FieldCeiling = &g_aMeshFieldCeiling[g_nNumMeshCeiling];
	g_nNumMeshCeiling++;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_FieldCeiling->material, sizeof(pMesh_FieldCeiling->material));
	pMesh_FieldCeiling->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_FieldCeiling->pos = pos;

	pMesh_FieldCeiling->rot = rot;


	// �u���b�N���̐ݒ�
	pMesh_FieldCeiling->nNumBlockX = nNumBlockX;
	pMesh_FieldCeiling->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_FieldCeiling->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_FieldCeiling->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_FieldCeiling->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_FieldCeiling->fBlockSizeX = fBlockSizeX;
	pMesh_FieldCeiling->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_FieldCeiling->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeiling->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_FieldCeiling->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_FieldCeiling->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_FieldCeiling->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_FieldCeiling->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_FieldCeiling->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.x = startX - nCntVtxX * pMesh_FieldCeiling->fBlockSizeX; //+(pMesh_FieldCeiling->nNumBlockX) * pMesh_FieldCeiling->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_FieldCeiling->fBlockSizeZ; //+pMesh_FieldCeiling->nNumBlockZ * pMesh_FieldCeiling->fBlockSizeZ

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_FieldCeiling->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_FieldCeiling->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_FieldCeiling->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_FieldCeiling->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	pMesh_Slope = &g_aMeshFieldSlope[g_nNumMeshSlope];
	g_nNumMeshSlope++;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_Slope->material, sizeof(pMesh_Slope->material));
	pMesh_Slope->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_Slope->pos = pos;

	pMesh_Slope->rot = rot;

	pMesh_Slope->yblock = endY - startY;
	pMesh_Slope->fBlockSizeY_X = float((pMesh_Slope->yblock + 1.0) / nNumBlockX);
	pMesh_Slope->fBlockSizeY_Z = float((pMesh_Slope->yblock + 1.0) / nNumBlockZ);

	// �u���b�N���̐ݒ�
	pMesh_Slope->nNumBlockX = nNumBlockX;
	pMesh_Slope->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_Slope->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_Slope->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_Slope->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_Slope->fBlockSizeX = fBlockSizeX;
	pMesh_Slope->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_Slope->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Slope->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_Slope->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_Slope->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_Slope->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;


		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_Slope->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_Slope->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_Slope->fBlockSizeX; //+(pMesh_Slope->nNumBlockX) * pMesh_Slope->fBlockSizeX
					//if slopecheck=1 ->��������E��̍⓹
					//if slopecheck=2 ->�E�����獶��̍⓹
					//if slopecheck=3 ->��ォ�牺���̍⓹
					//if slopecheck=4 ->���ォ��㉺�̍⓹
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

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_Slope->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_Slope->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_Slope->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_Slope->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	pMesh_SlopeWall = &g_slopewall[g_nNumMeshSlopeWall];
	g_nNumMeshSlopeWall++;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_SlopeWall->pos = possize;

	pMesh_SlopeWall->rot = rot;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_SlopeWall->material, sizeof(pMesh_SlopeWall->material));
	pMesh_SlopeWall->material.Diffuse = col;

	// �u���b�N���̐ݒ�
	pMesh_SlopeWall->nNumBlockX = 1;
	pMesh_SlopeWall->nNumBlockZ = 1;

	// ���_���̐ݒ�
	pMesh_SlopeWall->nNumVertex = 4;

	// �C���f�b�N�X���̐ݒ�
	pMesh_SlopeWall->nNumVertexIndex = 4;

	// �|���S�����̐ݒ�
	pMesh_SlopeWall->nNumPolygon = 2;



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_SlopeWall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_SlopeWall->vertexBuffer);
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
		GetDeviceContext()->Map(pMesh_SlopeWall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// ���_���W�̐ݒ�
		// ���_�O�ԁi����̒��_�j
		pVtx[0].Position = pos0 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�P�ԁi�E��̒��_�j
		pVtx[1].Position = pos1 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�Q�ԁi�����̒��_�j
		pVtx[2].Position = pos2 - D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�R�ԁi�E���̒��_�j
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
	

	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pg_BOSSField->material, sizeof(pg_BOSSField->material));
	pg_BOSSField->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pg_BOSSField->pos = pos;

	pg_BOSSField->rot = rot;
	
	// �u���b�N���̐ݒ�
	pg_BOSSField->nNumBlockX = nNumBlockX;
	pg_BOSSField->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pg_BOSSField->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pg_BOSSField->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pg_BOSSField->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pg_BOSSField->fBlockSizeX = fBlockSizeX;
	pg_BOSSField->fBlockSizeZ = fBlockSizeZ;
	
	

	// ���_�����������ɍ���Ă����i�g�ׁ̈j
	// �g�̏���
	//  �@�g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
	g_Vertex = new VERTEX_3D[pg_BOSSField->nNumVertex];
	g_Center = D3DXVECTOR3(0.0f, 0.0f, 900.0f);	// �g�̔����ꏊ
	g_Time = 0.0f;								// �g�̌o�ߎ���(+��-�Ƃœ����O���ɂȂ�)
	g_wave_frequency = 2.0f;					// �g�̎��g��(�㉺�^���̑���)
	g_wave_correction = 0.02f;					// �g�̋����␳(�ς��Ȃ��Ă��ǂ��Ǝv��)
	g_wave_amplitude = BOSS_WAVE_AMPLITUDE;					// �g�̐U��(�g�̍���)

	for (int nCntVtxZ = 0; nCntVtxZ < (pg_BOSSField->nNumBlockZ + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (pg_BOSSField->nNumBlockX + 1); nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.x = -(pg_BOSSField->nNumBlockX / 2.0f) * pg_BOSSField->fBlockSizeX + nCntVtxX * pg_BOSSField->fBlockSizeX;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.y = 0.0f;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.z = 900.0f+(pg_BOSSField->nNumBlockZ / 2.0f) * pg_BOSSField->fBlockSizeZ - nCntVtxZ * pg_BOSSField->fBlockSizeZ;

			float dx = g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.x - g_Center.x;
			float dz = g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.z - g_Center.z;

			// �g��̒��S�_����̋����𓾂�
			float len = (float)sqrt(dx * dx + dz * dz);

			// �g�̍������Asin�֐��œ���
		//  �@�g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
			//g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
			// �@���̐ݒ�
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �g�U���̐ݒ�
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			float texSizeX = 1.0f;
			float texSizeZ = 1.0f;
			// �e�N�X�`�����W�̐ݒ�
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
			g_Vertex[nCntVtxZ * (pg_BOSSField->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
		}
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif



		

		// ���_�o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * pg_BOSSField->nNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &pg_BOSSField->vertexBuffer);

		// �C���f�b�N�X�o�b�t�@����
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(unsigned short) * pg_BOSSField->nNumVertexIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &pg_BOSSField->indexBuffer);


		{
			// ���_�o�b�t�@�ւ̃|�C���^���擾
			D3D11_MAPPED_SUBRESOURCE msr;
			GetDeviceContext()->Map(pg_BOSSField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
			memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*pg_BOSSField->nNumVertex);
			GetDeviceContext()->Unmap(pg_BOSSField->vertexBuffer, 0);
		}

		{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
			// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
			D3D11_MAPPED_SUBRESOURCE msr;
			GetDeviceContext()->Map(pg_BOSSField->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			unsigned short *pIdx = (unsigned short*)msr.pData;

			int nCntIdx = 0;
			for (int nCntVtxZ = 0; nCntVtxZ < pg_BOSSField->nNumBlockZ; nCntVtxZ++)
			{
				if (nCntVtxZ > 0)
				{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
				{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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


	// �e�N�X�`������
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

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_RandomField->material, sizeof(pMesh_RandomField->material));
	pMesh_RandomField->material.Diffuse = col;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_RandomField->pos = pos;

	pMesh_RandomField->rot = rot;

	pMesh_RandomField->texnum = texnum;
	// �u���b�N���̐ݒ�
	pMesh_RandomField->nNumBlockX = nNumBlockX;
	pMesh_RandomField->nNumBlockZ = nNumBlockZ;

	// ���_���̐ݒ�
	pMesh_RandomField->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh_RandomField->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh_RandomField->nNumPolygon = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh_RandomField->fBlockSizeX = fBlockSizeX;
	pMesh_RandomField->fBlockSizeZ = fBlockSizeZ;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_RandomField->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_RandomField->vertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * pMesh_RandomField->nNumVertexIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_RandomField->indexBuffer);


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
		GetDeviceContext()->Map(pMesh_RandomField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int nCntVtxZ = 0; nCntVtxZ < (pMesh_RandomField->nNumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh_RandomField->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.x = startX + nCntVtxX * pMesh_RandomField->fBlockSizeX; //+(pMesh_RandomField->nNumBlockX) * pMesh_RandomField->fBlockSizeX 
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.y = startY+ (rand() % 200) / 100.0f * 30.0f;
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Position.z = startZ - nCntVtxZ * pMesh_RandomField->fBlockSizeZ; //+pMesh_RandomField->nNumBlockZ * pMesh_RandomField->fBlockSizeZ

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �g�U���̐ݒ�
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].TexCoord.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (pMesh_RandomField->nNumBlockX + 1) + nCntVtxX].TexCoord.y = texSizeZ * nCntVtxZ;
			}
		}

		GetDeviceContext()->Unmap(pMesh_RandomField->vertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(pMesh_RandomField->indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < pMesh_RandomField->nNumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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

	//�e�N�X�`������
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

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh_TriangleWall->pos = possize;

	pMesh_TriangleWall->rot = rot;

	// �}�e���A�����̏�����
	ZeroMemory(&pMesh_TriangleWall->material, sizeof(pMesh_TriangleWall->material));
	pMesh_TriangleWall->material.Diffuse = col;

	// �u���b�N���̐ݒ�
	pMesh_TriangleWall->nNumBlockX = 1;
	pMesh_TriangleWall->nNumBlockZ = 1;

	// ���_���̐ݒ�
	pMesh_TriangleWall->nNumVertex = 4;

	// �C���f�b�N�X���̐ݒ�
	pMesh_TriangleWall->nNumVertexIndex = 4;

	// �|���S�����̐ݒ�
	pMesh_TriangleWall->nNumPolygon = 2;



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * pMesh_TriangleWall->nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &pMesh_TriangleWall->vertexBuffer);
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
		GetDeviceContext()->Map(pMesh_TriangleWall->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		// ���_���W�̐ݒ�
		// ���_�O�ԁi����̒��_�j
		pVtx[0].Position = pos0 ;
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�P�ԁi�E��̒��_�j
		pVtx[1].Position = pos1 ;
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�Q�ԁi�����̒��_�j
		pVtx[2].Position = pos2;
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�R�ԁi�E���̒��_�j
		pVtx[3].Position = pos3 ;
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		GetDeviceContext()->Unmap(pMesh_TriangleWall->vertexBuffer, 0);

	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{

	MESH_MAP *pMesh_Field;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshField; nCntMeshField++)
	{
		pMesh_Field = &g_aMeshField[nCntMeshField];

		if (pMesh_Field->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh_Field->vertexBuffer->Release();
			pMesh_Field->vertexBuffer = NULL;
		}

		if (pMesh_Field->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_Field->indexBuffer->Release();
			pMesh_Field->indexBuffer = NULL;
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

void UninitMeshWall(void)
{
	MESH_MAP *pMesh_Wall;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshWall; nCntMeshField++)
	{
		pMesh_Wall = &g_aMeshWall[nCntMeshField];

		if (pMesh_Wall->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh_Wall->vertexBuffer->Release();
			pMesh_Wall->vertexBuffer = NULL;
		}

		if (pMesh_Wall->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_Wall->indexBuffer->Release();
			pMesh_Wall->indexBuffer = NULL;
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

void UninitMeshFieldCeiling(void)
{

	MESH_MAP *pMesh_FieldCeiling;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshCeiling; nCntMeshField++)
	{
		pMesh_FieldCeiling = &g_aMeshFieldCeiling[nCntMeshField];

		if (pMesh_FieldCeiling->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh_FieldCeiling->vertexBuffer->Release();
			pMesh_FieldCeiling->vertexBuffer = NULL;
		}

		if (pMesh_FieldCeiling->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_FieldCeiling->indexBuffer->Release();
			pMesh_FieldCeiling->indexBuffer = NULL;
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

void UninitMeshSlope(void)
{

	MESH_MAP *pMesh_Slope;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshSlope; nCntMeshField++)
	{
		pMesh_Slope = &g_aMeshFieldSlope[nCntMeshField];

		if (pMesh_Slope->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pMesh_Slope->vertexBuffer->Release();
			pMesh_Slope->vertexBuffer = NULL;
		}

		if (pMesh_Slope->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_Slope->indexBuffer->Release();
			pMesh_Slope->indexBuffer = NULL;
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

void UninitSlopeWall(void)
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

void UninitBOSSField(void)
{
	MESH_MAP *pg_BOSSField;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumBOSSField; nCntMeshField++)
	{
		pg_BOSSField = &g_BOSSField[nCntMeshField];

		if (pg_BOSSField->vertexBuffer)
		{// ���_�o�b�t�@�̉��
			pg_BOSSField->vertexBuffer->Release();
			pg_BOSSField->vertexBuffer = NULL;
		}

		if (pg_BOSSField->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pg_BOSSField->indexBuffer->Release();
			pg_BOSSField->indexBuffer = NULL;
		}
		if (g_Vertex)
		{
			delete[] g_Vertex;
			g_Vertex = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_BOSSFIELD_MAX; i++)
	{
		if (g_Texture_BOSSField[i])	g_Texture_BOSSField[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
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
		{// ���_�o�b�t�@�̉��
			pMesh_RandomField->vertexBuffer->Release();
			pMesh_RandomField->vertexBuffer = NULL;
		}

		if (pMesh_RandomField->indexBuffer)
		{// �C���f�b�N�X�o�b�t�@�̉��
			pMesh_RandomField->indexBuffer->Release();
			pMesh_RandomField->indexBuffer = NULL;
		}
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_RANDOM_FIELD_MAX; i++)
	{
		if (g_Texture_RandomField[i])	g_Texture_RandomField[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
	g_nNumMeshRandomField = 0;
}

void UninitTriangleWall(void)
{

	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_nNumMeshTriangleWall; nCntMeshField++)
	{
		g_Trianglewall[g_nNumMeshTriangleWall];

		if (g_Trianglewall[nCntMeshField].vertexBuffer)
		{// ���_�o�b�t�@�̉��
			g_Trianglewall[nCntMeshField].vertexBuffer->Release();
			g_Trianglewall[nCntMeshField].vertexBuffer = NULL;
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
		if (g_Texture_TriangleWall[i])	g_Texture_TriangleWall[i]->Release();
	}

	//�ǂݍ��ݐ������Z�b�g����
	g_nNumMeshTriangleWall = 0;
}


//=============================================================================
// �X�V����
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
	//		D3DXVECTOR3 HitPosition;		//��_
	//		D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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
		//	D3DXVECTOR3 HitPosition;		//��_
		//	D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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
	//		D3DXVECTOR3 HitPosition;		//��_
	//		D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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
	//		D3DXVECTOR3 HitPosition;		//��_
	//		D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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
	//		D3DXVECTOR3 HitPosition;		//��_
	//		D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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

#ifdef _DEBUG	// �f�o�b�O����\������
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
				// �g�̏���
				float dt = 0.05f;

				for (int z = 0; z < pg_BOSSField->nNumBlockZ; z++)
				{
					for (int x = 0; x < pg_BOSSField->nNumBlockX; x++)
					{
						float dx = g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.x - g_Center.x;
						float dz = g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.z - g_Center.z;

						// �g��̒��S�_����̋����𓾂�
						float len = (float)sqrt(dx * dx + dz * dz);

						if (player[j].namidause == true) {
							// �g�̍������Asin�֐��œ���
						//  �@�g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
							g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.y = sinf(-g_Time * g_wave_frequency + len * g_wave_correction) * g_wave_amplitude;
						}
						else {
							g_Vertex[z * (pg_BOSSField->nNumBlockX + 1) + x].Position.y = 0.0f;
						}



					}

				}
				g_Time += dt;


				// ���_�o�b�t�@�ɒl���Z�b�g����
				D3D11_MAPPED_SUBRESOURCE msr;
				GetDeviceContext()->Map(pg_BOSSField->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

				// �S���_���𖈉�㏑�����Ă���̂�DX11�ł͂��̕�����������ł�
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
	//	D3DXVECTOR3 HitPosition;		//��_
	//	D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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
	//	D3DXVECTOR3 HitPosition;		//��_
	//	D3DXVECTOR3 Normal;				//�Ԃ������|���S���̖@���x�N�g��(����)
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

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}



//=============================================================================
// �`�揈��
//=============================================================================
//��Ώ����Ȃ��A�o�g���G���A�̕ǂƒn�� �ƃ^�C�g����
void DrawMeshMapRestarea(void) {
	// ���C�e�B���O�I�t
	SetLightEnable(false);
	//�n��
	{
		MESH_MAP *pMesh_Field;
		

		for (int i = 0; i < 2; i++)
		{
			pMesh_Field = &g_aMeshField[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Field->mtxWorld);


			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);

		}
		
	}
	//��
	{
		MESH_MAP *pMesh_Wall;

		for (int i = 0; i < 10; i++)
		{
			pMesh_Wall = &g_aMeshWall[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}

		
	}
	//�V��
	{
		MESH_MAP *pMesh_FieldCeiling;



		for (int i = 0; i < 1; i++)
		{
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}

		
	}
	//�O�p�`
	{


		for (int i = 0; i < 4; i++)
		{


			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_Trianglewall[i].vertexBuffer, &stride, &offset);

			//// �C���f�b�N�X�o�b�t�@�ݒ�
			//GetDeviceContext()->IASetIndexBuffer(g_Trianglewall[i].indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_TriangleWall[g_TexNo_TriangleWall]);

			// �}�e���A���ݒ�
			SetMaterial(g_Trianglewall[i].material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Trianglewall[i].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Trianglewall[i].rot.y, g_Trianglewall[i].rot.x, g_Trianglewall[i].rot.z);
			D3DXMatrixMultiply(&g_Trianglewall[i].mtxWorld, &g_Trianglewall[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Trianglewall[i].pos.x, g_Trianglewall[i].pos.y, g_Trianglewall[i].pos.z);
			D3DXMatrixMultiply(&g_Trianglewall[i].mtxWorld, &g_Trianglewall[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&g_Trianglewall[i].mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->Draw(g_Trianglewall[i].nNumVertex, 0);
		}

		
	}
	//
	{

		MESH_MAP *pMesh_RandomField;
		
		for (int i = 0; i < 1; i++)
		{
			pMesh_RandomField = &g_aMeshRandomField[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_RandomField->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_RandomField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_RandomField[pMesh_RandomField->texnum]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_RandomField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_RandomField->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_RandomField->rot.y, pMesh_RandomField->rot.x, pMesh_RandomField->rot.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_RandomField->pos.x, pMesh_RandomField->pos.y, pMesh_RandomField->pos.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_RandomField->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_RandomField->nNumVertexIndex, 0, 0);
		}
	}
	// ���C�e�B���O�I��
	SetLightEnable(true);
}
void DrawMeshMapBattlearea(void) 
{
	DrawMeshMapSize();
	
	// �n�ʂ̕`�揈��
	{
		
		DrawMeshField_Behind();

		// �n�ʂ̕`�揈��
		DrawMeshSlope_Behind();

		DrawSlopeWall_Behind();

		// �ǂ̕`�揈��
		DrawMeshWall_Behind();

		//�V��̕`�揈��
		DrawMeshFieldCeiling_Behind();

	}
	DrawShadow();
	DrawParticle();
	// �e�̕`�揈��
	
	DrawTransparent();
	{
		DrawMeshField();
		// �n�ʂ̕`�揈��
		DrawMeshSlope();
		DrawSlopeWall();
		// �ǂ̕`�揈��
		DrawMeshWall();

		

		//�V��̕`�揈��
		DrawMeshFieldCeiling();

	}
	{
		DrawSlopeWallReverse();
		// �n�ʂ̕`�揈��
		DrawMeshSlopeReverse();
		// �ǂ̕`�揈��
		DrawMeshWallReverse();
		// �n�ʂ̕`�揈��
		DrawMeshFieldReverse();
		//�V��̕`�揈��
		DrawMeshFieldCeilingReverse();
		// �ǂ̕`�揈��
		DrawMeshWallReverse();

	}
}

void DrawMeshMapSize(void) {
	// ���C�e�B���O�I�t
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

				// ���_�o�b�t�@�ݒ�
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

				// �C���f�b�N�X�o�b�t�@�ݒ�
				GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

				// �v���~�e�B�u�g�|���W�ݒ�
				GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

				// �}�e���A���ݒ�
				SetMaterial(pMesh_Field->material);

				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
				D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
				D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&pMesh_Field->mtxWorld);


				// �|���S���̕`��
				GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);

			}
		}
		//// ���C�e�B���O�I��
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}

		//// ���C�e�B���O�I��
		//SetLightEnable(true);
	}
	//ceiling
	{
		MESH_MAP *pMesh_FieldCeiling;
		

		//// ���C�e�B���O�I�t
		//SetLightEnable(false);

		for (int i = 1; i < 4; i++)
		{
			if (gamedata->drawopen == false)
			{
				if (g_aMeshFieldCeiling[i].DrawCheck == true) { continue; }
			}
			pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}

		//// ���C�e�B���O�I��
		//SetLightEnable(true);

		
	}
	//����
	{
		MESH_MAP *pMesh_RandomField;
		int i;

		// ���C�e�B���O�I�t
		SetLightEnable(false);

		for (i = 1; i < g_nNumMeshRandomField; i++)
		{
			/*if (gamedata->Mapdraw == true)
			{
				if (g_aMeshRandomField[i].DrawCheck == true) { continue; }
			}*/
			pMesh_RandomField = &g_aMeshRandomField[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_RandomField->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_RandomField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_RandomField[pMesh_RandomField->texnum]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_RandomField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_RandomField->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_RandomField->rot.y, pMesh_RandomField->rot.x, pMesh_RandomField->rot.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_RandomField->pos.x, pMesh_RandomField->pos.y, pMesh_RandomField->pos.z);
			D3DXMatrixMultiply(&pMesh_RandomField->mtxWorld, &pMesh_RandomField->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_RandomField->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_RandomField->nNumVertexIndex, 0, 0);
		}

		
	}
	//Boss�G���A�̒n��
	{
		MESH_MAP *pg_BOSSField;


		// ���C�e�B���O�I�t
		SetLightEnable(false);

		for (int i = 0; i < g_nNumBOSSField; i++)
		{
			/*if (gamedata->Mapdraw == true)
			{
				if (g_BOSSField[i].DrawCheck == true) { continue; }
			}*/
			pg_BOSSField = &g_BOSSField[i];

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pg_BOSSField->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pg_BOSSField->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_BOSSField[g_TexNo_BOSSField]);

			// �}�e���A���ݒ�
			SetMaterial(pg_BOSSField->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pg_BOSSField->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pg_BOSSField->rot.y, pg_BOSSField->rot.x, pg_BOSSField->rot.z);
			D3DXMatrixMultiply(&pg_BOSSField->mtxWorld, &pg_BOSSField->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pg_BOSSField->pos.x, pg_BOSSField->pos.y, pg_BOSSField->pos.z);
			D3DXMatrixMultiply(&pg_BOSSField->mtxWorld, &pg_BOSSField->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pg_BOSSField->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pg_BOSSField->nNumVertexIndex, 0, 0);
		}
	}
	// ���C�e�B���O�I��
	SetLightEnable(true);
}

//player�O
void DrawMeshField(void)
{
	MESH_MAP *pMesh_Field;
	

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Field->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshWall(void)
{
	MESH_MAP *pMesh_Wall;
	
	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			if (i == 152) {
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[1]);
			}
			else
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);
			}


			// �}�e���A���ݒ�
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshFieldCeiling(void)
{
	MESH_MAP *pMesh_FieldCeiling;
	
	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}
		
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshSlope(void)
{
	MESH_MAP *pMesh_Slope;
	

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Slope->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Slope->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Slope->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Slope->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Slope->rot.y, pMesh_Slope->rot.x, pMesh_Slope->rot.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Slope->pos.x, pMesh_Slope->pos.y, pMesh_Slope->pos.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Slope->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Slope->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawSlopeWall(void)
{
	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].DrawCheck == true) { continue; }
		}
		if (g_slopewall[i].cameracheck == true)
		{

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
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}
//player��
void DrawMeshField_Behind(void)
{
	MESH_MAP *pMesh_Field;
	

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Field->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Field->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Field->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Field->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Field->rot.y, pMesh_Field->rot.x, pMesh_Field->rot.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Field->pos.x, pMesh_Field->pos.y, pMesh_Field->pos.z);
			D3DXMatrixMultiply(&pMesh_Field->mtxWorld, &pMesh_Field->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Field->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Field->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshWall_Behind(void)
{
	MESH_MAP *pMesh_Wall;
	

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Wall->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Wall->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			if (i == 152) {
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[1]);
			}
			else
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Wall[g_TexWallNo]);
			}


			// �}�e���A���ݒ�
			SetMaterial(pMesh_Wall->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Wall->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Wall->rot.y, pMesh_Wall->rot.x, pMesh_Wall->rot.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Wall->pos.x, pMesh_Wall->pos.y, pMesh_Wall->pos.z);
			D3DXMatrixMultiply(&pMesh_Wall->mtxWorld, &pMesh_Wall->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Wall->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Wall->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshFieldCeiling_Behind(void)
{
	MESH_MAP *pMesh_FieldCeiling;
	int i;

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_FieldCeiling->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_FieldCeiling->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Ceiling[g_TexNo_Ceiling]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_FieldCeiling->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_FieldCeiling->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_FieldCeiling->rot.y, pMesh_FieldCeiling->rot.x, pMesh_FieldCeiling->rot.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_FieldCeiling->pos.x, pMesh_FieldCeiling->pos.y, pMesh_FieldCeiling->pos.z);
			D3DXMatrixMultiply(&pMesh_FieldCeiling->mtxWorld, &pMesh_FieldCeiling->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_FieldCeiling->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_FieldCeiling->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawMeshSlope_Behind(void)
{
	MESH_MAP *pMesh_Slope;

	// ���C�e�B���O�I�t
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

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &pMesh_Slope->vertexBuffer, &stride, &offset);

			// �C���f�b�N�X�o�b�t�@�ݒ�
			GetDeviceContext()->IASetIndexBuffer(pMesh_Slope->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Slope[g_TexNo_Slope]);

			// �}�e���A���ݒ�
			SetMaterial(pMesh_Slope->material);

			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pMesh_Slope->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh_Slope->rot.y, pMesh_Slope->rot.x, pMesh_Slope->rot.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pMesh_Slope->pos.x, pMesh_Slope->pos.y, pMesh_Slope->pos.z);
			D3DXMatrixMultiply(&pMesh_Slope->mtxWorld, &pMesh_Slope->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&pMesh_Slope->mtxWorld);

			// �|���S���̕`��
			GetDeviceContext()->DrawIndexed(pMesh_Slope->nNumVertexIndex, 0, 0);
		}
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}

void DrawSlopeWall_Behind(void)
{
	// ���C�e�B���O�I�t
	SetLightEnable(false);

	for (int i = 0; i < g_nNumMeshSlopeWall; i++)
	{
		if (gamedata->drawopen == false)
		{
			if (g_slopewall[i].DrawCheck == true) { continue; }
		}
		if (g_slopewall[i].cameracheck == false)
		{

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
	}

	// ���C�e�B���O�I��
	SetLightEnable(true);
}



//=============================================================================
// �J������CHECK
//=============================================================================
void MapTransparentCheck(void)
{
	
	//D3DXVECTOR3 pPos = player[0].pos- camera->pos;
	D3DXVECTOR3 plong = camera->at- camera->pos ;//�J���������_���ʏ�A�J������player�̋���
	D3DXVECTOR3 ppos = player[0].pos;
	ppos.y += PLAYER_TALL / 2;
	float	kyori = (float)sqrt((plong.x*plong.x) + (plong.y*plong.y) + (plong.z*plong.z));
	//float	camera_Y_UP = (VIEW_ANGLE / 2);
	//float	camera_Y_DOWN = -(VIEW_ANGLE / 2);
	//float	camera_X_RIGHT = (float)atan( ((SCREEN_WIDTH / 2) / (SCREEN_HEIGHT / 2)) * (tanf(VIEW_ANGLE / 2)) );
	//float	camera_X_LEFT = -(float)atan( ((SCREEN_WIDTH / 2) / (SCREEN_HEIGHT / 2)) * (tanf(VIEW_ANGLE / 2)) );
	//float  camera_radius_XZ = tan(camera_X_RIGHT)*kyori;//camera.pos->camera.at�̎�����̕�MAX
	
	//field
	{
		//�J�����͈͓̔�����
		for (int i = 2; i < g_nNumMeshField; i++)
		{
			bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�
			
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
				//// ���_�o�b�t�@�ւ̃|�C���^���擾
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

			//�J�������ɔ���
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
		//PLAYER�̑O�㔻��
		for (int i = 10; i < g_nNumMeshField; i++)
		{
			bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�
			
			{
				
				D3DXVECTOR3 pp0 = g_fieldpoint[i].pos0;
				D3DXVECTOR3 pp1 = g_fieldpoint[i].pos1;
				D3DXVECTOR3 pp2 = g_fieldpoint[i].pos2;
				D3DXVECTOR3 pp3 = g_fieldpoint[i].pos3;
				//float result = (float)atan2(kyori, kyori2); //�J�������_�ƃJ���������_�̌��ʂ̊p�x
				bool ans1=RayHitTransparent(camera->pos, player[0].left_pos, pp0, pp1, pp2, pp3);
				bool ans2 = RayHitTransparent(camera->pos, player[0].right_pos, pp0, pp1,pp2, pp3);
				bool ans3 = RayHitTransparent(camera->pos, player[0].top_pos, pp0, pp1, pp2, pp3);
				bool ans4 = RayHitTransparent(camera->pos, player[0].down_pos, pp0, pp1, pp2, pp3);

				if (ans1==true || ans2 == true || ans3 == true || ans4 == true)
				{
					frontcheck = true;
				}
			}
			//�O�㔻��
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
		//�J�����͈͓̔�����
		for (int i = 10; i < g_nNumMeshWall; i++)
		{
			bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�
			
			//
			//MESH_MAP *pMesh_Wall;
			//pMesh_Wall = &g_aMeshWall[i];
			//// ���_�o�b�t�@�ւ̃|�C���^���擾
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
			//�J�������ɔ���
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
		//PLAYER�̑O�㔻��
		for (int i = 42; i < g_nNumMeshWall; i++)
		{
			bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�
			{
				D3DXVECTOR3 pp0 = g_wallpoint[i].pos0;
				D3DXVECTOR3 pp1 = g_wallpoint[i].pos1;
				D3DXVECTOR3 pp2 = g_wallpoint[i].pos2;
				D3DXVECTOR3 pp3 = g_wallpoint[i].pos3;
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
		
		//�J�����͈͓̔�����
		for (int i = 1; i < g_nNumMeshCeiling; i++)
		{
			bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

			//MESH_MAP *pMesh_FieldCeiling;
			//pMesh_FieldCeiling = &g_aMeshFieldCeiling[i];
			//// ���_�o�b�t�@�ւ̃|�C���^���擾
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
			//�J�������ɔ���
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
		//PLAYER�̑O�㔻��
		for (int i = 0; i < g_nNumMeshCeiling; i++)
		{
			bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�

			{
				
				D3DXVECTOR3 pp0 = g_ceilingpoint[i].pos0 ;
				D3DXVECTOR3 pp1 = g_ceilingpoint[i].pos1 ;
				D3DXVECTOR3 pp2 = g_ceilingpoint[i].pos2 ;
				D3DXVECTOR3 pp3 = g_ceilingpoint[i].pos3 ;
				
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
		//�J�����͈͓̔�����
		for (int i = 0; i < g_nNumMeshSlope; i++)
		{
			bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

			//MESH_MAP *pMesh_Slope;
			//pMesh_Slope = &g_aMeshFieldSlope[i];
			//// ���_�o�b�t�@�ւ̃|�C���^���擾
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
			//�J�������ɔ���
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
		//PLAYER�̑O�㔻��
		for (int i = 0; i < g_nNumMeshSlope; i++)
		{
			bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�

			{
				
				D3DXVECTOR3 pp0 = g_fieldslopepoint[i].pos0 ;
				D3DXVECTOR3 pp1 = g_fieldslopepoint[i].pos1 ;
				D3DXVECTOR3 pp2 = g_fieldslopepoint[i].pos2 ;
				D3DXVECTOR3 pp3 = g_fieldslopepoint[i].pos3 ;

				//float result = (float)atan2(kyori, kyori2); //�J�������_�ƃJ���������_�̌��ʂ̊p�x
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
		//�J�����͈͓̔�����
		for (int i = 0; i < g_nNumMeshSlopeWall; i++)
		{
			bool drawcheck = false;	//TRUE�Ȃ�΁A�J�����͈͓̔�

			//MESH_MAP *pMesh_SlopeWall;
			//pMesh_SlopeWall = &g_slopewall[i];
			//// ���_�o�b�t�@�ւ̃|�C���^���擾
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
			//�J�������ɔ���
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
		//PLAYER�̑O�㔻��
		for (int i = 0; i < g_nNumMeshSlopeWall; i++)
		{
			bool frontcheck = false;//true�Ȃ�΁APLAYER�̑O�ł�
			
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
// RAY����
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

		// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
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

	
		// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
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

		// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
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

		// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
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