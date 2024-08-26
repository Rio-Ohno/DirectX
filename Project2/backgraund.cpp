//============================================================
//
//シューティングゲー[backgraund.cpp]
//Author:Rio Ohno
//
//============================================================
#include"player.h"
#include"backgraund.h"

//マクロ定義
#define NUM_BG (3)                                                                //背景の数

LPDIRECT3DTEXTURE9 g_pTexturebackgraund[NUM_BG] = {};                             //テクスチャ(三枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbackgraund = NULL;                              //頂点情報を格納
float g_aPosTexU[NUM_BG];                                                         //テクスチャ座標の開始位置（U値）

//=============================================================================================================
//プレイヤーの更新処理
//=============================================================================================================
void Updatebackgraund()
{
	int nCntBG = 0;

	LPDIRECT3DDEVICE9 pDevice;                                    //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx2;                                              //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffbackgraund->Lock(0, 0, (void**)&pVtx2, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
	    //テクスチャ座標の開始位置（U値）の初期化
		g_aPosTexU[nCntBG] -= 0.008f;

		pVtx2[0].tex = D3DXVECTOR2(0.0f, g_aPosTexU[nCntBG]);
		pVtx2[1].tex = D3DXVECTOR2(1.0f, g_aPosTexU[nCntBG]);
		pVtx2[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_aPosTexU[nCntBG]);
		pVtx2[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_aPosTexU[nCntBG]);

  		pVtx2 += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffbackgraund->Unlock();
}

//=============================================================================================================
//プレイヤーの初期化処理
//=============================================================================================================
void Initbackgraund()
{
	LPDIRECT3DDEVICE9 pDevice;                 //デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg100.png",
		&g_pTexturebackgraund[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_pTexturebackgraund[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_pTexturebackgraund[2]);

	//テクスチャ座標の開始位置（U値）の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexU[nCntBG] = 0.0f;
	}

	//頂点バッファのの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffbackgraund,
		NULL);

	VERTEX_2D* pVtx2;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffbackgraund->Lock(0, 0, (void**)&pVtx2, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//頂点座標の設定
		pVtx2[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
		pVtx2[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx2[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx2[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx2[0].rhw = 1.0f;
		pVtx2[1].rhw = 1.0f;
		pVtx2[2].rhw = 1.0f;
		pVtx2[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx2[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


		//テクスチャ座標の設定
		pVtx2[0].tex = D3DXVECTOR2(0.0f, g_aPosTexU[nCntBG]);
		pVtx2[1].tex = D3DXVECTOR2(1.0f, g_aPosTexU[nCntBG]);
		pVtx2[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_aPosTexU[nCntBG]);
		pVtx2[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_aPosTexU[nCntBG]);

		pVtx2 += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffbackgraund->Unlock();

}

//=============================================================================================================
//プレイヤーの描画処理
//=============================================================================================================
void Drawbackgraund()
{
	LPDIRECT3DDEVICE9 pDevice;                 //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffbackgraund, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->SetTexture(0, g_pTexturebackgraund[nCntBG]);
		//BGの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG*4, 2);
	}

}

//=============================================================================================================
//BGの終了処理
//=============================================================================================================
void Uninitbackgraund()
{
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの破棄
		if (g_pTexturebackgraund[nCntBG] != NULL)
		{
			g_pTexturebackgraund[nCntBG]->Release();
			g_pTexturebackgraund[nCntBG] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffbackgraund != NULL)
	{
		g_pVtxBuffbackgraund->Release();
		g_pVtxBuffbackgraund= NULL;
	}
}