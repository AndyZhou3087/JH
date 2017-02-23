#include "GameScene.h"
#include "TopBar.h"
#include "HomeLayer.h"
#include "UIScroll.h"
#include "GameDataSave.h"
USING_NS_CC;

Nature* g_natureData;
Hero* g_heroData;
GameScene::GameScene()
{

}
GameScene::~GameScene()
{
	delete g_heroData;
	delete g_natureData;
	GameDataSave::purgeGameSave();
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	homeLayer = HomeLayer::create();
	addChild(homeLayer);

	g_natureData = new Nature;
	g_natureData->setTime(GameDataSave::getInstance()->getNatureTime());
	g_natureData->setWheather((EWheather)GameDataSave::getInstance()->getNatureWheather());

	g_heroData = new Hero;
	g_heroData->setLifeValue(100);
	g_heroData->setMaxLifeValue(100);
	g_heroData->setLiveDays(GameDataSave::getInstance()->getLiveDays());

	Sprite* bg = Sprite::createWithSpriteFrameName("ui/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 980));
	this->addChild(bg, 1);

	uiScroll = UIScroll::create(500.0f, 132.0f);
	uiScroll->setPosition(Vec2(visibleSize.width / 2, 980));
	addChild(uiScroll, 1);

	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	topBar->setScrollContainer(uiScroll);
	addChild(topBar);
	
	loadSaveData();

    return true;
}

void GameScene::loadSaveData()
{
	g_natureData->setReason((EReason)GameDataSave::getInstance()->getNatureReason());
	g_natureData->setWheather((EWheather)GameDataSave::getInstance()->getNatureWheather());
	g_natureData->setTime(GameDataSave::getInstance()->getNatureTime());
	g_natureData->setTemperature(GameDataSave::getInstance()->getNatureTemperature());
}

void GameScene::onExit()
{
	int ntime = g_natureData->getTime();
	GameDataSave::getInstance()->setNatureTime(ntime);
	Layer::onExit();
}