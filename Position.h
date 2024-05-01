#pragma once

//x,y좌표를 활용하여 맵에 데이터를 연결해준다.
// 메모리 효율과 포인터를 통해 칸의 주소를 저장하는 방식을 사용한 연결리스트를 사용
// 연결리스트를 위해 구조체,노드에 대한 헤더파일을 넣어둠

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
