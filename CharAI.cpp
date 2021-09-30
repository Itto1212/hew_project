#include "CharAI.h"
#include "GameObject.h"
#include "input.h"
#include <Windows.h>
#include <math.h>
#include "game.h"
#include "GameTimer.h"


// extern宣言
// 他のcppで宣言されているグローバル変数を、このcppで使いたい時に書く
extern GameObject* gpPlayer;
extern GameObject* gpMagic;

// プライベート関数　プロトタイプ宣言
static void PlayerUpdate(CharAI* pThis);
static void GhostUpdate(CharAI* pThis);
//static void GoblinUpdate(CharAI* pThis);
static void SlimeUpdate(CharAI* pThis);


void CharAI_Update(CharAI * pThis)
{
	switch (pThis->id) { // AIの種類IDで分岐

	case AI_ID_PLAYER: // プレイヤー
		PlayerUpdate(pThis);
		break;

	case AI_ID_GHOST: // ゴースト
		GhostUpdate(pThis);
		break;

	// 敵の種類を増やしたら↓にcaseを追加する
	case AI_ID_SLIME:
		SlimeUpdate(pThis);
		break;

	}

	pThis->time += GameTimer_GetDeltaTime();
}

// プレイヤーのAI
// ※プレイヤーはユーザー操作で動くので、キー処理を書いている
void PlayerUpdate(CharAI * pThis)
{
	GameObject* pPlayer = pThis->pOwner;
	CharController* pPlayerCtrl = &pPlayer->charController;

	if (pPlayer->action.isActive == false) {
		// プレイヤーのキー移動
		if (Input_GetKeyPress('A')) {  // 左
			CharController_Accelerate(pPlayerCtrl, ACCEL_LEFT);
			pPlayerCtrl->direction = DIR_LEFT;
		}
		if (Input_GetKeyPress('D')) {  // 右
			CharController_Accelerate(pPlayerCtrl, ACCEL_RIGHT);
			pPlayerCtrl->direction = DIR_RIGHT;
		}
		if (Input_GetKeyTrigger(VK_LBUTTON)) { // 左クリック：プレイヤー　魔法攻撃
			Game_UseWindShot(gpPlayer);
		}
		if (Input_GetKeyTrigger(VK_RBUTTON)) { // 右クリック：プレイヤー　剣攻撃
			Game_UseSwordAttack(gpPlayer);
		}
	}

	// プレイヤー移動中アニメーション
	pPlayer->animator.speed = fabsf(pPlayerCtrl->moveSpeedX / pPlayerCtrl->maxMoveSpeedX * 20.0f);
}

// ゴーストAI
void GhostUpdate(CharAI * pThis)
{
	GameObject* pGhost = pThis->pOwner;

	if (pGhost->action.isActive) { // アクション実行中ならAIは実行しない
		return;
	}

	switch (pThis->state) // AIの状態で分岐
	{
	case AI_STATE_INIT: // 初期
		pThis->basePosX = pGhost->posX; // 初期位置を基準位置として保存
		pThis->moveDirection = ACCEL_RIGHT; // 初期の向き
		pThis->state = AI_STATE_IDLE; // 「待機」に移る
		break;

	case AI_STATE_IDLE: // 待機　→　基準位置を中心に左右往復する
		if (pGhost->posX > pThis->basePosX + 0.15f) { // 基準位置＋0.15より右に行ったら方向転換
			pThis->moveDirection = ACCEL_LEFT;
			pGhost->charController.direction = DIR_LEFT;
		}
		else if (pGhost->posX < pThis->basePosX - 0.15f) { // 基準位置ー0.15より左に行ったら方向転換
			pThis->moveDirection = ACCEL_RIGHT;
			pGhost->charController.direction = DIR_RIGHT;
		}
		CharController_Accelerate(&pGhost->charController, pThis->moveDirection);

		if (pGhost->posX - gpPlayer->posX < 0.40f) { // ゴーストとプレイヤーの距離が0.40より近づいた
			pThis->state = AI_STATE_ATTACK; // 「攻撃」に移る
		}
		break;

	case AI_STATE_APPROACH: // 接近　※ゴーストでは使わない
		break;

	case AI_STATE_ATTACK: // 攻撃
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // 前回の攻撃から２秒経った
			if (pGhost->posX - gpPlayer->posX > 0.25f) { // プレイヤーとの距離が遠かったら
				Game_UseWindShot(pGhost); // ウインドショット使う
			}
			else { // プレイヤーとの距離が近かったら
				Game_UseSwordAttack(pGhost); // 直接攻撃を使う
			}

			pThis->lastAttackTime = pThis->time; // 攻撃時間を保存
		}

		if (pGhost->posX - gpPlayer->posX > 0.50f) { // プレイヤーとゴーストの距離が0.5より遠くなった
			pThis->state = AI_STATE_IDLE; // 「待機」に移る
		}
		break;

	case AI_STATE_ESCAPE: // 逃亡　※ゴーストでは使わない
		break;

	case AI_STATE_DEAD: // 戦闘不能　※HPを実装後に追加する
		break;

	}

	// 敵移動中アニメーション
	pGhost->animator.speed =
		fabsf(pGhost->charController.moveSpeedX / pGhost->charController.maxMoveSpeedX * 20.0f);
}

