#pragma once
#include "Position.h"

//���� ��ҵ��� ǥ���ϱ� ���� ���� ����ü �����
//���� ǥ���ϱ� ���� ���Ḯ��Ʈ�� ���(������,�����Ҵ�)
//�����Ҵ��� ���� �޸��� ȿ���� ���̰� ���Ḯ��Ʈ���� ��尣 ������ �����ͷ� .
typedef struct {
	int map_width;
	int map_height;
	char* mapData;
	int par;
	int fairway;
	int green;
	int rough;
	int ob;
	int hazard;
	int bunker;
	int putts;
	PositionList positionList;
} RoundInfo;

typedef struct _RoundInfoNode {
	RoundInfo *info;
	struct _RoundInfoNode* next;
} RoundInfoNode;

typedef struct {
	int size;
	RoundInfoNode* first;
	RoundInfoNode* last;
} RoundInfoList;

RoundInfo* RoundInfo_Create(int map_width, int map_height, int par, const char* mapData);
char RoundInfo_GetMapData(RoundInfo* roundInfo, int x, int y);
void RoundInfo_Destroy(RoundInfo* roundInfo);

RoundInfoList* RoundInfoList_Create();
void RoundInfoList_Add(RoundInfoList* roundInfoList, RoundInfo* roundInfo);
RoundInfo *RoundInfoList_Get(RoundInfoList* roundInfoList, int idx);
void RoundInfoList_Destroy(RoundInfoList* roundInfoList);
