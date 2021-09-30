#pragma once

struct GameObject;

struct Collider {

	bool isActive;
	GameObject* pOwner;

	// ��`
	float centerX, centerY; // ���S�_
	float sizeX, sizeY;  // �c���̒���

};


// �����蔻��̌��ʂ��i�[���邽�߂̍\����
struct COLLIDER_RESULT {

	bool isHit;
	float dx, dy;

};

// �������֐�
void Collider_Initialize(Collider* pThis, GameObject* pOwner, float x, float y, float sizeX, float sizeY);
// ���t���[���̍X�V����
void Collider_Update(Collider* pThis, float x, float y);
// �Q�̃R���C�_�[���Փ˂��Ă��邩���茋�ʂ�Ԃ��֐�
COLLIDER_RESULT Collider_Test(Collider* pThis, Collider* pOther);

