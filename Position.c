#include <stdio.h>
#include <stdlib.h>
#include "Position.h"

// x,y�� �����Ҵ�
Position* Position_Create(int x, int y) {
	Position* position = malloc(sizeof(Position));
	if (position) {
		position->x = x;
		position->y = y;
	}
	else {
		// �޸� �Ҵ� ����.
	}
	return position;
}

//Position �����Ҵ� ���� �Լ�
void Position_Destroy(Position* position) {
	if (position != NULL) {
		free(position);
	}
}
//���Ḯ��Ʈ ���� ���,������ ����
void PositionList_Init(PositionList* positionList) {
	positionList->size = 0;
	positionList->first = NULL;
	positionList->last = NULL;
}

//��带 �߰����� ���� ������ ���� ���Ḯ��Ʈ�� ���� ����
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

//current �����͸� Ȱ���ؼ� ������ �������� Ž���� �ϸ� ���� ��������
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


//PositionList �����Ҵ� ����
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