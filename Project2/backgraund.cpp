//============================================================
//
//�V���[�e�B���O�Q�[[backgraund.cpp]
//Author:Rio Ohno
//
//============================================================
#include"player.h"
#include"backgraund.h"

//�}�N����`
#define NUM_BG (3)                                                                //�w�i�̐�

LPDIRECT3DTEXTURE9 g_pTexturebackgraund[NUM_BG] = {};                             //�e�N�X�`��(�O����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbackgraund = NULL;                              //���_�����i�[
float g_aPosTexU[NUM_BG];                                                         //�e�N�X�`�����W�̊J�n�ʒu�iU�l�j

//=============================================================================================================
//�v���C���[�̍X�V����
//=============================================================================================================
void Updatebackgraund()
{
	int nCntBG = 0;

	LPDIRECT3DDEVICE9 pDevice;                                    //�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx2;                                              //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffbackgraund->Lock(0, 0, (void**)&pVtx2, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
	    //�e�N�X�`�����W�̊J�n�ʒu�iU�l�j�̏�����
		g_aPosTexU[nCntBG] -= 0.008f;

		pVtx2[0].tex = D3DXVECTOR2(0.0f, g_aPosTexU[nCntBG]);
		pVtx2[1].tex = D3DXVECTOR2(1.0f, g_aPosTexU[nCntBG]);
		pVtx2[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_aPosTexU[nCntBG]);
		pVtx2[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_aPosTexU[nCntBG]);

  		pVtx2 += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffbackgraund->Unlock();
}

//=============================================================================================================
//�v���C���[�̏���������
//=============================================================================================================
void Initbackgraund()
{
	LPDIRECT3DDEVICE9 pDevice;                 //�f�o�C�X�ւ̃|�C���^
	int nCntBG;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg100.png",
		&g_pTexturebackgraund[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_pTexturebackgraund[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_pTexturebackgraund[2]);

	//�e�N�X�`�����W�̊J�n�ʒu�iU�l�j�̏�����
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexU[nCntBG] = 0.0f;
	}

	//���_�o�b�t�@�̂̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffbackgraund,
		NULL);

	VERTEX_2D* pVtx2;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffbackgraund->Lock(0, 0, (void**)&pVtx2, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//���_���W�̐ݒ�
		pVtx2[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
		pVtx2[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx2[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx2[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx2[0].rhw = 1.0f;
		pVtx2[1].rhw = 1.0f;
		pVtx2[2].rhw = 1.0f;
		pVtx2[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx2[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx2[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


		//�e�N�X�`�����W�̐ݒ�
		pVtx2[0].tex = D3DXVECTOR2(0.0f, g_aPosTexU[nCntBG]);
		pVtx2[1].tex = D3DXVECTOR2(1.0f, g_aPosTexU[nCntBG]);
		pVtx2[2].tex = D3DXVECTOR2(0.0f, 1.0f + g_aPosTexU[nCntBG]);
		pVtx2[3].tex = D3DXVECTOR2(1.0f, 1.0f + g_aPosTexU[nCntBG]);

		pVtx2 += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffbackgraund->Unlock();

}

//=============================================================================================================
//�v���C���[�̕`�揈��
//=============================================================================================================
void Drawbackgraund()
{
	LPDIRECT3DDEVICE9 pDevice;                 //�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffbackgraund, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->SetTexture(0, g_pTexturebackgraund[nCntBG]);
		//BG�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG*4, 2);
	}

}

//=============================================================================================================
//BG�̏I������
//=============================================================================================================
void Uninitbackgraund()
{
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturebackgraund[nCntBG] != NULL)
		{
			g_pTexturebackgraund[nCntBG]->Release();
			g_pTexturebackgraund[nCntBG] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffbackgraund != NULL)
	{
		g_pVtxBuffbackgraund->Release();
		g_pVtxBuffbackgraund= NULL;
	}
}