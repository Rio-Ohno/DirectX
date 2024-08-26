//============================================================
//
//シューティングゲー[exploosion.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//プロトタイプ宣言
void InitExplosion();
void UninitExplosion();
void UpdateExplosion();
void DrawExplosion();
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fLength);

#endif // !_EXPLOSION_H