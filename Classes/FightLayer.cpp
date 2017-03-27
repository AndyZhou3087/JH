#include "FightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"

FightLayer::FightLayer()
{
}


FightLayer::~FightLayer()
{
}

FightLayer* FightLayer::create(std::string addrname, std::string npcname)
{
	FightLayer *pRet = new FightLayer();
	if (pRet && pRet->init(addrname, npcname))
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

bool FightLayer::init(std::string addrname, std::string npcname)
{
	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(addrname);

	cocos2d::ui::Text* npcnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcnametxt->setString(npcname);
	
	m_escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	m_fihgtScorll = UIScroll::create(650.0f, 450.0f);
	m_fihgtScorll->setPosition(Vec2(37, 128));
	csbnode->addChild(m_fihgtScorll);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (node->getTag() == 0)
		{

		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}
