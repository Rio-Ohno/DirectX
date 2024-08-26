//============================================================
//
//�V���[�e�B���O�Q�[[score.cpp]
//Author:Rio Ohno
//
//============================================================

#include"score.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;               //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;          //���_�o�b�t�@�ւ̃|�C���^
Score g_score[MAX_DIGIT];                                //�X�R�A�̍\����
int  g_nScore;                                           //�X�R�A�̒l

//=============================================================================================================
// �X�R�A�̏���������
//=============================================================================================================
void InitScore()
{
	LPDIRECT3DDEVICE9 pDevice;

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		g_score[nCount].bUse = false;
		g_score[nCount].fLength, g_score[nCount].fAngle = 0.0f;

		//�Ίp���̒������Z�o����
		g_score[nCount].fLength = sqrtf(NUM_WIDTH * NUM_WIDTH + NUM_HEIGHT * NUM_HEIGHT) / 2.0f;

		//�Ίp���̊p�x���Z�o����
		g_score[nCount].fAngle = atan2f(NUM_WIDTH, NUM_HEIGHT);

		g_score[nCount].pos = D3DXVECTOR3(1040.0f, 35.0f, 0.0f);//�ʒu��������

	}

	g_score[0].bUse = true;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureScore);

	g_nScore = 0;//�X�R�A��������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		//���_���̐ݒ�
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

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//�X�R�A�̏I������
//=============================================================================================================
void UninitScore()
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================================================
//�X�R�A�̍X�V����
//=============================================================================================================
void UpdateScore()
{//�X�R�A�̕\���ɉ����������������珑������


}

//=============================================================================================================
//�X�R�A�̕`�揈��
//=============================================================================================================
void DrawScore()
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCount = MAX_DIGIT-1; nCount >=0; nCount--)
	{
		if (g_score[nCount].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=============================================================================================================
//�X�R�A�̐ݒ菈��
//=============================================================================================================
void SetScore(int nScore)
{
	int aPosTexU[MAX_DIGIT];      //�e���̐������i�[
	int digit[2];                 //�e���̐����v�Z��10�̗ݏ�̊i�[
	int digitNum = 0;             //����

	VERTEX_2D* pVtx{};

	g_nScore = nScore;            //�X�R�A�O���[�o����

	digitNum = DigitNum(g_nScore);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (g_score[nCount].bUse == true)
		{
			digit[0] = Digit(MAX_DIGIT - nCount);
			digit[1] = Digit(MAX_DIGIT - (nCount + 1));

			//�e���̐�������
			aPosTexU[nCount] = g_nScore % digit[0] / digit[1];

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 1.0f);

			pVtx += 4;
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//�X�R�A���Z����
//=============================================================================================================
void AddScore(int nValue)
{
	int aPosTexU[MAX_DIGIT];//�e���̐������i�[
	//int digit[2];           //���v�Z�̂��߂̂��
	int digitNum=0;         //����

	VERTEX_2D* pVtx{};

	g_nScore += nValue;     //���Z

	digitNum = DigitNum(g_nScore);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (g_score[nCount].bUse == true)
		{

			//digit[0] = Digit(MAX_DIGIT - nCount);
			//digit[1] = Digit(MAX_DIGIT - (nCount + 1));

			////�e���̐�������
			//aPosTexU[nCount] = g_nScore % digit[0] / digit[1];                    //0=8����

			aPosTexU[nCount] = g_nScore % 10;                                       //0=1����
			g_nScore /= 10;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCount], 1.0f);

			pVtx += 4;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================================================
//10�̗ݏ�̌v�Z
//=============================================================================================================
int Digit(int ndigit)
{
	int Digit = 1;//10�̗ݏ�

	for(int nCount = 0;nCount < ndigit; nCount++)
	{ 
		Digit *= 10;
	}

	return Digit;
}

//=============================================================================================================
//�����J�E���g
//=============================================================================================================
int DigitNum(int nScore)
{
	int ndigit = 0, nData;//�����J�E���g

	nData = nScore;

	for (int nCount = 0; nCount < MAX_DIGIT; nCount++)
	{
		if (nData == 0)//�����g�p������
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
			ndigit++;//�����C���N�������g
		}
	}

	return ndigit;
}