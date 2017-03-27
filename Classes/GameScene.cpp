#include "GameScene.h"
#include "TopBar.h"
#include "HomeLayer.h"
#include "UIScroll.h"
#include "GameDataSave.h"
#include "StorageRoom.h"
#include "GlobalData.h"
#include "MapLayer.h"
USING_NS_CC;

Nature* g_nature;
Hero* g_hero;

GameScene* g_gameLayer = NULL;
GameScene::GameScene()
{

}
GameScene::~GameScene()
{
	GameDataSave::purgeGameSave();
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_gameLayer = GameScene::create();

    // add layer as a child to scene
	scene->addChild(g_gameLayer);

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
    
	GlobalData::loadResJsonData();
	GlobalData::loadHillResJsonData();
	GlobalData::loadMapJsonData();
	GlobalData::loadNpcJsonData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::string addrstr = GameDataSave::getInstance()->getHeroAddr();
	if (addrstr.compare("m1-1") == 0)
	{
		homeLayer = HomeLayer::create();
		addChild(homeLayer, 1, "homelayer");
	}
	else
	{
		g_maplayer = MapLayer::create();
		addChild(g_maplayer, 1, "maplayer");
	}

	g_nature = Nature::create();
	this->addChild(g_nature);

	g_hero = Hero::create();
	this->addChild(g_hero);

	loadSaveData();

	g_hero->setLifeValue(100);
	g_hero->setMaxLifeValue(100);


	Sprite* bg = Sprite::createWithSpriteFrameName("ui/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 960));
	this->addChild(bg, 3);

	uiScroll = UIScroll::create(500.0f, 132.0f);
	uiScroll->setPosition(Vec2(visibleSize.width / 2, 960));
	addChild(uiScroll, 3);

	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	topBar->setScrollContainer(uiScroll);
	addChild(topBar, 2);
	
	this->schedule(schedule_selector(GameScene::updata), 0.2f);
	this->schedule(schedule_selector(GameScene::timerSaveResData), 3.0f);

    return true;
}

void GameScene::loadSaveData()
{
	g_nature->setReason((EReason)GameDataSave::getInstance()->getNatureReason());
	g_nature->setWeather((EWeather)GameDataSave::getInstance()->getNatureWeather());
	g_nature->setTime(GameDataSave::getInstance()->getNatureTime());
	g_nature->setTemperature(GameDataSave::getInstance()->getNatureTemperature());
	g_nature->setPastDays(GameDataSave::getInstance()->getLiveDays());

	g_hero->setLifeValue(GameDataSave::getInstance()->getHeroLife());
	g_hero->setMaxLifeValue(GameDataSave::getInstance()->getHeroMaxLife());
	g_hero->setOutinjuryValue(GameDataSave::getInstance()->getHeroOutinjury());
	g_hero->setInnerinjuryValue(GameDataSave::getInstance()->getHeroInnerinjury());
	g_hero->setHungerValue(GameDataSave::getInstance()->getHeroHunger());
	g_hero->setSpiritValue(GameDataSave::getInstance()->getHeroSpirit());

	StorageRoom::loadStorageData();
	MyPackage::load();
	GlobalData::loadResData();
}

void GameScene::onExit()
{
	saveAllData();

	Layer::onExit();
}

void GameScene::saveAllData()
{
	GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	GameDataSave::getInstance()->setNatureWeather(g_nature->getWeather());
	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	GameDataSave::getInstance()->setNatureTime(g_nature->getTime());
	GameDataSave::getInstance()->setLiveDays(g_nature->getPastDays());
	GameDataSave::getInstance()->setHeroLife(g_hero->getLifeValue());
	GameDataSave::getInstance()->setHeroMaxLife(g_hero->getMaxLifeValue());
	GameDataSave::getInstance()->setHeroOutinjury(g_hero->getOutinjuryValue());
	GameDataSave::getInstance()->setHeroInnerinjury(g_hero->getInnerinjuryValue());
	GameDataSave::getInstance()->setHeroHunger(g_hero->getHungerValue());
	GameDataSave::getInstance()->setHeroSpirit(g_hero->getSpiritValue());
}

void GameScene::updata(float dt)
{
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i] == data->id)
			{
				if (data->count <= 0)
				{
					data->pastmin = 0;
					data->waittime += 1;
					if (data->waittime >= data->speed * data->max)
					{
						data->count = data->max;
						data->waittime = 0.0f;
					}
				}
				else
				{
					data->pastmin += 1;
					if (data->pastmin >= data->speed)
					{
						data->pastmin = 0;
						data->count++;
					}

					if (data->count >= data->max)
						data->count = data->max;
				}
			}
		}
	}
}

void GameScene::timerSaveResData(float dt)
{
	GlobalData::saveResData();
}