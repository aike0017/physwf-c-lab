#ifndef _SCENE_H
#define _SCENE_H

#include "Actor.h"
#include "Prop.h"
#include "PVEView.h"
#include "dwtype.h"
#include "EventDispatcher.h"

#include <map>

class World : public DWObject
{
public:
	World();
	~World();

	void initialize();

	void enterPVEView(ID mapid, Unit** heros, int numHeros);
	void leavePVEView();

	void enterPVPMap(ID mapid);
	void leavePVPMap();

	Actor* getActor(ID iid);
	Actor* getActor(const CCPoint &pos);
	void removeActor(ID iid);

	PVEView* pve();
private:
	void onEnterPVEBattle(Event* event);
	void onBattleMoveResult(Event* event);
	void onBattleAttakResult(Event* event);
	void onBattleStepClear(Event* event);
	void onUnitFlop(Event* event);
	void onGoldChange(Event* event);
	void onPVEStep(Event* event);
	void onBattleSuccess(Event* event);
private:
	std::map<ID, Actor*> *mActors;
	std::map<ID, Prop*> *mProps;

	PVEView* mPVEView;
};
#endif