//============================================================
//
//シューティングゲー[input.cpp]
//Author:Rio Ohno
//
//============================================================

#include"input.h"

//マクロ定義
#define NUM_KEY_MAX (256)       //キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aOldKeyState[NUM_KEY_MAX];
XINPUT_STATE g_joyKeyState;//ジョイパッドのプレス情報

//=============================================================================================================
//キーボードの初期化処理
//=============================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================================================
//キーボードの終了処理
//=============================================================================================================
void UninitKeyboard()
{
	//入力デバイス（キーボード）の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================================================
//キーボードの更新処理
//=============================================================================================================
void UpdateKeyboard()
{
	BYTE aKeyState[NUM_KEY_MAX];//キーボードの入力情報
	int nCntKey;

	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		g_aOldKeyState[nCntKey] = g_aKeyState[nCntKey];//一つ前の情報を取得
	}

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードのプレス情報を取得
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================================================
//キーボードのプレス情報を取得
//=============================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================================================
//キーボードの押された瞬間
//=============================================================================================================
bool KeyboardTrigger(int nKey)
{
	bool Trigger = false;
	if (g_aKeyState[nKey] & 0x80 && !(g_aOldKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//=============================================================================================================
//キーボードを離した瞬間
//=============================================================================================================
bool KeyboardRelease(int nKey)
{
	bool Trigger=false;
	if (g_aOldKeyState[nKey] & 0x80 && !(g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//=============================================================================================================
//キーボード押してる時間
//=============================================================================================================
bool KeyboardRepeat(int nKey)
{
	bool Trigger = false;
	if (g_aOldKeyState[nKey] & 0x80 && (g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//=============================================================================================================
//ジョイパッドの初期化処理
//=============================================================================================================
HRESULT InitJoypad()
{
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//=============================================================================================================
//ジョイパッドの終了処理
//=============================================================================================================
void UninitJoypad()
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//=============================================================================================================
//ジョイパッドの更新処理
//=============================================================================================================
void UpdateJoypad()
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joyKeyState = joykeyState;
	}
}

//=============================================================================================================
//ジョイパッドのプレス情報を取得
//=============================================================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}