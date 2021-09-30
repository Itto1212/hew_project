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
		// 移動処理
		*pPosX += pThis->moveSpeedX * GameTimer_GetDeltaTime();

		// 減速
		// moveSpeedXが：
		// ０のとき　→　減速必要なし
		// ＋の値の時　→　マイナスしていく
		// ーの値の時　→　プラスしていく
		if (pThis->moveSpeedX > 0.0f) {  // ＋の値の時
			pThis->moveSpeedX -= pThis->stopForceX;

			if (pThis->moveSpeedX < 0.0f) { // 減速しすぎてマイナスになった
				pThis->moveSpeedX = 0.0f;  // 停止させる
			}
		}
		else if (pThis->moveSpeedX < 0.0f) { // ーの値の時
			pThis->moveSpeedX += pThis->stopForceX;

			if (pThis->moveSpeedX > 0.0f) { // 減速しすぎてプラスになった
				pThis->moveSpeedX = 0.0f;  // 停止させる
			}
		}
		// ０の時は減速しない
	}
}

void CharController_Accelerate(CharController * pThis, float directionX)
{
	// 加速処理　directionXを使って
	pThis->moveSpeedX += pThis->accelForceX * directionX;
	// 速度が最高速度を超えないようにする
	if (pThis->moveSpeedX > pThis->maxMoveSpeedX) {
		pThis->moveSpeedX = pThis->maxMoveSpeedX;
	}
	if (pThis->moveSpeedX < -pThis->maxMoveSpeedX) {
		pThis->moveSpeedX = -pThis->maxMoveSpeedX;
	}
}
