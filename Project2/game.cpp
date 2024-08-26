//============================================================
//
//シューティングゲー[game.cpp]
//Author:Rio Ohno
//
//============================================================

#include"game.h"
#include"backgraund.h"
#include"player.h"
#include"enemy.h"
#include"bullet.h"
#include"input.h"
#include "exploosion.h"
#include"main.h"
#include"fade.h"
#include"score.h"
#include"effect.h"

//グローバル変数
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameStat = 0;

//=============================================================================================================
//ゲーム画面の初期化処理
//=============================================================================================================
void InitGame()
{
	//背景の初期化処理
	Initbackgraund();

	//プレイヤーの初期化処理
	InitPlayer();

	//エフェクトの初期化
	InitEffect();

	//弾の初期化
	InitBullet();

	//敵の初期化
	InitEnemy();
	SetEnemy(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 0, 2);

	//爆発の初期化
	InitExplosion();

	//スコアの初期化
	InitScore();

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameStat = 0;
}

//=============================================================================================================
//ゲーム画面の終了処理
//=============================================================================================================
void UninitGame()
{
	//背景の終了処理
	Uninitbackgraund();

	//プレイヤーの終了処理
	UninitPlayer();

	//スコアの終了処理
	UninitScore();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//敵の終了処理
	UninitEnemy();

	//爆発の終了処理
	UninitExplosion();

}

//=============================================================================================================
//ゲーム画面の更新処理
//=============================================================================================================
void UpdateGame()
{
	//背景の更新処理
	Updatebackgraund();

	//プレイヤーの更新処理
	UpdatePlayer();

	//弾の更新処理
	UpdateBullet();

	//エフェクトの更新処理
	UpdateEffect();

	//敵の更新処理
	UpdateEnemy();

	//爆発の更新処理
	UpdateExplosion();

	//スコアの更新処理
	UpdateScore();

	Enemy* pEnemy;                                                      //敵情報へのポインタ

	//敵の取得
	pEnemy = GetEnemy();

	if (KeyboardTrigger(DIK_M)==true)   //ゲームの終わる条件
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		//if (g_nNumEnemy <= 0)
		//{
		//	SetGameState(GAMESTATE_END);
		//}
		break;

	case GAMESTATE_END:
		g_nCounterGameStat++;
		if (g_nCounterGameStat >= 60)
		{
			g_nCounterGameStat = 0;
			g_gameState = GAMESTATE_NONE;

			//画面の設定
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================================================
//ゲーム画面の描画処理
//=============================================================================================================
void DrawGame()
{
	//背景の描画処理
	Drawbackgraund();

	//敵の描画処理
	DrawEnemy();

	//プレイヤーの描画処理
	DrawPlayer();

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//爆発の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();
}

//=============================================================================================================
//ゲームの状態の設定
//=============================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}

//=============================================================================================================
//ゲームの状態の取得
//=============================================================================================================
GAMESTATE GetGameState()
{
	return g_gameState;
}