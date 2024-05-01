#include <stdio.h>
#include <stdlib.h>
#include "RoundInfo.h"

//홀의 맵 크기설정(동적할당)
RoundInfo* RoundInfo_Create(int map_width, int map_height, int par, const char* mapData) {
	RoundInfo *roundInfo = malloc(sizeof(RoundInfo));
	if (roundInfo) {
		memset(roundInfo, 0, sizeof(RoundInfo));
		
		roundInfo->par = par;
		roundInfo->map_width = map_width;
		roundInfo->map_height = map_height;

		PositionList_Init(&roundInfo->positionList);

		roundInfo->mapData = malloc(map_width * map_height);
		memcpy(roundInfo->mapData, mapData, map_width * map_height);
	}
	else {
		// 메모리 할당 실패.
	}
	return roundInfo;
}

//맵에서 범위가 아닌 좌표값에 대한 데이터들 설정
char RoundInfo_GetMapData(RoundInfo* roundInfo, int x, int y) {
	if (x < 0 || y < 0) {
		return '.'; // 범위 밖은 OB로 처리
	}
	if (x >= roundInfo->map_width || y >= roundInfo->map_height) {
		return '.'; // 범위 밖은 OB로 처리
	}

	return roundInfo->mapData[roundInfo->map_width * (roundInfo->map_height - y - 1) + x];
}

//RoundInfo 동적할당 해제
void RoundInfo_Destroy(RoundInfo* roundInfo) {
	if (roundInfo) {
		if (roundInfo->mapData) {
			free(roundInfo->mapData);
			roundInfo->mapData = NULL;
		}
		free(roundInfo);
	}
}
//연결리스트 생성
RoundInfoList* RoundInfoList_Create() {
	RoundInfoList* roundInfoList = (RoundInfoList*)malloc(sizeof(RoundInfoList));
	if (roundInfoList != NULL) {
		roundInfoList->size = 0;
		roundInfoList->first = NULL;
		roundInfoList->last = NULL;
	}
	else {
		// 메모리 할당 실패
	}
	return roundInfoList;
}

//노드를 추가해줌 값이 없을때 다음 연결리스트의 헤드로 연결
void RoundInfoList_Add(RoundInfoList* roundInfoList, RoundInfo* roundInfo) {
	RoundInfoNode* node = (RoundInfoNode*)malloc(sizeof(RoundInfoNode));
	if (node) {
		node->info = roundInfo;
		node->next = NULL;
		if (roundInfoList->first == NULL) {
			roundInfoList->first = node;
			roundInfoList->last = node;
		}
		else {
			roundInfoList->last->next = node;
			roundInfoList->last = node;
		}
		roundInfoList->size++;
	}
	else {
		// 메모리 할당 실패
	}
}
//current 포인터를 활용해서 헤드부터 꼬리까지 탐색을 하며 값을 지정해줌
RoundInfo* RoundInfoList_Get(RoundInfoList* roundInfoList, int idx) {
	if (idx < 0) {
		return NULL;
	}
	if (idx >= roundInfoList->size) {
		return NULL;
	}
	RoundInfoNode* current = roundInfoList->first;
	int current_idx = 0;
	while (current_idx < idx) {
		current = current->next;
		current_idx++;
	}
	return current->info;
}

void RoundInfoList_Destroy(RoundInfoList* roundInfoList) {
	RoundInfoNode* current = roundInfoList->first;
	RoundInfoNode* next = NULL;
	while (current != NULL) {
		next = current->next;
		RoundInfo_Destroy(current->info);
		free(current);
		current = next;
	}
	roundInfoList->size = 0;
	free(roundInfoList);
}