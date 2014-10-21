#ifndef _PVEBATTLE_UI_H
#define _PVEBATTLE_UI_H

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace ui;

class PVEUIController;

class PVEUI : public UILayer
{
public:
	static PVEUI* create(CCLayer* pve);

	virtual void onEnter();
	virtual void onExit();
	virtual bool init();

	void hideDashBoard(float duration = 0.2);
	void showDashBoard(float duration = 0.2);
private:
	PVEUI();
	~PVEUI();

	void onLeftTouched(CCObject* object, TouchEventType type);
	void onForwardTouched(CCObject* object, TouchEventType type);
	void onBackwardTouched(CCObject* object, TouchEventType type);
	void onRightwardTouched(CCObject* object, TouchEventType type);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event);
private:
	CCSprite* mFrame;
	CCLayer* mPVEBattle;

	UILayout* mMask;

	UILayout* mDashBoard;
	UIButton* mBtnLeft;
	UIButton* mBtnForward;
	UIButton* mBtnBackward;
	UIButton* mBtnRight;

	UILayout* mStepCounter;
	UILabelBMFont* mLblStep;

	UILayout* mCoin;
	UILabelBMFont* mLblCoin;
};

/*PVEUIController*/
class PVEUIController : public CCTouchDelegate, public CCObject
{
public:
	PVEUIController(PVEUI* view);
	~PVEUIController();

	static PVEUIController* create(PVEUI* view);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event);
private:
	PVEUI* mView;
};
#endif