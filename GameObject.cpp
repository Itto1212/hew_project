#include "GameObject.h"
#include "GameTimer.h"


FRECT GameObject_GetXY(GameObject * pThis)
{
	// �|�C���^�@���@����ϐ��̑��݂��Ă�ꏊ��\�������i�Ԓn/�A�h���X�j�����邽�߂̔��i�ϐ��j

	// �A�h���X����\���̂̒��̕ϐ��ɃA�N�Z�X����ɂ̓A���[���Z�q�i->�j���g��
	float leftX = pThis->posX - pThis->sizeX / 2;
	float topY = pThis->posY + pThis->sizeY / 2;
	float rightX = leftX + pThis->sizeX;
	float bottomY = topY - pThis->sizeY;

	FRECT rect = { leftX, rightX, topY, bottomY };

	return rect;
}

FRECT GameObject_GetUV(GameObject * pThis)
{
	float leftU = pThis->uvinfo.offsetU + (float)pThis->animator.frame * pThis->uvinfo.sizeU;
	float rightU = leftU + pThis->uvinfo.sizeU;
	float topV = pThis->uvinfo.offsetV + (float)pThis->charController.direction * pThis->uvinfo.sizeV;
	float bottomV = topV + pThis->uvinfo.sizeV;

	FRECT rect;

	switch (pThis->animator.flip) { // flip�ϐ��ɓ����Ă��锽�]����ID�ŏ����𕪂���
	case FLIP_NONE: // ���]�Ȃ�
		rect = { leftU, rightU, topV, bottomV };
		break;
	case FLIP_VERTICAL: // �������]
		rect = { leftU, rightU, bottomV, topV };
		break;
	case FLIP_HORIZONTAL: // �������]
		rect = { rightU, leftU, topV, bottomV };
		break;
	}

	return rect;
}

void GameObject_Update(GameObject * pThis)
{
	// AI�̏���
	CharAI_Update(&pThis->ai);

	// �A�j���[�V�����̃R�}��i�߂�
	Animator_Update(&pThis->animator);

	// �L�����R���g���[�����g���Ă̈ړ�
	CharController_Update(&pThis->charController, &pThis->posX, &pThis->posY);

	// �A�N�V�����̍X�V
	Action_Update(&pThis->action);

	// ���������L���Ă���I�u�W�F�N�g�����邩�H
	if (pThis->pOwner != NULL) {
		// �����̉�ʍ��W�����L���Ă���I�u�W�F�N�g�̉�ʍ��W�{�����̃��[�J�����W
		pThis->posX = pThis->pOwner->posX + pThis->localPosX;
		pThis->posY = pThis->pOwner->posY + pThis->localPosY;
	}

	// �����蔻��̍��W���X�V
	Collider_Update(&pThis->collider, pThis->posX, pThis->posY);
}

