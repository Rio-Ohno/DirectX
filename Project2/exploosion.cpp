//============================================================
//
//シューティングゲー[exploosion.cpp]
//Author:Rio Ohno
//
//============================================================

#include "exploosion.h"
#include "main.h"

//マクロ定義
#define MAX_EXPLOSION (128)                                   //弾の最大数
#define EXPLOSION_WIDTH (50)                                  //ポリゴンの幅
#define EXPLOSION_HEIGHT (50)                                 //ポリゴンの高さ

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;                                          //位置
	D3DXVECTOR3 rot;                                          //向き
	D3DXCOLOR col;                                            //色
	bool bUse;                                                //使用しているかどう
	int nCounterAnim;                                         //アニメーションカウンター
	int nPatternAnim;                                         //アニメーションパターン
	float fLengthExplosion;                                   //ポリゴンの対角線の長さ
	float fAngleExplosion;                                    //ポリゴンの角度
}EXPLOSION;


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;           //頂点バッファへのポイン
EXPLOSION g_aExplosion[MAX_EXPLOSION];                        //弾の情報

//=============================================================================================================
//初期化処理
//=============================================================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion = NULL;

	//デバイスに取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Explosion000.png",
		&g_pTextureExplosion);


	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].fAngleExplosion = 0;
		//対角線の長さを算出する
		g_aExplosion[nCntExplosion].fLengthExplosion = sqrtf(EXPLOSION_WIDTH * EXPLOSION_WIDTH + EXPLOSION_HEIGHT * EXPLOSION_HEIGHT) / 2.0f;

		//対角線の角度を算出する
		g_aExplosion[nCntExplosion].fAngleExplosion = atan2f(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);

		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;                      //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
 		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f,1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める

	}
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================================================
//弾の終了処理
//=============================================================================================================
void UninitExplosion()
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================================================
//弾の更新処理
//=============================================================================================================
void UpdateExplosion()
{
	int nCntExplosion = 0;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用されているとき

			g_aExplosion[nCntExplosion].nCounterAnim++;
			if ((g_aExplosion[nCntExplosion].nCounterAnim % 10) == 0)
			{
				g_aExplosion[nCntExplosion].nCounterAnim = 0;

				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 10;

				if (g_aExplosion[nCntExplosion].nPatternAnim>8)//総パターン数を超えた
				{
					g_aExplosion[nCntExplosion].bUse = false;//使用していない状態にする
				}

				//テクスチャ座標の更新
				pVtx[0].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f+0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f+ 0.125f * g_aExplosion[nCntExplosion].nPatternAnim, 1.0f);
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================================================
//弾の描画処理
//=============================================================================================================
void DrawExplosion()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		pDevice->SetTexture(0, g_pTextureExplosion);
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntExplosion*4,2);
		}
	}
}

//=============================================================================================================
//爆発の設定処理
//=============================================================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fLength)
{
	VERTEX_2D* pVtx;
	int nCntExplosion;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			//弾が使用されていないとき
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].rot = rot;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].fLengthExplosion = fLength;

			//頂点座標の設定
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_aExplosion[nCntExplosion].rot.z - (D3DX_PI - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_aExplosion[nCntExplosion].rot.z - (D3DX_PI - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_aExplosion[nCntExplosion].rot.z + (D3DX_PI - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_aExplosion[nCntExplosion].rot.z + (D3DX_PI - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_aExplosion[nCntExplosion].rot.z + (0.0f - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_aExplosion[nCntExplosion].rot.z + (0.0f - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_aExplosion[nCntExplosion].rot.z - (0.0f - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_aExplosion[nCntExplosion].rot.z - (0.0f - g_aExplosion[nCntExplosion].fAngleExplosion)) * g_aExplosion[nCntExplosion].fLengthExplosion;
			pVtx[3].pos.z = 0.0f;

			g_aExplosion[nCntExplosion].bUse = true;//使用してる状態にする
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
