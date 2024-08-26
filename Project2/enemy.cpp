//============================================================
//
//シューティングゲー[enemy.cpp]
//Author:Rio Ohno
//
//============================================================

#include"enemy.h"
#include"bullet.h"
#include"exploosion.h"
#include"game.h"
#include"score.h"

//マクロ定義
#define NUM_ENEMY (4)                                   //敵の種類
#define ENEMY_WIDTH (50)                                //ポリゴンの幅
#define ENEMY_HEIGHT (50)                               //ポリゴンの高さ

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];                              //敵の情報
int g_nNumEnemy = 0;                         //画面に出ている敵の総数

//=============================================================================================================
//敵の初期化処理
//=============================================================================================================
void InitEnemy()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy = 0;;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ（４枚分）の読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\masuta-supa-ku.jpg",
		&g_apTextureEnemy[0]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\masuta-supa-ku.jpg",
	//	&g_apTextureEnemy[1]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\masuta-supa-ku.jpg",
	//	&g_apTextureEnemy[2]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\masuta-supa-ku.jpg",
	//	&g_apTextureEnemy[3]);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//対角線の長さを算出する
		g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;

		//対角線の角度を算出する
		g_aEnemy[nCntEnemy].fAngleEnemy = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);

		g_aEnemy[nCntEnemy].nCountState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

		g_aEnemy[nCntEnemy].bUse = false;                      //使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//敵情報の初期化
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================================================
//敵の終了処理
//=============================================================================================================
void UninitEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================================================
//敵の更新処理
//=============================================================================================================
void UpdateEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)//敵の最大数分回す
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].nCount++;
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				if (g_aEnemy[nCntEnemy].nCount == 40)//ｎCountが20の時に弾を出す
				{
					//使用されている敵が弾を発射する処理
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 5.0f, cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 5.0f, 0.0f), g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fLengthEnemy, 100.0f, BULLETTYPE_ENEMY);
					g_aEnemy[nCntEnemy].nCount = 0;
				}
				break;

			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCountState--;

				if (g_aEnemy[nCntEnemy].nCountState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					VERTEX_2D* pVtx;

					//頂点バッファをロックし、頂点データへのポインタを取得
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

					pVtx += 4;

					g_pVtxBuffEnemy->Unlock();

				}
				break;
			}
			g_aEnemy[nCntEnemy].move.x = 0;
		}
	}
}

//=============================================================================================================
//敵の描画処理
//=============================================================================================================
void DrawEnemy()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている時

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================================================
//敵の設定処理
//=============================================================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife)
{
	int nCntEnemy;
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されない

			//敵の情報
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nLife = nLife;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (0.0f - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (0.0f - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - (0.0f - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - (0.0f - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy ;
			pVtx[3].pos.z = 0.0f;

			g_aEnemy[nCntEnemy].bUse = true;//使用している状態にする
			g_nNumEnemy++;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================================================
//弾が当たった時の処理
//=============================================================================================================
void HitEnemy(int nCntEnemy,float nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//エフェクトの設定
		g_aEnemy[nCntEnemy].bUse = false;                       //使用してない状態にする
		AddScore(1000);                                         //スコア加算
		g_nNumEnemy--;                                          //画面上の敵の総数カウントダウン

		if (g_nNumEnemy <= 0)//リザルト画面へ
		{
			SetGameState(GAMESTATE_END);
		}

	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCountState = 5;                    //ダメージ状態を保つ時間を設定

		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;

		//頂点カラーの設定(赤)
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);

		g_pVtxBuffEnemy->Unlock();
	}
}

//=============================================================================================================
//敵の取得
//=============================================================================================================
Enemy* GetEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		return &g_aEnemy[nCntEnemy];
	}
}