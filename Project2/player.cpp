//============================================================
//
//シューティングゲー[player.cpp]
//Author:Rio Ohno
//
//============================================================

#include"player.h"
#include"input.h"
#include"bullet.h"
#include"enemy.h"
#include"exploosion.h"
#include"game.h"


//マクロ定義
#define MAX_SIZE (400.0f)                                                       //ポリゴンの最大(対角線)
#define MIN_SIZE (50.0f)                                                        //ポリゴンの最小(対角線)
#define NUM_WIDTH (100)                                                         //ポリゴンの幅
#define NUM_HEIGHT (200)	                                                    //ポリゴンの高さ

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;                                     //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer=NULL;                                  //頂点情報を格納
Player g_Player;

int g_nCounterAnimPlayer = 0;                                                   //アニメーションカウンタ
int g_nPatternAnimPlayer = 0;                                                   //アニメーションパターンNo.
static int nData = 0;                                                           //方向検知用

//=============================================================================================================
//プレイヤーの初期化処理
//=============================================================================================================
void InitPlayer()
{

	LPDIRECT3DDEVICE9 pDevice;                                                  //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\00000402.png",
		&g_pTexturePlayer);

	g_nCounterAnimPlayer = 0;                                //カウンターを初期化する
	g_nPatternAnimPlayer = 0;                                //パターンNO.を初期化する

	g_Player.pos = D3DXVECTOR3(640.0f,360.0f,0);             //位置を初期化する
	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);             //移動量を初期化する
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            //向きを初期化する
	g_Player.fLife = 1000.0f;                                //体力の初期化
	g_Player.state = PLAYERSTATE_NORMAL;                     //敵の状態の初期化
	g_Player.nCountState = 0;								 //状態管理用カウンターの初期化
	g_Player.bUse = true;                                    //自機を使用状態にする
	g_Player.nCntPlayer = 3;								 //残機数を初期化

	//対角線の長さを算出する
	g_Player.fLength = sqrtf(NUM_WIDTH* NUM_WIDTH + NUM_HEIGHT * NUM_HEIGHT) / 2.0f;

	//対角線の角度を算出する
	g_Player.fAngle = atan2f(NUM_WIDTH, NUM_HEIGHT);

	//頂点バッファのの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,                               
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;                                         //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;


	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================================================
//プレイヤーの描画処理
//=============================================================================================================
void DrawPlayer()
{
	LPDIRECT3DDEVICE9 pDevice;                 //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player.bUse == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}

	//DrawPlayerを一個で済ませたいなら設定->描画->設定->描画の順

	////テクスチャの設定
	//pDevice->SetTexture(0, g_pTexturePlayer2);

	////プレイヤーの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

}

