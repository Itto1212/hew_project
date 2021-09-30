#pragma once

#include "GameObject.h"

// ObjectGeneratorクラス

// メンバー関数プロトタイプ宣言

// ドラゴンとしてGameObjectを初期化する関数
void ObjectGenerator_SetDragon(GameObject* pObj);

// 背景としてGameObjectを初期化する関数
void ObjectGenerator_SetBG(GameObject* pObj);

void ObjectGenerator_SetTitleLogo(GameObject* pObj);

void ObjectGenerator_SetPressEnter(GameObject* pObj);

void ObjectGenerator_SetGameOver(GameObject* pObj);

void ObjectGenerator_SetStageClear(GameObject* pObj);

void ObjectGenerator_SetStageStart(GameObject* pObj);

void ObjectGenerator_SetFadePanel(GameObject* pObj);

// HPゲージの初期化
void ObjectGenerator_SetHpGauge(GameObject* pGauge, GameObject* pBase);

// 32x32キャラとしてGameObjectを初期化する関数
void ObjectGenerator_Character32x32(GameObject* pObj, int id);

// エフェクトとしてGameObjectを初期化する関数
void ObjectGenerator_SetFx(GameObject* pObj, int id);
