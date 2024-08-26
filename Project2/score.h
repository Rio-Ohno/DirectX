//============================================================
//
//シューティングゲー[score.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

//マクロ定義
#define MAX_DIGIT (8)                             //最大桁数
#define NUM_WIDTH (25)                            //ポリゴンの幅
#define NUM_HEIGHT (50)	                          //ポリゴンの高さ

//スコアの構造定義
typedef struct
{

	D3DXVECTOR3 pos;                              //位置
	float fLength;                                //対角線の長さ
	float fAngle;                                 //対角線の角度
	bool bUse;                                    //使用しているかどうか

}Score;

//プロトタイプ宣言
void InitScore();
void UninitScore();
void UpdateScore();
void DrawScore();
void SetScore(int nScore);
void AddScore(int nValue);
int Digit(int keta);
int DigitNum(int nScore);

#endif // !_SCORE_H_

