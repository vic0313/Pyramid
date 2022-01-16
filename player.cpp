//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "renderer.h"
#include "rope.h"
#include "collision.h"
#include "Map.h"
#include "MapTouch.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "GAME_battlearea.h"
#include "bullet.h"
#include "fade.h"
#include "enemy.h"
#include "particle.h"
#include "other_objects.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER					"data/MODEL/player_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BEHIT				"data/MODEL/player_body_beHit.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_PARTS_LEFT_THIGH	"data/MODEL/player_right_thigh.obj"
#define	MODEL_PLAYER_PARTS_LEFT_CALF	"data/MODEL/player_right_calf.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_THIGH	"data/MODEL/player_left_thigh.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_CALF	"data/MODEL/player_left_calf.obj"
#define	MODEL_PLAYER_PARTS_LEFT_ARM1	"data/MODEL/player_right_arm1.obj"
#define	MODEL_PLAYER_PARTS_LEFT_ARM2	"data/MODEL/player_right_arm2.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_ARM1	"data/MODEL/player_left_arm1.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_ARM2	"data/MODEL/player_left_arm2.obj"
#define	MODEL_PLAYER_PARTS_LEFT_HAND	"data/MODEL/player_right_hand.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_HAND	"data/MODEL/player_left_hand.obj"
#define	MODEL_PLAYER_PARTS_RIGHT_GUN	"data/MODEL/player_left_gun.obj"
#define	MODEL_PLAYER_PARTS_LEFT_GUN		"data/MODEL/player_right_gun.obj"
#define	MODEL_PLAYER_PARTS_HEAD			"data/MODEL/player_head.obj"

#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_MOVE_BACK			(3.0f)							// �ړ���
#define	VALUE_MOVE_BACK_SIDE			(4.0f)	
#define	BIGJUMP_MOVE			(7.0f)
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��
#define ROPE_MOVE_PART		(30)

#define GUN_CHANGE_TIME		(50.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER				g_Player[MAX_PLAYER];						// �v���C���[
// �v���C���[�̃p�[�c�p
static PLAYER				g_Parts[PLAYER_PARTS_MAX];			//0���呫;1������;2�E�呫;3�E����;4�����;5���艺;6�E���;7�E�艺;8����;9�E��;10�E��e;11����e;12��
static ENEMY				*enemyBOSS = GetEnemyBOSS();
static DX11_MODEL			player_model;
static DX11_MODEL			player_model_beHit;
//static PARTICLE				*twojump = GetTwojumpParticle();
static GAMEDATA *gamedata = GetData();
static OBJECT	*key = GetKeyObject();

static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static ROPE			*g_rope = GetRope();


