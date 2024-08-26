//============================================================
//
//�V���[�e�B���O�Q�[[effect.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _EHHECT_H_
#define _EFFECT_H_

#include "main.h"

//�G�t�F�N�g�̎��
typedef enum
{
	EFFECTTYPE_PLAYER = 0,
	EFFECTTYPE_ENEMY,
	EFFECTTYPE_MAX,
}EFFECTTYPE;

//�v���g�^�C�v�錾
void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col,float fRadius,float fLength, float nLife, EFFECTTYPE type);

#endif // !_EFFECT_H
