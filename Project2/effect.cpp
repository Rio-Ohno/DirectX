//============================================================
//
//�V���[�e�B���O�Q�[[effect.cpp]
//Author:Rio Ohno
//
//============================================================

#include"effect.h"
#include "enemy.h"
#include"player.h"

//�}�N����`
#define MAX_EFFECT (4096)                                  //�G�t�F�N�g�̍ő吔
#define EFFECT_WIDTH (50)                                  //�|���S���̕�
#define EFFECT_HEIGHT (50)                                 //�|���S���̍���

//�v���C���[�̃G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;                                       //�ʒu
	D3DXVECTOR3 move;                                      //�ړ���
	D3DXVECTOR3 rot;                                       //����
	D3DXCOLOR col;                                         //�F
	EFFECTTYPE type;                                       //���
	float fRadius;                                         //���a
	float fLife;                                           //����
	bool bUse;                                             //�g�p���Ă��邩�ǂ�
}Effect;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;                //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;           //���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];                              //�G�t�F�N�g�̏��

//=============================================================================================================
//����������
//=============================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect = NULL;

	//�f�o�C�X�Ɏ擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);


	//�G�t�F�N�g�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 25.0f;
		g_aEffect[nCntEffect].fLife = 0;
		g_aEffect[nCntEffect].bUse = false;                      //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
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

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================================================
//�G�t�F�N�g�̏I������
//=============================================================================================================
void UninitEffect()
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;

	}
}

//=============================================================================================================
//�G�t�F�N�g�̍X�V����
//=============================================================================================================
void UpdateEffect()
{
	int nCntEffect = NULL;

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���Ƃ�

			Enemy* pEnemy;                                                      //�G���ւ̃|�C���^
			Player* pPlayer;
			int nCntEnemy;

			//�G�̎擾
			pEnemy = GetEnemy();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (g_aEffect[nCntEffect].bUse == true)
				{
					if (g_aEffect[nCntEffect].type == EFFECTTYPE_PLAYER)//�v���C���[�̃G�t�F�N�g
					{
						//�G���g�p����Ă���Ƃ�
						if (g_aEffect[nCntEffect].pos.x >= pEnemy->pos.x + sinf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.x <= pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.y >= pEnemy->pos.y + cosf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
							&& g_aEffect[nCntEffect].pos.y <= pEnemy->pos.y + cosf(pEnemy->rot.z - (0.0f - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy)
						{

							g_aEffect[nCntEffect].bUse = false;
						}
					}
					else if (g_aEffect[nCntEffect].type == EFFECTTYPE_ENEMY)//�G�̃G�t�F�N�g
					{
						//�v���C���[�̎擾
						pPlayer = GetPlayer();

						//�v���C���[�Ƃ̓����蔻��
						if (g_aEffect[nCntEffect].pos.x >= pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.x <= pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.y >= pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aEffect[nCntEffect].pos.y <= pPlayer->pos.y + cosf(pPlayer->rot.z - (0.0f - pPlayer->fAngle)) * pPlayer->fLength)
						{							
							g_aEffect[nCntEffect].bUse = false;
						}
					}
				}

				//�X�e�[�g�������Ă����ł����̂��H
			}

			g_aEffect[nCntEffect].fRadius-=1.25f;

			//���_���W�̐ݒ�
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
				|| g_aEffect[nCntEffect].pos.y <= 0.0f || g_aEffect[nCntEffect].pos.y >= SCREEN_HEIGHT)//�G�t�F�N�g����ʊO�ɂł�
			{
				g_aEffect[nCntEffect].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

			g_aEffect[nCntEffect].fLife--;

			if (g_aEffect[nCntEffect].fLife <= 0)//�������s����
			{
				g_aEffect[nCntEffect].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================================================
//�G�t�F�N�g�̕`�揈��
//=============================================================================================================
void DrawEffect()
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�e�N�X�`���̐ݒ�
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			//�|���S���̕`��
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================================================
//�G�t�F�N�g�̐ݒ菈��
//=============================================================================================================
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col, float fRadius, float fLength, float fLife, EFFECTTYPE type)
{
	VERTEX_2D* pVtx;
	int nCntEffect;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//�G�t�F�N�g���g�p����Ă��Ȃ��Ƃ�
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fLife = fLife;
			g_aEffect[nCntEffect].type = type;

			//���_���W�̐ݒ�
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

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].fLife = fLife;
			g_aEffect[nCntEffect].bUse = true;//�g�p���Ă��Ԃɂ���
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

