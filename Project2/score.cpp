//============================================================
//
//シューティングゲー[score.cpp]
//Author:Rio Ohno
//
//============================================================

#include"score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;               //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;          //頂点バッファへのポインタ
Score g_score[MAX_DIGIT];                                //スコアの構造体
int  g_nScore;                                           //スコアの値

//=============================================================================================================
// スコアの初期化処理
//=============================================================================================================
void InitScore()
{
	LPDIRECT3DDEVICE9 pDevice;

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		g_score[nCount].bUse = false;
		g_score[nCount].fLength, g_score[nCount].fAngle = 0.0f;

		//対角線の長さを算出する
		g_score[nCount].fLength = sqrtf(NUM_WIDTH * NUM_WIDTH + NUM_HEIGHT * NUM_HEIGHT) / 2.0f;

		//対角線の角度を算出する
		g_score[nCount].fAngle = atan2f(NUM_WIDTH, NUM_HEIGHT);

		g_score[nCount].pos = D3DXVECTOR3(1040.0f, 35.0f, 0.0f);//位置を初期化

	}

	g_score[0].bUse = true;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureScore);

	g_nScore = 0;//スコアを初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		//頂点情報の設定
		pVtx[0].pos.x = g_score[nCount].pos.x - (20*nCount) + sinf(-(D3DX_PI - g_score[nCount].fAngle)) * g_score[nCount].fLength;
		pVtx[0].pos.y = g_score[nCount].pos.y + cosf(-(D3DX_PI - g_score[nCount].fAngle)) * g_score[nCount].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_score[nCount].pos.x - (20 * nCount) + sinf(D3DX_PI - g_score[nCount].fAngle) * g_score[nCount].fLength;
		pVtx[1].pos.y = g_score[nCount].pos.y + cosf(D3DX_PI - g_score[nCount].fAngle) * g_score[nCount].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_score[nCount].pos.x - (20 * nCount) + sinf(0.0f - g_score[nCount].fAngle) * g_score[nCount].fLength;
		pVtx[2].pos.y = g_score[nCount].pos.y + cosf(0.0f - g_score[nCount].fAngle) * g_score[nCount].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_score[nCount].pos.x - (20 * nCount) + sinf(-(0.0f - g_score[nCount].fAngle)) * g_score[nCount].fLength;
		pVtx[3].pos.y = g_score[nCount].pos.y + cosf(-(0.0f - g_score[nCount].fAngle)) * g_score[nCount].fLength;
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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//スコアの終了処理
//=============================================================================================================
void UninitScore()
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================================================
//スコアの更新処理
//=============================================================================================================
void UpdateScore()
{//スコアの表示に何か加えたかったら書く感じ


}

//=============================================================================================================
//スコアの描画処理
//=============================================================================================================
void DrawScore()
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCount = MAX_DIGIT-1; nCount >=0; nCount--)
	{
		if (g_score[nCount].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=============================================================================================================
//スコアの設定処理
//=============================================================================================================
void SetScore(int nScore)
{
	int aPosTexU[MAX_DIGIT];      //各桁の数字を格納
	int digit[2];                 //各桁の数字計算の10の累乗の格納
	int digitNum = 0;             //桁数

	VERTEX_2D* pVtx{};

	g_nScore = nScore;            //スコアグローバル化

	digitNum = DigitNum(g_nScore);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (g_score[nCount].bUse == true)
		{
			digit[0] = Digit(MAX_DIGIT - nCount);
			digit[1] = Digit(MAX_DIGIT - (nCount + 1));

			//各桁の数字を代入
			aPosTexU[nCount] = g_nScore % digit[0] / digit[1];

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 1.0f);

			pVtx += 4;
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//スコア加算処理
//=============================================================================================================
void AddScore(int nValue)
{
	int aPosTexU[MAX_DIGIT];//各桁の数字を格納
	//int digit[2];           //桁計算のためのやつ
	int digitNum=0;         //桁数

	VERTEX_2D* pVtx{};

	g_nScore += nValue;     //加算

	digitNum = DigitNum(g_nScore);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (g_score[nCount].bUse == true)
		{

			//digit[0] = Digit(MAX_DIGIT - nCount);
			//digit[1] = Digit(MAX_DIGIT - (nCount + 1));

			////各桁の数字を代入
			//aPosTexU[nCount] = g_nScore % digit[0] / digit[1];                    //0=8桁目

			aPosTexU[nCount] = g_nScore % 10;                                       //0=1桁目
			g_nScore /= 10;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 1.0f);

			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//10の累乗の計算
//=============================================================================================================
int Digit(int ndigit)
{
	int Digit = 1;//10の累乗

	for(int nCount = 0;nCount < ndigit; nCount++)
	{ 
		Digit *= 10;
	}

	return Digit;
}

//=============================================================================================================
//桁数カウント
//=============================================================================================================
int DigitNum(int nScore)
{
	int ndigit = 0, nData;//桁数カウント

	nData = nScore;

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (nData == 0)//桁分使用したい
		{
			for (nCount = 0; nCount < ndigit; nCount++)
			{
				g_score[nCount].bUse = true;
			}
			break;
		}
		else
		{
			nData /= 10;
			ndigit++;//桁数インクリメント
		}
	}

	return ndigit;
}