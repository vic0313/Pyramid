//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : GP11A132 43 �я���
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�
#define TEXTURE_BULLET_MAX			(1)			// �e�N�X�`���̐�
#define TEXTURE_TWOJUMP_MAX			(1)			// �e�N�X�`���̐�

#define	PARTICLE_SIZE_X		(20.0f)		// ���_�T�C�Y
#define	PARTICLE_SIZE_Y		(20.0f)		// ���_�T�C�Y
#define	PARTICLE_BULLET_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_BULLET_SIZE_Y		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_TWOJUMP_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_TWOJUMP_SIZE_Y		(40.0f)		// ���_�T�C�Y

#define	PARTICLE_DIED_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_DIED_SIZE_Y		(40.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_PARTICLE	(5.0f)		// �ړ����x



#define	DISP_SHADOW						// �e�̕\��

#define ROPE_PARTICLE_LIFE	(20)
#define BULLET_PARTICLE_LIFE	(20)
#define TOWJUMP_PARTICLE_LIFE	(5.0f)
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeRopeVertexParticle(void);
HRESULT MakeBulletVertexParticle(void);
HRESULT MakeTwojumpVertexParticle(void);
HRESULT MakeDiedVertexParticle(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ROPE			*g_rope = GetRope();
static PLAYER *pPlayer = GetPlayer();
static BULLET *g_bullet = GetBullet();
static GAMEDATA *gamedata = GetData();
static PARTICLE					g_Rope_Particle[MAX_PARTICLE_ROPE];		// �p�[�e�B�N�����[�N
static PARTICLE					g_Bullet_Particle[MAX_PARTICLE_BULLET];		// �p�[�e�B�N�����[�N
static PARTICLE					g_Twojump_Particle[MAX_PARTICLE_BULLET];		// �p�[�e�B�N�����[�N
static PARTICLE					died_Particle[MAX_PARTICLE_DIED];		// �p�[�e�B�N�����[�N

static ID3D11Buffer					*g_rope_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_rope_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11Buffer					*g_bullet_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_bullet_Texture[TEXTURE_BULLET_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11Buffer					*g_twojump_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_twojump_Texture[TEXTURE_TWOJUMP_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11Buffer					*died_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*died_Texture[TEXTURE_TWOJUMP_MAX] = { NULL };	// �e�N�X�`�����
//static D3DXMATRIX				g_mtxWorldParticle;				// ���[���h�}�g���b�N�X


static float					g_fWidthBase = 5.0f;			// ��̕�
//static float					g_fHeightBase = 10.0f;			// ��̍���
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h

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
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	{
		// ���_���̍쐬
		MakeRopeVertexParticle();

		// �e�N�X�`������
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_rope_TextureName[i],
				NULL,
				NULL,
				&g_rope_Texture[i],
				NULL);
		}



		// �p�[�e�B�N�����[�N�̏�����
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
		// ���_���̍쐬
		MakeBulletVertexParticle();

		// �e�N�X�`������
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_bullet_TextureName[i],
				NULL,
				NULL,
				&g_bullet_Texture[i],
				NULL);
		}



		// �p�[�e�B�N�����[�N�̏�����
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
		// ���_���̍쐬
		MakeTwojumpVertexParticle();

		// �e�N�X�`������
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_twojump_TextureName[i],
				NULL,
				NULL,
				&g_twojump_Texture[i],
				NULL);
		}



		// �p�[�e�B�N�����[�N�̏�����
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
		// ���_���̍쐬
		MakeDiedVertexParticle();

		// �e�N�X�`������
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				died_TextureName[i],
				NULL,
				NULL,
				&died_Texture[i],
				NULL);
		}

		// �p�[�e�B�N�����[�N�̏�����
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
// �I������
//=============================================================================
void UninitParticle(void)
{
	{
		//�e�N�X�`���̉��
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (g_rope_Texture[nCntTex] != NULL)
			{
				g_rope_Texture[nCntTex]->Release();
				g_rope_Texture[nCntTex] = NULL;
			}
		}

		// ���_�o�b�t�@�̉��
		if (g_rope_VertexBuffer != NULL)
		{
			g_rope_VertexBuffer->Release();
			g_rope_VertexBuffer = NULL;
		}
	}


	{
		//�e�N�X�`���̉��
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_bullet_Texture[i] != NULL)
			{
				g_bullet_Texture[i]->Release();
				g_bullet_Texture[i] = NULL;
			}
		}

		// ���_�o�b�t�@�̉��
		if (g_bullet_VertexBuffer != NULL)
		{
			g_bullet_VertexBuffer->Release();
			g_bullet_VertexBuffer = NULL;
		}
	}

	{
		//�e�N�X�`���̉��
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (g_twojump_Texture[nCntTex] != NULL)
			{
				g_twojump_Texture[nCntTex]->Release();
				g_twojump_Texture[nCntTex] = NULL;
			}
		}

		// ���_�o�b�t�@�̉��
		if (g_twojump_VertexBuffer != NULL)
		{
			g_twojump_VertexBuffer->Release();
			g_twojump_VertexBuffer = NULL;
		}
	}

	{
		//�e�N�X�`���̉��
		for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
		{
			if (died_Texture[nCntTex] != NULL)
			{
				died_Texture[nCntTex]->Release();
				died_Texture[nCntTex] = NULL;
			}
		}

		// ���_�o�b�t�@�̉��
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
// �X�V����
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
					//	// ���l�ݒ�
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
			//			{// �g�p��
			//				g_Rope_Particle[i].pos.x += g_Rope_Particle[i].move.x;
			//				g_Rope_Particle[i].pos.z += g_Rope_Particle[i].move.z;
			//				g_Rope_Particle[i].pos.y += g_Rope_Particle[i].move.y;
			//
			//#ifdef DISP_SHADOW
			//				if(g_Rope_Particle[i].nIdxShadow != -1)
			//				{// �e�g�p��
			//					float colA;
			//
			//					// �e�̈ʒu�ݒ�
			//					SetPositionShadow(g_Rope_Particle[i].nIdxShadow, D3DXVECTOR3(g_Rope_Particle[i].pos.x, 0.1f, g_Rope_Particle[i].pos.z));
			//
			//					// �e�̐F�̐ݒ�
			//					colA = g_Rope_Particle[i].material.Diffuse.a;
			//					SetColorShadow(g_Rope_Particle[i].nIdxShadow, D3DXCOLOR(0.5f, 0.5f, 0.5f, colA));
			//				}
			//#endif
			//}
			//}
			// �p�[�e�B�N������
			//		if((rand() % 2) == 0)



		}

		//bullet
		{
			for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
			{
				if (g_Bullet_Particle[i].bUse)
				{// �g�p��
					g_Bullet_Particle[i].pos.x += g_Bullet_Particle[i].move.x;
					g_Bullet_Particle[i].pos.z += g_Bullet_Particle[i].move.z;
					g_Bullet_Particle[i].pos.y += g_Bullet_Particle[i].move.y;
					//if (g_Bullet_Particle[i].pos.y <= g_Bullet_Particle[i].fSizeY / 2)
					//{// ���n����
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
							// ���l�ݒ�
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
				{// �g�p��
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
							// ���l�ݒ�
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
// �`�揈��
//=============================================================================
void DrawParticle(void) 
{
	// ���C�e�B���O�𖳌���
	SetLightEnable(false);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_ADD);

	// ���e�X�g��L����
	//SetAlphaTestEnable(true);

	// Z��r����
	SetDepthEnable(false);

	// �t�H�O����
	//SetFogEnable(false);
	CAMERA *cam = GetCamera();
	//void DrawRopeParticle(void)
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		CAMERA *cam = GetCamera();

		

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_rope_VertexBuffer, &stride, &offset);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_ROPE; i++)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_rope_Texture[g_Rope_Particle[i].g_TexNo]);
			if (g_Rope_Particle[i].bUse)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Rope_Particle[i].g_mtxWorldParticle);

				// �r���[�}�g���b�N�X���擾
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

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, g_Rope_Particle[i].scale.x, g_Rope_Particle[i].scale.y, g_Rope_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Rope_Particle[i].g_mtxWorldParticle, &g_Rope_Particle[i].g_mtxWorldParticle, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_Rope_Particle[i].pos.x, g_Rope_Particle[i].pos.y, g_Rope_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Rope_Particle[i].g_mtxWorldParticle, &g_Rope_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&g_Rope_Particle[i].g_mtxWorldParticle);

				// �}�e���A���ݒ�
				SetMaterial(g_Rope_Particle[i].material);

				// �|���S���̕`��
				GetDeviceContext()->Draw(4, 0);
			}
		}

		//// ���C�e�B���O��L����
		//SetLightEnable(true);

		//// �ʏ�u�����h�ɖ߂�
		//SetBlendState(BLEND_MODE_ALPHABLEND);

		//// Z��r�L��
		//SetDepthEnable(true);

		//// �t�H�O�L��
		////SetFogEnable(true);

	}

	//void DrawBulletParticle(void)
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		

		//// ���C�e�B���O�𖳌���
		//SetLightEnable(false);

		//// ���Z�����ɐݒ�
		//SetBlendState(BLEND_MODE_ADD);

		//// Z��r����
		//SetDepthEnable(false);

		// �t�H�O����
		//SetFogEnable(false);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_bullet_VertexBuffer, &stride, &offset);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_BULLET; i++)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_bullet_Texture[g_Bullet_Particle[i].g_TexNo]);
			if (g_Bullet_Particle[i].bUse)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Bullet_Particle[i].g_mtxWorldParticle);

				// �r���[�}�g���b�N�X���擾
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

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, g_Bullet_Particle[i].scale.x, g_Bullet_Particle[i].scale.y, g_Bullet_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Bullet_Particle[i].g_mtxWorldParticle, &g_Bullet_Particle[i].g_mtxWorldParticle, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_Bullet_Particle[i].pos.x, g_Bullet_Particle[i].pos.y, g_Bullet_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Bullet_Particle[i].g_mtxWorldParticle, &g_Bullet_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&g_Bullet_Particle[i].g_mtxWorldParticle);

				// �}�e���A���ݒ�
				SetMaterial(g_Bullet_Particle[i].material);

				// �|���S���̕`��
				GetDeviceContext()->Draw(4, 0);
			}
		}

	}


	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;
		CAMERA *cam = GetCamera();

		//// ���C�e�B���O�𖳌���
		//SetLightEnable(false);

		//// ���Z�����ɐݒ�
		//SetBlendState(BLEND_MODE_ADD);

		//// Z��r����
		//SetDepthEnable(false);

		//// �t�H�O����
		////SetFogEnable(false);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_twojump_VertexBuffer, &stride, &offset);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


		for (int i = 0; i < MAX_PARTICLE_TWOJUMP; i++)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_twojump_Texture[g_Twojump_Particle[i].g_TexNo]);
			if (g_Twojump_Particle[i].bUse==true)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Twojump_Particle[i].g_mtxWorldParticle);

				// �r���[�}�g���b�N�X���擾
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

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, g_Twojump_Particle[i].scale.x, g_Twojump_Particle[i].scale.y, g_Twojump_Particle[i].scale.z);
				D3DXMatrixMultiply(&g_Twojump_Particle[i].g_mtxWorldParticle, &g_Twojump_Particle[i].g_mtxWorldParticle, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_Twojump_Particle[i].pos.x, g_Twojump_Particle[i].pos.y, g_Twojump_Particle[i].pos.z);
				D3DXMatrixMultiply(&g_Twojump_Particle[i].g_mtxWorldParticle, &g_Twojump_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&g_Twojump_Particle[i].g_mtxWorldParticle);

				// �}�e���A���ݒ�
				SetMaterial(g_Twojump_Particle[i].material);

				// �|���S���̕`��
				GetDeviceContext()->Draw(4, 0);
			}
		}

		//// ���C�e�B���O��L����
		//SetLightEnable(true);

		//// �ʏ�u�����h�ɖ߂�
		//SetBlendState(BLEND_MODE_ALPHABLEND);

		//// Z��r�L��
		//SetDepthEnable(true);

		//// �t�H�O�L��
		////SetFogEnable(true);

	}

	//��
	{
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;


		//// ���C�e�B���O�𖳌���
		//SetLightEnable(false);

		//// ���Z�����ɐݒ�
		//SetBlendState(BLEND_MODE_ADD);

		//// Z��r����
		//SetDepthEnable(false);

		// �t�H�O����
		//SetFogEnable(false);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &died_VertexBuffer, &stride, &offset);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



		for (int i = 0; i < MAX_PARTICLE_DIED; i++)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &died_Texture[died_Particle[i].g_TexNo]);
			if (died_Particle[i].bUse)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&died_Particle[i].g_mtxWorldParticle);

				// �r���[�}�g���b�N�X���擾
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

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, died_Particle[i].scale.x, died_Particle[i].scale.y, died_Particle[i].scale.z);
				D3DXMatrixMultiply(&died_Particle[i].g_mtxWorldParticle, &died_Particle[i].g_mtxWorldParticle, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, died_Particle[i].pos.x, died_Particle[i].pos.y, died_Particle[i].pos.z);
				D3DXMatrixMultiply(&died_Particle[i].g_mtxWorldParticle, &died_Particle[i].g_mtxWorldParticle, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&died_Particle[i].g_mtxWorldParticle);

				// �}�e���A���ݒ�
				SetMaterial(died_Particle[i].material);

				// �|���S���̕`��
				GetDeviceContext()->Draw(4, 0);
			}
		}

	}

	// ���C�e�B���O��L����
	SetLightEnable(true);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(true);

	// ���e�X�g�𖳌���
	//SetAlphaTestEnable(false);

	// �t�H�O�L��
	SetFogEnable(true);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeRopeVertexParticle(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_rope_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_rope_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
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
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_bullet_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_bullet_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_BULLET_SIZE_X / 2, PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_BULLET_SIZE_X / 2, PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_BULLET_SIZE_X / 2, -PARTICLE_BULLET_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_BULLET_SIZE_X / 2, -PARTICLE_BULLET_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
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
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_twojump_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_twojump_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_TWOJUMP_SIZE_X / 2, PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_TWOJUMP_SIZE_X / 2, PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_TWOJUMP_SIZE_X / 2, -PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_TWOJUMP_SIZE_X / 2, -PARTICLE_TWOJUMP_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
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
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &died_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(died_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-PARTICLE_DIED_SIZE_X / 2, PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[1].Position = D3DXVECTOR3(PARTICLE_DIED_SIZE_X / 2, PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-PARTICLE_DIED_SIZE_X / 2, -PARTICLE_DIED_SIZE_Y / 2, 0.0f);
		vertex[3].Position = D3DXVECTOR3(PARTICLE_DIED_SIZE_X / 2, -PARTICLE_DIED_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		GetDeviceContext()->Unmap(died_VertexBuffer, 0);
	}

	return S_OK;
}
//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	g_Rope_Particle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// ���_���̍쐬
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
//			// �e�̐ݒ�
//			g_Bullet_Particle[i].nIdxShadow = CreateShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
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
// �e�̎擾
//=============================================================================
PARTICLE *GetTwojumpParticle(void)
{
	return &(g_Twojump_Particle[0]);
}
					