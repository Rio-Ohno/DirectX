//============================================================
//
//シューティングゲー[bullet.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX,
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet();
void UninitBullet();
void UpdateBullet();
void DrawBullet();
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fLength,float nLife,BULLETTYPE type);

#endif // !_BULLET_H