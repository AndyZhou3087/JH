#include "MapLayer.h"
#include "json.h"
#include "HomeLayer.h"
#include "HomeHill.h"
#include "GoWhereLayer.h"
#include "GameDataSave.h"
#include "Const.h"
#include "GameScene.h"

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
	Vec2 hpos = GameDataSave::getInstance()->getHeroPos();
	if (hpos.isZero())
		heroPos = mapbg->getChildByName("m1-1")->getPosition();
	else
		heroPos = hpos;
	herohead = Sprite::createWithSpriteFrameName("ui/herohead1.png");
	herohead->setAnchorPoint(Vec2(0.5, 0));
	herohead->setPosition(heroPos);
	mapscroll->addChild(herohead);

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
		addrname = node->getName();
		destPos = node->getPosition();
		m_distance = fabsf(heroPos.distance(destPos));
		WHERELAYER_TYPE type = ARRIVE;
		if (m_distance <= 1.0f)
			type = ARRIVE;
		else
			type = GOWHERE;
		Director::getInstance()->getRunningScene()->addChild(GoWhereLayer::create(addrname, type, m_distance));

	}
}

void MapLayer::showMoveToDest()
{
	float dt = m_distance / HERO_MOVE_SPEED;
	float permin = dt / (TIMESCALE*4.0f);
	g_nature->setTimeInterval(permin);
	this->scheduleOnce(schedule_selector(MapLayer::Arrive), 4.0f);
	herohead->runAction(MoveTo::create(4.0f, destPos));
}

void MapLayer::Arrive(float dt)
{
	g_nature->setTimeInterval(1);
	heroPos = destPos;
	GameDataSave::getInstance()->setHeroPos(destPos);

	Director::getInstance()->getRunningScene()->addChild(GoWhereLayer::create(addrname, ARRIVE));
}