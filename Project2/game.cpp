//============================================================
//
//�V���[�e�B���O�Q�[[game.cpp]
//Author:Rio Ohno
//
//============================================================

#include"game.h"
#include"backgraund.h"
#include"player.h"
#include"enemy.h"
#include"bullet.h"
#include"input.h"
#include "exploosion.h"
#include"main.h"
#include"fade.h"
#include"score.h"
#include"effect.h"

//�O���[�o���ϐ�
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameStat = 0;

//=============================================================================================================
//�Q�[����ʂ̏���������
//=============================================================================================================
void InitGame()
{
	//�w�i�̏���������
	Initbackgraund();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�e�̏�����
	InitBullet();

	//�G�̏�����
	InitEnemy();
	SetEnemy(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 0, 2);

	//�����̏�����
	InitExplosion();

	//�X�R�A�̏�����
	InitScore();

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameStat = 0;
}

//=============================================================================================================
//�Q�[����ʂ̏I������
//=============================================================================================================
void UninitGame()
{
	//�w�i�̏I������
	Uninitbackgraund();

	//�v���C���[�̏I������
	UninitPlayer();

	//�X�R�A�̏I������
	UninitScore();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�G�̏I������
	UninitEnemy();

	//�����̏I������
	UninitExplosion();

}

//=============================================================================================================
//�Q�[����ʂ̍X�V����
//=============================================================================================================
void UpdateGame()
{
	//�w�i�̍X�V����
	Updatebackgraund();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�e�̍X�V����
	UpdateBullet();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�G�̍X�V����
	UpdateEnemy();

	//�����̍X�V����
	UpdateExplosion();

	//�X�R�A�̍X�V����
	UpdateScore();

	Enemy* pEnemy;                                                      //�G���ւ̃|�C���^

	//�G�̎擾
	pEnemy = GetEnemy();

	if (KeyboardTrigger(DIK_M)==true)   //�Q�[���̏I������
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		//if (g_nNumEnemy <= 0)
		//{
		//	SetGameState(GAMESTATE_END);
		//}
		break;

	case GAMESTATE_END:
		g_nCounterGameStat++;
		if (g_nCounterGameStat >= 60)
		{
			g_nCounterGameStat = 0;
			g_gameState = GAMESTATE_NONE;

			//��ʂ̐ݒ�
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================================================
//�Q�[����ʂ̕`�揈��
//=============================================================================================================
void DrawGame()
{
	//�w�i�̕`�揈��
	Drawbackgraund();

	//�G�̕`�揈��
	DrawEnemy();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�e�̕`�揈��
	DrawBullet();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�����̕`�揈��
	DrawExplosion();

	//�X�R�A�̕`�揈��
	DrawScore();
}

//=============================================================================================================
//�Q�[���̏�Ԃ̐ݒ�
//=============================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}

//=============================================================================================================
//�Q�[���̏�Ԃ̎擾
//=============================================================================================================
GAMESTATE GetGameState()
{
	return g_gameState;
}