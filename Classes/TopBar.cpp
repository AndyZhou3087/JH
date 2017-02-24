#include "TopBar.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"

TopBar::TopBar()
{
	pastmin = g_nature->getTime();
}


TopBar::~TopBar()
{
}

bool TopBar::init()
{
	Node* csbnode = CSLoader::createNode("topBarNode.csb");
	this->addChild(csbnode);

	heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	heroimg->setName("hero");
	heroimg->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	reason = (cocos2d::ui::ImageView*)csbnode->getChildByName("reason");
	reason->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	reason->setName("reason");

	std::string str = StringUtils::format("ui/top_r_season%d.png", g_nature->getReason());
	reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	wheather = (cocos2d::ui::ImageView*)csbnode->getChildByName("wheather");
	wheather->setName("wheather");
	wheather->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("ui/top_weather%d.png", g_nature->getWheather());
	wheather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	wheather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::ImageView* livedaysincon = (cocos2d::ui::ImageView*)csbnode->getChildByName("livedaysincon");
	livedaysincon->setName("livedays");
	livedaysincon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* timeicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptimeicon");
	timeicon->setName("time");
	timeicon->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* temperature = (cocos2d::ui::ImageView*)csbnode->getChildByName("toptemperature");
	temperature->setName("temperature");
	temperature->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	livedayslbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("livedayslbl");
	livedayslbl->setName("livedays");
	livedayslbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("%d", g_nature->getPastDays());
	livedayslbl->setString(str);

	timelbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("timelbl");
	timelbl->setName("time");
	timelbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	str = StringUtils::format("%d", g_nature->getTime());
	timelbl->setString(str);

	templbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("templbl");
	templbl->setName("temperature");
	templbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("%d", g_nature->getTemperature());
	templbl->setString(str);

	cocos2d::ui::ImageView* outinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topoutinjurybg");
	outinjury->setName("outinjury");
	outinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* innerinjury = (cocos2d::ui::ImageView*)csbnode->getChildByName("topinnerinjurybg");
	innerinjury->setName("innerinjury");
	innerinjury->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* hunger = (cocos2d::ui::ImageView*)csbnode->getChildByName("tophungerbg");
	hunger->setName("hunger");
	hunger->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* spirit = (cocos2d::ui::ImageView*)csbnode->getChildByName("topspiritbg");
	spirit->setName("spirit");
	spirit->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	cocos2d::ui::ImageView* life = (cocos2d::ui::ImageView*)csbnode->getChildByName("toplifebg");
	life->setName("life");
	life->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	Sprite* sprite0 = Sprite::createWithSpriteFrameName("ui/topoutinjurybar.png");
	outinjuryBar = ProgressTimer::create(sprite0);
	outinjuryBar->setType(ProgressTimer::Type::BAR);
	outinjuryBar->setBarChangeRate(ccp(0, 1));
	outinjuryBar->setMidpoint(ccp(0, 0));
	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	outinjuryBar->setPosition(outinjury->getPosition());
	this->addChild(outinjuryBar);

	Sprite* sprite1 = Sprite::createWithSpriteFrameName("ui/topinnerinjurybar.png");
	innerinjuryBar = ProgressTimer::create(sprite1);
	innerinjuryBar->setType(ProgressTimer::Type::BAR);
	innerinjuryBar->setBarChangeRate(ccp(0, 1));
	innerinjuryBar->setMidpoint(ccp(0, 0));
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	innerinjuryBar->setPosition(innerinjury->getPosition());
	this->addChild(innerinjuryBar);

	Sprite* sprite2 = Sprite::createWithSpriteFrameName("ui/tophungerbar.png");
	hungerBar = ProgressTimer::create(sprite2);
	hungerBar->setType(ProgressTimer::Type::BAR);
	hungerBar->setBarChangeRate(ccp(0, 1));
	hungerBar->setMidpoint(ccp(0, 0));
	hungerBar->setPercentage(g_hero->getHungerValue());
	hungerBar->setPosition(hunger->getPosition());
	this->addChild(hungerBar);

	Sprite* sprite3 = Sprite::createWithSpriteFrameName("ui/topspiritbar.png");
	spiritBar = ProgressTimer::create(sprite3);
	spiritBar->setType(ProgressTimer::Type::BAR);
	spiritBar->setBarChangeRate(ccp(0, 1));
	spiritBar->setMidpoint(ccp(0, 0));
	spiritBar->setPercentage(g_hero->getSpiritValue());
	spiritBar->setPosition(spirit->getPosition());
	this->addChild(spiritBar);

	Sprite* sprite4 = Sprite::createWithSpriteFrameName("ui/toplifebar.png");
	lifeBar = ProgressTimer::create(sprite4);
	lifeBar->setType(ProgressTimer::Type::BAR);
	lifeBar->setBarChangeRate(ccp(0, 1));
	lifeBar->setMidpoint(ccp(0, 0));
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());
	lifeBar->setPosition(life->getPosition());
	this->addChild(lifeBar);

	schedule(schedule_selector(TopBar::updataUI), 0.2f);
	return true;
}

void TopBar::setScrollContainer(UIScroll* uiscroll)
{
	m_uiscroll = uiscroll;
}

void TopBar::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* cnode = (Node*)pSender;
		if (cnode->getName().compare("hero") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了hero"));
		}
		else if (cnode->getName().compare("reason") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了季节"));
		}
		else if (cnode->getName().compare("wheather") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了天气"));
		}
		else if (cnode->getName().compare("livedays") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了生存天数"));
		}
		else if (cnode->getName().compare("time") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了时间"));
		}
		else if (cnode->getName().compare("temperature") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了温度"));
		}
		else if (cnode->getName().compare("outinjury") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了外伤"));
		}
		else if (cnode->getName().compare("innerinjury") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了内伤"));
		}
		else if (cnode->getName().compare("hunger") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了饥饿"));
		}
		else if (cnode->getName().compare("spirit") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了精神"));
		}
		else if (cnode->getName().compare("life") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了生命值"));
		}

	}
}

void TopBar::updataUI(float dt)
{
	std::string str;
	pastmin += 10;
	if (pastmin >= 1440)
	{
		pastmin = 0;
		int livedays = g_nature->getPastDays();
		str = StringUtils::format("%d", livedays);
		livedayslbl->setString(str);

		str = StringUtils::format("ui/top_r_season%d.png", g_nature->getReason());
		reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		str = StringUtils::format("ui/top_weather%d.png", g_nature->getWheather());
		wheather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		wheather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		GameDataSave::getInstance()->setLiveDays(livedays);
		GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
		GameDataSave::getInstance()->setNatureWheather(g_nature->getWheather());
	}

	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	str = StringUtils::format("%02d:%02d", pastmin / 60, pastmin % 60);
	timelbl->setString(str);
	str = StringUtils::format("%d", g_nature->getTemperature());
	templbl->setString(str);

	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	hungerBar->setPercentage(g_hero->getHungerValue());
	spiritBar->setPercentage(g_hero->getSpiritValue());
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());
}
