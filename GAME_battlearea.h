//=============================================================================
//
// �o�g���G���A�̏��� [GAME_battlearea.h]
// Author : GP11A132 43 �я���
//
//=============================================================================
#pragma once

#include "main.h"




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameBattlearea(void);
void UninitGameBattlearea(void);
void UpdateGameBattlearea(void);
void DrawGameBattlearea(void);
void DrawGameBattlearea_True(void);

void HitCheck(void);
void TouchCheck(void);