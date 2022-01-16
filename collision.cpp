//=============================================================================
//
// 数学計算処理 [collision.cpp]
// Author : GP11A132 43 林尚頡
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static CAMERA		*camera = GetCamera();
//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool CollisionBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	D3DXVECTOR3 min1, max1;
	D3DXVECTOR3 min2, max2;

	min1.x = pos1.x - size1.x;
	min1.y = pos1.y - size1.y;
	min1.z = pos1.z - size1.z;
	max1.x = pos1.x + size1.x;
	max1.y = pos1.y + size1.y;
	max1.z = pos1.z + size1.z;

	min2.x = pos2.x - size2.x;
	min2.y = pos2.y - size2.y;
	min2.z = pos2.z - size2.z;
	max2.x = pos2.x + size2.x;
	max2.y = pos2.y + size2.y;
	max2.z = pos2.z + size2.z;

	//X軸の判定
	if (min1.x < max2.x && max1.x > min2.x)
	{
		//Y軸の判定
		if (min1.y < max2.y && max1.y > min2.y)
		{
			//Z軸の判定
			if (min1.z < max2.z && max1.z > min2.z)
			{
				//全ての条件がTRUEならヒット判定
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	//pos1とpos2をつなぐベクトルを作る
	D3DXVECTOR3 vDistance = pos1 - pos2;

	//vDistanceのベクトル長を得る
	float length;
	length = D3DXVec3Length(&vDistance);

	//1と2の半径を足して判定する
	//差分のベクトル長の方が小さければ
	//ヒットしている
	if (length < (size1 + size2))
	{
		return true;
	}

	return false;
}


//=============================================================================
// 内積(dot)
//=============================================================================
float dotProduct(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// ダイレクトＸでは、、、
	float ans = D3DXVec3Dot(v1, v2);
#endif

	return(ans);


}


//=============================================================================
// 外積(cross)
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// ダイレクトＸでは、、、
	D3DXVec3Cross(ret, v1, v2);
#endif

}


//=============================================================================
// レイキャスト
// p0, p1, p2　ポリゴンの３頂点
// pos0 始点
// pos1 終点
// hit　交点の返却用
// normal 法線ベクトルの返却用
// 当たっている場合、trueを返す
//=============================================================================
bool RayCast(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *hit, D3DXVECTOR3 *normal)
{
	
	D3DXVECTOR3		nor;		// ポリゴンの法線
	D3DXVECTOR3		vec1;
	D3DXVECTOR3		vec2;
	float			d1, d2;

	{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		D3DXVec3Normalize(&nor, &nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを1にして)
		*normal = nor;						// 求めた法線を入れておく
	}

	// ポリゴン平面と線分の内積とって衝突している可能性を調べる（鋭角なら＋、鈍角ならー、直角なら０）
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if ((d1 * d2) > 0.0f)
		{
			// 当たっている可能性は無い
			return(false);
		}
	}


	{	// ポリゴンと線分の交点を求める
		d1 = (float)fabs(d1);		//絶対値を求めている
		d2 = (float)fabs(d2);		//絶対値を求めている
		float a = d1 / (d1 + d2);							// 内分比

		D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0から交点へのベクトル
		D3DXVECTOR3		p3 = p0 + vec3;						// 交点
		*hit = p3;											// 求めた交点を入れておく

		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p0 - p2;

			// 各頂点と交点とのベクトル
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// 外積で各辺の法線を求めて、ポリゴンの法線との内積をとって符号をチェックする
			D3DXVECTOR3		n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(false);	// 当たっていない

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(false);	// 当たっていない
			
			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(false);	// 当たっていない
		}
	}

	return(true);	// 当たっている！(hitには当たっている交点が入っている。normalには法線が入っている)
}

