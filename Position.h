#pragma once

//x,y��ǥ�� Ȱ���Ͽ� �ʿ� �����͸� �������ش�.
// �޸� ȿ���� �����͸� ���� ĭ�� �ּҸ� �����ϴ� ����� ����� ���Ḯ��Ʈ�� ���
// ���Ḯ��Ʈ�� ���� ����ü,��忡 ���� ��������� �־��

typedef struct {
	int x;
	int y;
} Position;

typedef struct _PositionNode {
	Position* position;
	struct _PositionNode* next;
} PositionNode;

typedef struct {
	int size;
	PositionNode* first;
	PositionNode* last;
} PositionList;

Position* Position_Create(int x, int y);
void Position_Destroy(Position* position);

void PositionList_Init(PositionList* positionList);
void PositionList_Add(PositionList* positionList, Position* position);
Position* PositionList_Get(PositionList* positionList, int idx);
void PositionList_Destroy(PositionList* positionList);
