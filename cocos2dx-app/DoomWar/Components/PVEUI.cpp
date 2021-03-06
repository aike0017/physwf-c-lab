#include "PVEUI.h"
#include "System.h"


PVEUI::PVEUI()
{
}

PVEUI::~PVEUI()
{

}

bool PVEUI::init()
{
	TouchGroup::init();

	mMask = UILayout::create();
	mMask->setTouchEnabled(false);

	UIImageView* maskL = UIImageView::create();
	maskL->loadTexture("left-mask",UI_TEX_TYPE_PLIST);
	maskL->setScaleY(960);
	maskL->setAnchorPoint(CCPoint(0,0));
	maskL->setPosition(CCPoint(0, 0));
	mMask->addChild(maskL);

	UIImageView* maskUp = UIImageView::create();
	maskUp->loadTexture("up-mask", UI_TEX_TYPE_PLIST);
	maskUp->setScaleX(640);
	maskUp->setAnchorPoint(CCPoint(0, 1));
	maskUp->setPosition(CCPoint(0,960));
	mMask->addChild(maskUp);

	UIImageView* maskR = UIImageView::create();
	maskR->loadTexture("right-mask", UI_TEX_TYPE_PLIST);
	maskR->setScaleY(960);
	maskR->setAnchorPoint(CCPoint(1, 0));
	maskR->setPosition(CCPoint(640, 0));
	mMask->addChild(maskR);

	//UIImageView* maskD = UIImageView::create();
	//maskD->loadTexture("down-mask", UI_TEX_TYPE_PLIST);
	//maskD->setScaleX(640);
	//maskD->setAnchorPoint(CCPoint(0, 0));
	//mMask->addChild(maskD);

	addWidget(mMask);

	mDashBoard = UILayout::create();

	UIImageView* frame = UIImageView::create();
	frame->loadTexture("frame",UI_TEX_TYPE_PLIST);
	frame->setAnchorPoint(CCPoint(0, 0));
	mDashBoard->setSize(frame->getContentSize());
	mDashBoard->addChild(frame);

	mBtnLeft = UIButton::create();
	mBtnLeft->loadTextures("left-button", "left-button", "left-button",UI_TEX_TYPE_PLIST);
	mBtnLeft->setPosition(CCPoint(70,57));
	mDashBoard->addChild(mBtnLeft);

	mBtnForward = UIButton::create();
	mBtnForward->loadTextures("forward-button", "forward-button", "forward-button", UI_TEX_TYPE_PLIST);
	mBtnForward->setPosition(CCPoint(232, 54));
	mDashBoard->addChild(mBtnForward);

	mBtnBackward = UIButton::create();
	mBtnBackward->loadTextures("backward-button", "backward-button", "backward-button", UI_TEX_TYPE_PLIST);
	mBtnBackward->setPosition(CCPoint(405,52));
	mDashBoard->addChild(mBtnBackward);

	mBtnRight = UIButton::create();
	mBtnRight->loadTextures("right-button", "right-button", "right-button", UI_TEX_TYPE_PLIST);
	mBtnRight->setPosition(CCPoint(568, 55));
	mDashBoard->addChild(mBtnRight);

	addWidget(mDashBoard);

	mStepCounter = UILayout::create();
	UIImageView* scBg = UIImageView::create();
	scBg->loadTexture("step", UI_TEX_TYPE_PLIST);
	mStepCounter->addChild(scBg);
	mLblStep = UILabelBMFont::create();
	mLblStep->setFntFile("./Data/step-font.fnt");
	mLblStep->setText("0");
	mLblStep->setPosition(CCPoint(40, -3));
	mStepCounter->addChild(mLblStep);
	mStepCounter->setPosition(CCPoint(162, 915));
	addWidget(mStepCounter);

	mCoin = UILayout::create();
	UIImageView* coinBg = UIImageView::create();
	coinBg->loadTexture("earn", UI_TEX_TYPE_PLIST);
	mCoin->addChild(coinBg);
	mLblCoin = UILabelBMFont::create();
	mLblCoin->setFntFile("./Data/step-font.fnt");
	mLblCoin->setText("0");
	mLblCoin->setScale(0.5);
	mLblCoin->setPosition(CCPoint(20, -3));
	mCoin->addChild(mLblCoin);
	mCoin->setPosition(CCPoint(447, 921));

	addWidget(mCoin);

	mSumary = UILayout::create();
	mSumary->setPosition(CCPoint(320,480));
	mSumary->setTouchEnabled(true);

	UIImageView* bg = UIImageView::create();
	bg->loadTexture("sum_bg",UI_TEX_TYPE_PLIST);
	mSumary->addChild(bg);

	UIImageView* title = UIImageView::create();
	title->loadTexture("sum_title", UI_TEX_TYPE_PLIST);
	title->setPosition(CCPoint(0,247));
	mSumary->addChild(title);

	UIImageView* okBg = UIImageView::create();
	okBg->loadTexture("sum_cfrm_bg", UI_TEX_TYPE_PLIST);
	okBg->setPosition(CCPoint(0, -227));
	mSumary->addChild(okBg);
	
	mSumOk = UIButton::create();
	mSumOk->loadTextures("sum_cfrm_fg", "sum_cfrm_fg", "sum_cfrm_fg",UI_TEX_TYPE_PLIST);
	mSumOk->setPosition(CCPoint(0, -227));
	mSumary->addChild(mSumOk);

	mSumary->retain();
	//addWidget(mSumary);
	//setStarLevel(2);
	//UIButton
	return true;
}

