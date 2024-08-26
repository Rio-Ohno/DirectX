//============================================================
//
//�V���[�e�B���O�Q�[[bullet.cpp]
//Author:Rio Ohno
//
//============================================================

#include "bullet.h"
#include "enemy.h"
#include"exploosion.h"
#include"player.h"
#include"effect.h"

//�}�N����`
#define MAX_BULLET (128)                                   //�e�̍ő吔
#define BULLET_WIDTH (50)                                  //�|���S���̕�
#define BULLET_HEIGHT (50)                                 //�|���S���̍���

//�v���C���[�̒e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;                                       //�ʒu
	D3DXVECTOR3 move;                                      //�ړ���
	D3DXVECTOR3 rot;                                       //����
	BULLETTYPE type;                                       //���
	float fLife;                                           //����
	bool bUse;                                             //�g�p���Ă��邩�ǂ�
	float fLengthBullet;                                   //�|���S���̑Ίp���̒���
	float fAngleBullet;                                    //�|���S���̊p�x
}Bullet;


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;           //���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];                              //�e�̏��

//=============================================================================================================
//����������
//=============================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet = NULL;

	//�f�o�C�X�Ɏ擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);


	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�Ίp���̒������Z�o����
		g_aBullet[nCntBullet].fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

		//�Ίp���̊p�x���Z�o����
		g_aBullet[nCntBullet].fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

		g_aBullet[nCntBullet].fLife = 0;
		g_aBullet[nCntBullet].bUse = false;                      //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//�e�̏I������
//=============================================================================================================
void UninitBullet()
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;

	}
}

//=============================================================================================================
//�e�̍X�V����
//=============================================================================================================
void UpdateBullet()
{
	int nCntBullet = NULL;

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���Ƃ�

			Enemy* pEnemy;                                                      //�G���ւ̃|�C���^
			Player* pPlayer;
			int nCntEnemy;

			//�G�̎擾
			pEnemy = GetEnemy();
			//�v���C���[�̎擾
			pPlayer = GetPlayer();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
					if (g_aBullet[nCntBullet].bUse == true)
					{
						if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)//�v���C���[�̒e
						{

							SetEffect(g_aBullet[nCntBullet].pos,D3DXCOLOR(0.02f,0.1f,0.15f,0.2f),25.0f, g_aBullet[nCntBullet].fLengthBullet,10, EFFECTTYPE_PLAYER);

							//�G���g�p����Ă���Ƃ�
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x + sinf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y + cosf(pEnemy->rot.z - (D3DX_PI - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy
								&& g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + cosf(pEnemy->rot.z - (0.0f - pEnemy->fAngleEnemy)) * pEnemy->fLengthEnemy)
							{
								if (pEnemy->bUse == true)
								{
									//�G�̃q�b�g����
									HitEnemy(nCntEnemy, 1);
								}
									//�����̐ݒ�
									SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot, g_aBullet[nCntBullet].fLengthBullet);

									g_aBullet[nCntBullet].bUse = false;
							}
						}
						else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)//�G�̒e
						{	

							//�v���C���[�Ƃ̓����蔻��
							if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
								&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + cosf(pPlayer->rot.z - (0.0f - pPlayer->fAngle)) * pPlayer->fLength||pPlayer->state!=PLAYERSTATE_APPEAR)
							{
								if (pPlayer->bUse == true)
								{
									//�v���C���[�̃q�b�g����
									HitPlayer(400);
								}

								//�����̐ݒ�
								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot, g_aBullet[nCntBullet].fLengthBullet);

								g_aBullet[nCntBullet].bUse = false;
							}
						}
					}

					//�X�e�[�g�������Ă����ł����̂��H
			}

			//�e�̈ʒu�X�V
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			//���_���W�̐ݒ�
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
				                                  || g_aBullet[nCntBullet].pos.y <= 0.0f|| g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)//�e����ʊO�ɂł�
			{
				g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

			g_aBullet[nCntBullet].fLife --;

			if (g_aBullet[nCntBullet].fLife <= 0)//�������s����
			{
				g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[nCntBullet].rot,g_aBullet[nCntBullet].fLengthBullet);
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//�e�̕`�揈��
//=============================================================================================================
void DrawBullet()
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			//�|���S���̕`��
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBullet*4,2);
		}
	}
}

//=============================================================================================================
//�e�̐ݒ菈��
//=============================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fLength,float fLife,BULLETTYPE type)
{
	VERTEX_2D* pVtx;
	int nCntBullet;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�e���g�p����Ă��Ȃ��Ƃ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].fLife = fLife;
			g_aBullet[nCntBullet].type = type;

			//���_���W�̐ݒ�
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
			g_aBullet[nCntBullet].bUse = true;//�g�p���Ă��Ԃɂ���
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}
