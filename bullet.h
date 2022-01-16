//=============================================================================
//
// 弾処理 [bullet.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET		(30)	// 弾最大数
#define	MAX_ENEMY_BULLET_1		(30)	// 弾最大数
#define	MAX_ENEMY_BULLET_2		(30)	// 弾最大数
#define	MAX_ENEMY_BOSS		(100)	// 弾最大数
#define	BOSS_BULLET_ONENUM		(5)	// 弾最大数
#define BULLET_WIDTH	(20)
#define BULLET_HEIGHT	(20)
#define BULLET_LENGTH	(20)
//bigbullet
#define BULLET_CHECK_RADIUS	(20)

#define TEXTURE_BULLET_SIZE_X (20)
#define TEXTURE_BULLET_SIZE_Y (20)
#define TEXTURE_BULLETS_MAX			(1)			// テクスチャの数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 pos2;							// 分裂や上昇限界の位置
	D3DXVECTOR3 particlepos;					// 位置
	D3DXVECTOR3 scl;							// スケール
	MATERIAL	material;						// マテリアル
	D3DXVECTOR3 Hitcheck_size;
	bool		use;							// 使用しているかどうか
	bool		touchparticle;					// 使用しているかどうか

	D3DXVECTOR3 rot;							// 角度
	float		spd;							// 移動量
	bool		touchcheck;						//壁と地面当たり判定用
	D3DXVECTOR3 oneshoutmove;					//プレイヤーのshootingmode変更の時、弾の移動量
	bool		oneshoutcheck;					//プレイヤーのshootingmode変更の時、弾の移動の処理用

	float				time_down;				//下けの時間計算用
	float				time_downplus;			//下けの時間計算用
	float				time_up;				//上昇の時間計算用
	float				timeplus_up;			//上昇の時間計算用
	float				time_Split;				//分裂の時間計算用
	float				timeplus_Split;			//分裂の時間計算用

	bool		drawcheck;						//カメラCHECK用、if==true　弾の描画します
	D3DXVECTOR3 left_pos;						// カメラCHECK用の位置
	D3DXVECTOR3 right_pos;						// カメラCHECK用の位置
	D3DXVECTOR3 front_pos;						// カメラCHECK用の位置
	D3DXVECTOR3 back_pos;						// カメラCHECK用の位置
	D3DXVECTOR3 top_pos;						// カメラCHECK用の位置
	D3DXVECTOR3 down_pos;						// カメラCHECK用の位置
} BULLET;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int type);

BULLET *GetBullet(void);

BULLET *GetEnemyBullet1(void);

BULLET *GetEnemyBullet2(void);
BULLET *GetEnemyBulletBoss(void);

void BulletDrawCheck(void);