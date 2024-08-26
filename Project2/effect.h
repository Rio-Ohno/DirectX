//============================================================
//
//シューティングゲー[effect.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _EHHECT_H_
#define _EFFECT_H_

#include "main.h"

//エフェクトの種類
typedef enum
{
	EFFECTTYPE_PLAYER = 0,
	EFFECTTYPE_ENEMY,
	EFFECTTYPE_MAX,
}EFFECTTYPE;

//プロトタイプ宣言
void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col,float fRadius,float fLength, float nLife, EFFECTTYPE type);

#endif // !_EFFECT_H
