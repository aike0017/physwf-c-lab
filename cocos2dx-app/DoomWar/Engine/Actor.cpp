#include "Actor.h"
#include <math.h>
#include "ResourceManager.h"

Actor::Actor(CCSprite* layer)
{
	mLayer = layer;
	mPosition = new CCPoint();
}

void Actor::setData(Unit* data)
{
	mData = data;
	mSprite = ActorSprite::create(mData->cid);
	updatePosition();
	//mSprite->bloor()->setPercent((float)mData->health / (float)mData->maxHealth);
	mSprite->setPosition(*mPosition);
	mLayer->addChild(mSprite);
}

ID Actor::iid() const
{
	return mData->iid;
}

int Actor::health() const
{
	return mData->health;
}

void Actor::updatePosition()
{
	mPosition->x = mData->positon.x * GRID_SIZE + X_MARGIN + GRID_SIZE/2;
	mPosition->y = mData->positon.y * GRID_SIZE + GRID_SIZE / 2;
	//
}

CCPoint* Actor::position()
{
	return mPosition;
}

void Actor::updateHealth(int delta)
{
	mSprite->bloor()->setDelta((float)delta);
}

void Actor::shake(float delta)
{
	static float time;
	time += delta;
	float amp = .5 / time;
	float dx = amp * sin(time);
	float dy = amp * sin(time);
	
	CCPoint p = mSprite->getPosition();
	p.x += dx;
	p.y += dy;
	mSprite->setPosition(p);

	//if (amp < 0.1) return true;
}

void Actor::attack()
{
	
}

CCSprite* Actor::sprite() const
{
	return mSprite;
}

Unit* Actor::getData()
{
	return mData;
}