PVEUI* PVEUI::create(CCLayer* pve)
{
	PVEUI* pUi = new PVEUI();
	if (pUi && pUi->init())
	{
		pUi->autorelease();
		//pUi->addWidget(GUIReader::shareReader()->widgetFromJsonFile("./Data/UIPVE_1/UIPVE_1.ExportJson"));
		pUi->mPVEBattle = pve;
		//pUi->setAnchorPoint(ccp(-320, -480));
		return pUi;
	}
	return NULL;
}

void PVEUI::onEnter()
{
	TouchGroup::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(mController,1,false);
	mBtnLeft->addTouchEventListener(this, toucheventselector(PVEUI::onLeftTouched));
	mBtnForward->addTouchEventListener(this, toucheventselector(PVEUI::onForwardTouched));
	mBtnBackward->addTouchEventListener(this, toucheventselector(PVEUI::onBackwardTouched));
	mBtnRight->addTouchEventListener(this, toucheventselector(PVEUI::onRightwardTouched));
	//System::bag->addEventListener(Bag::BAG_GOLD_CHANGE, this, EventListener(&Scene::onUnitFlop))
}

void PVEUI::onExit()
{
	TouchGroup::onExit();
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(mController);
}

void PVEUI::hideDashBoard(float duration)
{
	CCPoint delta = CCPoint(0,-mDashBoard->getContentSize().height);
	CCMoveBy* move = CCMoveBy::create(duration, delta);
	mDashBoard->runAction(move);
}

void PVEUI::showDashBoard(float duration)
{
	CCPoint delta = CCPoint(0,mDashBoard->getContentSize().height);
	CCMoveBy* move = CCMoveBy::create(duration, delta);
	mDashBoard->runAction(move);
}

UILabelBMFont* PVEUI::labelGold() const
{
	return mLblCoin;
}

UILabelBMFont* PVEUI::labelStep() const
{
	return mLblStep;
}

UILayout* PVEUI::panelSumary() const
{
	return mSumary;
}

void PVEUI::setStarLevel(char level)
{
	for (int i = 0; i < level; i++)
	{
		UIImageView* star = UIImageView::create();
		star->loadTexture("sum_star", UI_TEX_TYPE_PLIST);
		star->setPosition(CCPoint(-100+i*100,100));
		mSumary->addChild(star);
	}
	for (int i = level; i < 3; i++)
	{
		UIImageView* star = UIImageView::create();
		star->loadTexture("sum_empty_star",UI_TEX_TYPE_PLIST);
		star->setPosition(CCPoint(-100 + i * 100, 100));
		mSumary->addChild(star);
	}
}

void PVEUI::onLeftTouched(CCObject* object, TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		System::pve->step(LEFTWARD);
	}
}

void PVEUI::onForwardTouched(CCObject* object, TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		System::pve->step(FORWARD);
	}
}

void PVEUI::onBackwardTouched(CCObject* object, TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		System::pve->step(BACKWARD);
	}
}

void PVEUI::onRightwardTouched(CCObject* object, TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		System::pve->step(RIGHTWARD);
	}
}

bool PVEUI::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	mPVEBattle->ccTouchBegan(touch,event);
	TouchGroup::ccTouchBegan(touch, event);
	return true;
}


void PVEUI::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	mPVEBattle->ccTouchMoved(touch, event);
	TouchGroup::ccTouchMoved(touch, event);
}

void PVEUI::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	mPVEBattle->ccTouchEnded(touch, event);
	TouchGroup::ccTouchEnded(touch, event);
}

void PVEUI::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
	mPVEBattle->ccTouchCancelled(touch, event);
	TouchGroup::ccTouchCancelled(touch, event);
}


