//============================================================
//
//�V���[�e�B���O�Q�[[enemy.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"main.h"

//�}�N����`
#define MAX_ENEMY (128)                    //�G�̍ő吔

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL=0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;                       //�ʒu
	D3DXVECTOR3 rot;                       //����
	D3DXVECTOR3 move;                      //����
	ENEMYSTATE state;                      //���
	int nCountState;                       //��ԊǗ��J�E���^�[
	int nLife;                             //�̗�
	int nType;                             //���
	int nCount;                            //�e�J�E���g�p
	bool bUse;                             //�g�p���Ă��邩�ǂ���
	float fLengthEnemy;                    //�|���S���̑Ίp���̒���
	float fAngleEnemy;                     //�|���S���̊p�x
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife);
void HitEnemy(int nCutEnemy, float nDamage);
Enemy* GetEnemy();

#endif // !_ENEMY_H_

