#include "ObjectGenerator.h"
#include <Windows.h>

void ObjectGenerator_SetDragon(GameObject * pObj)
{
	// �|�C���^�ϐ��̍��Ɂ�������ƁA�A�h���X�������Ă���ϐ����̂��̂Ƃ��ĐU�镑��
	*pObj = {
		0, 0,  // ���S���W
		0.25f, 0.25f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������

	pObj->uvinfo = { 0, 0, 80.0f/TEXTURE_SIZE_X, 64.0f/TEXTURE_SIZE_Y };  // �e�N�X�`������n��
}

void ObjectGenerator_SetBG(GameObject * pObj)
{
	*pObj = {
		0, 0,  // ���S���W
		2, 2,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������
	pObj->animator.isActive = false;  // �A�j���[�V����OFF

	pObj->uvinfo = { 0, 0.5f, 640.0f / TEXTURE_SIZE_X, 480.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��
}

void ObjectGenerator_SetTitleLogo(GameObject * pObj)
{
	*pObj = {
		0, 0.2f,  // ���S���W
		1.0f*1.5f, 0.37f*1.33f*1.5f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������
	pObj->animator.isActive = false;  // �A�j���[�V����OFF

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 0.5f,
		640.0f / TEXTURE_SIZE_X, 237.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_GREEN_BLINK);
}

void ObjectGenerator_SetPressEnter(GameObject * pObj)
{
	*pObj = {
		0, -0.5f,  // ���S���W
		0.50f, 0.12f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������
	pObj->animator.isActive = false;  // �A�j���[�V����OFF

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1261.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 36.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	pObj->color = { 1,1,1,1 };

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_BLINK);
}

void ObjectGenerator_SetGameOver(GameObject * pObj)
{
	*pObj = {
		0, 0,  // ���S���W
		1.0f, 0.45f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������
	pObj->animator.isActive = false;  // �A�j���[�V����OFF

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1261.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 80.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	pObj->color = { 1,1,1,1 };
}

void ObjectGenerator_SetStageClear(GameObject * pObj)
{
	*pObj = {
		0, 0,  // ���S���W
		1.2f, 0.45f,  // �T�C�Y
	};

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1350.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 60.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	pObj->color = { 1,1,1,1 };
}

void ObjectGenerator_SetStageStart(GameObject * pObj)
{
	*pObj = {
		0, 0,  // ���S���W
		1.0f, 0.50f,  // �T�C�Y
	};

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1400.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 80.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	pObj->color = { 1,1,1,0 };

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_FADEIN_FADEOUT);
}

void ObjectGenerator_SetFadePanel(GameObject * pObj)
{
	*pObj = {
		0, 0,  // ���S���W
		2, 2,  // �T�C�Y
	};

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1300.0f / TEXTURE_SIZE_Y,
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��
}

void ObjectGenerator_SetHpGauge(GameObject * pGauge, GameObject * pBase)
{
	*pGauge = {
		0, 0.75f,  // ���S���W
		1.0f, 0.1f,  // �T�C�Y
	};

	pGauge->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1300.0f / TEXTURE_SIZE_Y,
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y };  // �e�N�X�`������n��

	pGauge->color = { 0,1,0,1 };

	// ���n
	pBase->posX = pGauge->posX;
	pBase->posY = pGauge->posY;
	pBase->sizeX = pGauge->sizeX + 0.02f;
	pBase->sizeY = pGauge->sizeY + 0.02f;

	pBase->uvinfo = pGauge->uvinfo;

	pBase->color = { 0.5f,0.5f,0.5f,1 };
}

// ���̊֐��𐬗������邽�߂ɂ́A�e�N�X�`�����ȉ��̏����ō쐬����
// �E32x32�L�����͉����тŔz�u����
// �E32x32�L������UV(0, 0.25f)�̈ʒu������Ƃ��ĕ��ׂ�
void ObjectGenerator_Character32x32(GameObject * pObj, int id)
{
	// �|�C���^�ϐ��̍��Ɂ�������ƁA�A�h���X�������Ă���ϐ����̂��̂Ƃ��ĐU�镑��
	*pObj = {
		0, 0,  // ���S���W
		0.15f, 0.20f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������
	Animator_SetAnimInfo(&pObj->animator, ANIM_ID_CHAR32x32);

	// �e�N�X�`������n��
	pObj->uvinfo = {
		(float)id*96.0f/ TEXTURE_SIZE_X, 0.25f,  // �I�t�Z�b�gUV
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y  // �P�R�}�T�C�YUV
	};

	// �L�����N�^�[�R���g���[��������
	CharController_Initialize(&pObj->charController);

	//�����蔻��̏�����
	// centerX, centerY, sizeX, sizeY�́A�摜�̗]���Ȃǂɂ���Ē�������K�v����
	Collider_Initialize(&pObj->collider, pObj, 0, 0, 0.11f, 0.20f);

	Action_Initialize(&pObj->action, pObj);

	pObj->ai.pOwner = pObj;
}

void ObjectGenerator_SetFx(GameObject * pObj, int id)
{
	*pObj = {
		0, 0,  // ���S���W
		0.35f, 0.35f,  // �T�C�Y
	};

	Animator_Initialize(&pObj->animator);  // �A�j���[�V����������

	// �e�N�X�`������n��
	pObj->uvinfo = {
		0.0f, 0.3125f + (float)id*64.0f / TEXTURE_SIZE_Y,  // �I�t�Z�b�gUV
		64.0f / TEXTURE_SIZE_X, 64.0f / TEXTURE_SIZE_Y  // �P�R�}�T�C�YUV
	};

	// �L�����N�^�[�R���g���[��������
	CharController* pCtrl = &pObj->charController;
	CharController_Initialize(pCtrl);
	pCtrl->direction = 0;
	pCtrl->maxMoveSpeedX *= 3.0f;  // �f�t�H���g�̂R�{�̑��x
	pCtrl->accelForceX = pCtrl->maxMoveSpeedX*0.1f;

	//�����蔻��̏�����
	Collider_Initialize(&pObj->collider, pObj, 0, 0, 0.20f, 0.20f);

	if (id == 0) { // �E�C���h�V���b�g
		Animator_SetAnimInfo(&pObj->animator, ANIM_ID_WINDSHOT);

		pObj->sizeX = 0.20f;
		pObj->sizeY = 0.20f;
	}
	else if (id == 1) { // ���G�t�F�N�g
		Animator_SetAnimInfo(&pObj->animator, ANIM_ID_SWORD);

		pObj->sizeX = 0.30f;
		pObj->sizeY = 0.30f;
		pCtrl->isActive = false;
		pObj->collider.isActive = false;
	}

	

	
	// �A�N�V����������
	Action_Initialize(&pObj->action, pObj);
}
