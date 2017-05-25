#include "GameScene.h"
#include "TopBar.h"
#include "HomeLayer.h"
#include "UIScroll.h"
#include "GameDataSave.h"
#include "StorageRoom.h"
#include "GlobalData.h"
#include "MapLayer.h"
#include "CommonFuncs.h"
#include "ReviveLayer.h"
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
    
	//读取资源配置文件
	GlobalData::loadResJsonData();

	//读取后山资源列表配置文件
	GlobalData::loadHillResJsonData();

	//读取地图配置文件
	GlobalData::loadMapJsonData();

	//读取NPC配置文件
	GlobalData::loadNpcJsonData();

	//读取角色配置文件
	GlobalData::loadHeroAtrJsonData();

	//读取内功，外功配置文件
	GlobalData::loadWG_NGJsonData();

	//读取武器防具配置文件
	GlobalData::loadEquipJsonData();

	//读取剧情配置文件
	GlobalData::loadPlotMissionJsonData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//天气，季节数据
	g_nature = Nature::create();
	this->addChild(g_nature);

	//角色数据
	g_hero = Hero::create();
	this->addChild(g_hero);

	//
	loadSaveData();

	//角色目前在哪个地点，第一次进入家
	std::string addrstr = GameDataSave::getInstance()->getHeroAddr();
	if (addrstr.compare("m1-1") == 0)//家
	{
		homeLayer = HomeLayer::create();
		addChild(homeLayer, 1, "homelayer");
	}
	else//其他地图界面
	{
		g_maplayer = MapLayer::create();
		addChild(g_maplayer, 1, "maplayer");
	}

	//滚动文字的背景
	Sprite* bg = Sprite::createWithSpriteFrameName("ui/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 960));
	this->addChild(bg, 4);

	//滚动文字
	g_uiScroll = UIScroll::create(560.0f, 132.0f);
	g_uiScroll->setPosition(Vec2(visibleSize.width / 2, 960));
	addChild(g_uiScroll, 4);

	//任务属性和天气
	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	addChild(topBar, 3);
	
	this->schedule(schedule_selector(GameScene::updata), 0.2f);
	this->schedule(schedule_selector(GameScene::timerSaveResData), 3.0f);
	this->schedule(schedule_selector(GameScene::checkiflive), 1.0f);
    return true;
}

void GameScene::loadSaveData()
{
	//设置保存的季节
	g_nature->setReason((EReason)GameDataSave::getInstance()->getNatureReason());
	//设置保存的天气
	g_nature->setWeather((EWeather)GameDataSave::getInstance()->getNatureWeather());
	
	int cdata = GameDataSave::getInstance()->getNatureWeatherChangeCount();
	//设置保存的天气改变的次数
	g_nature->changeWeatherCount = cdata / 10000;
	
	//设置天气改变的随机小时数
	g_nature->changeWeatherRandow = cdata % 10000;

	//设置保存的时间
	g_nature->setTime(GameDataSave::getInstance()->getNatureTime());
	//温度
	g_nature->setTemperature(GameDataSave::getInstance()->getNatureTemperature());
	//天数
	g_nature->setPastDays(GameDataSave::getInstance()->getLiveDays());
	
	//外伤
	g_hero->setOutinjuryValue(GameDataSave::getInstance()->getHeroOutinjury());
	//内伤
	g_hero->setInnerinjuryValue(GameDataSave::getInstance()->getHeroInnerinjury());
	//饱食度
	g_hero->setHungerValue(GameDataSave::getInstance()->getHeroHunger());
	//精神
	g_hero->setSpiritValue(GameDataSave::getInstance()->getHeroSpirit());
	//角色ID
	int heroid = GameDataSave::getInstance()->getHeroId();
	g_hero->setHeadID(heroid);
	//角色名字
	g_hero->setMyName(GlobalData::map_heroAtr[heroid].name);
	//角色等级
	int lv = GameDataSave::getInstance()->getHeroLV();
	g_hero->setLVValue(lv);
	//角色经验值
	int exp = GameDataSave::getInstance()->getHeroExp();
	g_hero->setExpValue(exp);
	//角色是否在家
	g_hero->setIsOut(GameDataSave::getInstance()->getHeroIsOut());
	
	//角色生命值
	int hlife = GameDataSave::getInstance()->getHeroLife();
	if (hlife > -1)//-1 新的角色第一次开始玩
		g_hero->setLifeValue(hlife);
	else
	{
		g_hero->setLifeValue(g_hero->getMaxLifeValue());
	}

	//设置黑夜或白天

	if (g_nature->getTime() < 60 * 6 - 1 || g_nature->getTime() >= 18 * 60)
	{
		g_nature->setDayOrNight(Night);
	}
	else
	{
		g_nature->setDayOrNight(Day);
	}

	//读取保存的仓库数据
	StorageRoom::loadStorageData();
	//读取保存的背包数据
	MyPackage::load();
	//读取保存的资源数据
	GlobalData::loadResData();
	//读取保存的角色属性数据
	loadSavedHeroPropData();
}

