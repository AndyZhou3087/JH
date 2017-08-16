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
#include "NewerGuideLayer.h"
#include "ActivitScene.h"
#include "Const.h"
#include "HeroProperNode.h"
#include "MyParticle.h"
#include "GetVipRewardLayer.h"
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
	g_gameLayer = NULL;
	g_nature = NULL;
	g_hero = NULL;
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

	//读取后山资源列表配置文件
	GlobalData::loadHillResJsonData();

	//读取地图配置文件
	GlobalData::loadMapJsonData();

	//读取NPC配置文件
	GlobalData::loadNpcJsonData();

	//读取武功招式配置文件
	GlobalData::loadGftrickData();

	//读取技能配置文件
	GlobalData::loadGfskillData();

	//读取商城数据
	GlobalData::loadShopData();

	//加载武道大会挑战奖励数据
	GlobalData::loadChallengeRewardData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	GlobalData::g_gameStatus = GAMESTART;
	//天气，季节数据
	g_nature = Nature::create();
	this->addChild(g_nature);

	loadSaveNatureData();
	//角色数据
	g_hero = Hero::create();
	this->addChild(g_hero);

	loadSaveHeroData();

	//读取剧情配置文件
	GlobalData::loadPlotMissionJsonData(g_hero->getHeadID());

	//读取支线剧情配置文件
	GlobalData::loadBranchPlotMissionJsonData();

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
	g_uiScroll = UIScroll::create(620.0F, 140.0f);
	g_uiScroll->setPosition(Vec2(visibleSize.width / 2 + 25, 964));
	addChild(g_uiScroll, 4);

	//任务属性和天气
	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	addChild(topBar, 3, "topbar");
	
	this->schedule(schedule_selector(GameScene::updata), 1.0f);
	this->schedule(schedule_selector(GameScene::timerSaveData), 5.0f);
	this->schedule(schedule_selector(GameScene::checkiflive), 0.1f);
	
	//处理制暖
	std::string warmConfigStr = GlobalData::getMakeWarmConfig();
	if (warmConfigStr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		CommonFuncs::split(warmConfigStr, vec_retstr, "-");
		int startwarmtime = atoi(vec_retstr[0].c_str());
		int warmtime = atoi(vec_retstr[1].c_str());
		int curtimemin = g_nature->getPastDays() * 24 * 60 + g_nature->getTime();
		if (curtimemin - startwarmtime < warmtime)
		{
			g_nature->setIsMaKeWarm(true);
			float wpasttime = curtimemin - startwarmtime;
			g_nature->setWarmtime(warmtime);
			g_nature->setWarmpasttime(wpasttime);
		}
		else
		{
			g_nature->setTemperature(g_nature->getTemperature() - 15);
			GlobalData::setMakeWarmConfig("");
		}
	}
	ServerDataSwap::getInstance()->setDelegate(this);
	ServerDataSwap::getInstance()->vipIsOn();
    return true;
}

void GameScene::loadSaveNatureData()
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
	
	//设置黑夜或白天

	if (g_nature->getTime() < 60 * 6 - 1 || g_nature->getTime() >= 18 * 60)
	{
		g_nature->setDayOrNight(Night);
	}
	else
	{
		g_nature->setDayOrNight(Day);
	}
}

void GameScene::loadSaveHeroData()
{
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
	float hlife = GameDataSave::getInstance()->getHeroLife();
	if (hlife > -1)//-100 新的角色第一次开始玩
		g_hero->setLifeValue(hlife);
	else
	{
		g_hero->setLifeValue(g_hero->getMaxLifeValue());
	}

	int sex = GameDataSave::getInstance()->getHeroSex();
	if (sex < 0)
	{
		if (heroid != 4)
			g_hero->setSex(S_MAN);
		else
			g_hero->setSex(S_WOMEN);
	}
	else
		g_hero->setSex((H_SEX)sex);

	//读取保存的仓库数据
	StorageRoom::loadStorageData();
	//读取保存的背包数据
	MyPackage::load();
	//读取保存的资源数据
	GlobalData::loadResData();
	//读取保存的角色属性数据
	loadSavedHeroPropData();

	getNpcRandMap();

	GlobalData::loadTempGF_EquipData();
	GlobalData::loadNpcFriendJsonData();
	GlobalData::loadNpcMasterJsonData();
	GlobalData::loadFriendly();
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
		sdata.extype = GlobalData::getResExType(sdata.strid);//atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());
		sdata.exp = atoi(tmp2[5].c_str());
		sdata.goodvalue = atoi(tmp2[6].c_str());//耐久度
		if (tmp2.size() >= 9)
		{
			sdata.slv = atoi(tmp2[7].c_str());//
			sdata.tqu = atoi(tmp2[8].c_str());//
		}

		if (sdata.type == WEAPON || sdata.type == PROTECT_EQU)
		{
			std::vector<PackageData>::iterator it;
			for (it = StorageRoom::map_storageData[sdata.type].begin(); it != StorageRoom::map_storageData[sdata.type].end();)
			{
				if (it->strid.compare(sdata.strid) == 0 && sdata.count > 0)
				{
					sdata.goodvalue += it->goodvalue;
					if (it->slv < sdata.slv)
						it->slv = sdata.slv;
					if (it->lv < sdata.lv)
						it->lv = sdata.lv;
					it = StorageRoom::map_storageData[sdata.type].erase(it);
				}
				else
					++it;
			}
		}
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

	//保存资源数据
	GlobalData::saveResData();
	//保存角色装备栏数据
	HeroProperNode::saveData();
}

