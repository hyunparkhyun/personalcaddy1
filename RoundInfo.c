#include <stdio.h>
#include <stdlib.h>
#include "RoundInfo.h"

//Ȧ�� �� ũ�⼳��(�����Ҵ�)
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
		// �޸� �Ҵ� ����.
	}
	return roundInfo;
}

//�ʿ��� ������ �ƴ� ��ǥ���� ���� �����͵� ����
char RoundInfo_GetMapData(RoundInfo* roundInfo, int x, int y) {
	if (x < 0 || y < 0) {
		return '.'; // ���� ���� OB�� ó��
	}
	if (x >= roundInfo->map_width || y >= roundInfo->map_height) {
		return '.'; // ���� ���� OB�� ó��
	}

	return roundInfo->mapData[roundInfo->map_width * (roundInfo->map_height - y - 1) + x];
}

//RoundInfo �����Ҵ� ����
void RoundInfo_Destroy(RoundInfo* roundInfo) {
	if (roundInfo) {
		if (roundInfo->mapData) {
			free(roundInfo->mapData);
			roundInfo->mapData = NULL;
		}
		free(roundInfo);
	}
}
//���Ḯ��Ʈ ����
RoundInfoList* RoundInfoList_Create() {
	RoundInfoList* roundInfoList = (RoundInfoList*)malloc(sizeof(RoundInfoList));
	if (roundInfoList != NULL) {
		roundInfoList->size = 0;
		roundInfoList->first = NULL;
		roundInfoList->last = NULL;
	}
	else {
		// �޸� �Ҵ� ����
	}
	return roundInfoList;
}

//��带 �߰����� ���� ������ ���� ���Ḯ��Ʈ�� ���� ����
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
		// �޸� �Ҵ� ����
	}
}
//current �����͸� Ȱ���ؼ� ������ �������� Ž���� �ϸ� ���� ��������
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