void GameScene::loadSavedHeroPropData()
{
	//角色佩戴装备
	std::string strval = GameDataSave::getInstance()->getHeroProperData();
	std::vector<std::string> tmp;
	CommonFuncs::split(strval, tmp, ";");
	//解析
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
		sdata.goodvalue = atoi(tmp2[6].c_str());//耐久度
		sdata.name = tmp2[7];
		sdata.desc = tmp2[8];
		g_hero->setAtrByType((HeroAtrType)i, sdata);
		//g_hero->set [sdata.type].push_back(sdata);
	}
}

void GameScene::onExit()
{
	//退出保存数据
	saveAllData();

	Layer::onExit();
}

void GameScene::saveAllData()
{
	
	GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	GameDataSave::getInstance()->setNatureWeather(g_nature->getWeather());
	GameDataSave::getInstance()->setNatureWeatherChangeCount(g_nature->changeWeatherCount * 10000 + g_nature->changeWeatherRandow);
	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	GameDataSave::getInstance()->setNatureTime(g_nature->getTime());
	GameDataSave::getInstance()->setLiveDays(g_nature->getPastDays());
	GameDataSave::getInstance()->setHeroLife(g_hero->getLifeValue());
	GameDataSave::getInstance()->setHeroOutinjury(g_hero->getOutinjuryValue());
	GameDataSave::getInstance()->setHeroInnerinjury(g_hero->getInnerinjuryValue());
	GameDataSave::getInstance()->setHeroHunger(g_hero->getHungerValue());
	GameDataSave::getInstance()->setHeroSpirit(g_hero->getSpiritValue());
	GameDataSave::getInstance()->setHeroLV(g_hero->getLVValue());
	GameDataSave::getInstance()->setHeroId(g_hero->getHeadID());
	GameDataSave::getInstance()->setHeroExp(g_hero->getExpValue());
	GameDataSave::getInstance()->setHeroIsOut(g_hero->getIsOut());
}

void GameScene::updata(float dt)
{
	//更新资源列表数据
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (data->strid.compare(GlobalData::vec_hillResid[i]) == 0)
			{
				if (data->count <= 0)//采完，等待
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

					if (data->count >= data->max)//产出最大
						data->count = data->max;
				}
			}
		}
	}
}

void GameScene::timerSaveResData(float dt)
{
	GlobalData::saveResData();
	saveAllData();
}

void GameScene::checkiflive(float dt)
{
	//生命为0，死掉，弹出复活界面
	if (g_hero->getLifeValue() <= 0)
	{
		this->unschedule(schedule_selector(GameScene::checkiflive));
		Director::getInstance()->pause();
		ReviveLayer* layer = ReviveLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer, 10);
	}
}

void GameScene::heroRevive()
{
	//复活成功
	g_hero->revive();
	Director::getInstance()->resume();
	this->schedule(schedule_selector(GameScene::checkiflive), 1.0f);
}