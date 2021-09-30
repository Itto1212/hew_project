#include "CharController.h"
#include "GameTimer.h"
#include <math.h>

void CharController_Initialize(CharController * pThis)
{
	pThis->isActive = true;
	pThis->direction = DIR_RIGHT;
	pThis->maxMoveSpeedX = 0.6f;
	pThis->accelForceX = pThis->maxMoveSpeedX * 0.1f;
	pThis->stopForceX = pThis->accelForceX * 0.6f;
}

void CharController_Update(CharController * pThis, float* pPosX, float* pPosY)
{
	if (pThis->isActive) {
		// �ړ�����
		*pPosX += pThis->moveSpeedX * GameTimer_GetDeltaTime();

		// ����
		// moveSpeedX���F
		// �O�̂Ƃ��@���@�����K�v�Ȃ�
		// �{�̒l�̎��@���@�}�C�i�X���Ă���
		// �[�̒l�̎��@���@�v���X���Ă���
		if (pThis->moveSpeedX > 0.0f) {  // �{�̒l�̎�
			pThis->moveSpeedX -= pThis->stopForceX;

			if (pThis->moveSpeedX < 0.0f) { // �����������ă}�C�i�X�ɂȂ���
				pThis->moveSpeedX = 0.0f;  // ��~������
			}
		}
		else if (pThis->moveSpeedX < 0.0f) { // �[�̒l�̎�
			pThis->moveSpeedX += pThis->stopForceX;

			if (pThis->moveSpeedX > 0.0f) { // �����������ăv���X�ɂȂ���
				pThis->moveSpeedX = 0.0f;  // ��~������
			}
		}
		// �O�̎��͌������Ȃ�
	}
}

void CharController_Accelerate(CharController * pThis, float directionX)
{
	// ���������@directionX���g����
	pThis->moveSpeedX += pThis->accelForceX * directionX;
	// ���x���ō����x�𒴂��Ȃ��悤�ɂ���
	if (pThis->moveSpeedX > pThis->maxMoveSpeedX) {
		pThis->moveSpeedX = pThis->maxMoveSpeedX;
	}
	if (pThis->moveSpeedX < -pThis->maxMoveSpeedX) {
		pThis->moveSpeedX = -pThis->maxMoveSpeedX;
	}
}
