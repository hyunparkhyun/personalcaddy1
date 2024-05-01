#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "ShowRound.h"


#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "imm32.lib")

static int isRunning = 0;

#define RECT_WIDTH 15
#define RECT_HEIGHT 15

#define MARGIN 10

//선을 그려주는데 보조를 해주는 함수
void DrawLine(SDL_Renderer* renderer, RoundInfo* roundInfo, Position* p1, Position* p2, unsigned char r, unsigned char g, unsigned char b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);

	int x1 = MARGIN + p1->x * RECT_WIDTH + RECT_WIDTH / 2;
	int y1 = MARGIN + (roundInfo->map_height - p1->y - 1) * RECT_HEIGHT + RECT_HEIGHT / 2;
	int x2 = MARGIN + p2->x * RECT_WIDTH + RECT_WIDTH / 2;
	int y2 = MARGIN + (roundInfo->map_height - p2->y - 1) * RECT_HEIGHT + RECT_HEIGHT / 2;
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

//맵의 큰틀을 잡아줌 (맵의 크기,지형의 색 지정 ...) 
void ShowRoundInfo(int round, RoundInfo* roundInfo) {
	int show_round = 1;
	int window_width = roundInfo->map_width * RECT_WIDTH + MARGIN * 2;
	int window_height = roundInfo->map_height * RECT_HEIGHT + MARGIN * 2;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	char round_str[255];
	sprintf_s(round_str, 255, "ROUND %d", round);

	window = SDL_CreateWindow(round_str, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Position* start = Position_Create(0, 0);

	for (int x = 0; x < roundInfo->map_width; ++x) {
		for (int y = 0; y < roundInfo->map_height; ++y) {
			char mapData = RoundInfo_GetMapData(roundInfo, x, y);
			if (mapData == 'T') {
				start->x = x;
				start->y = y;
			}
		}
	}

	SDL_Event ev;

	while (show_round && isRunning) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_KEYDOWN:
				show_round = 0;
				break;
			}
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			SDL_Rect rect;
			for (int x_pos = 0; x_pos < roundInfo->map_width; ++x_pos) {
				for (int y_pos = 0; y_pos < roundInfo->map_height; ++y_pos) {
					rect.x = MARGIN + x_pos * RECT_WIDTH;
					rect.y = MARGIN + y_pos * RECT_HEIGHT;
					rect.w = RECT_WIDTH;
					rect.h = RECT_HEIGHT;

					char mapData = RoundInfo_GetMapData(roundInfo, x_pos, roundInfo->map_height - y_pos - 1);
					switch (mapData) {
					case 'F':
						SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // FAIRWAY의 색상.	진한 녹색
						break;
					case 'G':
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);	//밝은 녹색
						break;
					case 'B':
						SDL_SetRenderDrawColor(renderer, 218, 165, 32, 255);	//금색
						break;
					case 'R':
						SDL_SetRenderDrawColor(renderer, 85, 107, 47, 255);		//올리브 그린색
						break;
					case 'O':
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);		//빨간색
						break;
					case 'P':	//시작점 
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);	//핀,화이트
						break;
					case 'T':	//끝점
						SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);	//보라색
						break;
					case '.':
						SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);	//밝은 회색
						break;
					case 'H':
						SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);		//바다,파랑색
						break;
					default:
						SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // FAIRWAY의 색상.	진한 녹색

						roundInfo->fairway++;
						break;
					}				
					SDL_RenderFillRect(renderer, &rect);
				}
			}
			
			if (roundInfo->positionList.size > 0) {
				DrawLine(renderer, roundInfo, start, PositionList_Get(&(roundInfo->positionList), 0), 0x00, 0x00, 0x00);
				for (int idx = 0; idx < roundInfo->positionList.size - 1; ++idx) {
					DrawLine(renderer, roundInfo, PositionList_Get(&(roundInfo->positionList), idx), PositionList_Get(&(roundInfo->positionList), idx + 1), 0x00, 0x00, 0x00);
				}
			}

			SDL_RenderPresent(renderer);
		}
	}

	Position_Destroy(start);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

//콘솔쪽 입력받았을때 쓰레드를 활용
//SDL 라이브러리를 사용하여 다중 라운드 정보를 GUI에 표시하기 위한 함수.
static DWORD WINAPI gui_thread(LPVOID lpParameter) {			
	RoundInfoList* roundInfoList = (RoundInfoList*)lpParameter;

	SDL_Init(SDL_INIT_EVERYTHING);

	for (int idx = 0; idx < roundInfoList->size; ++idx) {
		RoundInfo* roundInfo = RoundInfoList_Get(roundInfoList, idx);
		ShowRoundInfo(idx + 1, roundInfo);
	}

	SDL_Quit();

	return 0;
}

//메인 쓰레ㅔ드의 실행을 차단하지 않고 GUI작업을 수행함.
void start_sdl_thread(RoundInfoList* roundInfoList) {		
	HANDLE handle = CreateThread(NULL, 0, gui_thread, roundInfoList, 0, NULL);
	if (handle != 0) {
		CloseHandle(handle);
	}
}


void start_show_round(RoundInfoList* roundInfoList) {
	isRunning = 1;

	start_sdl_thread(roundInfoList);
}

void end_show_round() {
	isRunning = 0;
}
