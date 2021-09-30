#include "collider.h"
#include "GameObject.h"
#include <math.h>


void Collider_Initialize(Collider * pThis, GameObject* pOwner, float x, float y, float sizeX, float sizeY)
{
	pThis->isActive = true;
	pThis->centerX = x;
	pThis->centerY = y;
	pThis->sizeX = sizeX;
	pThis->sizeY = sizeY;
	pThis->pOwner = pOwner;
}

void Collider_Update(Collider * pThis, float x, float y)
{
//	pThis->centerX = x;
//	pThis->centerY = y;
}

COLLIDER_RESULT Collider_Test(Collider * pThis, Collider * pOther)
{
	COLLIDER_RESULT res = { false };

	if (pThis->isActive == false || pOther->isActive == false) {
		return res;
	}

	// Collider�̐eGameObject�̍��W�𑫂��Ē��S�_���v�Z
	float this_centerX = pThis->pOwner->posX + pThis->centerX;
	float this_centerY = pThis->pOwner->posY + pThis->centerY;
	float other_centerX = pOther->pOwner->posX + pOther->centerX;
	float other_centerY = pOther->pOwner->posY + pOther->centerY;

	// ���S�_���m�̋������v�Z�i��Βl�j
	float distX = fabsf(other_centerX - this_centerX);
	float distY = fabsf(other_centerY - this_centerY);
	// �Q�̎l�p�`�̏c���̒����𑫂��ā��Q�����l���v�Z
	float halfTotalLenX = (pThis->sizeX + pOther->sizeX) / 2.0f;
	float halfTotalLenY = (pThis->sizeY + pOther->sizeY) / 2.0f;

	// ���̂Q�̏���������true�Ȃ�Փ˂��Ă���
	res.isHit = distX < halfTotalLenX && distY < halfTotalLenY;

	// �����߂��̋������v�Z����
	float dx1 = (other_centerX - pOther->sizeX / 2) - (this_centerX + pThis->sizeX / 2.0f);
	float dx2 = (other_centerX + pOther->sizeX / 2) - (this_centerX - pThis->sizeX / 2.0f);
	float dy1 = (this_centerY + pThis->sizeY / 2) - (other_centerY - pOther->sizeY / 2.0f);
	float dy2 = (other_centerY + pOther->sizeY / 2) - (this_centerY - pThis->sizeY / 2.0f);

	float dx = fabsf(dx1) < fabsf(dx2) ? dx1 : dx2;
	float dy = fabsf(dy1) < fabsf(dy2) ? dy1 : dy2;

	if (fabsf(dx) < fabsf(dy)) {
		res.dx = dx;
		res.dy = 0.0f;
	}
	else {
		res.dx = 0.0f;
		res.dy = dy;
	}

	return res;
}
