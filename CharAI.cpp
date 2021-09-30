#include "CharAI.h"
#include "GameObject.h"
#include "input.h"
#include <Windows.h>
#include <math.h>
#include "game.h"
#include "GameTimer.h"


// extern�錾
// ����cpp�Ő錾����Ă���O���[�o���ϐ����A����cpp�Ŏg���������ɏ���
extern GameObject* gpPlayer;
extern GameObject* gpMagic;

// �v���C�x�[�g�֐��@�v���g�^�C�v�錾
static void PlayerUpdate(CharAI* pThis);
static void GhostUpdate(CharAI* pThis);
//static void GoblinUpdate(CharAI* pThis);
static void SlimeUpdate(CharAI* pThis);


void CharAI_Update(CharAI * pThis)
{
	switch (pThis->id) { // AI�̎��ID�ŕ���

	case AI_ID_PLAYER: // �v���C���[
		PlayerUpdate(pThis);
		break;

	case AI_ID_GHOST: // �S�[�X�g
		GhostUpdate(pThis);
		break;

	// �G�̎�ނ𑝂₵���火��case��ǉ�����
	case AI_ID_SLIME:
		SlimeUpdate(pThis);
		break;

	}

	pThis->time += GameTimer_GetDeltaTime();
}

// �v���C���[��AI
// ���v���C���[�̓��[�U�[����œ����̂ŁA�L�[�����������Ă���
void PlayerUpdate(CharAI * pThis)
{
	GameObject* pPlayer = pThis->pOwner;
	CharController* pPlayerCtrl = &pPlayer->charController;

	if (pPlayer->action.isActive == false) {
		// �v���C���[�̃L�[�ړ�
		if (Input_GetKeyPress('A')) {  // ��
			CharController_Accelerate(pPlayerCtrl, ACCEL_LEFT);
			pPlayerCtrl->direction = DIR_LEFT;
		}
		if (Input_GetKeyPress('D')) {  // �E
			CharController_Accelerate(pPlayerCtrl, ACCEL_RIGHT);
			pPlayerCtrl->direction = DIR_RIGHT;
		}
		if (Input_GetKeyTrigger(VK_LBUTTON)) { // ���N���b�N�F�v���C���[�@���@�U��
			Game_UseWindShot(gpPlayer);
		}
		if (Input_GetKeyTrigger(VK_RBUTTON)) { // �E�N���b�N�F�v���C���[�@���U��
			Game_UseSwordAttack(gpPlayer);
		}
	}

	// �v���C���[�ړ����A�j���[�V����
	pPlayer->animator.speed = fabsf(pPlayerCtrl->moveSpeedX / pPlayerCtrl->maxMoveSpeedX * 20.0f);
}

// �S�[�X�gAI
void GhostUpdate(CharAI * pThis)
{
	GameObject* pGhost = pThis->pOwner;

	if (pGhost->action.isActive) { // �A�N�V�������s���Ȃ�AI�͎��s���Ȃ�
		return;
	}

	switch (pThis->state) // AI�̏�Ԃŕ���
	{
	case AI_STATE_INIT: // ����
		pThis->basePosX = pGhost->posX; // �����ʒu����ʒu�Ƃ��ĕۑ�
		pThis->moveDirection = ACCEL_RIGHT; // �����̌���
		pThis->state = AI_STATE_IDLE; // �u�ҋ@�v�Ɉڂ�
		break;

	case AI_STATE_IDLE: // �ҋ@�@���@��ʒu�𒆐S�ɍ��E��������
		if (pGhost->posX > pThis->basePosX + 0.15f) { // ��ʒu�{0.15���E�ɍs����������]��
			pThis->moveDirection = ACCEL_LEFT;
			pGhost->charController.direction = DIR_LEFT;
		}
		else if (pGhost->posX < pThis->basePosX - 0.15f) { // ��ʒu�[0.15��荶�ɍs����������]��
			pThis->moveDirection = ACCEL_RIGHT;
			pGhost->charController.direction = DIR_RIGHT;
		}
		CharController_Accelerate(&pGhost->charController, pThis->moveDirection);

		if (pGhost->posX - gpPlayer->posX < 0.40f) { // �S�[�X�g�ƃv���C���[�̋�����0.40���߂Â���
			pThis->state = AI_STATE_ATTACK; // �u�U���v�Ɉڂ�
		}
		break;

	case AI_STATE_APPROACH: // �ڋ߁@���S�[�X�g�ł͎g��Ȃ�
		break;

	case AI_STATE_ATTACK: // �U��
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // �O��̍U������Q�b�o����
			if (pGhost->posX - gpPlayer->posX > 0.25f) { // �v���C���[�Ƃ̋���������������
				Game_UseWindShot(pGhost); // �E�C���h�V���b�g�g��
			}
			else { // �v���C���[�Ƃ̋������߂�������
				Game_UseSwordAttack(pGhost); // ���ڍU�����g��
			}

			pThis->lastAttackTime = pThis->time; // �U�����Ԃ�ۑ�
		}

		if (pGhost->posX - gpPlayer->posX > 0.50f) { // �v���C���[�ƃS�[�X�g�̋�����0.5��艓���Ȃ���
			pThis->state = AI_STATE_IDLE; // �u�ҋ@�v�Ɉڂ�
		}
		break;

	case AI_STATE_ESCAPE: // ���S�@���S�[�X�g�ł͎g��Ȃ�
		break;

	case AI_STATE_DEAD: // �퓬�s�\�@��HP��������ɒǉ�����
		break;

	}

	// �G�ړ����A�j���[�V����
	pGhost->animator.speed =
		fabsf(pGhost->charController.moveSpeedX / pGhost->charController.maxMoveSpeedX * 20.0f);
}

