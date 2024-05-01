#include <stdio.h>
#include <stdlib.h>
#include "Position.h"

// x,y값 동적할당
Position* Position_Create(int x, int y) {
	Position* position = malloc(sizeof(Position));
	if (position) {
		position->x = x;
		position->y = y;
	}
	else {
		// 메모리 할당 실패.
	}
	return position;
}

//Position 동적할당 해제 함수
void Position_Destroy(Position* position) {
	if (position != NULL) {
		free(position);
	}
}
//연결리스트 생성 헤드,꼬리값 지정
void PositionList_Init(PositionList* positionList) {
	positionList->size = 0;
	positionList->first = NULL;
	positionList->last = NULL;
}

//노드를 추가해줌 값이 없을때 다음 연결리스트의 헤드로 연결
void PositionList_Add(PositionList* positionList, Position* position) {
	PositionNode* node = (PositionNode *)malloc(sizeof(PositionNode));
	node->position = position;
	node->next = NULL;
	if (node != NULL) {
		if (positionList->first == NULL) {
			positionList->first = node;
			positionList->last = node;
		}
		else {
			positionList->last->next = node;
			positionList->last = node;
		}
		positionList->size++;
	}
}

//current 포인터를 활용해서 헤드부터 꼬리까지 탐색을 하며 값을 지정해줌
Position* PositionList_Get(PositionList* positionList, int idx) {
	if (idx < 0) {
		return NULL;
	}
	if (idx >= positionList->size) {
		return NULL;
	}
	PositionNode* current = positionList->first;
	int current_idx = 0;
	while (current_idx < idx) {
		current = current->next;
		current_idx++;
	}
	return current->position;
}


//PositionList 동적할당 해제
void PositionList_Destroy(PositionList* positionList) {
	PositionNode* current = positionList->first;
	PositionNode* next = NULL;
	while (current != NULL) {
		next = current->next;
		Position_Destroy(current->position);
		free(current);
		current = next;
	}
}