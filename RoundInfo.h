#pragma once
#include "Position.h"

//맵의 요소들을 표현하기 위해 만든 구조체 멤버들
//맵을 표현하기 위해 연결리스트를 사용(포인터,동적할당)
//동적할당을 통해 메모리의 효율을 높이고 연결리스트에서 노드간 연결을 포인터로 .
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
