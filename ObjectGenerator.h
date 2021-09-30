#pragma once

#include "GameObject.h"

// ObjectGenerator�N���X

// �����o�[�֐��v���g�^�C�v�錾

// �h���S���Ƃ���GameObject������������֐�
void ObjectGenerator_SetDragon(GameObject* pObj);

// �w�i�Ƃ���GameObject������������֐�
void ObjectGenerator_SetBG(GameObject* pObj);

void ObjectGenerator_SetTitleLogo(GameObject* pObj);

void ObjectGenerator_SetPressEnter(GameObject* pObj);

void ObjectGenerator_SetGameOver(GameObject* pObj);

void ObjectGenerator_SetStageClear(GameObject* pObj);

void ObjectGenerator_SetStageStart(GameObject* pObj);

void ObjectGenerator_SetFadePanel(GameObject* pObj);

// HP�Q�[�W�̏�����
void ObjectGenerator_SetHpGauge(GameObject* pGauge, GameObject* pBase);

// 32x32�L�����Ƃ���GameObject������������֐�
void ObjectGenerator_Character32x32(GameObject* pObj, int id);

// �G�t�F�N�g�Ƃ���GameObject������������֐�
void ObjectGenerator_SetFx(GameObject* pObj, int id);
