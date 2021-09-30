#pragma once

// テーブルアニメーションでカウンターを０に戻せ、という合図
#define ANIMATION_LOOP  -1
#define ANIMATION_END   -2

struct AnimInfo {

	const int* pTable;
	int tableSize;
	float speed;

};

enum {
	ANIM_ID_DEFAULT,
	ANIM_ID_CHAR32x32,
	ANIM_ID_WINDSHOT,
	ANIM_ID_SWORD,
};

const AnimInfo* AnimInfo_Get(int id);
