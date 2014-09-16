#ifndef _EFFECT_H
#define _EFFECT_H

#include "cocos2d.h"
#include "dwtype.h"
#include "Actor.h"

using namespace cocos2d;

class Effect : public CCSprite
{
public:
	virtual bool tick(float delta) = 0;
	virtual void fire() = 0;
protected:
	CCSprite* mLayer;
};

class BulletEffect : public Effect
{
public:
	virtual bool tick(float delta);
	virtual void fire();
	virtual void onEnter();
	virtual void onExit();
	static BulletEffect* create(ID cid, ID attacker, ID victim);
private:
	BulletEffect(CCSprite* layer);
	~BulletEffect();

	void getFrameNameByCID(ID cid, char* name);
private:
	CCAction* mAction;
	CCPoint* mTarget;
	float mSpeed;
	CCPoint mDir;
};

class HackEffect : public Effect
{
public:
	virtual bool tick(float delta);
	virtual void fire();
	virtual void onEnter();
	virtual void onExit();
	static HackEffect* create(ID cid, ID attacker, ID victim);
private:
	HackEffect(CCSprite* layer);
	~HackEffect();
private:
	CCAction* mAction;
	CCPoint mDir;
	CCPoint* mTarget;
};

class ArcEffect : public Effect
{
public:
	virtual bool tick(float delta);
	virtual void fire();
	virtual void draw();
	void drawArc(float x1, float y1, float x2, float y2, float distance);
	static ArcEffect* create(ID cid, ID attacker, ID victim);
private:
	ArcEffect();
	~ArcEffect();
private:
	Actor* mAttacker;
	Actor* mVictim;
	CCPoint* mTarget;
};
#endif