void SlimeUpdate(CharAI * pThis)
{
	GameObject* pSlime = pThis->pOwner;

	if (pSlime->action.isActive) { // アクション実行中ならAIは実行しない
		return;
	}

	switch (pThis->state) // AIの状態で分岐
	{
	case AI_STATE_INIT: // 初期
		pThis->basePosX = pSlime->posX; // 初期位置を基準位置として保存
		pThis->moveDirection = ACCEL_RIGHT; // 初期の向き
		pThis->state = AI_STATE_IDLE; // 「待機」に移る
		break;

	case AI_STATE_IDLE: // 待機　→　基準位置を中心に左右往復する
		// 遷移条件
		if (pSlime->posX - gpPlayer->posX < 0.5f) {
			pThis->state = AI_STATE_APPROACH;  // 接近に移る
		}
		break;

	case AI_STATE_APPROACH: // 接近　※ゴーストでは使わない
		CharController_Accelerate(&pSlime->charController, ACCEL_LEFT);
		pSlime->charController.direction = DIR_LEFT; // 左向け
		// 遷移条件
		if (pSlime->posX - gpPlayer->posX < 0.25f) {
			pThis->state = AI_STATE_ATTACK;  // 攻撃に移る
		}
		break;

	case AI_STATE_ATTACK: // 攻撃
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // 前回の攻撃から２秒経った
			Game_UseSwordAttack(pSlime); // 直接攻撃を使う

			pThis->lastAttackTime = pThis->time; // 攻撃時間を保存
		}
		break;

	case AI_STATE_ESCAPE: // 逃亡　※ゴーストでは使わない
		break;

	case AI_STATE_DEAD: // 戦闘不能　※HPを実装後に追加する
		break;

	}

	// 敵移動中アニメーション
	pSlime->animator.speed =
		fabsf(pSlime->charController.moveSpeedX / pSlime->charController.maxMoveSpeedX * 20.0f);
}

void GoblinUpdate(CharAI * pThis)
{
	GameObject* pGoblin = pThis->pOwner;

	if (pGoblin->action.isActive) { // アクション実行中ならAIは実行しない
		return;
	}

	switch (pThis->state) // AIの状態で分岐
	{
	case AI_STATE_INIT: // 初期
		pThis->basePosX = pGoblin->posX; // 初期位置を基準位置として保存
		pThis->moveDirection = ACCEL_RIGHT; // 初期の向き
		pThis->state = AI_STATE_IDLE; // 「待機」に移る
		break;

	case AI_STATE_IDLE: // 待機
		//CharController_Accelerate(&pGoblin->charController, ACCEL_LEFT);
		// 遷移条件
		if (pGoblin->posX - gpPlayer->posX < 0.5f) { // プレイヤーとの距離が0.5より近くなったら
			pThis->state = AI_STATE_APPROACH; // 「接近」に移る
		}
		break;

	case AI_STATE_APPROACH: // 接近
		// だるまさんがころんだパターン
		if (gpPlayer->charController.direction == DIR_LEFT) { // プレイヤーが左向きなら
			CharController_Accelerate(&pGoblin->charController, ACCEL_LEFT); // ゴブリン左に移動
			pGoblin->charController.direction = DIR_LEFT;
		}
		else { // プレイヤーが右向き
			if (pGoblin->posX - gpPlayer->posX < 0.5f) { // プレイヤーとの距離が0.5より近くなったら
				CharController_Accelerate(&pGoblin->charController, ACCEL_RIGHT); // ゴブリン右に移動
			}
		}
		// 遷移条件
		if (pGoblin->posX - gpPlayer->posX < 0.25f) { // プレイヤーとの距離が0.25より近くなったら
			pThis->state = AI_STATE_ATTACK; // 攻撃に移る
		}
		break;

	case AI_STATE_ATTACK: // 攻撃
		if (pThis->time > pThis->lastAttackTime + 2.0f) { // 前回の攻撃から２秒経った
			Game_UseSwordAttack(pGoblin); // 直接攻撃を使う
			pThis->lastAttackTime = pThis->time; // 攻撃時間を保存
		}
		break;

	case AI_STATE_ESCAPE: // 逃亡　※ゴーストでは使わない
		break;

	case AI_STATE_DEAD: // 戦闘不能　※HPを実装後に追加する
		break;

	}

	// 敵移動中アニメーション
	pGoblin->animator.speed =
		fabsf(pGoblin->charController.moveSpeedX / pGoblin->charController.maxMoveSpeedX * 20.0f);
}
