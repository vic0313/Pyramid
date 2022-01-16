//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : GP11A132�@43�@�я���
//
//=============================================================================
#include <time.h>
#include "main.h"
#include "timer.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(4)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static GAMEDATA					*gamedata = GetData();
static char *g_TexturName[] = {
	"data/TEXTURE/number16x32.png",			//�����f��
	"data/TEXTURE/yellow.jpg",				//�t���[��
	"data/TEXTURE/timersozai1.jpg",
	"data/TEXTURE/timersozai2.jpg",
};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static D3DXVECTOR3				g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�


static double					g_Time;					   // 0����v�Z�̎���
static int					g_Time_real;			   // TIME_MAX����0�܂Ōv�Z�̎���
static int					g_Time_minute;
static int					g_Time_second;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTime(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �v���C���[�̏�����
	g_Use   = true;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = D3DXVECTOR3(SCREEN_WIDTH-40, 40.0f, 0.0f);
	g_TexNo = 0;

	g_Time = 0;				// �^�C�}�[�̏�����

	gamedata->start = clock();		//�Q�[���J�n�������̎��A�^�C�}�[�X�^�[�g

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTime(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTime(void)
{
	
	

	//���͌o�̎���
	gamedata->end = clock();
	

	g_Time = (gamedata->end- gamedata->start)/1000.0+ gamedata->time;			//�P��:�b  end-start�̒P�ʂ̓~���b�@�@��:1234�~���b=1.234�b
	//g_Time_real = Time_MAX - g_Time;		//�P��:�b
	g_Time_real = (int)g_Time;
	g_Time_minute = (g_Time_real / 60);		//�P��:��
	g_Time_second = (g_Time_real % 60);		//�P��:�b


	if (g_Time > Time_MAX)
	{
		g_Time = Time_MAX;
	}


	
#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTime(void)
{
	if (gamedata->DescriptionFirst == true)
	{
		// Z��r�Ȃ��@// ��Z�o�b�t�@�[
		SetDepthEnable(false);

		// ���C�e�B���O�𖳌�
		SetLightEnable(false);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// �}�g���N�X�ݒ�
		SetWorldViewProjection2D();

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		//�t���[���̕`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
			// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Pos.x - g_w * 5;	// �t���[���̕\���ʒuX
			float py = g_Pos.y;				// �t���[���̕\���ʒuY
			float pw = g_w * 14;				// �t���[���̕\����
			float ph = g_h * 2;				// �t���[���̕\������

			float tw = 1.0f;		// �e�N�X�`���̕�
			float th = 1.0f;		// �e�N�X�`���̍���
			float tx = tw;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		//�t���[���̕`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
			// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Pos.x - (g_w * 5);	// �t���[���̕\���ʒuX
			float py = g_Pos.y;				// �t���[���̕\���ʒuY
			float pw = g_w;				// �t���[���̕\����
			float ph = g_h;				// �t���[���̕\������

			float tw = 1.0f;		// �e�N�X�`���̕�
			float th = 1.0f;		// �e�N�X�`���̍���
			float tx = tw;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		//�^�C�}�[�����̊O�g�̕`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// ��������������
			int number1 = g_Time_minute;
			int number2 = g_Time_second;

			for (int i = 0; i < TIME_DIGIT; i++)
			{
				if (i < 2)
				{
					//�b�����̊O�g�̕`��

					// ����\�����錅�̐���
					float x = (float)(number2 % 10);

					// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = g_Pos.x - (g_w * i * 3);	// �^�C�}�[�̕b�̕\���ʒuX
					float py = g_Pos.y;			// �^�C�}�[�̕\���ʒuY
					float pw = g_h + 10;				// �^�C�}�[�̕\����
					float ph = g_h + 10;				// �^�C�}�[�̕\������

					float tw = 1.0f;		// �e�N�X�`���̕�
					float th = 1.0f;		// �e�N�X�`���̍���
					float tx = x * tw;			// �e�N�X�`���̍���X���W
					float ty = 0.0f;			// �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);

					// ���̌���
					number2 /= 10;
				}
				else
				{
					//�������̊O�g�̕`��

					// ����\�����錅�̐���
					float x = (float)(number1 % 10);

					// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = g_Pos.x - TEXTURE_WIDTH - (g_w * i * 3);	// �^�C�}�[�̕��̕\���ʒuX
					float py = g_Pos.y;			// �^�C�}�[�̕\���ʒuY
					float pw = g_h + 10;				// �^�C�}�[�̕\����
					float ph = g_h + 10;				// �^�C�}�[�̕\������

					float tw = 1.0f;		// �e�N�X�`���̕�
					float th = 1.0f;		// �e�N�X�`���̍���
					float tx = x * tw;			// �e�N�X�`���̍���X���W
					float ty = 0.0f;			// �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);

					// ���̌���
					number1 /= 10;
				}

			}
		}


		//�����̕`��
		{

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// ��������������
			int number1 = g_Time_minute;
			int number2 = g_Time_second;

			for (int i = 0; i < TIME_DIGIT; i++)
			{
				if (i < 2)
				{
					//�b�̐����̕`��

					// ����\�����錅�̐���
					float x = (float)(number2 % 10);

					// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = g_Pos.x - (g_w * 3 * i);	// �^�C�}�[�̕b�̕\���ʒuX
					float py = g_Pos.y;			// �^�C�}�[�̕\���ʒuY
					float pw = g_w;				// �^�C�}�[�̕\����
					float ph = g_h;				// �^�C�}�[�̕\������

					float tw = 1.0f / 10;		// �e�N�X�`���̕�
					float th = 1.0f / 1;		// �e�N�X�`���̍���
					float tx = x * tw;			// �e�N�X�`���̍���X���W
					float ty = 0.0f;			// �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f));

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);

					// ���̌���
					number2 /= 10;
				}
				else
				{
					//���̐����̕`��

					// ����\�����錅�̐���
					float x = (float)(number1 % 10);

					// �^�C�}�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = g_Pos.x - TEXTURE_WIDTH - (g_w * 3 * i);	// �^�C�}�[�̕��̕\���ʒuX
					float py = g_Pos.y;			// �^�C�}�[�̕\���ʒuY
					float pw = g_w;				// �^�C�}�[�̕\����
					float ph = g_h;				// �^�C�}�[�̕\������

					float tw = 1.0f / 10;		// �e�N�X�`���̕�
					float th = 1.0f / 1;		// �e�N�X�`���̍���
					float tx = x * tw;			// �e�N�X�`���̍���X���W
					float ty = 0.0f;			// �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f));

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);

					// ���̌���
					number1 /= 10;
				}

			}
		}


		// ���C�e�B���O��L����
		SetLightEnable(true);

		// Z��r����
		SetDepthEnable(true);

	}
	
}




//=============================================================================
// �^�C�}�[�\���̂̐擪�A�h���X���擾
//=============================================================================
double GetTime(void)
{
	return g_Time;
}

int GetMinute(void)
{
	return g_Time_minute;
}

int GetSecond(void)
{
	return g_Time_second;
}


clock_t *getstart(void)
{
	return &start;
}

clock_t *getend(void)
{
	return &end;
}