static float roty = D3DX_PI;
static float roty_move = 0.0f;
static float rope_movepart = ROPE_MOVE_PART;
static int playerdirection;//0����;1����;2�E��;3���O��;4.�E�O��
//////////////////////////////////////////////////////////////////////
// �v���C���[�̊K�w�A�j���[�V�����f�[�^
//////////////////////////////////////////////////////////////////////

	// ���A�j���f�[�^
	static INTERPOLATION_DATA move_tbl_1_1[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//1
		{ D3DXVECTOR3(0.0f, 2.0f, 15.0f), D3DXVECTOR3(-D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	10.0f, 25.0f), D3DXVECTOR3(-D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 2.0f, 15.0f), D3DXVECTOR3(-D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5
		{ D3DXVECTOR3(0.0f, 2.0f, -15.0f), D3DXVECTOR3(D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//6
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 }, //7
		{ D3DXVECTOR3(0.0f, 2.0f, -15.0f), D3DXVECTOR3(D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 }, //8
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//9

	};
	static INTERPOLATION_DATA move_tbl_1_2[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),         D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,2.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5},
		{ D3DXVECTOR3(0.0f,4.0f, -8.0f), D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,8.0f, -12.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,8.0f,-10.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,10.0f,-10.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,5.0f, -8.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },

	};
	static INTERPOLATION_DATA move_tbl_2_1[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5
		{ D3DXVECTOR3(0.0f, 2.0f, -15.0f), D3DXVECTOR3(D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//6
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 }, //7
		{ D3DXVECTOR3(0.0f, 2.0f, -15.0f), D3DXVECTOR3(D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 }, //8
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//1
		{ D3DXVECTOR3(0.0f, 2.0f, 15.0f), D3DXVECTOR3(-D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	10.0f, 25.0f), D3DXVECTOR3(-D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 2.0f, 15.0f), D3DXVECTOR3(-D3DX_PI / 8, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),				D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//9

	};
	static INTERPOLATION_DATA move_tbl_2_2[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f,8.0f,-10.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5
		{ D3DXVECTOR3(0.0f,10.0f,-10.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//6
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//7
		{ D3DXVECTOR3(0.0f,5.0f, -8.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//8
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),         D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//1
		{ D3DXVECTOR3(0.0f,2.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5},//2
		{ D3DXVECTOR3(0.0f,4.0f, -8.0f), D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f,8.0f, -12.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f,8.0f,-10.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5

	};

	static INTERPOLATION_DATA no_move_tbl[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5
		{ D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f),1 },//5

	};

	static INTERPOLATION_DATA rope_move_tbl[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7
		{ D3DXVECTOR3(0.0f,	10.0f, -20.0f), D3DXVECTOR3(D3DX_PI / 4 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 }, //7

	};
	static INTERPOLATION_DATA rope_move_tbl_2[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,12.0f, -10.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },

	};
	static INTERPOLATION_DATA rope_arm_tbl[] = {	// pos, rot, scl, frame

		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },
		{ D3DXVECTOR3(0.0f,20.0f, -40.0f), D3DXVECTOR3(D3DX_PI / 3, 0.0f, 0.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f),10 },

	};
	// ��A�j���f�[�^
	static INTERPOLATION_DATA arm_tbl_1_1[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//1
		{ D3DXVECTOR3(0.0f, 5.0f, -23.0f),	D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	18.0f,-40.0f),	D3DXVECTOR3(D3DX_PI / 3 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 5.0f, -23.0f),	D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5
		{ D3DXVECTOR3(0.0f, 5.0f, 23.0f),	D3DXVECTOR3(-D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	18.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 3 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 5.0f, 23.0f),	D3DXVECTOR3(-D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5


	};
	static INTERPOLATION_DATA arm_tbl_1_2[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//1
		{ D3DXVECTOR3(0.0f, 5.0f, 23.0f),	D3DXVECTOR3(-D3DX_PI /6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	18.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI/ 3 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 5.0f, 23.0f),	D3DXVECTOR3(-D3DX_PI /6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5
		{ D3DXVECTOR3(0.0f, 5.0f, -23.0f),	D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//2
		{ D3DXVECTOR3(0.0f,	18.0f,- 43.0f),	D3DXVECTOR3(D3DX_PI / 3 , 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//3
		{ D3DXVECTOR3(0.0f, 5.0f, -23.0f),	D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//4
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),			D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },//5



	};
	static INTERPOLATION_DATA arm_tbl_2[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	35.0f, 35.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		
		

	};
	//�ˌ��̎�
	static INTERPOLATION_DATA shout_tbl_0[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	45.0f, 45.0f),	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_tbl_1[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI/2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_tbl_2[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-45.0f,	45.0f, 16.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_tbl_3[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(20.0f,	45.0f, 40.0f),	D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_tbl_4[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(-20.0f,	45.0f, 42.0f),	D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	//�ˌ��̓�
	static INTERPOLATION_DATA shout_head_tbl_1[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_head_tbl_2[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_head_tbl_3[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
	static INTERPOLATION_DATA shout_head_tbl_4[] = {	// pos, rot, scl, frame
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
		{ D3DXVECTOR3(0.0f,	0.0f, 0.0f),	D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),5 },
	};
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	//static MODE mode = GetMode();
	//LoadModel(MODEL_PLAYER, &player_model);
	for (int i = 0; i < MAX_PLAYER; i++) 
	{
		switch (gamedata->modenow)
		{
		case MODE_TITLE:
		{
			g_Player[0].use = false;
			g_Player[0].use = true;
			LoadModel(MODEL_PLAYER, &player_model);
			LoadModel(MODEL_PLAYER_BEHIT, &player_model_beHit);
			
			LoadModel(MODEL_PLAYER_PARTS_LEFT_THIGH, &g_Parts[0].model);
			LoadModel(MODEL_PLAYER_PARTS_LEFT_CALF, &g_Parts[1].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_THIGH, &g_Parts[2].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_CALF, &g_Parts[3].model);
			LoadModel(MODEL_PLAYER_PARTS_LEFT_ARM1, &g_Parts[4].model);
			LoadModel(MODEL_PLAYER_PARTS_LEFT_ARM2, &g_Parts[5].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_ARM1, &g_Parts[6].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_ARM2, &g_Parts[7].model);
			LoadModel(MODEL_PLAYER_PARTS_LEFT_HAND, &g_Parts[8].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_HAND, &g_Parts[9].model);
			LoadModel(MODEL_PLAYER_PARTS_RIGHT_GUN, &g_Parts[10].model);
			LoadModel(MODEL_PLAYER_PARTS_LEFT_GUN, &g_Parts[11].model);
			LoadModel(MODEL_PLAYER_PARTS_HEAD, &g_Parts[12].model);
			D3DXVECTOR3 pos = g_Player[0].pos;
			pos.y = 1.0f;
			g_Player[0].shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
			
		}
		case MODE_RESTAREA:
		{
			g_Player[0].pos = D3DXVECTOR3(0.0f, 0.0f, -7600.0f);
			g_Player[0].movecheck = true;
			g_Player[0].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		
			
		}
		break;
		case MODE_BATTLEAREA:
		{
			g_Player[0].pos = D3DXVECTOR3(PLAYER_FIRST_POS_X, PLAYER_FIRST_POS_Y, PLAYER_FIRST_POS_Z);
			g_Player[0].movecheck = true;
			g_Player[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
			break;
		case MODE_RESULT:
		{
			g_Player[0].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			g_Player[0].pos = D3DXVECTOR3(0.0f, 0.0f, -8250.0f);
			g_Player[0].movecheck = false;
		}
		break;
		default:
			break;
		}
		
		g_Player[0].left_pos = g_Player[0].pos;
		g_Player[0].right_pos= g_Player[0].pos;
		g_Player[0].top_pos = g_Player[0].pos;
		g_Player[0].down_pos = g_Player[0].pos;
		g_Player[0].center_pos =g_Player[0].pos;

		g_Player[0].downpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[0].ropepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_Player[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[0].rot_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[0].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Player[0].time = 0.0f;
		g_Player[0].timeplus = 0.01f;
		g_Player[0].time_bigjump = 0.0f;
		g_Player[0].time_bigjumpplus = 0.01f;
		g_Player[0].time_down = 0.0f;
		g_Player[0].time_downplus = 0.01f;
		g_Player[0].time_rope = 0.0f;
		g_Player[0].time_ropeplus = 0.01f;
		g_Player[0].time_gunmod_use=0.0f;
		g_Player[0].spd = 0.0f;		// �ړ��X�s�[�h�N���A
		g_Player[0].camera_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[0].camera_pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[0].wall_touch=0;
		g_Player[0].jumpOK=false;
		g_Player[0].jumping = false;
		g_Player[0].bigjumpOK = false;
		g_Player[0].bigjumping = false;
		g_Player[0].downOK=true;
		g_Player[0].ropeuse_OK = false;
		g_Player[0].ropeup_OK = false;
		g_Player[0].ropemode = 0;
		g_Player[0].namidause = false;

		
		
		g_Player[0].touchsize= D3DXVECTOR3(PLAYER_SIZE, (PLAYER_TALL/2), PLAYER_SIZE);

		g_Player[0].beHit = false;
		g_Player[0].enemytouch = false;
		g_Player[0].beHitTime = PLAYER_HIT_CD;
		g_Player[0].beHitTime_plus = 0.02f;
		g_Player[0].beTouchTime = PLAYER_HIT_CD;
		g_Player[0].beTouchTime_plus = 0.02f;


		

		//g_Player[0].radius = 5.0f;
		g_Player[0].load = 1;
		g_Player[0].parent = NULL;		// �{�́i�e�j�Ȃ̂�NULL������
	}
	if (gamedata->playernum > 0)
	{

		g_Player[gamedata->nowplayernum].use = true;
	}
	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		
			g_Parts[i].use = true;

			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Parts[i].pos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
			g_Parts[i].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			g_Parts[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			// �e�q�֌W
			g_Parts[i].parent = &g_Player[0];		// �� �����ɐe�̃A�h���X������
		//	g_Parts[�r].parent= &g_Player[j];		// �r��������e�͖{�́i�v���C���[�j
		//	g_Parts[��].parent= &g_Player[�r];	// �w���r�̎q���������ꍇ�̗�

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
			g_Parts[i].tbl_adr = no_move_tbl;	// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
			g_Parts[i].move_time = 0.0f;	// ���s���Ԃ��N���A

			// �p�[�c�̓ǂݍ���
			g_Parts[i].load = 1;
	}
	
	for(int i=0;i<MAX_PLAYER;i++)
	{
		if (g_Player[0].use == true)
		{
			g_Parts[0].parent = &g_Player[0];		// 

			g_Parts[1].parent = &g_Parts[0];

			g_Parts[2].parent = &g_Player[0];		// 


			g_Parts[3].parent = &g_Parts[2];

			g_Parts[4].parent = &g_Player[0];		// 


			g_Parts[5].parent = &g_Parts[4];

			g_Parts[6].parent = &g_Player[0];		// 


			g_Parts[7].parent = &g_Parts[6];

			g_Parts[8].parent = &g_Parts[5];

			g_Parts[9].parent = &g_Parts[7];

			g_Parts[10].parent = &g_Parts[7];
			g_Parts[10].use = false;

			g_Parts[11].parent = &g_Parts[5];
			g_Parts[11].use = false;

			g_Parts[12].parent = &g_Player[0];
		}
	}
			
		playerdirection = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//for (int i = 0; i < MAX_PLAYER; i++)
	//{

	//	// ���f���̉������
	//	UnloadModel(&g_Player[0].model);
	//}
	UnloadModel(&player_model);
	UnloadModel(&player_model_beHit);
	
	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == 1)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = 0;
		}
	}
}

//float roty = 0.0f;
//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		
		if (g_Player[0].use == false) {
			continue;
		}
		CAMERA *cam = GetCamera();

		if (g_Player[0].beHit == true)
		{
			/*if (g_Player[0].enemytouch == true)
			{
				g_Player[0].pos.x -= sinf(g_Player[0].rot_move.y) * 3.0f;
				g_Player[0].pos.z -= cosf(g_Player[0].rot_move.y) * 3.0f;
			}*/
			g_Player[0].beHitTime -= g_Player[0].beHitTime_plus;
			if (g_Player[0].beHitTime <= 0)
			{
				g_Player[0].beHit = false;
				g_Player[0].beHitTime = PLAYER_HIT_CD;
			}
		}

		
		{
			g_Parts[0].parent = &g_Player[0];		// 
			g_Parts[2].parent = &g_Player[0];		// 
			g_Parts[4].parent = &g_Player[0];		// 
			g_Parts[6].parent = &g_Player[0];		// 
			g_Parts[12].parent = &g_Player[0];
		}
		
		
		if (g_Player[0].pos.x >= -550.0f &&g_Player[0].pos.x <= 550.0f
			&&g_Player[0].pos.z >= 300.0f&&g_Player[0].pos.z <=1500.0f)
		{
			g_Player[0].spd *= 0.4f;
		}
		else {
			g_Player[0].spd *= 0.9f;
		}

		g_Player[0].touchcheck = false;
		g_Player[0].touchcheck_field= false;
		g_Player[0].ropepos = g_Player[0].pos;
		g_Player[0].ropepos.y = (g_Player[0].pos.y + PLAYER_TALL);
		g_Player[0].time_gunmod_use *= 0.9f;
		MeshfieldTouch(1);
		MeshceilingTouch(1);
		MeshwallTouch(1);
		SlopewallTouch(1);
		if (g_Player[0].touchcheck_field == false)
		{
			g_Player[0].jumpOK = false;
		}


		if (g_Player[0].movecheck == true)
		{
			// �ړ�����
			if (GetKeyboardPress(DIK_A))
			{
				g_Player[0].spd = VALUE_MOVE;
				roty = D3DX_PI / 2;
				//roty_2 = D3DX_PI / 2;
				roty_move = D3DX_PI / 2;
				playerdirection = 1;
			}
			if (GetKeyboardPress(DIK_D))
			{
				g_Player[0].spd = VALUE_MOVE;
				roty = -D3DX_PI / 2;
				//roty_2 = -D3DX_PI / 2;
				roty_move = -D3DX_PI / 2;
				playerdirection = 2;
			}
			if (GetKeyboardPress(DIK_W))
			{
				g_Player[0].spd = VALUE_MOVE;
				//roty_2 = D3DX_PI;
				roty = D3DX_PI;
				roty_move = D3DX_PI;
				playerdirection = 0;
			}
			if (GetKeyboardPress(DIK_S))
			{
				g_Player[0].spd = VALUE_MOVE_BACK;
				//roty_2 = D3DX_PI;
				roty_move = 0.0f;
				roty = D3DX_PI;
				playerdirection = 0;
			}
			if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_A))
			{
				g_Player[0].spd = VALUE_MOVE;
				//roty_2 = (3*D3DX_PI) / 4;
				roty = (3 * D3DX_PI) / 4;
				roty_move = (3 * D3DX_PI) / 4;
				playerdirection = 3;
			}
			else if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_D))
			{
				g_Player[0].spd = VALUE_MOVE;
				//roty_2 = -(3 * D3DX_PI) / 4;
				roty = -(3 * D3DX_PI) / 4;
				roty_move = -(3 * D3DX_PI) / 4;
				playerdirection = 4;
			}
			if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_A))
			{
				g_Player[0].spd = VALUE_MOVE_BACK_SIDE;
				//roty_2 = (3 * D3DX_PI) / 4;
				roty_move = D3DX_PI / 4;
				roty = -(3 * D3DX_PI) / 4;
				playerdirection = 4;
			}
			else if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_D))
			{
				g_Player[0].spd = VALUE_MOVE_BACK_SIDE;
				//roty_2 = -D3DX_PI / 4;
				roty_move = -D3DX_PI / 4;

				roty = (3 * D3DX_PI) / 4;
				playerdirection = 3;
			}

			if (GetKeyboardTrigger(DIK_SPACE))
			{
				if (g_Player[0].jumpOK == true) {		//JUMP�\���
					g_Player[0].jumpOK = false;			//JUMP�s�\���
					g_Player[0].jumping = true;			//JUMP�̎��s
					g_Player[0].downOK = false;			//������~
				}
			}
		}
		
		if (GetKeyboardTrigger(DIK_E)&&gamedata->twojump==true)
		{
			if (g_Player[0].bigjumpOK == true) {		//JUMP�\���
				g_Player[0].bigjumpOK = false;
				g_Player[0].jumpOK = false;			//JUMP�s�\���
				g_Player[0].jumping = false;			//JUMP�̎��s
				g_Player[0].bigjumping = true;
				g_Player[0].bigjumping_UP = true;
				if(gamedata->bigjumpset==false)gamedata->bigjumpset = true;
			}
		}

		bool ansrope = IsMouseRightTriggered();

		//if (GetKeyboardTrigger(DIK_Q))
		if (ansrope==true)
		{
			if (g_Player[0].ropeuse_OK == false) {

				g_rope[0].point_set = false;			//ROPE�̍��WSET
				g_Player[0].ropeuse_OK = true;			//PLAYER��ROPE��Ԃ�TRUE
				g_Player[0].ropeup_OK = true;			//�ړ�����CHECK
			}

		}
		// �e���ˏ���
		//if (GetKeyboardTrigger(DIK_F))
		bool ans = IsMouseLeftTriggered();
		if (ans == true&& g_rope[0].point_set_finsh != true)
		{
			D3DXVECTOR3 bulletpos = g_Player[0].pos;
			bulletpos.y += PLAYER_TALL / 2 + 10.0f;
			switch (playerdirection)
			{
			case 0:
				bulletpos.z += (sinf(g_Player[0].rot.y) * 15) - (cosf(g_Player[0].rot.y) * 35);
				bulletpos.x -= (cosf(g_Player[0].rot.y) * 15) + (sinf(g_Player[0].rot.y) * 35);
				break;
			case 1:
				bulletpos.z += (sinf(g_Player[0].rot.y) * 40);
				bulletpos.x -= (cosf(g_Player[0].rot.y) * 40);
				break;
			case 2:
				bulletpos.z -= (sinf(g_Player[0].rot.y) * 40);
				bulletpos.x += (cosf(g_Player[0].rot.y) * 40);
				break;
			case 3:
				
				bulletpos.z += (cosf(cam->rot.y) * 40);
				bulletpos.x += (sinf(cam->rot.y ) *40);
				break;
			case 4:
				bulletpos.z += (cosf(cam->rot.y) * 40);
				bulletpos.x += (sinf(cam->rot.y) * 40);
				break;
			default:
				break;
			}
			D3DXVECTOR3 shoutrot = cam->rot;
			shoutrot.y += D3DX_PI;
			SetBullet(bulletpos, shoutrot);
			//SetBullet(bulletpos, g_Player[0].rot);

			g_Player[0].time_gunmod_use = GUN_CHANGE_TIME;

		}
		//
		if (g_Player[0].time_gunmod_use >= 1.0 && g_rope[0].point_set_finsh != true)
		{
			if (playerdirection == 2 || playerdirection == 4)
			{
				g_Parts[11].use = true;
				g_Parts[10].use = false;
			}
			else 
			{
				g_Parts[10].use = true;
				g_Parts[11].use = false;
			}
			
		}
		if (g_Player[0].time_gunmod_use < 1.0 || g_rope[0].point_set_finsh == true)
		{
			g_Parts[10].use = false;
			g_Parts[11].use = false;
		}

		//�A�j���`�����W��
		{
			if (g_Player[0].spd > 1.0f && g_rope[0].point_set_finsh != true)
			{
				g_Parts[0].tbl_adr = move_tbl_1_1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
				g_Parts[1].tbl_adr = move_tbl_1_2;
				g_Parts[2].tbl_adr = move_tbl_2_1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
				g_Parts[3].tbl_adr = move_tbl_2_2;
				g_Parts[4].tbl_adr = arm_tbl_1_1;
				g_Parts[5].tbl_adr = arm_tbl_2;
				g_Parts[6].tbl_adr = arm_tbl_1_2;
				g_Parts[7].tbl_adr = arm_tbl_2;
				g_Parts[12].tbl_adr = no_move_tbl;
				if (g_Player[0].time_gunmod_use > 1.0f) 
				{
					switch (playerdirection)
					{
					case 0:
						g_Parts[6].tbl_adr = shout_tbl_0;
						g_Parts[7].tbl_adr = no_move_tbl;
						g_Parts[12].tbl_adr = no_move_tbl;
						break;
					case 1:
						g_Parts[6].tbl_adr = shout_tbl_1;
						g_Parts[7].tbl_adr = no_move_tbl;
						g_Parts[12].tbl_adr = shout_head_tbl_1;
						break;
					case 2:
						g_Parts[4].tbl_adr = shout_tbl_2;
						g_Parts[5].tbl_adr = no_move_tbl;
						g_Parts[6].tbl_adr = arm_tbl_1_2;
						g_Parts[7].tbl_adr = arm_tbl_2;
						g_Parts[12].tbl_adr = shout_head_tbl_2;
						break;
					case 3:
						g_Parts[6].tbl_adr = shout_tbl_3;
						g_Parts[7].tbl_adr = no_move_tbl;
						g_Parts[12].tbl_adr = shout_head_tbl_3;
						break;
					case 4:
						g_Parts[4].tbl_adr = shout_tbl_4;
						g_Parts[5].tbl_adr = no_move_tbl;
						g_Parts[12].tbl_adr = shout_head_tbl_4;
						break;
					default:
						break;
					}
				}
				
			}
			else if (g_Player[0].spd <= 1.0f &&g_rope[0].point_set_finsh != true) 
			{
				for (int j = 0; j < PLAYER_PARTS_MAX; j++)
				{
					g_Parts[j].tbl_adr = no_move_tbl;
				}
				if (g_Player[0].time_gunmod_use >= 1.0) {
					switch (playerdirection)
					{
					case 0:
						g_Parts[12].tbl_adr = no_move_tbl;
						g_Parts[6].tbl_adr = shout_tbl_0;
						break;
					case 1:
						g_Parts[12].tbl_adr = shout_head_tbl_1;
						g_Parts[6].tbl_adr = shout_tbl_1;
						break;
					case 2:
						g_Parts[12].tbl_adr = shout_head_tbl_2;
						g_Parts[4].tbl_adr = shout_tbl_2;
						break;
					case 3:
						g_Parts[12].tbl_adr = shout_head_tbl_3;
						g_Parts[6].tbl_adr = shout_tbl_3;
						break;
					case 4:
						g_Parts[12].tbl_adr = shout_head_tbl_4;
						g_Parts[4].tbl_adr = shout_tbl_4;

						
						break;
					default:
						break;
					}
				}
			}
			else if (g_rope[0].point_set_finsh == true) {
				g_Parts[0].tbl_adr = rope_move_tbl;
				g_Parts[1].tbl_adr = rope_move_tbl_2;
				g_Parts[2].tbl_adr = rope_move_tbl;
				g_Parts[3].tbl_adr = rope_move_tbl_2;
				g_Parts[4].tbl_adr = rope_arm_tbl;
				g_Parts[5].tbl_adr = no_move_tbl;
				g_Parts[6].tbl_adr = rope_arm_tbl;
				g_Parts[7].tbl_adr = no_move_tbl;
			}
			

			/*if (g_Player[0].time_gunmod_use>1.0f && g_rope[0].point_set_finsh != true)
			{
				g_Parts[6].tbl_adr = shout_tbl_2;
				g_Parts[7].tbl_adr = no_move_tbl;
				
			}*/
			
			
		}
		
		//JUMP����
		{
			if (g_Player[0].jumping == true) {
				float	jumopos = 5.0f;							// XYZ�ړ��ʂ��v�Z���Ă���
				int nowNo = (int)g_Player[0].time;				// �������ł���e�[�u���ԍ������o���Ă���
				float nowTime = g_Player[0].time - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
				float jumopos2 = jumopos;
				jumopos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				jumopos2 -= jumopos;
				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				g_Player[0].pos.y += jumopos2;
				cam->pos.y += jumopos2;
				cam->at.y += jumopos2;
				g_Player[0].timeplus += 0.001f;
				g_Player[0].time += g_Player[0].timeplus;		// ���Ԃ�i�߂Ă���
				if (g_Player[0].time >= 1 || g_rope[0].point_set_finsh == true) {
					g_Player[0].jumping = false;
					g_Player[0].downOK = true;
					g_Player[0].time = 0;
					g_Player[0].timeplus = 0.01f;

				}
				else if (g_Player[0].bigjumping == true) {
					g_Player[0].jumping = false;
					g_Player[0].time = 0;
					g_Player[0].timeplus = 0.01f;
				}
			}
			else if (g_Player[0].jumping == false) {
				g_Player[0].time = 0;
				g_Player[0].timeplus = 0.01f;
			}
		}

		//BIGJUMP����
		
		{
			
			if (g_Player[0].bigjumping == true) 
			{
				//g_Player[0].spd = BIGJUMP_MOVE;
				
				if (g_Player[0].bigjumping_UP !=false)
				{
					g_Player[0].downOK = false;
					float	bigjumppos = 10.0f;							// XYZ�ړ��ʂ��v�Z���Ă���
					int nowNo = (int)g_Player[0].time_bigjump;				// �������ł���e�[�u���ԍ������o���Ă���
					float nowTime = g_Player[0].time_bigjump - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
					float bigjumppos2 = bigjumppos;
					bigjumppos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
					bigjumppos2 -= bigjumppos;
					g_Player[0].pos.y += bigjumppos2;
					cam->pos.y += bigjumppos2;
					cam->at.y += bigjumppos2;
					g_Player[0].time_bigjumpplus += 0.001f;
					g_Player[0].time_bigjump += g_Player[0].time_bigjumpplus;		// ���Ԃ�i�߂Ă���
					if (g_Player[0].time_bigjump >= 1) {
						g_Player[0].bigjumping_UP = false;
						g_Player[0].downOK = true;

					}
				}
				
				if (g_rope[0].point_set_finsh == true) 
				{
					g_Player[0].bigjumping_UP = false;
					g_Player[0].bigjumping = false;
					g_Player[0].time_bigjump = 0;
					g_Player[0].time_bigjumpplus = 0.01f;
				}
			}
			else if (g_Player[0].bigjumping==false) {
				g_Player[0].time_bigjump = 0;
				g_Player[0].time_bigjumpplus = 0.01f;
			}
			
			
		}

		//��������
		{
			if (g_Player[0].downOK == true)
			{
				float	downpos = 7.0f;							// XYZ�ړ��ʂ��v�Z���Ă���
				float nowTime = g_Player[0].time_down;			// ���ԕ����ł��鏭�������o���Ă���
				downpos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				g_Player[0].downpos.y = g_Player[0].pos.y;
				g_Player[0].pos.y -= downpos;
				cam->pos.y -= downpos;
				cam->at.y -= downpos;
				g_Player[0].time_downplus += 0.001f;
				g_Player[0].time_down += g_Player[0].time_downplus;	// ���Ԃ�i�߂Ă���

			}else if (g_Player[0].downOK == false) {
				g_Player[0].time_downplus = 0.01f;
				g_Player[0].time_down = 0;
			}
		}


		//rope�̏���
		{
			if (g_rope[0].point_set_finsh == true) {
				D3DXVECTOR3 ppos=g_rope[0].end_pos - g_rope[0].start_pos;
				 float x_change = ppos.x;
				 float y_change = ppos.y;
				 float z_change = ppos.z;
				if (g_Player[0].ropemode == 0) 
				{
					//��POS->endpos
					g_Player[0].downOK = false;
					g_Player[0].movecheck = false;
					g_Player[0].time_ropeplus=0.01f;
					int nowNo = (int)g_Player[0].time_rope;				// �������ł���e�[�u���ԍ������o���Ă���
					float nowTime = g_Player[0].time_rope - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
					
					/*g_Player[0].ropepos.y = (g_Player[0].pos.y + PLAYER_TALL);*/
					MeshfieldTouch(1);
					MeshceilingTouch(1);
					MeshwallTouch(1);
					SlopewallTouch(1);
					
					
					
					
					if (g_Player[0].touchcheck != true) {
							g_Player[0].pos.x += (x_change / rope_movepart);
							g_Player[0].pos.z += (z_change / rope_movepart);
							cam->at.x += (x_change / rope_movepart);
							cam->pos.x += (x_change / rope_movepart);
							cam->at.z += (z_change / rope_movepart);
							cam->pos.z += (z_change / rope_movepart);
							
					}
					
					if (g_Player[0].touchcheck_field != true)
					{
						cam->pos.y += (y_change / 5);
						g_Player[0].pos.y += (y_change / 5);
						
					}
					else if (g_Player[0].touchcheck_field == true)
					{
						if (y_change > 0) {
							g_Player[0].pos.y += (y_change / 5);
						}
						
					}
					g_Player[0].time_rope += g_Player[0].time_ropeplus;
					if (nowTime > 0.07f) {
						if (g_Player[0].jumpOK == true) {

							//g_Player[0].spd = 20.0f;
							g_Player[0].time_rope = 0.0f;
							g_Player[0].ropemode = 3;
						}
						g_Player[0].ropemode = 1;
						g_Player[0].time_rope = 0.0f;
					}
				}else if (g_Player[0].ropemode == 1) {
					//endpos->�ō��_
					g_Player[0].movecheck = false;
					int nowNo = (int)g_Player[0].time_rope;				// �������ł���e�[�u���ԍ������o���Ă���
					float nowTime = g_Player[0].time_rope - nowNo;		// ���ԕ����ł��鏭�������o���Ă���
					//cam->at.y -= (y_change / 10);
					MeshfieldTouch(1);
					MeshceilingTouch(1);
					MeshwallTouch(1);
					SlopewallTouch(1);
					

					if (g_Player[0].touchcheck != true) {
						
						g_Player[0].pos.x += (x_change / rope_movepart);
						g_Player[0].pos.z += (z_change / rope_movepart);
						cam->at.x += (x_change / rope_movepart);
						cam->pos.x += (x_change / rope_movepart);
						cam->at.z += (z_change / rope_movepart);
						cam->pos.z += (z_change / rope_movepart);
						rope_movepart += 1;
					}
					
					
					if (g_Player[0].touchcheck_field != true)
					{
						cam->pos.y += (y_change / 10);
						g_Player[0].pos.y += (y_change / 10);

					}
					else if (g_Player[0].touchcheck_field == true)
					{
						if (y_change > 0) {
							g_Player[0].pos.y += (y_change / 10);
						}

					}

					// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
					//Y�̎��Ԑi�߂Ă���
					g_Player[0].time_rope += g_Player[0].time_ropeplus;
					if (nowTime > 0.05f) {
						g_Player[0].ropemode = 3;
						g_Player[0].time_rope = 0.0f;
					}
					else if (g_Player[0].jumpOK == true) {
						//g_Player[0].spd = 10.0f;
						g_Player[0].time_rope = 0.0f;
						g_Player[0].ropemode = 3;
					}
					
				}
				else if (g_Player[0].ropemode == 3) {
					//����
					g_Player[0].downOK = true;
					g_Player[0].ropeup_OK = false;
					g_rope[0].use = false;
					g_Player[0].movecheck = true;
					g_Player[0].ropeuse_OK = false;
					g_rope[0].point_set_finsh = false;
					g_Player[0].time_rope = 0.0f;
					g_Player[0].time_ropeplus = 0.01f;
					g_Player[0].ropemode = 0;
					rope_movepart = ROPE_MOVE_PART;
				}
				
				//X��
						// ���Ԃ�i�߂Ă���
				if (g_Player[0].ropeup_OK == true) 
				{
					g_Player[0].downOK = false;				//�����~�܂�
					

				}
			}

		}
		
		
		if(g_Player[0].movecheck == true)
		{
			//g_Player[0].ropepos = g_Player[0].pos;
				// �L�����N�^�[�̉�]�ƃJ�����̉�]

				g_Player[0].rot.y = roty + cam->rot.y;
				g_Player[0].rot_move.y = roty_move + cam->rot.y;
				if (g_Player[0].enemytouch == false)
				{
					g_Player[0].pos.x -= sinf(g_Player[0].rot_move.y) * g_Player[0].spd;
					g_Player[0].pos.z -= cosf(g_Player[0].rot_move.y) * g_Player[0].spd;
					g_Player[0].beTouchTime = PLAYER_HIT_CD;
				}
				else
				{
					g_Player[0].beTouchTime -= g_Player[0].beTouchTime_plus;
					g_Player[0].pos.x += sinf(g_Player[0].rot_move.y) * 3.0f;
					g_Player[0].pos.z += cosf(g_Player[0].rot_move.y) * 3.0f;
					if (g_Player[0].beTouchTime <= 0)
					{
						g_Player[0].enemytouch = false;
						g_Player[0].beTouchTime = PLAYER_HIT_CD;
					}
				}
				
				
		}
		else if (g_Player[0].movecheck == false) {
			
			g_Player[0].rot.y = roty + cam->rot.y;
		}

		{
			g_Player[0].left_pos = g_Player[0].pos;
			g_Player[0].right_pos = g_Player[0].pos;
			g_Player[0].top_pos = g_Player[0].pos;
			g_Player[0].down_pos = g_Player[0].pos;
			g_Player[0].down_pos.y += 5.0f;
			g_Player[0].center_pos = g_Player[0].pos;
			g_Player[0].left_pos.x -= cosf(cam->rot.y)*(PLAYER_SIZE-5);
			g_Player[0].left_pos.y += PLAYER_TALL / 2;
			g_Player[0].left_pos.z += sinf(cam->rot.y)*(PLAYER_SIZE - 5);

			g_Player[0].right_pos.x += cosf(cam->rot.y)*(PLAYER_SIZE - 5);
			g_Player[0].right_pos.y += PLAYER_TALL / 2;
			g_Player[0].right_pos.z -= sinf(cam->rot.y)*(PLAYER_SIZE - 5);

			//g_Player[0].top_pos.x += sinf(PLAYER_SIZE2);
			g_Player[0].top_pos.y += PLAYER_TALL;
			//g_Player[0].top_pos.z += cosf(PLAYER_SIZE2);
			g_Player[0].center_pos.y += PLAYER_TALL/2;
			//g_Player[0].down_pos.x -= sinf(PLAYER_SIZE2);
			//g_Player[0].down_pos.y += (PLAYER_TALL/2) - PLAYER_SIZE2;
			//g_Player[0].down_pos.z -= cosf(PLAYER_SIZE2);
			
		}
		
		
		
		
		g_Player[0].wall_touch = 0;
		
		// �K�w�A�j���[�V����
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{
			// �g���Ă���Ȃ珈������
			/*if (g_Parts[j].use == true)
			{*/
				// �ړ�����
				int		index = (int)g_Parts[j].move_time;
				float	time = g_Parts[j].move_time - index;
				int		size = sizeof(move_tbl_1_1) / sizeof(INTERPOLATION_DATA);

				float dt = 1.0f / g_Parts[j].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��

				if (g_Player[0].touchcheck_field == true)
				{
					g_Parts[j].move_time += dt;					// �A�j���[�V�����̍��v���Ԃɑ���
				}


				if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_Parts[j].move_time = 0.0f;
					index = 0;
				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				D3DXVECTOR3 vec = g_Parts[j].tbl_adr[index + 1].pos - g_Parts[j].tbl_adr[index].pos;
				g_Parts[j].pos = g_Parts[j].tbl_adr[index].pos + vec * time;

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				D3DXVECTOR3 rot = g_Parts[j].tbl_adr[index + 1].rot - g_Parts[j].tbl_adr[index].rot;
				g_Parts[j].rot = g_Parts[j].tbl_adr[index].rot + rot * time;

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				D3DXVECTOR3 scl = g_Parts[j].tbl_adr[index + 1].scl - g_Parts[j].tbl_adr[index].scl;
				g_Parts[j].scl = g_Parts[j].tbl_adr[index].scl + scl * time;

			//}
		}
		
		//static MODE mode = GetMode();
		switch (gamedata->modenow)
		{
		case MODE_RESTAREA:
		{
			if ((g_Player[0].pos.x + PLAYER_SIZE) >= 500.0f)
			{
				g_Player[0].pos.x = 500.0f - PLAYER_SIZE;
			}
			else if ((g_Player[0].pos.x - PLAYER_SIZE) <= -500.0f)
			{
				g_Player[0].pos.x = (-500.0f) + PLAYER_SIZE;
			}

			if ((g_Player[0].pos.z + PLAYER_SIZE) >= -7000.0f)
			{
				g_Player[0].pos.z = (-7000.0f) - PLAYER_SIZE;
			}
			else if ((g_Player[0].pos.z - PLAYER_SIZE) <= -9000.0f)
			{
				g_Player[0].pos.z = (-9000.0f) + PLAYER_SIZE;
			}

			if (g_Player[0].pos.x >= -150.0f &&g_Player[0].pos.x <= 150.0f
				&&g_Player[0].pos.z >= -7050.0f&&g_Player[0].pos.y <= 160.0f)
			{
				SetFade(FADE_OUT, MODE_BATTLEAREA);
			}
		}
		break;
		case MODE_BATTLEAREA:
		{
			if (g_Player[0].pos.x >= -150.0f &&g_Player[0].pos.x <= 150.0f&&g_Player[0].pos.z <= -1800.0f)
			{
				if (enemyBOSS->use == true)
				{
					SetFade(FADE_OUT, MODE_RESTAREA);
				}
				else 
				{
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
			
		}
			break;
		default:
			break;
		}
		
		

		// �e���v���C���[�̈ʒu�ɍ��킹��
		D3DXVECTOR3 shadow_pos = g_Player[0].pos;
		shadow_pos.y =1.0f;
		float shadow_size= PLAYER_SHADOW_SIZE;
		if (g_Player[0].pos.z >= -1500.0f || g_Player[0].pos.z < -7000.0f)
		{
			for (int i = 0; i < MAX_MESH_FIELD; i++)
			{

				if (g_Player[0].pos.x >= g_fieldpoint[i].pos0.x&&g_Player[0].pos.x <= g_fieldpoint[i].pos1.x
					&&g_Player[0].pos.z >= g_fieldpoint[i].pos2.z&&g_Player[0].pos.z <= g_fieldpoint[i].pos0.z
					&&g_Player[0].pos.y >= g_fieldpoint[i].pos0.y)
				{
					if (g_fieldpoint[i].pos0.y > shadow_pos.y)
					{
						shadow_pos.y = g_fieldpoint[i].pos0.y + 1.0f;
					}
				}
			}
		}
		
		//�����̏�
		{
			if (g_Player[0].pos.x <= -600.0f&&g_Player[0].pos.z >= 1170.0f&&g_Player[0].pos.z <= 1300.0f)
			{
				if (g_Player[0].pos.y > -150.0f)
				{
					shadow_pos.y = -155.0f;
				}
				else
				{
					shadow_pos.y = -200.0f;
				}
			}
			else  if (g_Player[0].pos.x <= -600.0f&&g_Player[0].pos.z >= 820.0f&&g_Player[0].pos.z <= 1000.0f)
			{
				if (g_Player[0].pos.y > -150.0f)
				{
					shadow_pos.y = -155.0f;
				}
				else
				{
					shadow_pos.y = -200.0f;
				}
			}
			else  if (g_Player[0].pos.x <= -600.0f&&g_Player[0].pos.z >= 300.0f&&g_Player[0].pos.z <= 620.0f)
			{
				if (g_Player[0].pos.y > -150.0f)
				{
					shadow_pos.y = -155.0f;
				}
				else
				{
					shadow_pos.y = -200.0f;
				}
			}
			else  if (g_Player[0].pos.x >= 600.0f&&g_Player[0].pos.x <= 1500.0f
				&&g_Player[0].pos.z >= 1000.0f&&g_Player[0].pos.z <= 1300.0f)
			{
				if (g_Player[0].pos.y > -150.0f)
				{
					shadow_pos.y = -155.0f;
				}
				else
				{
					shadow_pos.y = -200.0f;
				}
			}
		}

		if (g_Player[0].pos.y - shadow_pos.y >= PLAYER_SHADOW_SIZE_YPOS_MAX)
		{
			shadow_size = PLAYER_SHADOW_SIZE_MAX;
		}
		else
		{
			if (g_Player[0].pos.y != shadow_pos.y)
			{
				shadow_size = PLAYER_SHADOW_SIZE-(PLAYER_SHADOW_SIZE - PLAYER_SHADOW_SIZE_MAX)*((g_Player[0].pos.y - shadow_pos.y) / PLAYER_SHADOW_SIZE_YPOS_MAX);
			}
			
		}
		SetPositionShadow(g_Player[0].shadowIdx, shadow_pos, shadow_size, shadow_size);


		//�����ƒn�ʉ��̔���
		{
			if (g_Player[0].pos.x >= -1500.0f&&g_Player[0].pos.x <= -600.0f
				&&g_Player[0].pos.z >= 1170.0f&&g_Player[0].pos.z <= 1300.0f)
			{
				if (g_Player[0].pos.y <= -150.0f) {
					gamedata->playerHPnow = 0.0f;

					//g_Player[0].pos.y = -160.0f;
					//g_Player[0].time_downplus = 0.01f;
					//g_Player[0].time_down = 0;
					//g_Player[0].jumpOK = true;
					//g_Player[0].bigjumpOK = true;
					//g_Player[0].touchcheck_field = true;
					//g_Player[0].bigjumping = false;

				}
			}
			else  if (g_Player[0].pos.x >= -1500.0f&&g_Player[0].pos.x <= -600.0f
				&&g_Player[0].pos.z >= 820.0f&&g_Player[0].pos.z <= 1000.0f)
			{
				if (g_Player[0].pos.y <= -150.0f) {

					gamedata->playerHPnow = 0.0f;
					//g_Player[0].pos.y = -160.0f;
					//g_Player[0].time_downplus = 0.01f;
					//g_Player[0].time_down = 0;
					//g_Player[0].jumpOK = true;
					//g_Player[0].bigjumpOK = true;
					//g_Player[0].touchcheck_field = true;
					//g_Player[0].bigjumping = false;
				}
			}
			else  if (g_Player[0].pos.x >= -1500.0f&&g_Player[0].pos.x <= -600.0f
				&&g_Player[0].pos.z >= 300.0f&&g_Player[0].pos.z <= 620.0f)
			{
				if (g_Player[0].pos.y <= -150.0f) {

					gamedata->playerHPnow = 0.0f;
					//g_Player[0].pos.y = -160.0f;
					//g_Player[0].time_downplus = 0.01f;
					//g_Player[0].time_down = 0;
					//g_Player[0].jumpOK = true;
					//g_Player[0].bigjumpOK = true;
					//g_Player[0].touchcheck_field = true;
					//g_Player[0].bigjumping = false;
				}
			}
			else  if (g_Player[0].pos.x >= 600.0f&&g_Player[0].pos.x <= 1500.0f
				&&g_Player[0].pos.z >= 1000.0f&&g_Player[0].pos.z <= 1300.0f)
			{
				if (g_Player[0].pos.y <= -150.0f) {
					gamedata->playerHPnow = 0.0f;
					//g_Player[0].pos.y = -160.0f;
					//g_Player[0].time_downplus = 0.01f;
					//g_Player[0].time_down = 0;
					//g_Player[0].jumpOK = true;
					//g_Player[0].bigjumpOK = true;
					//g_Player[0].touchcheck_field = true;
					//g_Player[0].bigjumping = false;
				}
			}
			else
			{
				if (g_Player[0].pos.y <= 1.0f) {

					g_Player[0].pos.y = 1.0f;
					g_Player[0].time_downplus = 0.01f;
					g_Player[0].time_down = 0;
					g_Player[0].jumpOK = true;
					g_Player[0].bigjumpOK = true;
					g_Player[0].touchcheck_field = true;
					g_Player[0].bigjumping = false;
				}
			}

		}


		//KEY��TOUCH
		{
			for (int k = 0; k < MAX_KEY; k++)
			{
				if (key[k].use == false)continue;
				D3DXVECTOR3 pos_key= g_Player[0].pos;
				pos_key.y += (PLAYER_TALL / 2);
				bool ans_key=CollisionBB(pos_key,key[k].pos, g_Player[0].touchsize, key[k].touchsize);
				if (ans_key == true)
				{
					key[k].use = false;
					PlaySound(SOUND_LABEL_SE_laser000);
					gamedata->getkey++;
					if (gamedata->getkey == MAX_KEY)
					{
						PlaySound(SOUND_LABEL_SE_bomb000);
					}
				}
			}
			
		}


		if (gamedata->playerHPnow <= 0)
		{
			
			if (gamedata->playernum>0)
			{
				gamedata->dying = true;
				for (int j = 0; j < MAX_PARTICLE_DIED; j++)
				{
					SetDieParticle(g_Player[0].pos);
				}
				
				g_Player[0].use = false;
				
				gamedata->playernum--;
				if (gamedata->playernum <= 0)
				{
					//gamedata->playerHPnow = gamedata->playerMAXHP;
					for (int j = 0; j < MAX_PARTICLE_DIED; j++)
					{
						SetDieParticle(g_Player[0].pos);
					}
					gamedata->gameover = true;
					gamedata->dying = true;
					g_Player[0].use = false;

				}
				else
				{
					gamedata->playerHPnow = gamedata->playerMAXHP;
				}


			}
			else if (gamedata->playernum<=0)
			{
				//gamedata->playerHPnow = gamedata->playerMAXHP;
				for (int j = 0; j < MAX_PARTICLE_DIED; j++)
				{
					SetDieParticle(g_Player[0].pos);
				}
				gamedata->gameover = true;
				gamedata->dying = true;
				g_Player[0].use = false;
				
			}
			
		}




	}
	
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (gamedata->dying == true|| gamedata->gameover==true)
	{
		return;
	}
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_PLAYER; i++) {
		if (g_Player[0].use == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Player[0].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_Player[0].scl.x, g_Player[0].scl.y, g_Player[0].scl.z);
			D3DXMatrixMultiply(&g_Player[0].mtxWorld, &g_Player[0].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[0].rot.y + D3DX_PI, g_Player[0].rot.x, g_Player[0].rot.z);
			D3DXMatrixMultiply(&g_Player[0].mtxWorld, &g_Player[0].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Player[0].pos.x, g_Player[0].pos.y, g_Player[0].pos.z);
			D3DXMatrixMultiply(&g_Player[0].mtxWorld, &g_Player[0].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&g_Player[0].mtxWorld);

			if (g_Player[0].beHit == true)
			{
				// ���f���`��
				DrawModel(&player_model_beHit);
			}
			else
			{
				// ���f���`��
				DrawModel(&player_model);
			}
			
		}
	}
	
	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Parts[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Parts[i].rot.y, g_Parts[i].rot.x, g_Parts[i].rot.z);
		D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &g_Parts[i].parent->mtxWorld);
			// ��
			// g_Player.mtxWorld���w���Ă���
		}

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Parts[i].mtxWorld);

		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

