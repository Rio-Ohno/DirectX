//============================================================
//
//シューティングゲー[main.h]
//Author:Rio Ohno
//
//=============================================================

#ifndef _MAIN_H_
#define _MAIN_H_


#include<windows.h>
#include"d3dx9.h"
#define DIRECTUNPUT_VERSION (0x0800)        //ビルド時・x告・・pマクロ
#include"dinput.h"                          //入・・揩?K要
#include"Xinput.h"                          //ジョイパッドに必要
#include"xaudio2.h"

//ライブラリ・潟塔N 
#pragma comment(lib,"d3d9.lib")             //描・理・K要
#pragma comment(lib,"d3dx9.lib")            //[d3d9.lib]・g張ライブラリ
#pragma comment(lib,"dxguid.lib")           //DirectXコンポーネント（部品）使用・K要
#pragma comment(lib,"winmm.lib")            //システム時刻誌・?K要
#pragma comment(lib,"dinput8.lib")          //入・・揩?K要
#pragma comment(lib,"xinput.lib")           //ジョイパッド処理に必要

//マクロ定義
#define CLASS_NAME "WindowClass"			//ウィンドウクラス・ｼ前
#define WINDOW_NAME "DirectX"        	    //ウィンドウ・ｼ前（キャプション・ｼ前）
#define SCREEN_WIDTH (1280)					//ウィンドウ・・
#define SCREEN_HEIGHT (720)				    //ウィンドウ・ｂｳ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//頂点情報[2D]の構造体
typedef struct
{

	D3DXVECTOR3 pos;                        //頂点座標
	float rhw;                              //座標・ｷ用係数（1.0f・?閨j
	D3DCOLOR col;                           //頂点カラー
	D3DXVECTOR2 tex;                        //テクスチャ座標

}VERTEX_2D;

//画面モードの種類
typedef enum
{
	MODE_TITLE=0,                           //タイトル画面
	MODE_GAME,                              //ゲーム画面
	MODE_RESULT,                            //リザルト画面
	MODE_MAX
}MODE;

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit();
void Update();
void Draw();
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode();

#endif // !_MAIN_H_