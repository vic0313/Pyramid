//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : GP11A132�@43�@�я���
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "time.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Time_MAX			(5999)		// MAX���ԁ@�P��:�b  ��3��=60*3=180�b
#define TIME_DIGIT			(4)			// ����


static clock_t start, end;				//�^�C�}�[�̊J�n�ƌv�Z���̎���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);



double GetTime(void);
int GetMinute(void);
int GetSecond(void);


clock_t *getstart(void);
clock_t *getend(void);