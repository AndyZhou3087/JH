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

static Vec2 heroPos;

MapLayer* g_maplayer = NULL;
MapLayer::MapLayer()
{
}


MapLayer::~MapLayer()
{
}

bool MapLayer::init()
{
	Node* csbnode = CSLoader::createNode("mapLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ScrollView* mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	cocos2d::ui::Widget* mapbg = (cocos2d::ui::Widget*)mapscroll->getChildByName("mapbg");
	int mapnamecount = mapbg->getChildrenCount();
	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)mapbg->getChildren().at(i);
		mapname->addTouchEventListener(CC_CALLBACK_2(MapLayer::onclick, this));

	}
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = mapscroll->getContentSize();
	Vec2 pos = mapbg->getChildren().at(0)->getPosition();

	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));

	m_distance = 0.0f;
	std::string addr = GameDataSave::getInstance()->getHeroAddr();
	heroPos = mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(heroPos);

	mapscroll->addChild(m_herohead);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		m_addrname = node->getName();
		m_destPos = node->getPosition();
		m_distance = fabsf(heroPos.distance(m_destPos));
		WHERELAYER_TYPE type = ARRIVE;
		if (m_distance <= 1.0f)
			type = ARRIVE;
		else
			type = GOWHERE;
		Director::getInstance()->getRunningScene()->addChild(GoWhereLayer::create(m_addrname, type, m_distance));

	}
}

void MapLayer::showMoveToDest()
{
	float dt = m_distance / HERO_MOVE_SPEED;
	float permin = dt / (TIMESCALE*4.0f);
	g_nature->setTimeInterval(permin);
	this->scheduleOnce(schedule_selector(MapLayer::Arrive), 4.0f);
	m_herohead->runAction(MoveTo::create(4.0f, m_destPos));
}

void MapLayer::Arrive(float dt)
{
	g_nature->setTimeInterval(1);
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
	Director::getInstance()->getRunningScene()->addChild(GoWhereLayer::create(m_addrname, ARRIVE));
}