#pragma once

struct GameObject;

struct Action {

	bool isActive; // アクション実行中かどうか表す
	int id; // アクションのID　※下のenum定数を代入する
	float time; // アクション開始からの経過時間
	GameObject* pOwner; // このアクションを持っているGameObjectインスタンス

};

enum {
	ACTION_NONE,
	ACTION_ATTACK1,
	ACTION_MAGIC,
	ACTION_FX_SWORD,
	ACTION_FX_WINDSHOT,
	ACTION_KILLED,
	ACTION_BLINK,
	ACTION_FADEIN_FADEOUT,
	ACTION_GREEN_BLINK,
};

// メンバー関数

// アクションの初期化。親のGameObjectアドレスを渡す。
void Action_Initialize(Action* pThis, GameObject* pObj);

// IDでアクションを指定して開始する
void Action_Set(Action* pThis, int actionId);

// アクションの内容を実行するために毎フレーム呼び出される関数
void Action_Update(Action* pThis);
