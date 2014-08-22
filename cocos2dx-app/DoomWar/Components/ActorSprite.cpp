#include "ActorSprite.h"
#include "ResourceManager.h"

ActorSprite::ActorSprite(ID cid)
{

}

ActorSprite::~ActorSprite()
{

}

void ActorSprite::update(float delta)
{

}

ActorSprite* ActorSprite::create(ID cid)
{
	ActorSprite* pActor = new ActorSprite(cid);
	if (pActor && pActor->init())
	{
		pActor->autorelease();
		char strcid[20] = {0};
		getFrameNameByCID(cid,strcid);
		CCSpriteFrame* frame = ResourceManager::instance()->getSpriteFrame(strcid);
		pActor->mBody = CCSprite::createWithSpriteFrame(frame);
		pActor->addChild(pActor->mBody);
		pActor->mBody->setAnchorPoint(ccp(0,0));
		pActor->mBloodBar = BloodBar::create(1);
		pActor->mBloodBar->setAnchorPoint(ccp(0, 0));
		pActor->addChild(pActor->mBloodBar);
		//blood bar
		//armor
		return pActor;
	}
	return NULL;
}

void ActorSprite::getFrameNameByCID(ID cid, char* name)
{
	switch (cid)
	{
	case 1:
	{
			  sprintf(name, "%s", "archer.png");
			  break;
	}
	case 2:
	{
			  sprintf(name, "%s", "BarriersFlower.png");
			  break;
	}
	case 3:
	{
			  sprintf(name, "%s", "gnome.png");
			  break;
	}
	default:
		break;
	}
}

BloodBar* ActorSprite::bloor() const
{
	return mBloodBar;
}

BloodBar::BloodBar(float percent)
{
	mTexture = new CCTexture2D();
	setPercent(percent);
	memset(mPixels, 0, sizeof(mPixels)); 
}

BloodBar::~BloodBar()
{

}

BloodBar* BloodBar::create(float percent)
{
	BloodBar* pBloor = new BloodBar(percent);
	if (pBloor && pBloor->initWithTexture(pBloor->mTexture))
	{
		pBloor->autorelease();
		return pBloor;
	}
	return NULL;
}

void BloodBar::setPercent(float percent)
{
	mPercent = percent;
	unsigned int prog = (1.0 - percent / 10) * 100.0;
	float n = prog / 100.0;
	float p = n - floor(n);
	unsigned int temp1 = 0xFF0000 * (1.0 - p);
	unsigned int temp2 = 0xFF0000 * p;
	unsigned int color = (temp1 >> 16) + (temp2 >> 8);
	//memset(mPixels, 0, sizeof(mPixels));
	std::fill_n(mPixels, 96, 0xFF000000);
	color += 0xFF000000;
	std::fill_n(mPixels, 96 * percent, color);
	for (int i = 1; i < 8; i++)
	{
		memcpy(mPixels + 96 * i, mPixels, 96 * sizeof(unsigned int));
	}
	CCSize size(96, 8);
	mTexture->initWithData((const void*)&mPixels, kCCTexture2DPixelFormat_RGBA8888, 96, 8, size);
}




