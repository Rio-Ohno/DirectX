//============================================================
//
//シューティングゲー[enemy.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"main.h"

//マクロ定義
#define MAX_ENEMY (128)                    //敵の最大数

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL=0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;                       //位置
	D3DXVECTOR3 rot;                       //向き
	D3DXVECTOR3 move;                      //向き
	ENEMYSTATE state;                      //状態
	int nCountState;                       //状態管理カウンター
	int nLife;                             //体力
	int nType;                             //種類
	int nCount;                            //弾カウント用
	bool bUse;                             //使用しているかどうか
	float fLengthEnemy;                    //ポリゴンの対角線の長さ
	float fAngleEnemy;                     //ポリゴンの角度
}Enemy;

//プロトタイプ宣言
void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife);
void HitEnemy(int nCutEnemy, float nDamage);
Enemy* GetEnemy();

#endif // !_ENEMY_H_

