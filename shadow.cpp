//=============================================================================
//
// �e���� [shadow.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "shadow.h"
#include "renderer.h"
#include "meshMap.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�

#define	SHADOW_SIZE_X		(50.0f)			// ���_�T�C�Y
#define	SHADOW_SIZE_Z		(50.0f)			// ���_�T�C�Y

#define	MAX_SHADOW			(100)			// �e�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scl;		// �X�P�[��
	MATERIAL	material;	// �}�e���A��
	bool		bUse;		// �g�p���Ă��邩�ǂ���

} SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����


static SHADOW					g_aShadow[MAX_SHADOW];		// �e���[�N
static int						g_TexNo;					// �e�N�X�`���ԍ�

static char* g_TextureName[] = {
	"data/TEXTURE/shadow000.jpg",
	//"data/TEXTURE/effect000.png",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexShadow();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	// �e���[�N������
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		ZeroMemory(&g_aShadow[nCntShadow].material, sizeof(g_aShadow[nCntShadow].material));
		g_aShadow[nCntShadow].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])	g_Texture[i]->Release();
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	// ���Z����
	SetBlendState(BLEND_MODE_SUBTRACT);
	// ���C�e�B���O�𖳌�
	SetLightEnable(false);
	// Z��r�Ȃ�
	SetDepthEnable(false);

	// �t�H�O����
	//SetFogEnable(false);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(g_aShadow[nCntShadow].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aShadow[nCntShadow].scl.x, g_aShadow[nCntShadow].scl.y, g_aShadow[nCntShadow].scl.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&g_aShadow[nCntShadow].mtxWorld);

			// �}�e���A���̐ݒ�
			SetMaterial(g_aShadow[nCntShadow].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// �ʏ�u�����h
	SetBlendState(BLEND_MODE_ALPHABLEND);
	// Z��r����
	SetDepthEnable(true);
	// ���C�e�B���O��L����
	SetLightEnable(true);
	// �t�H�O�L��
	//SetFogEnable(true);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexShadow()
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		vertex[1].Position = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		vertex[2].Position = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
		vertex[3].Position = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

		// �@���̐ݒ�
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �g�U���̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
		vertex[1].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
		vertex[2].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
		vertex[3].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �e�̍쐬
//=============================================================================
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].scl = D3DXVECTOR3(fSizeX, 1.0f, fSizeZ);
			g_aShadow[nCntShadow].bUse = true;

			nIdxShadow = nCntShadow;
			break;
		}
	}

	return nIdxShadow;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorShadow(int nIdxShadow, D3DXCOLOR col)
{
	g_aShadow[nIdxShadow].material.Diffuse = col;
}

//=============================================================================
// �e�̔j��
//=============================================================================
void ReleaseShadow(int nIdxShadow)
{
	if(nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].scl = D3DXVECTOR3(fSizeX, 1.0f, fSizeZ);
}

