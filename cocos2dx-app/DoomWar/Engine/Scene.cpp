#include "Scene.h"
#include <string.h>
#include "DWLoadingScene.h"
#include "PVEBattleScene.h"
#include "System.h"

Scene::Scene() :mLoading(NULL), mPVEScene(NULL)
{
	
}

Scene::~Scene()
{

}

void Scene::initialize()
{
	System::pve->initialize();
}

void Scene::enterPVEMap(ID mapid, Unit** heros, int numHeros)
{
	System::pve->addEventListener(PVEBattle::BATTLE_ENTER_MAP, this, EventListener(&Scene::onEnterPVEBattle));
	System::pve->enter(mapid, heros, numHeros);
}

void Scene::onEnterPVEBattle(Event* event)
{
	//to do add enemys and barriers
	mPVEScene = PVEBattleScene::create();
	//to do load map

	mActors = new std::map<ID, Actor*>();
	std::map<ID,Unit*>* heros = System::pve->heros();
	for (std::map<ID,Unit*>::iterator it=heros->begin(); it != heros->end(); it++)
	{
		Actor* actor = new Actor(mPVEScene->layerActor());
		actor->setData(it->second,true);
		(*mActors)[actor->iid()] = actor;
	}

	std::map<ID, Unit*>* barriers = System::pve->barriers();
	for (std::map<ID, Unit*>::iterator it = barriers->begin(); it != barriers->end(); it++)
	{
		Actor* actor = new Actor(mPVEScene->layerActor());
		actor->setData(it->second);
		actor->hideBlood();
		(*mActors)[actor->iid()] = actor;
	}

	std::map<ID, Unit*>* enemys = System::pve->enemys();
	for (std::map<ID, Unit*>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		Actor* actor = new Actor(mPVEScene->layerActor());
		actor->setData(it->second);
		(*mActors)[actor->iid()] = actor;
	}

	mProps = new std::map<ID, Prop*>();

	CCDirector::sharedDirector()->replaceScene(mPVEScene->scene());

	System::pve->start();

	System::pve->addEventListener(PVEBattle::BATTLE_MOVE_SUCCESS, this, EventListener(&Scene::onBattleMoveResult));
	System::pve->addEventListener(PVEBattle::BATTLE_MOVE_HERO_SUCESS, this, EventListener(&Scene::onBattleMoveResult));
	System::pve->addEventListener(PVEBattle::BATTLE_MOVE_FAILED, this, EventListener(&Scene::onBattleMoveResult));
	System::pve->addEventListener(PVEBattle::BATTLE_ATTACK_RESULT, this, EventListener(&Scene::onBattleAttakResult));
}

void Scene::onBattleMoveResult(Event *event)
{
	if (event->type == PVEBattle::BATTLE_MOVE_SUCCESS)
	{
		MoveResult* result = (MoveResult*)event->data;
		CommandParallel* cmds = CommandParallel::create();
		int i = 0;
		ID iid;
		while (iid = result->moveUnits[i])
		{
			(*mActors)[iid]->calculateNextPosition();
			CommandMove *move = CommandMove::create((*mActors)[iid]);
			cmds->addCommand(move);
			i++;
		}
		CommandScroll* cmd = CommandScroll::create(mPVEScene, result->dir);
		cmds->addCommand(cmd);
		i = 0;
		while (result->skills[i].giver)
		{
			Command* skill = CommandSkill::create(&result->skills[i]);
			cmds->addCommand(skill);
			i++;
		}
		i = 0;
		while (result->buffs[i].owner)
		{
			Command* buff = CommandBuff::create(&result->buffs[i]);
			cmds->addCommand(buff);
			i++;
		}
		mPVEScene->addCommand(cmds);
		i = 0;
		CommandParallel* picks = CommandParallel::create();
		while (iid = result->pick[i])
		{
			std::map<ID, Prop*>::iterator it;
			if ((it = mProps->find(iid)) != mProps->end())
			{
				CommandPick* pCmd = CommandPick::create(it->second);
				picks->addCommand(pCmd);
				mProps->erase(it);
			}
			i++;
		}
		mPVEScene->addCommand(picks);
		i = 0;
		while (iid = result->comeIntoView[i])
		{
			Actor* actor = new Actor(mPVEScene->layerActor());
			Unit* unit = System::pve->getUnit(iid);
			CCAssert(unit != NULL, "Unit is null!");
			actor->setData(unit);
			(*mActors)[actor->iid()] = actor;
			i++;
		}
		i = 0;
		CommandParallel* enemysMove = CommandParallel::create();
		while (iid = result->enemys[i])
		{
			(*mActors)[iid]->calculateNextPosition();
			CommandMove *move = CommandMove::create((*mActors)[iid]);
			enemysMove->addCommand(move);
			i++;
		}
		mPVEScene->addCommand(enemysMove);
	}
	else if (event->type == PVEBattle::BATTLE_MOVE_FAILED)
	{
		//ID* heroCantMove = (ID*)event->data;
	}
	else if (event->type == PVEBattle::BATTLE_MOVE_HERO_SUCESS)
	{
		MoveResult *result = (MoveResult *)event->data;
		ID iid = result->moveUnits[0];
		(*mActors)[iid]->updatePosition();
		int i = 0;
		CommandParallel* enemysMove = CommandParallel::create();
		while (iid = result->enemys[i])
		{
			(*mActors)[iid]->calculateNextPosition();
			CommandMove *move = CommandMove::create((*mActors)[iid]);
			enemysMove->addCommand(move);
			i++;
		}
		mPVEScene->addCommand(enemysMove);
	}
}

void Scene::onBattleAttakResult(Event* event)
{
	AttackResult* aResults = (AttackResult*)event->data;
	
	while (aResults->count)
	{
		int i = 0;
		while (i < aResults->count)
		{
			SkillResult* sResult = aResults->results + i;
			Command* attack = CommandSkill::create(sResult);
			mPVEScene->addCommand(attack);
			Actor* victim = getActor(sResult->recipient);
			//CommandProgress* progress = CommandProgress::create(sResult->value, victim);
			//mPVEScene->addCommand(progress);
			if (sResult->healthLeft <= 0)
			{
				CommandDie* die = CommandDie::create(victim);
				mPVEScene->addCommand(die);
				mActors->erase(mActors->find(sResult->recipient));
			}
			i++;
		}
		if (aResults->loot)
		{
			Item* loot = System::pve->getItem(aResults->loot);
			Prop* prop = new Prop(mPVEScene->layerProp());
			prop->bindData(loot);
			(*mProps)[loot->iid] = prop;
		}
		aResults++;
	}
}

void Scene::leavePVEMap()
{

}

Actor* Scene::getActor(ID iid)
{
	std::map<ID, Actor*>::iterator res = mActors->find(iid);
	assert(res != mActors->end());
	return res->second;
}

PVEBattleScene* Scene::pve()
{
	return mPVEScene;
}

Actor* Scene::getActorByGrid(const CCPoint &pos)
{
	CCRect bounds;
	for (std::map<ID, Actor*>::iterator it = mActors->begin(); it != mActors->end(); it++)
	{

		bounds.origin = it->second->sprite()->getPosition();
		bounds.origin.x -= GRID_SIZE / 2;
		bounds.origin.y -= GRID_SIZE / 2;
		bounds.size = CCSize(GRID_SIZE,GRID_SIZE);
		if (bounds.containsPoint(pos)) return it->second;
	}
	return NULL;
}


