#include "Game.h"
#include "ResourceManager.h"
#include "DWLoadingScene.h"

Game::Game()
{

}

Game::~Game()
{

}

Game* Game::mInstance = NULL;

Game* Game::instance()
{
	if (mInstance == NULL)
		mInstance = new Game();
	return mInstance;
}

void Game::pause()
{
	CCDirector::sharedDirector()->stopAnimation();
}

void Game::resume()
{
	CCDirector::sharedDirector()->startAnimation();
}

void Game::setup()
{
	mLoading = DWLoadingScene::scene();
	CCDirector::sharedDirector()->runWithScene(mLoading);
	ResourceManager::instance()->load("units.png",this, callfuncO_selector(onUnitSpriteComplete));
}

void Game::onUnitSpriteComplete(CCObject* tex)
{
	CCTexture2D* texUnits = (CCTexture2D*)tex;
	ResourceManager::instance()->addSpriteFramePack("units.plist", texUnits);
	//ResourceManager::instance()->load("map.png", this, callfuncO_selector(onMapSpriteComplete));
	onMapSpriteComplete(NULL);
}

void Game::onMapSpriteComplete(CCObject* tex)
{
	onAllComplete();
}

void Game::onAllComplete()
{
	//temp 
	mLoading->release();

	mPVEScene = PVEBattleScene::scene();
	CCDirector::sharedDirector()->runWithScene(mPVEScene);
	
}