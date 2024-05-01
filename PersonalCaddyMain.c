#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include "RoundInfo.h"
#include "ShowRound.h"

////////////////////////////////////////////////////////////////////////////////////////////////
/*
// 1. �������� MAP ������. (20x20, 10x10)
//
// 2. �������� MAP �����Ϳ��� �� ��ġ�� ���� FAIRWAY, GREEN, ROUGH, OB(��Ÿ), HAZARD(��Ÿ),  BUNKER, PIN, TEE, ������ ����.
//
// 3. ���ھ� ī�忡 ����� �����ʹ�
//  - �� ���庰
//      - ��ü Shot��
//      - Shot��� ��ġ�� fairway, green, rough, ob, hazard, bunker�� �� Ƚ��
//      - �׸� ���� �ö��� ���� Putts ��.
// 4. ����ڴ� �� ���帶�� TEE ��ġ���� �����Ͽ� PIN�� ������ ������ Shot ��� ��ġ�� �Է��Ѵ�. 
//		�� ������ ��� ��ġ���� PIN������ ����� �˷��ش�.
// 5. 18��������� ����� ������ ���ھ� ī�� ������ ����ؼ� �����ش�.
*/
////////////////////////////////////////////////////////////////////////////////////////////////
// �����Ҵ�� ������, ���Ḯ��Ʈ Ȱ��(���̵�)


// 'F' : Fairway
// 'G' : Green
// 'B' : bunker
// 'R' : ROUGH
// 'H' : HAZARD
// 'P' : Ȧ ��ġ
// 'T' : ���� ��ġ
// 'O' : OB
// '.' : Fairway

//�޴�â
void show_menu() {
	system("cls");
	printf("********************************************\n");
	printf("����� �����ϼ���\n");
	printf("1. GPS\n");
	printf("2. ���ھ�ī�� ����\n");
	printf("3. ������\n");
	printf("********************************************\n");

}

//�ܼ�â�� ���� ��ġ�� x,y��ǥ�� ���� �Է¹޴´�
Position *input_shot_result(int shot_try) {
	int x = 0;
	int y = 0;
	printf("[+%d] ���� x ��ġ��?\n", shot_try);
	rewind(stdin);
	scanf_s("%d", &x);
	printf("[+%d] ���� y ��ġ��?\n", shot_try);
	rewind(stdin);
	scanf_s("%d", &y);
	return Position_Create(x, y);
}


//���� ��ġ�� �ɱ����� �Ÿ��� ������ִ� �Լ�
double distance(Position* a, Position* b) {
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}


//���� ������ ��ҵ��� ���������ִ� �Լ�
void plus_round_map_data(RoundInfo* roundInfo, char mapData) {
	// 'F' : Fairway
	// 'G' : Green
	// 'B' : bunker
	// 'R' : ROUGH
	// 'H' : HAZARD
	// 'P' : Ȧ ��ġ
	// 'T' : ���� ��ġ
	// 'O' : OB
	// '.' : Fairway

	switch (mapData) {
	case 'F':
		roundInfo->fairway++;
		break;
	case 'O':
		roundInfo->ob++;
		break;
	case 'G':
		roundInfo->green++;
		roundInfo->putts++;
		break;
	case 'B':
		roundInfo->bunker++;
		break;
	case 'R':
		roundInfo->rough++;
		break;
	case 'H':
		roundInfo->hazard++;
		break;
	case 'P':	//������ 
		//roundInfo->putts;
		//break;
	case 'T':	//����
		//roundInfo->putts;
		//break;
	case '.':
		roundInfo->fairway++;
		break;
		
	default:
		roundInfo->ob++;
		break;
	}
}

