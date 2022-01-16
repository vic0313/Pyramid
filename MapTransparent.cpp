//=============================================================================
//
// �v���C���[�O�̓����摜�̏��� [MapTransparent.cpp]
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
#include "MapTransparent.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_TRIANGLE_MAX	(1)

#define	TREE_WIDTH			(80.0f)			// ���_�T�C�Y
#define	TREE_HEIGHT			(80.0f)			// ���_�T�C�Y
#define	RADIUS			(10.0f)			// ���_�T�C�Y

#define TEXTURE_MAX					(1)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		fWidth;			// ��
	float		fHeight;		// ����
	int			nIdxShadow;		// �eID
	bool		bUse;			// �g�p���Ă��邩�ǂ���
	ID3D11Buffer		*g_VertexBuffer;

}Transparent;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertex(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER  *player = GetPlayer();
static CAMERA *camera = GetCamera();
static GAMEDATA *gamedata = GetData();
//static ID3D11Buffer					*g_Transparent[i].g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_TextureTransparent[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static Transparent					g_Transparent[MAX_Transparent];	// �؃��[�N
static int					g_TexTransparentNo;			// �e�N�X�`���ԍ�
static bool					g_bAlpaTest;		// �A���t�@�e�X�gON/OFF

static char *g_TextureTransparentName[] =
{
	"data/TEXTURE/fade_black.png",
};
//�������p



//=============================================================================
// ����������
//=============================================================================
HRESULT InitTransparent(void)
{
	float rot = 0.0f;
	//MakeVertex();
	// �e�N�X�`������
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


		// ���_�o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_Transparent[i].g_VertexBuffer);

		// ���_�o�b�t�@�ɒl���Z�b�g����
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_Transparent[i].g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;



		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
		vertex[1].Position = D3DXVECTOR3(fWidth, fHeight, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		vertex[3].Position = D3DXVECTOR3(fWidth, -fHeight, 0.0f);

		// �g�U���̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
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
// �I������
//=============================================================================
void UninitTransparent(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_TextureTransparent[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_TextureTransparent[nCntTex]->Release();
			g_TextureTransparent[nCntTex] = NULL;
		}
	}

	for (int i = 0; i < MAX_Transparent; i++)
	{
		if (g_Transparent[i].g_VertexBuffer != NULL)
		{// ���_�o�b�t�@�̉��
			g_Transparent[i].g_VertexBuffer->Release();
			g_Transparent[i].g_VertexBuffer = NULL;
		}
	}
	
}
//=============================================================================
// �X�V����
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


	// �A���t�@�e�X�gON/OFF
	if (GetKeyboardTrigger(DIK_F1))
	{
		//�O�����Z�q
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
// �`�揈��
//=============================================================================

void DrawTransparent(void)
{
	if (camera->shoutchange == false) { return; }
	// ���e�X�g�ݒ�
	//if (g_bAlpaTest == true)
	//{
	//	// ���e�X�g��L����
	//	SetAlphaTestEnable(true);
	//}
	//// Z��r�Ȃ��@// ��Z�o�b�t�@�[
	//SetDepthEnable(false);


	// ���C�e�B���O�𖳌�
	SetLightEnable(false);
	for (int i = 0; i < MAX_Transparent; i++)
	{
		if (g_Transparent[i].bUse)
		{
			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// ���_�o�b�t�@�ݒ�
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			GetDeviceContext()->IASetVertexBuffers(0, 1, &g_Transparent[i].g_VertexBuffer, &stride, &offset);

			// �v���~�e�B�u�g�|���W�ݒ�
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			{
				if (g_Transparent[i].bUse)
				{
					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_Transparent[i].mtxWorld);

					// �r���[�}�g���b�N�X���擾
					//CAMERA *cam = GetCamera();

					//myxView�ɃJ�����s������o��
					mtxView = camera->mtxView;

					// �|���S���𐳖ʂɌ�����
#if 0
			// �t�s����v�Z�ŋ��߂�

			//�֗��Ȋ֐��Ȃ̂ł����A���x
					D3DXMatrixInverse(&g_Transparent[i].mtxWorld, NULL, &mtxView);

					//�n�������Ɍv�Z���Ă���̂Ŗ{���s�K�v�ȕ��s�ړ��̒l�܂œ��ꂿ��
					g_Transparent[i].mtxWorld._41 = 0.0f;	//�ړ�Tx�l���N���A
					g_Transparent[i].mtxWorld._42 = 0.0f;	//�ړ�Ty�l���N���A
					g_Transparent[i].mtxWorld._43 = 0.0f;	//�ړ�Tz�l���N���A
#else 
			//�����s�񁨍s�Ɨ񂪓����v�f�̍s��(��4*4�̍s��Ƃ�)
			//DirectX�̃J�����s��͐����s��ɂȂ��Ă��܂�
			//�����s��ɂ͐���������
			//�����s��̓]�u�s��͋t�s��ɂȂ�
			//�]�u�s�񁨍s��̍��ォ��E���֌������Ď΂߂̐�������
			//���̐���ΏۂƂ��č��E�㉺����ւ����s��

			//���̂����͒������I�I
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

					// �X�P�[���𔽉f
					D3DXMatrixScaling(&mtxScale, g_Transparent[i].scl.x,
						g_Transparent[i].scl.y,
						g_Transparent[i].scl.z);
					D3DXMatrixMultiply(&g_Transparent[i].mtxWorld, &g_Transparent[i].mtxWorld, &mtxScale);

					// �ړ��𔽉f
					D3DXMatrixTranslation(&mtxTranslate, g_Transparent[i].pos.x,
						g_Transparent[i].pos.y,
						g_Transparent[i].pos.z);
					D3DXMatrixMultiply(&g_Transparent[i].mtxWorld, &g_Transparent[i].mtxWorld, &mtxTranslate);


					// ���[���h�}�g���b�N�X�̐ݒ�
					SetWorldMatrix(&g_Transparent[i].mtxWorld);

					// �}�e���A���ݒ�
					SetMaterial(g_Transparent[i].material);

					// �e�N�X�`���ݒ�
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_TextureTransparent[TEXTURE_MAX]);

					// �|���S���̕`��
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}

	}


	// ���C�e�B���O��L����
	SetLightEnable(true);

	// ���e�X�g�𖳌���
	//SetAlphaTestEnable(false);


	// //Z��r����
	//SetDepthEnable(true);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertex(void)
{
	for (int i = 0; i < MAX_Transparent; i++)
	{
		// ���_�o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_Transparent[i].g_VertexBuffer);

		// ���_�o�b�t�@�ɒl���Z�b�g����
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_Transparent[i].g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float fWidth = 100.0f;
		float fHeight = 100.0f;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-fWidth / 2.0f, fHeight / 2.0f, 0.0f);
		vertex[1].Position = D3DXVECTOR3(fWidth / 2.0f, fHeight / 2.0f, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-fWidth / 2.0f, -fHeight / 2.0f, 0.0f);
		vertex[3].Position = D3DXVECTOR3(fWidth / 2.0f, -fHeight / 2.0f, 0.0f);

		// �g�U���̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_Transparent[i].g_VertexBuffer, 0);

	}
	
	return S_OK;
}
