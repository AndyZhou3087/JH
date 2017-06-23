﻿#include "MapLayer.h"
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
#include "FightLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "UnityAdsMana.h"
#endif

MapLayer* g_maplayer = NULL;
MapLayer::MapLayer()
{
	ismoving = false;
	m_isDraging = false;
	m_distance = 0.0f;
}


MapLayer::~MapLayer()
{
	ismoving = false;
}

bool MapLayer::init()
{
	Node* csbnode = CSLoader::createNode("mapLayer.csb");
	this->addChild(csbnode);

	m_mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	m_mapscroll->setScrollBarEnabled(false);
	m_mapscroll->setSwallowTouches(false);

	m_mapbg = (cocos2d::ui::Widget*)m_mapscroll->getChildByName("mapbg");

	int mapnamecount = GlobalData::map_maps.size();
	int heroposindex = 0;
	std::string addr = GameDataSave::getInstance()->getHeroAddr();

	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		mapname->addTouchEventListener(CC_CALLBACK_2(MapLayer::onclick, this));
		mapname->setSwallowTouches(false);
		mapname->setVisible(false);
		if (mapname->getName().compare(addr) == 0)
			heroposindex = i;
	}

	Vec2 pos = m_mapbg->getChildren().at(heroposindex)->getPosition();

	mapMoveTo(pos);

	m_heroPos = m_mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getHeadID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(m_heroPos);

	m_mapscroll->addChild(m_herohead);

	updateUnlockChapter();

	for (int i = 0; i < 2; i++)
	{
		std::string missionstr = StringUtils::format("ui/mapmission%d_0.png", i);
		m_smissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		m_smissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_smissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_smissionIcon[i]);

		missionstr = StringUtils::format("ui/mapmission%d_1.png", i);
		m_dmissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		m_dmissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_dmissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_dmissionIcon[i]);
		updataPlotMissionIcon(i);
	}

	cocos2d::ui::Widget* shopbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("shopbtn");
	shopbtn->addTouchEventListener(CC_CALLBACK_2(MapLayer::onShop, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		m_isDraging = false;
		m_startClickX = touch->getLocation().x;
		m_startClickY = touch->getLocation().y;
		return true;
	};

	listener->onTouchMoved = [=](Touch *touch, Event *event)
	{
		if (fabsf(m_startClickX - touch->getLocation().x) > 20 || fabsf(m_startClickY - touch->getLocation().y) > 20)
			m_isDraging = true;
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
		if (m_isDraging || ismoving)
		{
			return;
		}
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		Node* node = (Node*)pSender;
		m_addrname = node->getName();
		m_destPos = node->getPosition();
		m_distance = fabsf(m_heroPos.distance(m_destPos));
		WHERELAYER_TYPE type = ARRIVE;
		if (m_distance <= 1.0f)
			type = ARRIVE;
		else
			type = GOWHERE;

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(GoWhereLayer::create(m_addrname, type, m_distance), 2);

	}
}

void MapLayer::heroPauseMoving()
{
	//g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	//m_herohead->pause();
}

void MapLayer::heroResumeMoving()
{
	//if (ismoving)
	//{
	//	g_nature->setTimeInterval(TIMESCALE * 8);
	//	m_herohead->resume();
	//}
}

void MapLayer::showRobberFight(float dt)
{
	//heroPauseMoving();
	//g_gameLayer->addChild(FightLayer::create(m_addrname, "n001"), 5, "fightlayer");
}

void MapLayer::showMoveToDest()
{
	ismoving = true;
	g_nature->setTimeInterval(TIMESCALE * 8);

	float dt = moveToDestTime(m_distance);

	m_herohead->runAction(Sequence::create(MoveTo::create(dt / (TIMESCALE * 8.0f), m_destPos), CallFunc::create(CC_CALLBACK_0(MapLayer::Arrive, this)), NULL));

	//int sec = dt / (TIMESCALE * 8.0f);
	//if (sec >= 2)
	//{
	//	int r = GlobalData::createRandomNum(100);
	//	int rnd = g_nature->getDayOrNight() == Night ? 30 : 20;

	//	if (r < rnd)
	//	{
	//		int r1 = GlobalData::createRandomNum(sec - 1) + 1;

	//		this->scheduleOnce(schedule_selector(MapLayer::showRobberFight), r1);
	//	}
	//}
}

