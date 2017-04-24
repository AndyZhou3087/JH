#include "GameScene.h"
#include "TopBar.h"
#include "HomeLayer.h"
#include "UIScroll.h"
#include "GameDataSave.h"
#include "StorageRoom.h"
#include "GlobalData.h"
#include "MapLayer.h"
#include "CommonFuncs.h"
USING_NS_CC;

Nature* g_nature;
Hero* g_hero;

GameScene* g_gameLayer = NULL;

UIScroll* g_uiScroll;
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
	GlobalData::loadHeroAtrJsonData();
	GlobalData::loadWG_NGJsonData();
	GlobalData::loadEquipJsonData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	g_nature = Nature::create();
	this->addChild(g_nature);

	g_hero = Hero::create();
	this->addChild(g_hero);

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

	loadSaveData();


	Sprite* bg = Sprite::createWithSpriteFrameName("ui/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 960));
	this->addChild(bg, 3);

	g_uiScroll = UIScroll::create(500.0f, 132.0f);
	g_uiScroll->setPosition(Vec2(visibleSize.width / 2, 960));
	addChild(g_uiScroll, 3);

	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	topBar->setScrollContainer(g_uiScroll);
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
	

	g_hero->setOutinjuryValue(GameDataSave::getInstance()->getHeroOutinjury());
	g_hero->setInnerinjuryValue(GameDataSave::getInstance()->getHeroInnerinjury());
	g_hero->setHungerValue(GameDataSave::getInstance()->getHeroHunger());
	g_hero->setSpiritValue(GameDataSave::getInstance()->getHeroSpirit());
	int heroid = GameDataSave::getInstance()->getHeroId();
	g_hero->setID(heroid);
	g_hero->setMyName(GlobalData::map_heroAtr[heroid].name);
	int lv = GameDataSave::getInstance()->getHeroLV();
	g_hero->setLVValue(lv);
	int exp = GameDataSave::getInstance()->getHeroExp();
	g_hero->setExpValue(exp);
	g_hero->setIsOut(GameDataSave::getInstance()->getHeroIsOut());

	g_hero->setAtkValue(GlobalData::map_heroAtr[heroid].vec_atk[lv]);
	g_hero->setDfValue(GlobalData::map_heroAtr[heroid].vec_df[lv]);
	g_hero->setMaxLifeValue(GlobalData::map_heroAtr[heroid].vec_maxhp[lv]);
	
	int hlife = GameDataSave::getInstance()->getHeroLife();
	if (hlife > -1)
		g_hero->setLifeValue(hlife);
	else
	{
		g_hero->setLifeValue(g_hero->getMaxLifeValue());
	}

	StorageRoom::loadStorageData();
	MyPackage::load();
	GlobalData::loadResData();
	loadSavedHeroPropData();
}

void GameScene::loadSavedHeroPropData()
{
	std::string strval = GameDataSave::getInstance()->getHeroProperData();
	std::vector<std::string> tmp;
	CommonFuncs::split(strval, tmp, ";");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		CommonFuncs::split(tmp[i], tmp2, "-");

		PackageData sdata;
		sdata.strid = tmp2[0];
		sdata.type = atoi(tmp2[1].c_str());
		sdata.count = atoi(tmp2[2].c_str());
		sdata.extype = atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());
		sdata.exp = atoi(tmp2[5].c_str());
		sdata.goodvalue = atoi(tmp2[6].c_str());
		sdata.name = tmp2[7];
		sdata.desc = tmp2[8];
		g_hero->setAtrByType((HeroAtrType)i, sdata);
		//g_hero->set [sdata.type].push_back(sdata);
	}
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
	GameDataSave::getInstance()->setHeroOutinjury(g_hero->getOutinjuryValue());
	GameDataSave::getInstance()->setHeroInnerinjury(g_hero->getInnerinjuryValue());
	GameDataSave::getInstance()->setHeroHunger(g_hero->getHungerValue());
	GameDataSave::getInstance()->setHeroSpirit(g_hero->getSpiritValue());
	GameDataSave::getInstance()->setHeroLV(g_hero->getLVValue());
	GameDataSave::getInstance()->setHeroId(g_hero->getID());
	GameDataSave::getInstance()->setHeroExp(g_hero->getExpValue());
	GameDataSave::getInstance()->setHeroIsOut(g_hero->getIsOut());
}

void GameScene::updata(float dt)
{
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (data->strid.compare(GlobalData::vec_hillResid[i]) == 0)
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