#pragma once

#include "AnimInfo.h"



// Animator�N���X

// �K�v�ȕϐ���ϐ��Z�b�g�Ƃ��č\���̂ɂ���
struct Animator {

	int frame;  // ���݂̃A�j���̃t���[���ԍ�������
	float time;  // �f���^�^�C�������Z�������Ԃ����ϐ�
	float speed;  // �A�j���[�V�����̍Đ����x

	const AnimInfo* pInfo; // �e�[�u���A�j���[�V�����̏��

	//�A�j���[�V������ON/OFF��؂�ւ��邽�߂̕ϐ�
	bool isActive;

	int flip; // ���]������ID���w�肷��

};


enum { // ���]������ID
	FLIP_NONE, // ���]���Ȃ�
	FLIP_VERTICAL, // �������]
	FLIP_HORIZONTAL // �������]
};


// �\���̂̕ϐ�������������֐�
void Animator_Initialize(Animator* pThis);

// ���t���[���Ăяo����A�A�j���[�V�����̃R�}��i�߂�֐�
void Animator_Update(Animator* pThis);

// �A�j���[�V�����e�[�u����ID���w�肵�ăZ�b�g����
void Animator_SetAnimInfo(Animator* pThis, int id);
