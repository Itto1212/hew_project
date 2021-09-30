#include "QuestMaster.h"
#include "CharAI.h"


static const EnemyType gGhost = { 1, AI_ID_GHOST, 25.0f }; // �S�[�X�g
static const EnemyType gSoldier = { 2, AI_ID_GOBLIN, 40.0f }; // ���m
static const EnemyType gSkeleton = { 3, AI_ID_SLIME, 30.0f }; // �[��


static const Stage gStageSet0 = {
	1,
	gSoldier, 0.5f
};
static const Stage gStageSet1 = {
	2,
	gSoldier, 0.5f, gSoldier, 0.6f,
};
static const Stage gStageSet2 = {
	3,
	gSoldier, 0.5f, gGhost, 0.6f, gSkeleton, 0.7f
};
static const Stage gStageSet3 = {
	5,
	gSkeleton, 0.5f, gSkeleton, 0.6f, gSkeleton, 0.7f, gSkeleton, 0.8f, gSkeleton, 0.9f
};


static const Quest gQuestMaster[MAX_QUEST] =
{
	{ // �N�G�X�g0
		1, gStageSet0 // �X�e�[�W��, �X�e�[�W�f�[�^, ...
	},
	{ // �N�G�X�g1
		2, gStageSet1, gStageSet2 // �X�e�[�W��, �X�e�[�W�f�[�^, ...
	},
	{ // �N�G�X�g1
		3, gStageSet1, gStageSet2, gStageSet3 // �X�e�[�W��, �X�e�[�W�f�[�^, ...
	}
};


const Quest * QuestMaster_GetQuest(int questID)
{
	return &gQuestMaster[questID];
}

const Stage * QuestMaster_GetStage(int questID, int stageID)
{
	return &gQuestMaster[questID].stages[stageID];
}
