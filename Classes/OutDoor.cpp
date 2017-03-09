#include "OutDoor.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "MapLayer.h"

OutDoor::OutDoor()
{

}


OutDoor::~OutDoor()
{
}

bool OutDoor::init()
{

	m_csbnode = CSLoader::createNode("outDoorLayer.csb");
	m_csbnode->setPosition(Vec2(0, -100));
	this->addChild(m_csbnode);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onBack, this));


	cocos2d::ui::Button* outbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("outbtn");
	outbtn->addTouchEventListener(CC_CALLBACK_2(OutDoor::onOut, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void OutDoor::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void OutDoor::onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->getRunningScene()->addChild(MapLayer::create());
		this->getParent()->removeFromParentAndCleanup(true);

	}
}