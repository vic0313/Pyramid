//=============================================================================
//
// ���w�v�Z���� [collision.cpp]
// Author : GP11A132 43 �я���
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "Map.h"
#include "meshMap.h"
#include "meshMapReverse.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MAP_POINT	*g_fieldpoint = GetFieldPoint();
static CAMERA		*camera = GetCamera();
//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
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

	//X���̔���
	if (min1.x < max2.x && max1.x > min2.x)
	{
		//Y���̔���
		if (min1.y < max2.y && max1.y > min2.y)
		{
			//Z���̔���
			if (min1.z < max2.z && max1.z > min2.z)
			{
				//�S�Ă̏�����TRUE�Ȃ�q�b�g����
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	//pos1��pos2���Ȃ��x�N�g�������
	D3DXVECTOR3 vDistance = pos1 - pos2;

	//vDistance�̃x�N�g�����𓾂�
	float length;
	length = D3DXVec3Length(&vDistance);

	//1��2�̔��a�𑫂��Ĕ��肷��
	//�����̃x�N�g�����̕������������
	//�q�b�g���Ă���
	if (length < (size1 + size2))
	{
		return true;
	}

	return false;
}


//=============================================================================
// ����(dot)
//=============================================================================
float dotProduct(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// �_�C���N�g�w�ł́A�A�A
	float ans = D3DXVec3Dot(v1, v2);
#endif

	return(ans);


}


//=============================================================================
// �O��(cross)
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// �_�C���N�g�w�ł́A�A�A
	D3DXVec3Cross(ret, v1, v2);
#endif

}


//=============================================================================
// ���C�L���X�g
// p0, p1, p2�@�|���S���̂R���_
// pos0 �n�_
// pos1 �I�_
// hit�@��_�̕ԋp�p
// normal �@���x�N�g���̕ԋp�p
// �������Ă���ꍇ�Atrue��Ԃ�
//=============================================================================
bool RayCast(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *hit, D3DXVECTOR3 *normal)
{
	
	D3DXVECTOR3		nor;		// �|���S���̖@��
	D3DXVECTOR3		vec1;
	D3DXVECTOR3		vec2;
	float			d1, d2;

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�(���̏����͌Œ蕨�Ȃ�\��Init()�ōs���Ă����Ɨǂ�)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		D3DXVec3Normalize(&nor, &nor);		// �v�Z���₷���悤�ɖ@�����m�[�}���C�Y���Ă���(���̃x�N�g���̒�����1�ɂ���)
		*normal = nor;						// ���߂��@�������Ă���
	}

	// �|���S�����ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�i�s�p�Ȃ�{�A�݊p�Ȃ�[�A���p�Ȃ�O�j
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// ���߂��|���S���̖@���ƂQ�̃x�N�g���i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if ((d1 * d2) > 0.0f)
		{
			// �������Ă���\���͖���
			return(false);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		d1 = (float)fabs(d1);		//��Βl�����߂Ă���
		d2 = (float)fabs(d2);		//��Βl�����߂Ă���
		float a = d1 / (d1 + d2);							// ������

		D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;	// p0�����_�ւ̃x�N�g��
		D3DXVECTOR3		p3 = p0 + vec3;						// ��_
		*hit = p3;											// ���߂���_�����Ă���

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// �O�ςŊe�ӂ̖@�������߂āA�|���S���̖@���Ƃ̓��ς��Ƃ��ĕ������`�F�b�N����
			D3DXVECTOR3		n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(false);	// �������Ă��Ȃ�

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(false);	// �������Ă��Ȃ�
			
			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(false);	// �������Ă��Ȃ�
		}
	}

	return(true);	// �������Ă���I(hit�ɂ͓������Ă����_�������Ă���Bnormal�ɂ͖@���������Ă���)
}