//�޴�â���� 1�� gps����� ������� ������ ��°���,�ִ� ���� par��2��� ���� position ���Ḯ��Ʈ�� Ȱ���ؼ� ���� �����ް� �ʿ� ǥ��,������ ǥ��,Ÿ��Ƚ������ ����.
void gps_input_round(int round, RoundInfo* roundInfo) {
	int par = roundInfo->par;
	int max_shot = par * 2;

	Position* start = Position_Create(0, 0);
	Position* hole = Position_Create(0, 0);

	for (int x = 0; x < roundInfo->map_width; ++x) {
		for (int y = 0; y < roundInfo->map_height; ++y) {
			char mapData = RoundInfo_GetMapData(roundInfo, x, y);
			if (mapData == 'T') {
				start->x = x;
				start->y = y;
			}
			else if (mapData == 'P') {
				hole->x = x;
				hole->y = y;
			}
		}
	}

	system("cls");
	printf("********************************************\n");
	printf(" %d ��Ȧ\n", round);
	printf("\n");
	printf("�� ��ġ : %d, %d\n", hole->x, hole->y);
	printf("\n");
	printf("���� ��ġ : %d, %d\n", start->x, start->y);
	printf("\n");
	printf("�ɱ��� ���� �Ÿ� : %f\n", distance(hole, start));
	printf("********************************************\n");

	for (int shot_try = 0; shot_try < max_shot; ++shot_try) {
		Position* shot_position = input_shot_result(shot_try);
		PositionList_Add(&(roundInfo->positionList), shot_position);
		char mapData = RoundInfo_GetMapData(roundInfo, shot_position->x, shot_position->y);

		plus_round_map_data(roundInfo, mapData);

		if (mapData == 'P') {
			printf("MATCH!!\n");
			break;
		}
		system("cls");
		printf("********************************************\n");
		printf(" %d ��Ȧ\n", round);
		printf("\n");
		printf("�� ��ġ : %d, %d\n", hole->x, hole->y);
		printf("\n");
		printf("���� ��ġ : %d, %d\n", shot_position->x, shot_position->y);
		printf("\n");
		printf("�ɱ��� ����  �Ÿ� : %f\n", distance(hole, shot_position));
		printf("********************************************\n");

	}

	Position_Destroy(start);
	Position_Destroy(hole);

}

void gps_input(RoundInfoList *roundInfoList) {
	for (int idx = 0; idx < roundInfoList->size; ++idx) {
		RoundInfo *roundInfo = RoundInfoList_Get(roundInfoList, idx);
		gps_input_round(idx + 1, roundInfo);
	}	
}

//1�� gps��ɿ��� ���Ḯ��Ʈ�� ���� ���� ������ ���ο� ���Ḯ��Ʈ�� �־��ְ� �� ������ ���ھ�ī��� �����ؼ� ���.
void show_scorecard(RoundInfoList *roundInfoList) {	
	int roundNumber = 1;
	RoundInfoNode* currentNode = roundInfoList->first;
	RoundInfo* info = currentNode->info;
	while (currentNode != NULL) {
		RoundInfo* info = currentNode->info;
		printf(" %d �� Ȧ:\n", roundNumber);
		printf("  Fairway ���� Ƚ��: %d\n", info->fairway);
		//printf("  Greens in regulation: %d\n", info->green);
		printf("  Bunkers ���� Ƚ��: %d\n", info->bunker);
		printf("  Roughs ���� Ƚ��: %d\n", info->rough);
		printf("  Hazards ���� Ƚ��: %d\n", info->hazard);
		printf("  ��Ʈ��: %d\n", info->putts);
		printf("  OB ���� Ƚ��: %d\n", info->ob);
		printf("  Ÿ��(��Ÿ ����): %d\n", info->fairway + info->bunker + info->rough + info->hazard * 2 + info->putts + info->ob * 3);
		printf("\n");

		currentNode = currentNode->next;
		roundNumber++; // ���� ���� ��ȣ�� ������Ʈ
	}
}

// switch���� ���� � ���� �����Ŀ� ���� ��� ����� �޸��� (�޴�â)
int select_menu(RoundInfoList* roundInfoList) {
	show_menu();
	int selected = 0;
	printf(">");
	rewind(stdin);
	scanf_s("%d", &selected);
	switch (selected) {
	case 1:
		gps_input(roundInfoList);
		return 0;
		break;
	case 2:
		show_scorecard(roundInfoList);
		start_show_round(roundInfoList);
		rewind(stdin);
		printf("\n");
		printf("Press 'q' to quit or any other key to return to menu.\n");
		printf(">");
		char exitInput = getchar(); // ����� �Է� �ޱ�
		if (exitInput == 'q') {
			end_show_round();
			return -1; // 'q'�� 'Q' �Է� �� ����
		}
		return 0;
	case 3:
		return -1;
	}
	return 0;
}

const char round_01_map[] =			//20by20		18���� ���� ��������
	".....GGGGGPGGGG....."
	".....GGGGGGGGGG....."
	"...................."
	"...B..........BB...."
	"...BB..........B...."
	"...................."
	"...................."
	"HHH..............OOO"
	"HHH..............OOO"	
	"HHHRR..........RROOO"
	"HHHRR..........RROOO"
	"HHHRR........BBBROOO"
	"HHHRR........BBBROOO"
	"HHHRR........BBBROOO"
	"HHHRR..........RROOO"
	"HHHRR..........RROOO"
	"HHHRR..........RROOO"
	"HHHRR..........RROOO"
	"HHHRR..........RROOO"
	"THHHRR.........RROOO"
	;
	