void MapLayer::Arrive()
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	m_heroPos = m_destPos;
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
		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue> 0)
			str.append(CommonFuncs::gbk2utf("你骑着马儿，一溜烟的来到了")); 
		else
			str.append(CommonFuncs::gbk2utf("你跑得双腿发麻，来到了"));

		str.append(GlobalData::map_maps[m_addrname].cname);
		str.append(CommonFuncs::gbk2utf("，"));
		str.append(GlobalData::map_maps[m_addrname].desc);
		str.append(CommonFuncs::gbk2utf("。"));
		str.append(npcnames);
		g_uiScroll->addEventText(str.c_str());

		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->strid.compare("74") == 0)
		{
			g_hero->getAtrByType(H_MOUNT)->goodvalue--;

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue < 0)
			{
				g_hero->getAtrByType(H_MOUNT)->goodvalue = 0;
			}
			else if (g_hero->getAtrByType(H_MOUNT)->goodvalue == 20 || g_hero->getAtrByType(H_MOUNT)->goodvalue == 10)
			{
				std::string descstr = StringUtils::format("%s%s%d", g_hero->getAtrByType(H_MOUNT)->name.c_str(), CommonFuncs::gbk2utf("生命仅剩").c_str(), g_hero->getAtrByType(H_MOUNT)->goodvalue);

				g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
			}
		}
	}
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(GoWhereLayer::create(m_addrname, ARRIVE), 2);

	this->scheduleOnce(schedule_selector(MapLayer::finishMove), 0.1f);
}

void MapLayer::finishMove(float dt)
{
	ismoving = false;
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!GlobalData::getNoComments())
		alterView();
#endif
}

void MapLayer::updataPlotMissionIcon(int type)
{

	m_smissionIcon[type]->stopAllActions();
	m_smissionIcon[type]->setVisible(false);
	m_dmissionIcon[type]->stopAllActions();
	m_dmissionIcon[type]->setVisible(false);

	PlotMissionData* plotData = NULL;
	int plotindex = 0;
	if (type == 0)
	{
		plotindex = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[plotindex];
	}
	else
	{
		plotindex = GlobalData::getBranchPlotMissionIndex();
		if (GlobalData::vec_BranchPlotMissionData[plotindex].unlockchapter <= GlobalData::getUnlockChapter())
			plotData = &GlobalData::vec_BranchPlotMissionData[plotindex];
	}
	if (plotData != NULL)
	{
		std::string snpc = plotData->snpc;
		std::string dnpc = plotData->dnpc;
		int mapnamecount = GlobalData::map_maps.size();

		for (int i = 0; i < mapnamecount; i++)
		{
			cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
			for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
			{
				if (snpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
				{
					if (plotData->status == M_NONE)
					{
						if (plotData->words.size() <= 0)
						{
							plotData->status = M_DOING;
						}
						else
						{
							m_smissionIcon[type]->setPosition(mapname->getPosition());
							m_smissionIcon[type]->setVisible(true);
							m_smissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						}
					}
					else
					{
						m_smissionIcon[type]->stopAllActions();
						m_smissionIcon[type]->setVisible(false);
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
					if (plotData->status == M_DOING)
					{
						m_dmissionIcon[type]->setVisible(true);
						m_dmissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						m_dmissionIcon[type]->setPosition(mapname->getPosition());
					}
					else
					{
						m_dmissionIcon[type]->stopAllActions();
						m_dmissionIcon[type]->setVisible(false);
					}
				}
			}
		}
	}
}

void MapLayer::mapMoveTo(Vec2 pos)
{
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = m_mapscroll->getContentSize();
	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	if (pos.x + scollviewsize.width / 2 > m_mapscroll->getInnerContainerSize().width)
		offsetx = m_mapscroll->getInnerContainerSize().width - scollviewsize.width;
	if (pos.y + scollviewsize.height / 2 > m_mapscroll->getInnerContainerSize().height)
		offsety = m_mapscroll->getInnerContainerSize().height - scollviewsize.height;

	m_mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));
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

float MapLayer::moveToDestTime(float distance)
{
	float dt = distance * HERO_MOVE_SPEED;
	if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue > 0)
	{
		int index = -1;
		for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
		{
			ResData rdata = GlobalData::vec_resData[i];
			if (rdata.strid.compare(g_hero->getAtrByType(H_MOUNT)->strid) == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			int ep = GlobalData::vec_resData[index].ep[0];
			dt *= 100.0f / (100.0f + ep);
		}
	}
	return dt;
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