void SlimeUpdate(CharAI * pThis)
{
	GameObject* pSlime = pThis->pOwner;

	if (pSlime->action.isActive) { // �A�N�V�������s���Ȃ�AI�͎��s���Ȃ�
		return;
	}

	switch (pThis->state) // AI�̏�Ԃŕ���
	{
	case AI_STATE_INIT: // ����
		pThis->basePosX = pSlime->posX; // �����ʒu����ʒu�Ƃ��ĕۑ�
		pThis->moveDirection = ACCEL_RIGHT; // �����̌���
		pThis->state = AI_STATE_IDLE; // �u�ҋ@�v�Ɉڂ�
		break;

	case AI_STATE_IDLE: // �ҋ@�@���@��ʒu�𒆐S�ɍ��E��������
		// �J�ڏ���
		if (pSlime->posX - gpPlayer->posX < 0.5f) {
			pThis->state = AI_STATE_APPROACH;  // �ڋ߂Ɉڂ�
		}
		break;

	case AI_STATE_APPROACH: // �ڋ߁@���S�[�X�g�ł͎g��Ȃ�
		CharController_Accelerate(&pSlime->charController, ACCEL_LEFT);
		pSlime->charController.direction = DIR_LEFT; // ������
		// �J�ڏ���
		if (pSlime->posX - gpPlayer->posX < 0.25f) {
			pThis->state = AI_STATE_ATTACK;  // �U���Ɉڂ�
		}
		break;

	case AI_STATE_ATTACK: // �U��
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // �O��̍U������Q�b�o����
			Game_UseSwordAttack(pSlime); // ���ڍU�����g��

			pThis->lastAttackTime = pThis->time; // �U�����Ԃ�ۑ�
		}
		break;

	case AI_STATE_ESCAPE: // ���S�@���S�[�X�g�ł͎g��Ȃ�
		break;

	case AI_STATE_DEAD: // �퓬�s�\�@��HP��������ɒǉ�����
		break;

	}

	// �G�ړ����A�j���[�V����
	pSlime->animator.speed =
		fabsf(pSlime->charController.moveSpeedX / pSlime->charController.maxMoveSpeedX * 20.0f);
}

void GoblinUpdate(CharAI * pThis)
{
	GameObject* pGoblin = pThis->pOwner;

	if (pGoblin->action.isActive) { // �A�N�V�������s���Ȃ�AI�͎��s���Ȃ�
		return;
	}

	switch (pThis->state) // AI�̏�Ԃŕ���
	{
	case AI_STATE_INIT: // ����
		pThis->basePosX = pGoblin->posX; // �����ʒu����ʒu�Ƃ��ĕۑ�
		pThis->moveDirection = ACCEL_RIGHT; // �����̌���
		pThis->state = AI_STATE_IDLE; // �u�ҋ@�v�Ɉڂ�
		break;

	case AI_STATE_IDLE: // �ҋ@
		//CharController_Accelerate(&pGoblin->charController, ACCEL_LEFT);
		// �J�ڏ���
		if (pGoblin->posX - gpPlayer->posX < 0.5f) { // �v���C���[�Ƃ̋�����0.5���߂��Ȃ�����
			pThis->state = AI_STATE_APPROACH; // �u�ڋ߁v�Ɉڂ�
		}
		break;

	case AI_STATE_APPROACH: // �ڋ�
		// ����܂��񂪂���񂾃p�^�[��
		if (gpPlayer->charController.direction == DIR_LEFT) { // �v���C���[���������Ȃ�
			CharController_Accelerate(&pGoblin->charController, ACCEL_LEFT); // �S�u�������Ɉړ�
			pGoblin->charController.direction = DIR_LEFT;
		}
		else { // �v���C���[���E����
			if (pGoblin->posX - gpPlayer->posX < 0.5f) { // �v���C���[�Ƃ̋�����0.5���߂��Ȃ�����
				CharController_Accelerate(&pGoblin->charController, ACCEL_RIGHT); // �S�u�����E�Ɉړ�
			}
		}
		// �J�ڏ���
		if (pGoblin->posX - gpPlayer->posX < 0.25f) { // �v���C���[�Ƃ̋�����0.25���߂��Ȃ�����
			pThis->state = AI_STATE_ATTACK; // �U���Ɉڂ�
		}
		break;

	case AI_STATE_ATTACK: // �U��
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // �O��̍U������Q�b�o����
			Game_UseSwordAttack(pGoblin); // ���ڍU�����g��
			pThis->lastAttackTime = pThis->time; // �U�����Ԃ�ۑ�
		}
		break;

	case AI_STATE_ESCAPE: // ���S�@���S�[�X�g�ł͎g��Ȃ�
		break;

	case AI_STATE_DEAD: // �퓬�s�\�@��HP��������ɒǉ�����
		break;

	}

	// �G�ړ����A�j���[�V����
	pGoblin->animator.speed =
		fabsf(pGoblin->charController.moveSpeedX / pGoblin->charController.maxMoveSpeedX * 20.0f);
}
