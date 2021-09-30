#pragma once

// AIクラス
struct CharAI {

	int id; // AIのID　→　敵の種類ごとにIDがある
	int state; // AIの状態　→　待機、攻撃、逃亡など
	struct GameObject* pOwner; // このAIの所有者

	float basePosX; // 所有者の基準位置
	float moveDirection; // 所有者の進行方向
	float time; // 時間管理用
	float lastAttackTime; // 前回攻撃時間

};

// AIのIDリスト
enum {
	AI_ID_NONE,
	AI_ID_PLAYER,
	AI_ID_GHOST,

	// ↓　敵の種類を増やしたらここにIDを追加する
	AI_ID_GOBLIN,
	AI_ID_SLIME, // スライム

};

// AIの状態リスト
enum {
	AI_STATE_INIT, // 初期
	AI_STATE_IDLE, // 待機
	AI_STATE_APPROACH, // 接近
	AI_STATE_ATTACK, // 攻撃
	AI_STATE_ESCAPE, // 逃亡
	AI_STATE_DEAD // 戦闘不能
};

// AIの更新（AI実行）
void CharAI_Update(CharAI* pThis);
