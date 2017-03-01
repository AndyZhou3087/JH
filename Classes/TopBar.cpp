#include "TopBar.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "SysSmallBox.h"

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

	weather = (cocos2d::ui::ImageView*)csbnode->getChildByName("weather");
	weather->setName("weather");
	weather->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("ui/top_weather%d.png", g_nature->getWeather());
	weather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	weather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

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
		SysSmallBox* sbox = NULL;
		if (cnode->getName().compare("hero") == 0)
		{
			m_uiscroll->addEventText(CommonFuncs::gbk2utf("点击了hero"));
		}
		else if (cnode->getName().compare("reason") == 0)
		{
			int rv = g_nature->getReason();
			std::string str = StringUtils::format("ui/top_r_season%d.png", rv);
			sbox = SysSmallBox::create(BoxType::REASON, str, reasonname[rv], reasondesc1[rv], reasondesc[rv]);
		}
		else if (cnode->getName().compare("weather") == 0)
		{
			int rw = g_nature->getWeather();
			std::string str = StringUtils::format("ui/top_weather%d.png", rw);
			sbox = SysSmallBox::create(BoxType::WEATHER, str, weathername[rw], weatherdesc1[rw], weatherdesc[rw]);
		}
		else if (cnode->getName().compare("livedays") == 0)
		{
			std::string str = StringUtils::format("%d天", g_nature->getPastDays());
			sbox = SysSmallBox::create(BoxType::LIVEDAYS, "ui/toplivedaysicon.png", str,"", liveDayDesc);

		}
		else if (cnode->getName().compare("time") == 0)
		{
			std::string str = StringUtils::format("%02d:%02d", g_nature->getTime() / 60, g_nature->getTime() % 60);
			sbox = SysSmallBox::create(BoxType::TIME, "ui/toptimeicon.png", str, "", timeDesc);
		}
		else if (cnode->getName().compare("temperature") == 0)
		{
			std::string str = StringUtils::format("%d℃", g_nature->getTemperature());
			sbox = SysSmallBox::create(BoxType::TEMPERATURE, "ui/toptemperature.png", str, "", "");
		}
		else if (cnode->getName().compare("outinjury") == 0)
		{
			std::string str = "外伤";
			sbox = SysSmallBox::create(BoxType::OUTERINJURY, "ui/topoutinjurybg.png", str, "", "");
		}
		else if (cnode->getName().compare("innerinjury") == 0)
		{
			std::string str = "内伤";
			sbox = SysSmallBox::create(BoxType::OUTERINJURY, "ui/topinnerinjurybg.png", str, "", "");
		}
		else if (cnode->getName().compare("hunger") == 0)
		{
			std::string str = "饱食度";
			sbox = SysSmallBox::create(BoxType::HUNGER, "ui/tophungerbg.png", str, "", "");
		}
		else if (cnode->getName().compare("spirit") == 0)
		{
			std::string str = "精神";
			sbox = SysSmallBox::create(BoxType::SPIRIT, "ui/topspiritbg.png", str, "", "");
		}
		else if (cnode->getName().compare("life") == 0)
		{
			std::string str = "生命";
			sbox = SysSmallBox::create(BoxType::LIFE, "ui/toplifebg.png", str, "", "");
		}
		if (sbox != NULL)
			this->getParent()->addChild(sbox, 2);
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

		str = StringUtils::format("ui/top_weather%d.png", g_nature->getWeather());
		weather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		weather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		GameDataSave::getInstance()->setLiveDays(livedays);
		GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
		GameDataSave::getInstance()->setNatureWeather(g_nature->getWeather());
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
