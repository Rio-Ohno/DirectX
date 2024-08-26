//============================================================
//
//シューティングゲー[input.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include"main.h"

//キー入力の種類
typedef enum
{
	JOYKEY_UP=0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_L3,
	JOYKEY_R3,
	JOYKEY_L1,
	JOYKEY_R1,
	JOYKEY_L2,//軸判定で使えない
	JOYKEY_R2,//軸判定で使えない
	JOYKEY_A,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_MAX
}JOYKEY;

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard();
void UpdateKeyboard();
bool GetKeyboardPress(int nKey);
bool KeyboardTrigger(int nKey);
bool KeyboardRelease(int nKey);
bool KeyboardRepeat(int nKey);

HRESULT InitJoypad(void);
void UninitJoypad();
void UpdateJoypad();
bool GetJoypadPress(JOYKEY key);
//bool GetJoypadTrigger(JOYKEY key);
//bool GetJoypadRelease(JOYKEY key);
//bool GetJoypadRepeat(JOYKEY key);

#endif // !_INPUT_H_

