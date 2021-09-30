#include "animator.h"
#include "GameTimer.h"

void Animator_Initialize(Animator * pThis)
{
	pThis->frame = 0;
	pThis->time = 0.0f;
	pThis->isActive = true;  // �f�t�H���g�̓A�j���[�V����ON

	// �e�[�u���A�j���[�V�����̃f�[�^���擾���Đݒ�
	Animator_SetAnimInfo(pThis, ANIM_ID_DEFAULT);
}

void Animator_Update(Animator * pThis)
{
	if (pThis->isActive) {  // �A�j���[�V������ON�̎��������������s
		const int* table = pThis->pInfo->pTable;

		// �f���^�^�C�����擾���ĉ��Z
		pThis->time += GameTimer_GetDeltaTime() * pThis->speed;

		// �e�[�u�����Q�Ƃ���̂Ɏg���Y�����ϐ�
		// �Q�[�����[�v��~���ɔz��̈�I�[�o�[��h�����߁����g��
		int animationCounter = (int)pThis->time % pThis->pInfo->tableSize;

		// ���[�v�̍Ō�ɒB������
		if (table[animationCounter] == ANIMATION_LOOP) {
			animationCounter = 0;
			pThis->time = 0;
		}
		else if (table[animationCounter] == ANIMATION_END) {
			animationCounter = 0;
			pThis->time = 0;
			pThis->isActive = false;
		}

		pThis->frame = table[animationCounter];  // �e�[�u�����猻�݂̃R�}�ԍ����
	}
}

void Animator_SetAnimInfo(Animator * pThis, int id)
{
	pThis->pInfo = AnimInfo_Get(id);
	pThis->speed = pThis->pInfo->speed;
}
