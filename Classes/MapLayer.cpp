#include "MapLayer.h"
#include "json.h"
#include "HomeLayer.h"
#include "HomeHill.h"
#include "GoWhereLayer.h"
#include "GameDataSave.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "UnlockLayer.h"
#include "NewerGuideLayer.h"
#include "AnalyticUtil.h"

static Vec2 heroPos;

MapLayer* g_maplayer = NULL;
MapLayer::MapLayer()
{
	ismoving = false;
}


MapLayer::~MapLayer()
{
	ismoving = false;
}

bool MapLayer::init()
{
	Node* csbnode = CSLoader::createNode("mapLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ScrollView* mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	mapscroll->setScrollBarEnabled(false);

	m_mapbg = (cocos2d::ui::Widget*)mapscroll->getChildByName("mapbg");
	int mapnamecount = GlobalData::map_maps.size();
	int heroposindex = 0;
	std::string addr = GameDataSave::getInstance()->getHeroAddr();

	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		mapname->addTouchEventListener(CC_CALLBACK_2(MapLayer::onclick, this));
		mapname->setVisible(false);
		if (mapname->getName().compare(addr) == 0)
			heroposindex = i;
	}
	float offsetx = 0.0f;
	float offsety = 0.0f;

	Size scollviewsize = mapscroll->getContentSize();
	Vec2 pos = m_mapbg->getChildren().at(heroposindex)->getPosition();

	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	if (pos.x + scollviewsize.width/2 > mapscroll->getInnerContainerSize().width)
		offsetx = mapscroll->getInnerContainerSize().width - scollviewsize.width/2;
	if (pos.y + scollviewsize.height/2 > mapscroll->getInnerContainerSize().height)
		offsety = mapscroll->getInnerContainerSize().height - scollviewsize.height/2;

	mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));

	m_distance = 0.0f;

	heroPos = m_mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getHeadID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(heroPos);

	mapscroll->addChild(m_herohead);

	updateUnlockChapter();

	m_smissionIcon = Sprite::createWithSpriteFrameName("ui/mapmission0.png");
	m_smissionIcon->setAnchorPoint(Vec2(0.5, 0));
	m_smissionIcon->setVisible(false);
	m_mapbg->addChild(m_smissionIcon);

	m_dmissionIcon = Sprite::createWithSpriteFrameName("ui/mapmission1.png");
	m_dmissionIcon->setAnchorPoint(Vec2(0.5, 0));
	m_dmissionIcon->setVisible(false);
	m_mapbg->addChild(m_dmissionIcon);

	updataPlotMissionIcon();

	cocos2d::ui::Widget* shopbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("shopbtn");
	shopbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onShop, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);

	return true;
}

void MapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (ismoving)
			return;
		Node* node = (Node*)pSender;
		m_addrname = node->getName();
		m_destPos = node->getPosition();
		m_distance = fabsf(heroPos.distance(m_destPos));
		WHERELAYER_TYPE type = ARRIVE;
		if (m_distance <= 1.0f)
			type = ARRIVE;
		else
			type = GOWHERE;

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(GoWhereLayer::create(m_addrname, type, m_distance), 2);

	}
}

void MapLayer::showMoveToDest()
{
	ismoving = true;
	float dt = m_distance * HERO_MOVE_SPEED;
	
	g_nature->setTimeInterval(TIMESCALE * 8);
	this->scheduleOnce(schedule_selector(MapLayer::Arrive), dt / (TIMESCALE * 8.0f));
	m_herohead->runAction(MoveTo::create(dt / (TIMESCALE * 8.0f), m_destPos));
	this->scheduleOnce(schedule_selector(MapLayer::movefinish), dt / (TIMESCALE * 8.0f));
}

void MapLayer::movefinish(float dt)
{
	ismoving = false;
}

