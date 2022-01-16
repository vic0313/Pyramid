//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "GAME_battlearea.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER		(1)					// プレイヤーの数
#define PLAYER_SIZE		(18.0f)
#define PLAYER_TALL		(70.0f)
#define PLAYER_PARTS_MAX	(13)								// プレイヤーのパーツの数

#define PLAYER_FIRST_POS_X  (0.0f)
#define PLAYER_FIRST_POS_Z  (-1750.0f)
#define PLAYER_FIRST_POS_Y  (0.0f)

#define PLAYER_SIZE2		(40.0f)//壁透明化用
#define PLAYER_HIT_CD		(0.5f)

#define PLAYER_SHADOW_SIZE	(1.5f)							// 影の大きさ
#define PLAYER_SHADOW_SIZE_MAX	(0.0f)							// 影の大きさ
#define PLAYER_SHADOW_SIZE_YPOS_MAX	(110.0f)							// 影の大きさ
//*****************************************************************************
// 構造体定義
//*****************************************************************************
 struct PLAYER
{
	D3DXVECTOR3			pos;				// モデルの位置

	//壁透明化用
	D3DXVECTOR3			left_pos;
	D3DXVECTOR3			right_pos;
	D3DXVECTOR3			top_pos;
	D3DXVECTOR3			down_pos;
	D3DXVECTOR3			center_pos;

	D3DXVECTOR3			downpos;
	D3DXVECTOR3			ropepos;
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			rot_move;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3			touchsize;
	bool				beHit;
	bool				enemytouch;
	float				beHitTime;
	float				beHitTime_plus;
	float				beTouchTime;
	float				beTouchTime_plus;
	float				spd;				// 移動スピード
	
	DX11_MODEL			model;				// モデル情報

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

	int					shadowIdx;			// 影のインデックス番号

	bool				use;				// 生きるかどうか
	//float				radius;				// 半径

	int				wall_touch;

	float				time;				//JUMPの時間計算用
	float				timeplus;				//JUMPの時間計算用
	float				time_bigjump;				//JUMPの時間計算用
	float				time_bigjumpplus;				//JUMPの時間計算用
	float				time_down;				//JUMPの時間計算用
	float				time_downplus;				//JUMPの時間計算用
	float				time_rope;				//JUMPの時間計算用
	float				time_ropeplus;				//JUMPの時間計算用
	float				time_gunmod_use;
	bool				movecheck;
	bool				touchcheck;
	bool				touchcheck_field;
	bool				jumpOK;
	bool				jumping;
	bool				bigjumpOK;
	bool				bigjumping;
	bool				bigjumping_UP;
	bool				downOK;
	bool				namidause;
	D3DXVECTOR3			camera_pos;
	D3DXVECTOR3			camera_pos2;
	bool				ropeuse_OK;
	bool				ropeup_OK;
	int					ropemode;

	// 階層アニメーション用のメンバー変数
	int					load;				// パーツの読み込み
	INTERPOLATION_DATA	*tbl_adr;			// アニメデータのテーブル先頭アドレス
	float				move_time;			// 実行時間

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;			// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	D3DXQUATERNION		Quaternion;

	//D3DXVECTOR3			UpVector;			// 自分が立っている所
	
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);


