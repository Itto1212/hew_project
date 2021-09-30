#include "action.h"
#include "GameObject.h"
#include "GameTimer.h"


void Attack1_Update(Action* pThis);
void Magic_Update(Action* pThis);
void FxSword_Update(Action* pThis);
void FxWindshot_Update(Action* pThis);
void Killed_Update(Action* pThis);
void Blink_Update(Action* pThis);
void Finfout_Update(Action* pThis);
void Greenblink_Update(Action* pThis);


void Action_Initialize(Action * pThis, GameObject* pObj)
{
	pThis->pOwner = pObj;
	Action_Set(pThis, ACTION_NONE);
}

void Action_Set(Action * pThis, int actionId)
{
	pThis->id = actionId;
	pThis->isActive = true;
	pThis->time = 0.0f;
}

void Action_Update(Action * pThis)
{
	if (pThis->isActive) {
		switch (pThis->id) {
		case ACTION_NONE:
			pThis->isActive = false;
			break;

		case ACTION_ATTACK1:
			Attack1_Update(pThis);
			break;

		case ACTION_MAGIC:
			Magic_Update(pThis);
			break;

		case ACTION_FX_SWORD:
			FxSword_Update(pThis);
			break;

		case ACTION_FX_WINDSHOT:
			FxWindshot_Update(pThis);
			break;

		case ACTION_KILLED:
			Killed_Update(pThis);
			break;

		case ACTION_BLINK:
			Blink_Update(pThis);
			break;

		case ACTION_FADEIN_FADEOUT:
			Finfout_Update(pThis);
			break;

		case ACTION_GREEN_BLINK:
			Greenblink_Update(pThis);
			break;
		}

		pThis->time += GameTimer_GetDeltaTime();
	}
}

void Attack1_Update(Action * pThis)
{
	float time = pThis->time;
	float direction = pThis->pOwner->charController.direction == DIR_RIGHT ? 1.0f : -1.0f; // ������ۑ�

	if (time >= 0.0f && time < 0.5f) {
		pThis->pOwner->posX += 0.005f * direction; // �O�i
		pThis->pOwner->animator.frame = 0; // �A�j���[�V�������O�R�}�ڂŌŒ�
	}
	if (time >= 0.5f) {
		pThis->isActive = false;
	}
}

void Magic_Update(Action * pThis)
{
	float time = pThis->time;

	float direction = pThis->pOwner->charController.direction == DIR_RIGHT ? -1.0f : 1.0f; // ������ۑ�

	if (time >= 0.0f && time < 0.2f) {
		// ������ƌ��ɉ�����
		pThis->pOwner->posX += 0.005f*direction;
		pThis->pOwner->animator.frame = 2;
		pThis->pOwner->charController.moveSpeedX = 0.0f;
	}
	if (time >= 0.2f && time < 0.5f) {
		// �r�����̍d��
		pThis->pOwner->animator.frame = 1;
	}
	if (time >= 0.5f && time < 1.0f) {
		// ���@���������̍d��
		pThis->pOwner->animator.frame = 0;
	}
	if (time >= 1.0f) {
		// �A�N�V�����I��
		pThis->isActive = false;
	}
}

void FxSword_Update(Action * pThis)
{
	float time = pThis->time;
	GameObject* pOwner = pThis->pOwner;

	if (time == 0.0f) {
		// �A�j���[�V�����J�n
		pOwner->animator.time = 0;
		pOwner->animator.isActive = true;
	}
	if (time >= 0.3f) {
		// �����蔻��ON
		pOwner->collider.isActive = true;
	}
	if (time >= 0.5f) {
		// �A�N�V�����I��
		pThis->isActive = false;
		// �����蔻��OFF
		pOwner->collider.isActive = false;
	}
}

void FxWindshot_Update(Action * pThis)
{
	float time = pThis->time;
	CharController* pCtrl = &pThis->pOwner->charController;

	float direction = pCtrl->moveSpeedX < 0.0f ? -1.0f : 1.0f;  // ��s������ۑ�

	if (time == 0.0f) {
		pThis->pOwner->sizeX = 0.01f; // �������T�C�Y����J�n
		pThis->pOwner->sizeY = 0.01f;
	}
	if (time < 0.5f) {
		pCtrl->moveSpeedX = pCtrl->stopForceX * 1.3f * direction; // �ړ����x�����ňێ�
		pThis->pOwner->sizeX *= 1.1f; // �T�C�Y��傫�����Ă���
		pThis->pOwner->sizeY *= 1.1f;
	}
	else if (time < 1.5f) {
		CharController_Accelerate(pCtrl, direction); // �������ԓ��͔��ł���
	}
	else {
		// �������ԏI���Ŏ��R����
		pThis->isActive = false;
		pThis->pOwner->posX = 3.0f;
	}
}

void Killed_Update(Action * pThis)
{
	float time = pThis->time;
	auto& color = pThis->pOwner->color;

	if (time < 0.5f) {
		color = { 0,0,0, 1.0f-time*2.0f };
	}
	else {
		pThis->isActive = false;
	}
}

void Blink_Update(Action * pThis)
{
	float time = pThis->time;
	auto& color = pThis->pOwner->color;

	float alphaParam = time - (int)time;

	if (alphaParam < 0.5f) {
		color.a = alphaParam * 2.0f;
	}
	else {
		color.a = 1.0f - (alphaParam - 0.5f) * 2.0f;
	}
}

void Finfout_Update(Action * pThis)
{
	float time = pThis->time;
	auto& color = pThis->pOwner->color;

	if (time < 1.0f) {
		color = { 0.5f,0.5f,0.5f,0 };
	}
	else if (time < 1.5f) {
		color.a = (time-1.0f) * 2.0f;
	}
	else if (time < 3.0f) {
		if (color.r < 1.0f) {
			color.r += 0.01f;
			color.g += 0.01f;
			color.b += 0.01f;
			color.a = 1.0f;
		}
	}
	else if (time < 3.5f) {
		color.a = 1.0f - (time - 3.0f) * 2.0f;
	}
	else {
		color.a = 0.0f;
		pThis->isActive = false;
	}
}

void Greenblink_Update(Action * pThis)
{
	float time = pThis->time;
	auto& color = pThis->pOwner->color;

	int param = (int)(time*10.0f)%24;

	if (param < 12) {
		color.r -= 0.01f;
		color.b -= 0.005f;
	}
	else {
		color.r += 0.01f;
		color.b += 0.005f;
	}
}
