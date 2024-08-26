//============================================================
//
//シューティングゲー[bullet.cpp]
//Author:Rio Ohno
//
//============================================================

#include "bullet.h"
#include "enemy.h"
#include"exploosion.h"
#include"player.h"
#include"effect.h"

//マクロ定義
#define MAX_BULLET (128)                                   //弾の最大数
#define BULLET_WIDTH (50)                                  //ポリゴンの幅
#define BULLET_HEIGHT (50)                                 //ポリゴンの高さ

//プレイヤーの弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;                                       //位置
	D3DXVECTOR3 move;                                      //移動量
	D3DXVECTOR3 rot;                                       //向き
	BULLETTYPE type;                                       //種類
	float fLife;                                           //寿命
	bool bUse;                                             //使用しているかどう
	float fLengthBullet;                                   //ポリゴンの対角線の長さ
	float fAngleBullet;                                    //ポリゴンの角度
}Bullet;


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;           //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];                              //弾の情報

//=============================================================================================================
//初期化処理
//=============================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet = NULL;

	//デバイスに取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);


	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//対角線の長さを算出する
		g_aBullet[nCntBullet].fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

		//対角線の角度を算出する
		g_aBullet[nCntBullet].fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

		g_aBullet[nCntBullet].fLife = 0;
		g_aBullet[nCntBullet].bUse = false;                      //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める

	}
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//弾の終了処理
//=============================================================================================================
void UninitBullet()
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;

	}
}

//=============================================================================================================
//弾の更新処理
//=============================================================================================================
void UpdateBullet()
{
	int nCntBullet = NULL;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されているとき

			Enemy* pEnemy;                                                      //敵情報へのポインタ
			Player* pPlayer;
			int nCntEnemy;

			//敵の取得
			pEnemy = GetEnemy();
			//プレイヤーの取得
			pPlayer = GetPlayer();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
					if (g_aBullet[nCntBullet].bUse == true)
					{
						if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)//プレイヤーの弾
						{

							SetEffect(g_aBullet[nCntBullet].pos,D3DXCOLOR(0.02f,0.1f,0.15f,0.2f),25.0f, g_aBullet[nCntBullet].fLengthBullet,10, EFFECTTYPE_PLAYER);

							//敵が使用されているとき
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x + sinf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y + cosf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + cosf(pEnemy->rot.z - (0.0f - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy)
							{
								if (pEnemy->bUse == true)
								{
									//敵のヒット処理
									HitEnemy(nCntEnemy, 1);
								}
									//爆発の設定
									SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot, g_aBullet[nCntBullet].fLengthBullet);

									g_aBullet[nCntBullet].bUse = false;
							}
						}
						else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)//敵の弾
						{	

							//プレイヤーとの当たり判定
							if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + cosf(pPlayer->rot.z - (0.0f - pPlayer->fAngle)) * pPlayer->fLength||pPlayer->state!=PLAYERSTATE_APPEAR)
							{
								if (pPlayer->bUse == true)
								{
									//プレイヤーのヒット処理
									HitPlayer(400);
								}

								//爆発の設定
								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot, g_aBullet[nCntBullet].fLengthBullet);

								g_aBullet[nCntBullet].bUse = false;
							}
						}
					}

					//ステート処理ってここでいいのか？
			}

			//弾の位置更新
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.z = 0.0f;

			if (g_aBullet[nCntBullet].pos.x<=0.0f|| g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH
				                                  || g_aBullet[nCntBullet].pos.y <= 0.0f|| g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)//弾が画面外にでた
			{
				g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			}

			g_aBullet[nCntBullet].fLife --;

			if (g_aBullet[nCntBullet].fLife <= 0)//寿命が尽きた
			{
				g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot,g_aBullet[nCntBullet].fLengthBullet);
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//弾の描画処理
//=============================================================================================================
void DrawBullet()
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			//ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBullet*4,2);
		}
	}
}

//=============================================================================================================
//弾の設定処理
//=============================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fLength,float fLife,BULLETTYPE type)
{
	VERTEX_2D* pVtx;
	int nCntBullet;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//弾が使用されていないとき
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].fLife = fLife;
			g_aBullet[nCntBullet].type = type;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].fLengthBullet = fLength;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].fLife = fLife;
			g_aBullet[nCntBullet].bUse = true;//使用してる状態にする
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}
