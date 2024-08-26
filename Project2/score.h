//============================================================
//
//�V���[�e�B���O�Q�[[score.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

//�}�N����`
#define MAX_DIGIT (8)                             //�ő包��
#define NUM_WIDTH (25)                            //�|���S���̕�
#define NUM_HEIGHT (50)	                          //�|���S���̍���

//�X�R�A�̍\����`
typedef struct
{

	D3DXVECTOR3 pos;                              //�ʒu
	float fLength;                                //�Ίp���̒���
	float fAngle;                                 //�Ίp���̊p�x
	bool bUse;                                    //�g�p���Ă��邩�ǂ���

}Score;

//�v���g�^�C�v�錾
void InitScore();
void UninitScore();
void UpdateScore();
void DrawScore();
void SetScore(int nScore);
void AddScore(int nValue);
int Digit(int keta);
int DigitNum(int nScore);

#endif // !_SCORE_H_

