//============================================================
//
//シューティングゲー[fade.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _FADE_H_
#define _FADE_H_

#include"main.h"

//フェードの状態
typedef enum
{
	FADE_NONE=0,//真っ暗
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade();
void UpdateFade();
void DrawFade();
void SetFade(MODE modeNext);
FADE GetFade();

#endif // !_FADE_H_

