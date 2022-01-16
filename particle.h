//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PARTICLE_ROPE		(512)		// パーティクル最大数
#define	MAX_PARTICLE_BULLET		(1000)		// パーティクル最大数
#define	MAX_PARTICLE_TWOJUMP		(20)		// パーティクル最大数
#define	MAX_PARTICLE_DIED		(100)


#define	PARTICLE_BULLET_NUM		(5)		// パーティクル最大数

#define TWOJUMP_RADIUS		(30)
#define TWOJUMP_YPLUE		(0.01f)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 回転
	D3DXVECTOR3		scale;			// スケール
	D3DXVECTOR3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	bool			bUse;			// 使用しているかどうか

	int						g_TexNo;// テクスチャ番号
	D3DXMATRIX				g_mtxWorldParticle;// ワールドマトリックス
	D3DXVECTOR3				g_posBase;// ビルボード発生位置
	float				time;				//JUMPの時間計算用
	float				timeplus;			//JUMPの時間計算用
	D3DXVECTOR3				moveway;
	//bool			usecheck;
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT InitRopeParticle(void);
//void UninitRopeParticle(void);
//void UpdateRopeParticle(void);
//void DrawRopeParticle(void);

//HRESULT InitBulletParticle(void);
//void UninitBulletParticle(void);
//void UpdateBulletParticle(void);
//void DrawBulletParticle(void);

HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

PARTICLE *GetTwojumpParticle(void);
int SetBulletParticle(D3DXVECTOR3 bulletpos);
int SetDieParticle(D3DXVECTOR3 diedpos);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

