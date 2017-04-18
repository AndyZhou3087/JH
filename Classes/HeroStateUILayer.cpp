#include "HeroStateUILayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "HeroProperNode.h"
#include "GameScene.h"
#include "Hero.h"

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
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);

	Node* heroproper = HeroProperNode::create();
	heroproper->setPosition(Vec2(360, 810));
	m_csbnode->addChild(heroproper);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onBack, this));

	for (int i = 0; i < 11; i++)
	{
		std::string str = StringUtils::format("herostate%d", i);
		herostatus[i] = (cocos2d::ui::Text*)m_csbnode->getChildByName(str);
	}
	this->schedule(schedule_selector(HeroStateUILayer::updateStatus),0.2f);
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

void HeroStateUILayer::updateStatus(float dt)
{
	int index = 0;
	int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
	for (int i = 0; i < 7; i++)
	{
		if (g_hero->getHungerValue() >= valuerange[i])
		{
			index = i;
			break;
		}
	}
	herostatus[0]->setString(CommonFuncs::gbk2utf(hungerdesc1[index].c_str()));

	index = 0;
	for (int i = 0; i < 7; i++)
	{
		if (g_hero->getInnerinjuryValue() >= valuerange[i])
		{
			index = i;
			break;
		}
	}
	herostatus[1]->setString(CommonFuncs::gbk2utf(innerInjurydesc1[index].c_str()));

	index = 0;
	for (int i = 0; i < 7; i++)
	{
		if (g_hero->getOutinjuryValue() >= valuerange[i])
		{
			index = i;
			break;
		}
	}

	herostatus[2]->setString(CommonFuncs::gbk2utf(outInjurydesc1[index].c_str()));

	index = 0;
	for (int i = 0; i < 7; i++)
	{
		if (g_hero->getSpiritValue() >= valuerange[i])
		{
			index = i;
			break;
		}
	}
	herostatus[3]->setString(CommonFuncs::gbk2utf(spiritInjurydesc1[index].c_str()));

	herostatus[4]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_GATHER).count > 0 ? "快速":"一般"));
	herostatus[5]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_FELL).count > 0 ? "快速" : "一般"));
	herostatus[6]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_EXCAVATE).count > 0 ? "快速" : "一般"));
	herostatus[7]->setString(CommonFuncs::gbk2utf("一般"));
	std::string str = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
	herostatus[8]->setString(str);

	str = StringUtils::format("%d", g_hero->getAtkValue());
	herostatus[9]->setString(str);

	str = StringUtils::format("%d", g_hero->getDfValue());
	herostatus[10]->setString(str);
}