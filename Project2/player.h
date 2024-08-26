//============================================================
//
//シューティングゲー[player.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _Player_H_
#define _Player_H_

#include"main.h"

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NORMAL = 0,								//通常時
	PLAYERSTATE_DAMAGE,									//ダメージ状態
	PLAYERSTATE_APPEAR,									//復活時の無敵状態
	PLAYERSTATE_DEATH,									//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの構造定義
typedef struct
{
	D3DXVECTOR3 pos;                                    //位置
	D3DXVECTOR3 move;                                   //移動量
	D3DXVECTOR3 rot;                                    //向き
	PLAYERSTATE state;                                  //プレイヤーの状態
	int nCountState;                                    //状態管理カウンター
	int nCntPlayer;										//プレイヤーの残機数
	float fLength;                                      //対角線の長さ
	float fAngle;                                       //対角線の角度
	float fLife;                                        //体力
	bool bUse;                                          //使用しているかどうか
}Player;

//プロトタイプ宣言
void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
void HitPlayer(float fDamage);
void StatePlayer();
Player* GetPlayer();

#endif // !_Player_H_
