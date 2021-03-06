#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <log/Log.h>

#include "game.h"
#include "net.h"
#include "listener.h"
#include "../shared/msg.h"
#include "input.h"


Game game;
HANDLE hNetThread;

LLSceneController* scene;

void onAskWaitMore(char* data)
{
	printf("Wait for Player(s):\n");
	printf("You can wait for more players, or start the game immediately!\n");
	game.phase = GAME_PHASE_WAIT_FOR_PLAYERS;
	scene->showWaitForMorePlayers();
}

void onPlayerJoin(char* data)
{
	printf("Player joined!\n");
	scene->showPlayerJoin();
}

void onHostWaitMorePlayers(char* data)
{
	printf("Host are waiting for more players!\n");
}

void onDealResult(char* data)
{
	// conn();
	MSG_NOTI_DEAL_RESULT msg;
	memcpy(&msg,data,sizeof(MSG_NOTI_DEAL_RESULT));
	Log::info("onDealResult");
	printf("cards:\n");
	for(int i=0;i<17;i++)
	{
		printf("%d,",msg.result.cards[i].rank);
	}
	printf("\n");
	printf("loot:\n");
	for(int i=0;i<3;i++)
	{
		printf("%d,",msg.result.odd[i].rank);
	}
	printf("\n");
	scene->showDealResult();
}

void onWaitLoot(char* data)
{
	printf("Please input a loot score:\n");
	printf("0 for not loot,\n");
	printf("1-3 for loot.\n");
	game.phase = GAME_PHASE_WAIT_FOR_LOOT;
}

void onLootScore(char* data)
{
	MSG_NOTI_LOOT_SCORE msg;
	memcpy(&msg,data,sizeof(MSG_NOTI_LOOT_SCORE));
	printf("Player %d loot,score:%d\n",msg.who,msg.score);
	scene->showLootScore();
}

void onWaitCards(char* data)
{
	game.phase = GAME_PHASE_WAIT_FOR_CARDS;
	printf("Wait for your cards:\n");
}

void onPlayCards(char* data)
{
	game.phase = GAME_PHASE_WAIT_FOR_CARDS;
	MSG_NOTI_PLAY_CARDS msg;
	memcpy(&msg,data,sizeof(MSG_NOTI_PLAY_CARDS));
	printf("Player %d play cards:\n");
	scene->showPlayCards();
}

DWORD WINAPI net_thread(LPVOID pParam)
{
	while (true)
	{
		net_read();
		listener_loop();
		Sleep(0);
	}
	return 0;
}

void init(LLSceneController* s)
{
	char fname[30];
	sprintf(fname,"LLclient%8d.log",(int)time(NULL));
	
	Log::init(fname);
	net_init();
	init_input();
	
	scene = s;
	addMsgListener(MSG_NOTI_ASK_WAIT_MORE_1000,onAskWaitMore);
	addMsgListener(MSG_NOTI_PLAYER_JOIN_1001,onPlayerJoin);
	addMsgListener(MSG_NOTI_HOST_WAIT_PLAYERS_1002,onHostWaitMorePlayers);
	addMsgListener(MSG_NOTI_DEAL_RESULT_1004,onDealResult);
	addMsgListener(MSG_NOTI_WAIT_LOOT_1005,onWaitLoot);
	addMsgListener(MSG_NOTI_LOOT_SCORE_1007,onLootScore);
	addMsgListener(MSG_NOTI_WAIT_CARDS_1008,onWaitCards);
	addMsgListener(MSG_NOTI_PLAY_CARDS_1010,onPlayCards);

	hNetThread = CreateThread(NULL, 0, net_thread, NULL, 0, NULL);
}

