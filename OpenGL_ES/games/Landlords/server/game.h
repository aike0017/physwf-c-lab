#ifndef _GAME_H
#define _GAME_H

#include "rules.h"


typedef struct player_t {
	int id;
	Card cards[20];
	int num_cards;
	int singles[20];
	int num_singles;
	int pairs[20];
	int num_pairs;
	int triples[20];
	int num_triples;
	int bombs[20];
	int num_bombs;
	int turn;
	bool isLord;
	bool isAI;
} Player,*PPlayer;

typedef struct deal_result_t {
	int playerIds[3];
	Card cards[20];
	Card loot[3];
} DealResult, *PDealResult;

typedef struct game_t {
	Card pool[NUM_CARDS];
	Player players[NUM_PLAYERS];
	int loot_turn;
	int loot_scores[NUM_PLAYERS];
	int turn;
	DealResult dresult;
	int landlord;//players index
} Game, *PGame;

void init();
void start();

#endif