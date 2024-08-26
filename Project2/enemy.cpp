//============================================================
//
//�V���[�e�B���O�Q�[[enemy.cpp]
//Author:Rio Ohno
//
//============================================================

#include"enemy.h"
#include"bullet.h"
#include"exploosion.h"
#include"game.h"
#include"score.h"

//�}�N����`
#define NUM_ENEMY (4)                                   //�G�̎��
#define ENEMY_WIDTH (50)                                //�|���S���̕�
#define ENEMY_HEIGHT (50)                               //�|���S���̍���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];                              //�G�̏��
int g_nNumEnemy = 0;                         //��ʂɏo�Ă���G�̑���

//=============================================================================================================
//�G�̏���������
//=============================================================================================================
void InitEnemy()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy = 0;;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���i�S�����j�̓Ǎ�
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

		//�Ίp���̒������Z�o����
		g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;

		//�Ίp���̊p�x���Z�o����
		g_aEnemy[nCntEnemy].fAngleEnemy = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);

		g_aEnemy[nCntEnemy].nCountState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

		g_aEnemy[nCntEnemy].bUse = false;                      //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�G���̏�����
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================================================
//�G�̏I������
//=============================================================================================================
void UninitEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================================================
//�G�̍X�V����
//=============================================================================================================
void UpdateEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)//�G�̍ő吔����
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].nCount++;
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				if (g_aEnemy[nCntEnemy].nCount == 40)//��Count��20�̎��ɒe���o��
				{
					//�g�p����Ă���G���e�𔭎˂��鏈��
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

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

					//���_�J���[�̐ݒ�
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
//�G�̕`�揈��
//=============================================================================================================
void DrawEnemy()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă��鎞

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================================================
//�G�̐ݒ菈��
//=============================================================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife)
{
	int nCntEnemy;
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ȃ�

			//�G�̏��
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nLife = nLife;

			//���_���W�̐ݒ�
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

			g_aEnemy[nCntEnemy].bUse = true;//�g�p���Ă����Ԃɂ���
			g_nNumEnemy++;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================================================
//�e�������������̏���
//=============================================================================================================
void HitEnemy(int nCntEnemy,float nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//�G�t�F�N�g�̐ݒ�
		g_aEnemy[nCntEnemy].bUse = false;                       //�g�p���ĂȂ���Ԃɂ���
		AddScore(1000);                                         //�X�R�A���Z
		g_nNumEnemy--;                                          //��ʏ�̓G�̑����J�E���g�_�E��

		if (g_nNumEnemy <= 0)//���U���g��ʂ�
		{
			SetGameState(GAMESTATE_END);
		}

	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCountState = 5;                    //�_���[�W��Ԃ�ۂ��Ԃ�ݒ�

		VERTEX_2D* pVtx;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;

		//���_�J���[�̐ݒ�(��)
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f);

		g_pVtxBuffEnemy->Unlock();
	}
}

//=============================================================================================================
//�G�̎擾
//=============================================================================================================
Enemy* GetEnemy()
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		return &g_aEnemy[nCntEnemy];
	}
}