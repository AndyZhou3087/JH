#include "GoWhereLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "HomeHill.h"
#include "HomeLayer.h"
#include "MapLayer.h"
#include "GameScene.h"
#include "TempStorageLayer.h"
#include "NpcLayer.h"
#include "SoundManager.h"
#include "GameDataSave.h"
#include "NewerGuideLayer.h"

GoWhereLayer::GoWhereLayer()
{

}


GoWhereLayer::~GoWhereLayer()
{
}

GoWhereLayer* GoWhereLayer::create(std::string addrname, WHERELAYER_TYPE type, float distance)
{
	GoWhereLayer *pRet = new GoWhereLayer();
	if (pRet && pRet->init(addrname, type, distance))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool GoWhereLayer::init(std::string addrid, WHERELAYER_TYPE type, float distance)
{
	Node* csbnode = CSLoader::createNode("goWhereLayer.csb");
	this->addChild(csbnode);

	m_addrstr = addrid;
	m_type = type;

	MapData mdata = GlobalData::map_maps[addrid];

	//名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(mdata.desc);

	//距离 ，换算成时间
	cocos2d::ui::Text* fast = (cocos2d::ui::Text*)csbnode->getChildByName("fast");

	float fmin = distance * HERO_MOVE_SPEED;

	int minute = (int)fmin;
	std::string faststr;
	if (minute / 60 > 0)
		faststr = StringUtils::format("%d小时%d分钟", minute / 60, minute%60);
	else
		faststr = StringUtils::format("%d分钟", minute);
	
	fast->setString(CommonFuncs::gbk2utf(faststr.c_str()));

	cocos2d::ui::Text* fasttitle = (cocos2d::ui::Text*)csbnode->getChildByName("text1");
	
	//地点的类型
	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("ui/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	typeimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onClose, this));

	m_gobtn = (cocos2d::ui::Button*)csbnode->getChildByName("gobtn");
	m_gobtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onGO, this));

	//物品存放
	m_stbtn = (cocos2d::ui::Button*)csbnode->getChildByName("stbtn");
	m_stbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onST, this));

	m_stredpoint = (cocos2d::ui::Widget*)m_stbtn->getChildByName("redpoint");
	checkRedPoint(0);

	m_enterbtn = (cocos2d::ui::Button*)csbnode->getChildByName("enterbtn");
	m_enterbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onComeIn, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(GoWhereLayer::onClose, this));

	if (type == GOWHERE)
	{
		closebtn->setVisible(true);
		m_gobtn->setVisible(true);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(false);
		fast->setVisible(true);
		fasttitle->setVisible(true);
	}
	else
	{
		closebtn->setVisible(false);
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(true);
		m_enterbtn->setVisible(true);
		fast->setVisible(false);
		fasttitle->setVisible(false);
		if (m_addrstr.compare("m1-1") == 0)//家，没有物品存放
		{
			m_stbtn->setVisible(false);
			m_enterbtn->setPositionX(360);
		}
	}
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(GoWhereLayer::checkRedPoint), 1.0f);
	return true;
}


void GoWhereLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	this->scheduleOnce(schedule_selector(GoWhereLayer::delayShowNewerGuide), 0.1f);
}

void GoWhereLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
	}
}

void GoWhereLayer::onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
		if (g_maplayer != NULL)//地图上移动角色头像
			g_maplayer->showMoveToDest();
	}
}

void GoWhereLayer::onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		TempStorageLayer* layer = TempStorageLayer::create(m_addrstr);

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(layer, 2, "TempStorageLayer");
	}
}

void GoWhereLayer::onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (m_addrstr.compare("m1-1") == 0)//进入家
		{
			g_gameLayer->addChild(HomeLayer::create(), 1, "homelayer");

		}
		else if (m_addrstr.compare("m1-2") == 0)//进入后山
		{
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(HomeHill::create(), 2, "homehill");
		}
		else//进入NPC
		{
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(NpcLayer::create(m_addrstr), 2, "npclayer");
		}
		this->removeFromParentAndCleanup(true);
	}
}



void GoWhereLayer::checkRedPoint(float dt)
{
	if (GameDataSave::getInstance()->getTempStorage(m_addrstr).length() > 0)
		m_stredpoint->setVisible(true);
	else
		m_stredpoint->setVisible(false);
}

void GoWhereLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 21 || step == 41)
		nodes.push_back(m_gobtn);
	else if (step == 22 || step == 42)
		nodes.push_back(m_enterbtn);	
	else if (step == 51)
		nodes.push_back(m_stbtn);
	g_gameLayer->showNewerGuide(step, nodes);
}

void GoWhereLayer::delayShowNewerGuide(float dt)
{
	if (NewerGuideLayer::checkifNewerGuide(21))
		showNewerGuide(21);
	else if (NewerGuideLayer::checkifNewerGuide(22))
		showNewerGuide(22);
	else if (NewerGuideLayer::checkifNewerGuide(41))
		showNewerGuide(41);
	else if (NewerGuideLayer::checkifNewerGuide(42))
		showNewerGuide(42);
	else if (NewerGuideLayer::checkifNewerGuide(51))
	{
		if (m_type == ARRIVE)
		{
			std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrstr);
			std::vector<std::string> vec_retstr;
			CommonFuncs::split(datastr, vec_retstr, ";");
			if (vec_retstr.size() > 0)
				showNewerGuide(51);
		}
	}
}