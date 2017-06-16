#include "HeroStateUILayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "HeroProperNode.h"
#include "GameScene.h"
#include "Hero.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "PauseLayer.h"

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

	HeroProperNode* heroproper = HeroProperNode::create();
	heroproper->setPosition(Vec2(360, 810));
	m_csbnode->addChild(heroproper);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onBack, this));

	cocos2d::ui::Button* pausebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("pausebtn");
	pausebtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onPause, this));

	for (int i = 0; i < sizeof(herostatus) / sizeof(herostatus[0]); i++)
	{
		std::string str = StringUtils::format("herostate%d", i);
		herostatus[i] = (cocos2d::ui::Text*)m_csbnode->getChildByName(str);
	}
	updateStatus(0);
	//////layer 点击事件，屏蔽下层事件
	this->schedule(schedule_selector(HeroStateUILayer::updateStatus), 1.0f);
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
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);

		TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
		if (topbar != NULL)
			topbar->showNewerGuide(13);
	}
}

void HeroStateUILayer::onPause(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Director::getInstance()->pause();
		this->addChild(PauseLayer::create(), 2);
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
	//饥饿属性
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
	//内伤属性
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
	//外伤属性
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
	//精神属性
	herostatus[3]->setString(CommonFuncs::gbk2utf(spiritInjurydesc1[index].c_str()));
	//采集速度
	herostatus[4]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_GATHER)->count > 0 ? "快速":"一般"));
	//砍伐速度
	herostatus[5]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_FELL)->count > 0 ? "快速" : "一般"));
	//挖掘速度
	herostatus[6]->setString(CommonFuncs::gbk2utf(g_hero->getAtrByType(H_EXCAVATE)->count > 0 ? "快速" : "一般"));

	herostatus[7]->setString(CommonFuncs::gbk2utf("一般"));

	//生命值属性
	std::string str = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), GlobalData::map_heroAtr[g_hero->getHeadID()].vec_maxhp[g_hero->getLVValue()]);
	herostatus[8]->setString(str);


	str = StringUtils::format("%d", g_hero->getTotalAtck());
	herostatus[9]->setString(str);

	int df = g_hero->getTotalDf();

	str = StringUtils::format("%d", g_hero->getTotalDf());
	herostatus[10]->setString(str);

	//经验值属性
	str = StringUtils::format("%d", g_hero->getExpValue());
	herostatus[11]->setString(str);
	//等级属性
	str = StringUtils::format("%d", g_hero->getLVValue() + 1);
	herostatus[12]->setString(str);
}

void HeroStateUILayer::onExit()
{
	Layer::onExit();
}

void HeroStateUILayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	nodes.push_back(m_csbnode->getChildByName("backbtn"));
	g_gameLayer->showNewerGuide(step, nodes);
}