const char round_02_map[] =			//20by20		18���� ���� ��������
"FFFFFFFFFFFFFFFFFFFF"
"FFFFFFFFFFFFFFFFFFFF"
"...................."
"...................."
"........GGGGG......."
"........GGPGG......."
"........GGGGG......."
"...........BBB......"
"...........BBB......"
"....HHHHHHH........."
"....HHHHHHH........."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"......FFFF.....RRR.."
"..T................."
"...................."
;
const char round_03_map[] =			//20by20		18���� ���� ��������
"...................."
"..GGGGGGGGGGG......."
"..GGPGGGGGGGG......."
"..GGGGGGGGGGG......."
"...GGGGGGGGG........"
"...................."
".O............H....."
".O............H....."
".O............H....."
".O............H....."
".O............H....."
"RRRRRRB............."
"RRRRRRB............."
"RRRRRRB............."
"RRRRRRB............."
"RRRRRRB............."
"...................."
"...................."
".T.................."
"...................";


const char round_04_map[] =			//20by20		18���� ���� ��������
"...................."
"....GGGGG..........."
"....GGPGG..........."
"....GGGGG..........."
"...................."
"...................."
"...................."
"...................."
".....B.......HH....."
"....B.........HH...."
"...B...........HH..."
"..B...........HHH..."
".B.............HH..."
"...................."
"...................."
"...................."
"...................."
"...................."
"...................."
".........T.........."
;
const char round_05_map[] =
"HHHHHHHHHHHHHHHHHHHH"
"H..................H"
"H..................H"
"H..................H"
"H..................H"
"H..................H"
"H..BBBBBBBBBB......H"
"H..BBBBBBBBBB......H"
"H..BBBPBBBBBB......H"
"H..BBBBBBBBBB......H"
"H..BBBBBBBBBB......H"
"H..................H"
"H..................H"
"H..................H"
"H..................H"
"H..................H"
"H................T.H"
"H..................H"
"H..................H"
"....................";

const char round_06_map[] =
"OOOOOOOOOOOOOOOOOOOO"
"O..................O"
"O.....GGGGGGGGGGG..O"
"O.....GGGGGGGGGPG..O"
"O.....GGGGGGGGGGG..O"
"O......GGGGG.......O"
"O..................O"
"OOOOO......HHH.....O"
"OOOOO......HHH.....O"
"OOOOO......HHH.....O"
"O..................O"
"O..................O"
"O..........BBBB....O"
"O..........BBBB....O"
"O..........BBBB....O"
"O..........BBBB....O"
"O..................O"
"O..................O"
"O.....T............O"
"....................";



//���� ��������
RoundInfoList* load_roundinfo_list() {
	RoundInfoList* roundInfoList = RoundInfoList_Create();	//���� width height top mapdata

	RoundInfo* round_01 = RoundInfo_Create(20, 20, 4, round_01_map);
	RoundInfo* round_02 = RoundInfo_Create(20, 20, 4, round_02_map);
	RoundInfo* round_03 = RoundInfo_Create(20, 20, 4, round_03_map);
	RoundInfo* round_04 = RoundInfo_Create(20, 20, 4, round_04_map);
	RoundInfo* round_05 = RoundInfo_Create(20, 20, 4, round_05_map);
	RoundInfo* round_06 = RoundInfo_Create(20, 20, 4, round_06_map);

	RoundInfoList_Add(roundInfoList, round_01);
	RoundInfoList_Add(roundInfoList, round_02);
	RoundInfoList_Add(roundInfoList, round_03);
	RoundInfoList_Add(roundInfoList, round_04);
	RoundInfoList_Add(roundInfoList, round_05);
	RoundInfoList_Add(roundInfoList, round_06);


	return roundInfoList;
}


int main(int argc, const char* argv[]) {
	RoundInfoList* roundInfoList = load_roundinfo_list();		//������������Ʈ���� ũ������Ʈ�� �Լ�

	while (select_menu(roundInfoList) >= 0); //��ȣ�� gps�� �Է¹ް� �ٽ� ���θ޴��� ���ƿ�����

	RoundInfoList_Destroy(roundInfoList); //  Destory �Լ� ȣ�� �����Ҵ� ����

	return 0;
}