void MapLayer::Arrive(float dt)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	heroPos = m_destPos;
	GameDataSave::getInstance()->setHeroAddr(m_addrname);

	if (m_distance > 1.0f)
	{
		std::string npcnames;
		int npcsize = GlobalData::map_maps[m_addrname].npcs.size();
		if (npcsize > 0)
		{
			npcnames.append(CommonFuncs::gbk2utf("这里有"));
			for (int i = 0; i < npcsize; i++)
			{
				npcnames.append(GlobalData::map_npcs[GlobalData::map_maps[m_addrname].npcs[i]].name);
				if (i == npcsize - 1)
					npcnames.append(CommonFuncs::gbk2utf("。"));
				else
					npcnames.append(CommonFuncs::gbk2utf("，"));
			}
		}

		std::string str;
		str.append(CommonFuncs::gbk2utf("你跑得双腿发麻，来到了"));
		str.append(GlobalData::map_maps[m_addrname].cname);
		str.append(CommonFuncs::gbk2utf("，"));
		str.append(GlobalData::map_maps[m_addrname].desc);
		str.append(CommonFuncs::gbk2utf("。"));
		str.append(npcnames);
		g_uiScroll->addEventText(str.c_str());
	}
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(GoWhereLayer::create(m_addrname, ARRIVE), 2);
}

void MapLayer::onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		ShopLayer* shopLayer = ShopLayer::create();
		Director::getInstance()->getRunningScene()->addChild(shopLayer);
	}
}

void MapLayer::updateUnlockChapter()
{
	std::map<std::string, MapData>::iterator it;

	for (it = GlobalData::map_maps.begin(); it != GlobalData::map_maps.end(); ++it)
	{
		std::string mapid = GlobalData::map_maps[it->first].strid;
		cocos2d::ui::Widget* mapNamImage = (cocos2d::ui::Widget*)m_mapbg->getChildByName(mapid);

		std::vector<std::string> tmp;

		CommonFuncs::split(mapid, tmp, "-");
		int mapchapter = atoi(tmp[0].substr(1, tmp[0].size() - 1).c_str());
		if (mapchapter <= GlobalData::getUnlockChapter())
		{
			if (mapNamImage != NULL)
				mapNamImage->setVisible(true);
		}
	}
}

void MapLayer::showUnlockLayer(float dt)
{
	Director::getInstance()->getRunningScene()->addChild(UnlockLayer::create(), 10);
	updateUnlockChapter();

#ifdef ANALYTICS
	std::string unlockstr = StringUtils::format("u%d", GlobalData::getUnlockChapter());
	AnalyticUtil::onEvent(unlockstr.c_str());
#endif
}

void MapLayer::updataPlotMissionIcon()
{
	
	std::string snpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc;
	std::string dnpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].dnpc;
	int mapnamecount = GlobalData::map_maps.size();
	m_smissionIcon->setVisible(false);
	m_smissionIcon->setVisible(false);

	int plotindex = GlobalData::getPlotMissionIndex();
	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
		{
			if (snpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
			{
				if (GlobalData::vec_PlotMissionData[plotindex].status == M_NONE)
				{
					if (GlobalData::vec_PlotMissionData[plotindex].words.size() <= 0)
					{
						GlobalData::vec_PlotMissionData[plotindex].status = M_DOING;
					}
					else
					{
						m_smissionIcon->setVisible(true);
						m_smissionIcon->runAction(RepeatForever::create(Blink::create(2, 3)));
						m_smissionIcon->setPosition(mapname->getPosition());
					}
				}
				else
				{
					m_smissionIcon->stopAllActions();
					m_smissionIcon->setVisible(false);
				}
			}
		}
	}
	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
		{
			if (dnpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
			{
				if (GlobalData::vec_PlotMissionData[plotindex].status == M_DOING)
				{
					m_dmissionIcon->setVisible(true);
					m_dmissionIcon->runAction(RepeatForever::create(Blink::create(2, 3)));
					m_dmissionIcon->setPosition(mapname->getPosition());
				}
				else
				{
					m_dmissionIcon->stopAllActions();
					m_dmissionIcon->setVisible(false);
				}
			}
		}
	}
}


void MapLayer::delayShowMapNewerGuide(float dt)
{
	if (NewerGuideLayer::checkifNewerGuide(20))
		showNewerGuide(20);
	else if (NewerGuideLayer::checkifNewerGuide(40))
		showNewerGuide(40);
	else if (NewerGuideLayer::checkifNewerGuide(48))
		showNewerGuide(48);
}

void MapLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 20)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-2"));
	}
	else if (step == 40)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-1"));
	}
	else if (step == 48)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-4"));
	}
	g_gameLayer->showNewerGuide(step, nodes);
}