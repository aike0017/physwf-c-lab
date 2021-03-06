#ifndef _CONFIG_H
#define _CONFIG_H

#include "Unit.h"
#include "dwtype.h"
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

class HeroConfig;
class BarrierConfig;
class MonsterConfig;
class SkillConfig;
class ItemConfig;

class Config : public DWObject
{
public:
	typedef void(DWObject::*OnConfigLoaded)(xmlNodePtr root);
public:
	static HeroConfig* hero;
	static BarrierConfig* barrier;
	static MonsterConfig* monster;
	static SkillConfig* skill;
	static ItemConfig* item;
protected:
	void constructUnitWithXML(Unit* unit, xmlNodePtr node);
	void parseGSet(GSet* set, xmlChar* szGset);
	void parseRange(Skill* skill, xmlChar* szRange);
	void parseRange2(Skill* skill, xmlChar* szRange);
	void parseView(Unit* unit, xmlChar* szView);
	void parseSkills(Unit* unit,xmlChar* szSkills);
	void constructSkillWithXML(Skill* skill, xmlNodePtr node);
	void constructBuffWithXML(Buff* buff, xmlNodePtr node);
	void constructItemWithXML(Item* item, xmlNodePtr node);
	void constructGSetWithXML(GSet* set, xmlNodePtr node);
private:
	Grid* mGBuffer;
};

class HeroConfig : public Config
{
public:
	Unit* create(ID cid);
	void onHeroConfigLoaded(xmlNodePtr root);
private:
	std::map<ID, Unit*> mMaster;
};

class BarrierConfig : public Config
{
public:
	Unit* create(ID cid);
	void onBarrierConfigLoaded(xmlNodePtr root);
private:
	std::map<ID, Unit*> mMaster;
};

class MonsterConfig : public Config
{
public:
	Unit* create(ID cid);
	void onMonsterConfigLoaded(xmlNodePtr root);
private:
	std::map<ID, Unit*> mMaster;
};

class SkillConfig : public Config
{
public:
	void fill(Skill* skill,ID cid);
	void fill(Buff* buff, ID cid);
	void fill(GSet* range, ID cid);
	void onSkillConfigLoaded(xmlNodePtr root);
	void onBuffConfigLoaded(xmlNodePtr root);
	void onGSetConfigLoaded(xmlNodePtr root);
private:
	std::map<ID, Skill*> mSkills;
	std::map<ID, Buff*> mBuffs;
	std::map<ID, GSet*> mGSet;
};

class ItemConfig :public Config
{
public:
	Item* create(ID cid);
	void onItemConfigLoaded(xmlNodePtr root);
private:
	std::map<ID, Item*> mItems;
};


#endif