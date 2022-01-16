//=============================================================================
//
// UI���� [UI.h]
// Author : GP11A132�@43�@�я���
//
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX			(99999)		// �X�R�A�̍ő�l
#define SCORE_DIGIT			(5)			// ����

#define	HP_PRICE			(50)
#define	ATK_PRICE			(80)
#define	TWOJUMP_PRICE		(100)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PAUSE {
	bool					g_Use;						// true:�g���Ă���  false:���g�p
	float					g_w, g_h;					// ���ƍ���
	D3DXVECTOR3				g_Pos;						// �|���S���̍��W
	int						g_TexNo;					// �e�N�X�`���ԍ�
	ID3D11Buffer			*g_VertexBuffer;
	D3DXCOLOR				color;

	 D3DXMATRIX				mtxWorld;
	 D3DXVECTOR3 scl;			// �X�P�[��
	 MATERIAL	material;		// �}�e���A��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTittle(void);
void UninitTittle(void);
void UpdateTittle(void);
void DrawTittle(void);

HRESULT InitBuying(void);
void UninitBuying(void);
void UpdateBuying(void);
void DrawBuying(void);
void DrawBuyingMark(void);

HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);

HRESULT InitResultUI(void);
void UninitResultUI(void);
void UpdateResultUI(void);
void DrawResultUI(void);
