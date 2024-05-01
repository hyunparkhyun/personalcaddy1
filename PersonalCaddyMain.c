#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include "RoundInfo.h"
#include "ShowRound.h"

////////////////////////////////////////////////////////////////////////////////////////////////
/*
// 1. 골프장의 MAP 데이터. (20x20, 10x10)
//
// 2. 골프장의 MAP 데이터에는 각 위치에 따라 FAIRWAY, GREEN, ROUGH, OB(벌타), HAZARD(벌타),  BUNKER, PIN, TEE, 정보가 있음.
//
// 3. 스코어 카드에 기록할 데이터는
//  - 각 라운드별
//      - 전체 Shot수
//      - Shot결과 위치가 fairway, green, rough, ob, hazard, bunker에 들어간 횟수
//      - 그린 위에 올라갔을 때의 Putts 수.
// 4. 사용자는 매 라운드마다 TEE 위치에서 시작하여 PIN에 도달할 때까지 Shot 결과 위치를 입력한다. 
//		매 샷마다 결과 위치에서 PIN까지의 결과를 알려준다.
// 5. 18라운드까지의 결과가 끝나면 스코어 카드 내용을 요약해서 보여준다.
*/
////////////////////////////////////////////////////////////////////////////////////////////////
// 동적할당과 포인터, 연결리스트 활용(난이도)


// 'F' : Fairway
// 'G' : Green
// 'B' : bunker
// 'R' : ROUGH
// 'H' : HAZARD
// 'P' : 홀 위치
// 'T' : 시작 위치
// 'O' : OB
// '.' : Fairway

//메뉴창
void show_menu() {
	system("cls");
	printf("********************************************\n");
	printf("기능을 선택하세요\n");
	printf("1. GPS\n");
	printf("2. 스코어카드 보기\n");
	printf("3. 끝내기\n");
	printf("********************************************\n");

}

//콘솔창에 샷의 위치를 x,y좌표를 통해 입력받는다
Position *input_shot_result(int shot_try) {
	int x = 0;
	int y = 0;
	printf("[+%d] 볼의 x 위치는?\n", shot_try);
	rewind(stdin);
	scanf_s("%d", &x);
	printf("[+%d] 볼의 y 위치는?\n", shot_try);
	rewind(stdin);
	scanf_s("%d", &y);
	return Position_Create(x, y);
}


//볼의 위치와 핀까지의 거리를 계산해주는 함수
double distance(Position* a, Position* b) {
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}


//맵의 데이터 요소들을 증가시켜주는 함수
void plus_round_map_data(RoundInfo* roundInfo, char mapData) {
	// 'F' : Fairway
	// 'G' : Green
	// 'B' : bunker
	// 'R' : ROUGH
	// 'H' : HAZARD
	// 'P' : 홀 위치
	// 'T' : 시작 위치
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
	case 'P':	//시작점 
		//roundInfo->putts;
		//break;
	case 'T':	//끝점
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

//메뉴창에서 1번 gps기능을 골랐을때 나오는 출력값들,최대 샷은 par의2배로 설정 position 연결리스트를 활용해서 값을 지정받고 맵에 표현,지형에 표현,타격횟수까지 연결.
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
	printf(" %d 번홀\n", round);
	printf("\n");
	printf("핀 위치 : %d, %d\n", hole->x, hole->y);
	printf("\n");
	printf("볼의 위치 : %d, %d\n", start->x, start->y);
	printf("\n");
	printf("핀까지 남은 거리 : %f\n", distance(hole, start));
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
		printf(" %d 번홀\n", round);
		printf("\n");
		printf("핀 위치 : %d, %d\n", hole->x, hole->y);
		printf("\n");
		printf("볼의 위치 : %d, %d\n", shot_position->x, shot_position->y);
		printf("\n");
		printf("핀까지 남은  거리 : %f\n", distance(hole, shot_position));
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

//1번 gps기능에서 연결리스트를 통해 얻은 값들을 새로운 연결리스트에 넣어주고 그 값들을 스코어카드로 지정해서 출력.
void show_scorecard(RoundInfoList *roundInfoList) {	
	int roundNumber = 1;
	RoundInfoNode* currentNode = roundInfoList->first;
	RoundInfo* info = currentNode->info;
	while (currentNode != NULL) {
		RoundInfo* info = currentNode->info;
		printf(" %d 번 홀:\n", roundNumber);
		printf("  Fairway 안착 횟수: %d\n", info->fairway);
		//printf("  Greens in regulation: %d\n", info->green);
		printf("  Bunkers 안착 횟수: %d\n", info->bunker);
		printf("  Roughs 안착 횟수: %d\n", info->rough);
		printf("  Hazards 안착 횟수: %d\n", info->hazard);
		printf("  퍼트수: %d\n", info->putts);
		printf("  OB 안착 횟수: %d\n", info->ob);
		printf("  타수(벌타 포함): %d\n", info->fairway + info->bunker + info->rough + info->hazard * 2 + info->putts + info->ob * 3);
		printf("\n");

		currentNode = currentNode->next;
		roundNumber++; // 다음 라운드 번호로 업데이트
	}
}

// switch문을 통해 어떤 값이 들어가느냐에 따라 사용 기능을 달리함 (메뉴창)
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
		char exitInput = getchar(); // 사용자 입력 받기
		if (exitInput == 'q') {
			end_show_round();
			return -1; // 'q'나 'Q' 입력 시 종료
		}
		return 0;
	case 3:
		return -1;
	}
	return 0;
}

const char round_01_map[] =			//20by20		18개의 맵을 만들어야함
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
	
const char round_02_map[] =			//20by20		18개의 맵을 만들어야함
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
const char round_03_map[] =			//20by20		18개의 맵을 만들어야함
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


const char round_04_map[] =			//20by20		18개의 맵을 만들어야함
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



//맵을 지정해줌
RoundInfoList* load_roundinfo_list() {
	RoundInfoList* roundInfoList = RoundInfoList_Create();	//맵의 width height top mapdata

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
	RoundInfoList* roundInfoList = load_roundinfo_list();		//라운드인포리스트에서 크리에이트를 함수

	while (select_menu(roundInfoList) >= 0); //등호는 gps로 입력받고 다시 메인메뉴로 돌아오게함

	RoundInfoList_Destroy(roundInfoList); //  Destory 함수 호출 동적할당 해제

	return 0;
}
