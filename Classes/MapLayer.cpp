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

	m_mapbg = (cocos2d::ui::Widget*)m_mapscroll->getChildByName("mapbg");
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

	Vec2 pos = m_mapbg->getChildren().at(heroposindex)->getPosition();

	mapMoveTo(pos);

	heroPos = m_mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getHeadID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(heroPos);

	m_mapscroll->addChild(m_herohead);

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

	if (g_hero->getAtrByType(H_MOUNT)->count > 0)
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

	m_herohead->runAction(Sequence::create(MoveTo::create(dt / (TIMESCALE * 8.0f), m_destPos), CallFunc::create(CC_CALLBACK_0(MapLayer::Arrive, this)), NULL));

}

void MapLayer::Arrive()
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
		if (g_hero->getAtrByType(H_MOUNT)->count > 0)
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

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue <= 0)
			{
				PackageData data;
				data.count = -1;
				g_hero->setAtrByType(H_MOUNT, data);
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("你的白马累死了！！"), 25, Color3B(204, 4, 4));
			}
		}
	}
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(GoWhereLayer::create(m_addrname, ARRIVE), 2);
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
}

void MapLayer::updataPlotMissionIcon()
{
	
	std::string snpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc;
	std::string dnpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].dnpc;

	m_smissionIcon->setVisible(false);
	m_smissionIcon->setVisible(false);

	int mapnamecount = GlobalData::map_maps.size();
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

//此方法无法实现地图移动到任务地点的功能
void MapLayer::mapScrollPlotAddr(Vec2 pos)
{
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = m_mapscroll->getContentSize();
	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	if (pos.x + scollviewsize.width / 2 > m_mapscroll->getInnerContainerSize().width)
		offsetx = m_mapscroll->getInnerContainerSize().width - scollviewsize.width / 2;
	if (pos.y + scollviewsize.height / 2 > m_mapscroll->getInnerContainerSize().height)
		offsety = m_mapscroll->getInnerContainerSize().height - scollviewsize.height / 2;

	float h = m_mapscroll->getInnerContainerSize().height - m_mapscroll->getContentSize().height;
	float w = m_mapscroll->getInnerContainerSize().width - m_mapscroll->getContentSize().width;

	float percentx = offsetx * 100 / w;
	float percenty = offsety * 100 / h;
	m_mapscroll->scrollToPercentBothDirection(Vec2(percentx, percenty), 1, false);
}


void MapLayer::showPlotAddr()
{
	std::string plotaddr;

	PlotMissionData *mdata = &GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()];
	std::string snpc = mdata->snpc;
	std::string dnpc = mdata->dnpc;

	if (mdata->status == M_NONE && !mdata->isshowsnpc)
	{
		mdata->isshowsnpc = true;
		int mapnamecount = GlobalData::map_maps.size();
		for (int i = 0; i < mapnamecount; i++)
		{
			cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
			for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
			{
				if (snpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
				{
					mapScrollPlotAddr(mapname->getPosition());
					return;
				}
			}
		}
	}
	else if (mdata->status == M_DOING && !mdata->isshowdnpc)
	{
		mdata->isshowdnpc = true;
		int mapnamecount = GlobalData::map_maps.size();
		for (int i = 0; i < mapnamecount; i++)
		{
			cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
			for (unsigned int m = 0; m < GlobalData::map_maps[mapname->getName()].npcs.size(); m++)
			{
				if (dnpc.compare(GlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
				{
					mapScrollPlotAddr(mapname->getPosition());
					return;
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