//============================================================
//
//シューティングゲー[effect.cpp]
//Author:Rio Ohno
//
//============================================================

#include"effect.h"
#include "enemy.h"
#include"player.h"

//マクロ定義
#define MAX_EFFECT (4096)                                  //エフェクトの最大数
#define EFFECT_WIDTH (50)                                  //ポリゴンの幅
#define EFFECT_HEIGHT (50)                                 //ポリゴンの高さ

//プレイヤーのエフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;                                       //位置
	D3DXVECTOR3 move;                                      //移動量
	D3DXVECTOR3 rot;                                       //向き
	D3DXCOLOR col;                                         //色
	EFFECTTYPE type;                                       //種類
	float fRadius;                                         //半径
	float fLife;                                           //寿命
	bool bUse;                                             //使用しているかどう
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;                //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;           //頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];                              //エフェクトの情報

//=============================================================================================================
//初期化処理
//=============================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect = NULL;

	//デバイスに取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);


	//エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 25.0f;
		g_aEffect[nCntEffect].fLife = 0;
		g_aEffect[nCntEffect].bUse = false;                      //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める

	}
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================================================
//エフェクトの終了処理
//=============================================================================================================
void UninitEffect()
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;

	}
}

//=============================================================================================================
//エフェクトの更新処理
//=============================================================================================================
void UpdateEffect()
{
	int nCntEffect = NULL;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されているとき

			Enemy* pEnemy;                                                      //敵情報へのポインタ
			Player* pPlayer;
			int nCntEnemy;

			//敵の取得
			pEnemy = GetEnemy();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (g_aEffect[nCntEffect].bUse == true)
				{
					if (g_aEffect[nCntEffect].type == EFFECTTYPE_PLAYER)//プレイヤーのエフェクト
					{
						//敵が使用されているとき
						if (g_aEffect[nCntEffect].pos.x >= pEnemy->pos.x + sinf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.x <= pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.y >= pEnemy->pos.y + cosf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.y <= pEnemy->pos.y + cosf(pEnemy->rot.z - (0.0f - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy)
						{

							g_aEffect[nCntEffect].bUse = false;
						}
					}
					else if (g_aEffect[nCntEffect].type == EFFECTTYPE_ENEMY)//敵のエフェクト
					{
						//プレイヤーの取得
						pPlayer = GetPlayer();

						//プレイヤーとの当たり判定
						if (g_aEffect[nCntEffect].pos.x >= pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.x <= pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.y >= pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.y <= pPlayer->pos.y + cosf(pPlayer->rot.z - (0.0f - pPlayer->fAngle)) * pPlayer->fLength)
						{							
							g_aEffect[nCntEffect].bUse = false;
						}
					}
				}

				//ステート処理ってここでいいのか？
			}

			g_aEffect[nCntEffect].fRadius-=1.25f;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.z = 0.0f;

			if (g_aEffect[nCntEffect].pos.x <= 0.0f || g_aEffect[nCntEffect].pos.x >= SCREEN_WIDTH
				|| g_aEffect[nCntEffect].pos.y <= 0.0f || g_aEffect[nCntEffect].pos.y >= SCREEN_HEIGHT)//エフェクトが画面外にでた
			{
				g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
			}

			g_aEffect[nCntEffect].fLife--;

			if (g_aEffect[nCntEffect].fLife <= 0)//寿命が尽きた
			{
				g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================================================
//エフェクトの描画処理
//=============================================================================================================
void DrawEffect()
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//テクスチャの設定
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================================================
//エフェクトの設定処理
//=============================================================================================================
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col, float fRadius, float fLength, float fLife, EFFECTTYPE type)
{
	VERTEX_2D* pVtx;
	int nCntEffect;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//エフェクトが使用されていないとき
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fLife = fLife;
			g_aEffect[nCntEffect].type = type;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.z = 0.0f;

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].fLife = fLife;
			g_aEffect[nCntEffect].bUse = true;//使用してる状態にする
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

