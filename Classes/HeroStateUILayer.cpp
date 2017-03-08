#include "HeroStateUILayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "HeroProperNode.h"

const std::string name[] = { "食物", "药材", "武器", "防具", "内功", "外功", "资源1", "资源2"};

HeroStateUILayer::HeroStateUILayer()
{

}


HeroStateUILayer::~HeroStateUILayer()
{
}

bool HeroStateUILayer::init()
{

	m_csbnode = CSLoader::createNode("heroStateLayer.csb");
	m_csbnode->setPosition(Vec2(0, -100));
	this->addChild(m_csbnode);

	Node* heroproper = HeroProperNode::create();
	heroproper->setPosition(Vec2(360, 810));
	m_csbnode->addChild(heroproper);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onBack, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void HeroStateUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}