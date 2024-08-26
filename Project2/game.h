//============================================================
//
//シューティングゲー[game.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

typedef enum
{
	GAMESTATE_NONE=0,
	GAMESTATE_NORMAL,
	GAMESTATE_END,
	GAMESTATE_MAX
}GAMESTATE;

//プロトタイプ宣言
void InitGame();
void UninitGame();
void UpdateGame();
void DrawGame();
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState();
#endif // !_GAME_H_

