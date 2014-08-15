#include "Game.h"
#include "ResourceManager.h"
#include "Engine.h"

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
	Engine::scene->showLoading();
	ResourceManager::instance()->load("./Data/units.png", this, callfuncO_selector(Game::onUnitSpriteComplete));
}

void Game::onUnitSpriteComplete(CCObject* tex)
{
	CCTexture2D* texUnits = (CCTexture2D*)tex;
	ResourceManager::instance()->addSpriteFramePack("./Data/units.plist", texUnits);
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
	Engine::scene->initialize();
	Engine::scene->enterPVEMap(0);
}