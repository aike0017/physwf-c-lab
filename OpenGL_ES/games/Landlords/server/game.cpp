#include <stdio.h>
#include <string.h>

#include <random.h>
#include <log/Log.h>

#include "../shared/ai.h"

#include "game.h"
#include "services.h"
#include "listener.h"
#include "LLLog.h"

void addListeners();

Game game;

void init()
{
	Log::info("init game!");
	for(int i=(int)SPADE;i<=(int)DIAMOND;i++)
	{
		for(int j=0;j<13;j++)
		{
			game.pool[i*13+j].rank = j+1;
			game.pool[i*13+j].suit = (Suit)i;
		}
	}
	game.pool[52].rank = JOKER_BLACK_RANK;
	game.pool[53].rank = JOKER_RED_RANK;
	LL_log_pool();
	init_service();
}

void check_players()
{
	for(int i=0;i<NUM_PLAYERS;i++)
	{
		if(i>game.num_players-1)
		{
			game.players[i].isAI = true;
		}
	}
}

void shuffle(int times)
{
	for(int i=0;i<times;i++)
	{
		int index1 = rand_by_range(0,NUM_CARDS-1);
		int index2 = rand_by_range(0,NUM_CARDS-1);
		Card temp = game.pool[index1];
		game.pool[index1].rank = game.pool[index2].rank;
		game.pool[index1].suit = game.pool[index2].suit;
		game.pool[index2].rank = temp.rank;
		game.pool[index2].suit = temp.suit;
	}
	LL_log_pool();
}

void deal_cards()
{
	int cards_left = NUM_CARDS;
	while(cards_left>3)
	{
		for(int j=0;j<NUM_PLAYERS;j++)
		{
			PPlayer player = &game.players[j];
			player->cards[player->num_cards++] = game.pool[cards_left-1];
			cards_left--;
		}
	}
	LL_log_player_cards();
	
	PDealResult results = game.dresults;
	for(int j=0;j<NUM_PLAYERS;j++)
	{
		PPlayer player = &game.players[j];
		for(int i=0;i<player->num_cards;i++)
		{
			results[j].cards[i] = player->cards[i];
		}
	}
	const int odd_num = 3;
	for(int j=0;j<odd_num;j++)
	{
		results[0].odd[j] = game.pool[cards_left-1];
		results[1].odd[j] = game.pool[cards_left-1];
		results[2].odd[j] = game.pool[cards_left-1];
		cards_left --;
	}
	LL_log_deal_result();
	//send deal result
	Log::info("send deal result");
	for(int j=0;j<NUM_PLAYERS;j++)
	{
		send_deal_result(j,&results[j]);
	}
}

void wait_for_loot()
{
	Log::info("wait_for_loot");
	int loot_turn = game.loot_turn;
	if(loot_turn >= NUM_PLAYERS)
	{
		//send msg play start
		send_wait_cards();
		return;
	}
	
	if(game.players[loot_turn].isAI)
	{
		game.loot_scores[loot_turn] = calculate_loot_score(&game.players[loot_turn]);
		broadcast_loot_score(loot_turn, game.loot_scores[loot_turn]);
		if(game.loot_scores[loot_turn] >= 3)
		{
			//send msg play start
			send_wait_cards();
		}
		else
		{
			game.loot_turn++;
			wait_for_loot();
		}
	}
	else
	{
		//send msg wait for loot
		Log::info("send_wait_for_loot(%d)",loot_turn);
		send_wait_for_loot(loot_turn);
		//add listener
	}
}

void start()
{
	// memset(players,0,sizeof(players));
	Log::info("start");
	check_players();
	shuffle(10);
	deal_cards();
	
	wait_for_loot();
	addListeners();
}

void turn()
{
	int next = game.landlord + (game.turn % NUM_PLAYERS);
	
	game.turn++;
}

void run()
{
	while(true)
	{
		listener_loop();
		Sleep(10);
	}
}

void onPlayerJoin(int total,int pid, PConnection connection)
{
	game.players[pid].connection = connection;
	game.players[pid].isAI = false;
	game.num_players++;
	
	for(int i=0;i<NUM_PLAYERS;i++)
	{
		if(i != pid) send_player_joined(i,pid);
	}
	
	if(total == 1)
	{
		game.players[pid].isHost = true;
		//send wait for players
		send_ask_wait_more(pid);
	}
	else if(total < NUM_PLAYERS)
	{
		//send host is waiting for players
		send_host_waiting_players(pid);
	}
	else if(total == NUM_PLAYERS)
	{
		start();
	}
}

void onPlayerLeft(int total,int pid)
{
	// game.player[pid].isHost = false;
	// for()
	// {
	// }
}

void onHostStartGame(char* data)
{
	start();
}

void onClinetLoot(char* data)
{
	Log::info("onClinetLoot");
	MSG_SUBM_LOOT_SCORE msg;
	memcpy(&msg,data,sizeof(MSG_SUBM_LOOT_SCORE));
	int loot_turn = game.loot_turn;
	game.loot_scores[loot_turn] = msg.score;
	Log::info("loot score:%d",msg.score);
	broadcast_loot_score(loot_turn, msg.score);
	game.loot_turn++;
	wait_for_loot();
}

void onPlayCards(char* data)
{
	//to do verify cards
	MSG_SUBM_PLAY_CARDS msg;
	memcpy(&msg, data, sizeof(MSG_SUBM_PLAY_CARDS));
	game.cur_hand = msg.hand;
	//broadcast cards
	
	broadcast_play_cards();
	game.turn++;
	send_wait_cards();
}

void addListeners()
{
	addMsgListener(MSG_SUBM_START_GAME_1003,onHostStartGame);
	addMsgListener(MSG_SUBM_LOOT_SCORE_1006,onClinetLoot);
	addMsgListener(MSG_SUBM_PLAY_CARDS_1009,onPlayCards);
}