//=============================================================================================================
//プレイヤーの更新処理
//=============================================================================================================
void UpdatePlayer()
{
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGE:

		g_Player.nCountState--;

		if (g_Player.nCountState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			VERTEX_2D* pVtx;

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			//頂点バッファをアンロックする
			g_pVtxBuffPlayer->Unlock();

		}

		break;

	case PLAYERSTATE_APPEAR:

		g_Player.bUse = true;

		g_Player.nCountState--;

		break;

	case PLAYERSTATE_DEATH:
		g_Player.nCountState--;

		if (g_Player.nCountState <= 0)
		{
			SetGameState(GAMESTATE_END);

			return;//updateから抜ける
		}

		break;

	}

	if (g_Player.bUse == true)
	{
		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
		{
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{//Wキー||上（ゲームパッド）が押された
				g_Player.move.y += cosf(-D3DX_PI * 0.75f) * 0.5f;
				g_Player.move.x += sinf(-D3DX_PI * 0.75f) * 0.5f;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//Sキー||下（ゲームパッド）が押された
				g_Player.move.y += cosf(-D3DX_PI * 0.25) * 0.5f;
				g_Player.move.x += sinf(-D3DX_PI * 0.25f) * 0.5f;
			}
			else
			{//Aキー||左（ゲームパッド）だけ
				g_Player.move.x -= 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
		{//Dキーが押された

			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{//Wキーが押された
				g_Player.move.y += cosf(D3DX_PI * 0.75f) * 0.5f;
				g_Player.move.x += sinf(D3DX_PI * 0.75f) * 0.5f;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//Sキーが押された
				g_Player.move.y += cosf(D3DX_PI * 0.25f) * 0.5f;
				g_Player.move.x += sinf(D3DX_PI * 0.25f) * 0.5f;
			}
			else
			{//Dキーだけ
				g_Player.move.x += 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//Wキーが押された
			g_Player.move.y -= 0.5f;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//sキーが押された
			g_Player.move.y += 0.5f;
		}

		if (GetKeyboardPress(DIK_RIGHT) == true)
		{//右
			g_Player.rot.z -= 0.05f;
		}
		else if (GetKeyboardPress(DIK_LEFT) == true)
		{//左
			g_Player.rot.z += 0.05f;
		}

		if (GetKeyboardPress(DIK_UP) == true)
		{//上
			if (g_Player.fLength < MAX_SIZE)
			{
				g_Player.fLength += 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//下
			if (g_Player.fLength > MIN_SIZE)
			{
				g_Player.fLength -= 0.5f;
			}
		}

		if (GetKeyboardPress(DIK_R) == true)
		{//Rキー
			InitPlayer();
			InitBullet();
			InitEnemy();
			SetEnemy(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 0, 2);
		}

		if (KeyboardTrigger(DIK_SPACE) == true || GetJoypadPress(JOYKEY_X) == true)
		{//スペースキー||Xキー（ゲームパッド）
			SetBullet(g_Player.pos, D3DXVECTOR3(sinf(g_Player.rot.z + D3DX_PI) * 7.5f, cosf(g_Player.rot.z + D3DX_PI) * 7.5f, 0.0f), g_Player.rot, g_Player.fLength, 100, BULLETTYPE_PLAYER);
			//SetBullet(g_Player.pos, D3DXVECTOR3(sinf(g_Player.rot.z + D3DX_PI*0.75f) * 7.5f, cosf(g_Player.rot.z + D3DX_PI*0.75f) * 7.5f, 0.0f), g_Player.rot, g_Player.fLength,100,BULLETTYPE_PLAYER);
			//SetBullet(g_Player.pos, D3DXVECTOR3(sinf(g_Player.rot.z + (-D3DX_PI*0.75f)) * 7.5f, cosf(g_Player.rot.z + (-D3DX_PI*0.75f)) * 7.5f, 0.0f), g_Player.rot,g_Player.fLength,100,BULLETTYPE_PLAYER);
		}

		//位置を更新
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		//移動量を更新（減衰）
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.08f;
		g_Player.move.y += (0.0f - g_Player.move.y) * 0.08f;//あんまりいらないから数字でかめにしてる

		VERTEX_2D* pVtx;//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.z = 0.0f;

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}
}

//=============================================================================================================
//プレイヤーの終了処理
//=============================================================================================================
void UninitPlayer()
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================================================
//弾が当たった時の処理
//=============================================================================================================
void HitPlayer(float fDamage)
{
	g_Player.fLife -= fDamage;

	if (g_Player.fLife <= 0)
	{
		g_Player.nCountState = 15;
		g_Player.bUse = false;
		g_Player.state = PLAYERSTATE_APPEAR;
	}
	else if (g_Player.nCntPlayer <= 0)
	{
		g_Player.nCountState = 15;
		g_Player.bUse = false;
		g_Player.state = PLAYERSTATE_DEATH;
	}
	else
	{
		g_Player.nCountState = 5;                    //ダメージ状態を保つ時間を設定
		g_Player.state = PLAYERSTATE_DAMAGE;

		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();

	}
}

//=============================================================================================================
//プレイヤーの取得
//=============================================================================================================
Player*GetPlayer()
{
	return &g_Player;
}
