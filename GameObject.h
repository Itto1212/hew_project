#pragma once
// ���@���̖��߂������Ă����ƁA�w�b�_�[�����x���C���N���[�h����Ē�`�d���G���[��
// �@�@�o��̂�h�����Ƃ��ł���B

#include "animator.h"
#include "CharController.h"
#include "UvInfo.h"
#include "collider.h"
#include "action.h"
#include "CharAI.h"

// GameObject�N���X

// GameObject�N���X�ŕK�v�ɂȂ�ϐ��Z�b�g���\���̂Ƃ��Ē�`
struct GameObject {

	// �ϐ������Ă���
	float posX, posY;  // ���S�_�̍��W
	float sizeX, sizeY; // �傫��

	// �K�w�\���ɂ��邽�߂̕ϐ�
	// ���̃I�u�W�F�N�g�����L���Ă���I�u�W�F�N�g�̒��S����ǂ̈ʒu�ɂ��邩
	// ����̈ʒu�����_�Ƃ������W�l�����[�J�����W�Ƃ���
	// ���L����Ă���I�u�W�F�N�g�̈ʒu�́A���L���Ă���I�u�W�F�N�g�̍��W�{���[�J�����W�Ō��܂�
	float localPosX, localPosY;
	GameObject* pOwner; // ���L���Ă���I�u�W�F�N�g�̃A�h���X

	CharController charController;  // �L�����ړ��̃N���X�ϐ�

	Animator animator;  // �A�j���[�V�����̃N���X�ϐ�

	struct {
		float r, g, b, a;
	} color;

	UvInfo uvinfo; // �e�N�X�`���̏��

	Collider collider;  // �����蔻��

	Action action; // �A�N�V����

	CharAI ai;  // AI

	float hp, maxHp;  // HP / MAX HP

};


// ���E�㉺�̂S�̍��W�l�����\����
struct FRECT {

	float left;
	float right;
	float top;
	float bottom;

};


// �����@�����o�[�֐��̃v���g�^�C�v�錾

// �X�v���C�g�̂S����XY���W���v�Z���ĕԂ��֐�
FRECT GameObject_GetXY(GameObject* pThis);

// �X�v���C�g�̂S����UV���W���v�Z���ĕԂ��֐�
FRECT GameObject_GetUV(GameObject* pThis);

// ���t���[���̍X�V����
void GameObject_Update(GameObject* pThis);


