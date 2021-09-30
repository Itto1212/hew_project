#include "GameObject.h"
#include "GameTimer.h"


FRECT GameObject_GetXY(GameObject * pThis)
{
	// ポインタ　→　ある変数の存在してる場所を表す数字（番地/アドレス）を入れるための箱（変数）

	// アドレスから構造体の中の変数にアクセスするにはアロー演算子（->）を使う
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

	switch (pThis->animator.flip) { // flip変数に入っている反転処理IDで処理を分ける
	case FLIP_NONE: // 反転なし
		rect = { leftU, rightU, topV, bottomV };
		break;
	case FLIP_VERTICAL: // 垂直反転
		rect = { leftU, rightU, bottomV, topV };
		break;
	case FLIP_HORIZONTAL: // 水平反転
		rect = { rightU, leftU, topV, bottomV };
		break;
	}

	return rect;
}

void GameObject_Update(GameObject * pThis)
{
	// AIの処理
	CharAI_Update(&pThis->ai);

	// アニメーションのコマを進める
	Animator_Update(&pThis->animator);

	// キャラコントローラを使っての移動
	CharController_Update(&pThis->charController, &pThis->posX, &pThis->posY);

	// アクションの更新
	Action_Update(&pThis->action);

	// 自分を所有しているオブジェクトがあるか？
	if (pThis->pOwner != NULL) {
		// 自分の画面座標＝所有しているオブジェクトの画面座標＋自分のローカル座標
		pThis->posX = pThis->pOwner->posX + pThis->localPosX;
		pThis->posY = pThis->pOwner->posY + pThis->localPosY;
	}

	// 当たり判定の座標を更新
	Collider_Update(&pThis->collider, pThis->posX, pThis->posY);
}

