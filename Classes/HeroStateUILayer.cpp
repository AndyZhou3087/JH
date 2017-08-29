﻿#include "HeroStateUILayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "HeroProperNode.h"
#include "GameScene.h"
#include "Hero.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "PauseLayer.h"
#include "GameDataSave.h"
#include "MapLayer.h"
#include "MixGFNode.h"

HeroStateUILayer::HeroStateUILayer()
{

}


HeroStateUILayer::~HeroStateUILayer()
{
}

bool HeroStateUILayer::init()
{

	m_csbnode = CSLoader::createNode("heroStateLayer.csb");
	this->addChild(m_csbnode, 0, "csbnode");

	HeroProperNode* heroproper = HeroProperNode::create();
	heroproper->setPosition(Vec2(360, 770));
	m_csbnode->addChild(heroproper, 0, "HeroProperNode");

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onBack, this));

	cocos2d::ui::Button* pausebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("pausebtn");
	pausebtn->addTouchEventListener(CC_CALLBACK_2(HeroStateUILayer::onPause, this));

	for (int i = 0; i < sizeof(herostatus) / sizeof(herostatus[0]); i++)
	{
		std::string str = StringUtils::format("herostate%d", i + 1);
		herostatus[i] = (cocos2d::ui::Text*)m_csbnode->getChildByName(str);
	}

	m_heroexptimelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("heroexptime");
	m_gfexptimelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("gfexptime");

	m_heroexpendtime = GameDataSave::getInstance()->getHeroExpEndTime();
	m_gfexpendtime = GameDataSave::getInstance()->getGfEndTime();

	updateStatus(0);

	MixGFNode* mixnode = MixGFNode::create();
	mixnode->setPosition(Vec2(360, 220));
	m_csbnode->addChild(mixnode, 0, "mixnode");
	if (g_hero->getIsOut())
		mixnode->setVisible(false);

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
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);

		TopBar* topbar = (TopBar*)g_gameLayer->getChildByName("topbar");
		if (topbar != NULL)
			topbar->showNewerGuide(13);
	}
}

void HeroStateUILayer::onPause(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::g_gameStatus = GAMEPAUSE;
		if (g_hero->getIsOut() && g_maplayer != NULL)
		{
			g_maplayer->heroPauseMoving();
		}
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

	//攻击属性
	std::string str = StringUtils::format("%d", g_hero->getTotalAtck());
	herostatus[0]->setString(str);

	int df = g_hero->getTotalDf();

	//防御属性
	str = StringUtils::format("%d", g_hero->getTotalDf());
	herostatus[1]->setString(str);

	//经验值属性
	str = StringUtils::format("%d/%d", g_hero->getExpValue(), GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp[g_hero->getLVValue()]);
	herostatus[2]->setString(str);
	//等级属性
	str = StringUtils::format("%d", g_hero->getLVValue() + 1);
	int lvmax = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp.size();
	if (g_hero->getLVValue() + 1 >= lvmax)
		str = StringUtils::format("%d%s", lvmax, CommonFuncs::gbk2utf("（满级）").c_str());
	herostatus[3]->setString(str);

	//生命值属性
	str = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
	herostatus[4]->setString(str);

	std::map<std::string, FriendlyData>::iterator it;
	for (it = GlobalData::map_myfriendly.begin(); it != GlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (GlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			herostatus[5]->setString(GlobalData::map_npcs[nid].name);
			break;
		}
	}
	//性别
	if (g_hero->getSex() == S_NONE)
	{
		m_csbnode->getChildByName("herostatetext6")->setVisible(true);
		herostatus[6]->setVisible(true);
		herostatus[6]->setString(CommonFuncs::gbk2utf("不详"));
	}

	int letftime1 = m_heroexpendtime - GlobalData::getSysSecTime();
	if (letftime1 > 0)
	{
		std::string str;
		int day = letftime1 / 86400;
		int sectime = letftime1 % 86400;
		if (day > 0)
			str = StringUtils::format("经验药水效果剩%d天%02d:%02d:%02d", day, sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		else
			str = StringUtils::format("经验药水效果剩%02d:%02d:%02d", sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		m_heroexptimelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		m_heroexptimelbl->setVisible(true);
	}
	else
	{
		m_heroexptimelbl->setVisible(false);
	}

	int letftime2 = m_gfexpendtime - GlobalData::getSysSecTime();
	if (letftime2 > 0)
	{
		std::string str;
		int day = letftime2 / 86400;
		int sectime = letftime2 % 86400;
		if (day > 0)
			str = StringUtils::format("大力丸效果剩%d天%02d:%02d:%02d", day, sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		else
			str = StringUtils::format("大力丸效果剩%02d:%02d:%02d", sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		m_gfexptimelbl->setString(CommonFuncs::gbk2utf(str.c_str()));
		m_gfexptimelbl->setVisible(true);
	}
	else
	{
		m_gfexptimelbl->setVisible(false);
	}
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