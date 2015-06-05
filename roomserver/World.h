#ifndef _HALL_H
#define _HALL_H

#include <map>

#include "type.h"
#include "Protocol.h"
#include "Room.h"
#include "Zone.h"

class World : public Zone
{
public:
	World(ServiceConnection* game);
	~World();
public:
	void initialize();
	void addGateWay(ServiceConnection* conn);
private:
	void onGatewayMessage(ServiceConnection* conn, char* head, char* body);
	void onNewPlayer(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
	void onDestroyPlayer(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
	void onReqEnterRoom(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
	void onReqLeaveRoom(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
	void onReqJoinGame(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
	void onReqLeaveGame(ServiceConnection* conn, MSG_HEAD_BACK* head, char* body);
private:
	err_t tryEnterRoom(Room* room, Player* player);
	void enterRoomSuccess(ServiceConnection* conn, Player* player, err_t reason);
	void enterRoomFailed(ServiceConnection* conn, Player* player, err_t reason);
	err_t tryLeaveRoom(Room* room, Player* player);
	void leaveRoomSuccess(ServiceConnection* conn, Player* player, err_t reason);
	void leaveRoomFailed(ServiceConnection* conn, Player* player, err_t reason);
	void enterGameSuccess(ServiceConnection* conn, Player* player, err_t reason);
	void enterGameFailed(ServiceConnection* conn, Player* player, err_t reason);
	void leaveGameSuccess(ServiceConnection* conn, Player* player, err_t reason);
	void leaveGameFailed(ServiceConnection* conn, Player* player, err_t reason);
private:
	inline Room* findRoom(rid_t rid);
private:
	ServiceConnection* pGame;
	std::map<rid_t, Room*> mRooms;
};

typedef std::map<rid_t, Room*> map_room;
#endif