void GameScene::updata(float dt)
{
	GlobalData::setTimeGiftLeftTime(GlobalData::getTimeGiftLeftTime() - 1);
	if (GlobalData::g_gameStatus != GAMESTART)
		return;

	if (g_hero != NULL && g_hero->getIsWDChallenge())
		return;

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
					data->pastmin = 0.0f;
					data->waittime += g_nature->getTimeInterval();
					if (data->waittime >= data->speed[g_nature->getReason()] * data->max)
					{
						data->count = data->max;
						data->waittime = 0.0f;
					}
				}
				else
				{
					data->pastmin += g_nature->getTimeInterval();
					if (data->pastmin >= data->speed[g_nature->getReason()])
					{
						int pcount = data->pastmin / data->speed[g_nature->getReason()];
						data->count += pcount;

						float leftmin = data->pastmin - (data->speed[g_nature->getReason()] * pcount);
						data->pastmin = leftmin;
	
					}

					if (data->count >= data->max)//产出最大
					{
						data->pastmin = 0;
						data->count = data->max;
					}
				}
			}
		}
	}
}

void GameScene::timerSaveData(float dt)
{
	saveAllData();
}

void GameScene::checkiflive(float dt)
{
	//生命为0，死掉，弹出复活界面
	if (g_hero != NULL && g_hero->getLifeValue() <= 0.0f && !GlobalData::isPopingScene)
	{
		this->unschedule(schedule_selector(GameScene::checkiflive));
		topBar->stopLoseAnim();
		GlobalData::g_gameStatus = GAMEPAUSE;
		
		if (g_hero->getIsOut() && g_maplayer != NULL)
		{
			g_maplayer->heroPauseMoving();
		}

		ReviveLayer* layer = ReviveLayer::create();
		g_gameLayer->addChild(layer, 10, "revivelayer");
	}
}

void GameScene::heroRevive()
{
	//复活成功
	g_hero->revive();
	topBar->stopLoseAnim();
	GlobalData::g_gameStatus = GAMESTART;
	if (g_maplayer != NULL && g_hero->getIsOut())
		g_maplayer->heroResumeMoving();
	this->schedule(schedule_selector(GameScene::checkiflive), 0.1f);
}

void GameScene::delayShowOutScence(float dt)
{
	Scene* scene = ActivitScene::createScene("images/cout.jpg", CommonFuncs::gbk2utf("出门..."));
	if (scene != NULL)
	{
		auto transition = TransitionCrossFade::create(0.5f, scene);
		Director::getInstance()->pushScene(transition);
	}
	this->scheduleOnce(schedule_selector(GameScene::showGOOut), 0.1f);
}

void GameScene::showGOOut(float dt)
{
	g_maplayer = MapLayer::create();
	g_gameLayer->addChild(g_maplayer, 1, "maplayer");
	g_gameLayer->removeChildByName("homelayer");
	g_hero->setIsOut(true);
	if (g_maplayer != NULL)
		g_maplayer->scheduleOnce(schedule_selector(MapLayer::delayShowMapNewerGuide), 0.2f);
}

void GameScene::getNpcRandMap()
{
	int pos = GameDataSave::getInstance()->getWxbMapPos();
	GlobalData::setWxbMapPos(pos);
	GlobalData::map_maps[wxbinmap[pos]].npcs.push_back("n012");

	pos = GameDataSave::getInstance()->getDgqbMapPos();
	GlobalData::setDgqbMapPos(pos);
	GlobalData::map_maps[dgqbinmap[pos]].npcs.push_back("n091");
}

void GameScene::showNewerGuide(int step, std::vector<Node*> nodes)
{
	if (NewerGuideLayer::checkifNewerGuide(step))
	{
		m_newerStep = step;
		m_newerNode = nodes;
		this->scheduleOnce(schedule_selector(GameScene::delayShowNewerGuide),0.05f);
	}
}

void GameScene::delayShowNewerGuide(float dt)
{
	if (g_NewerGuideLayer == NULL)
	{
		g_NewerGuideLayer = NewerGuideLayer::create(m_newerStep, m_newerNode);
		if (g_gameLayer != NULL)
			g_gameLayer->addChild(g_NewerGuideLayer, 10);
	}
}

void GameScene::onSuccess()
{
	if (GlobalData::vec_buyVipIds.size() > 0)
	{
		GetVipRewardLayer* layer = GetVipRewardLayer::create();
		if (g_gameLayer != NULL)
			g_gameLayer->addChild(layer, 10);
	}
}

void GameScene::onErr(int errcode)
{

}