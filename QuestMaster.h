#pragma once

#define MAX_STAGE_ENEMY   5
#define MAX_QUEST_STAGE   3
#define MAX_QUEST         3

struct EnemyType
{
	int spriteID;
	int aiID;
	float hp;
};

struct StageEnemy
{
	EnemyType type;
	float posX;
};

struct Stage
{
	int numEnemy;
	StageEnemy enemies[MAX_STAGE_ENEMY];
};

struct Quest
{
	int numStage;
	Stage stages[MAX_QUEST_STAGE];
};


const Quest* QuestMaster_GetQuest(int questID);
const Stage* QuestMaster_GetStage(int questID, int stageID);
