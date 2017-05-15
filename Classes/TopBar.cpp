#include "TopBar.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "SysSmallBox.h"
#include "HeroStateUILayer.h"
#include "SoundManager.h"
#include "Const.h"
#include "ActivitScene.h"

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
	std::string heroidstr = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);
	heroimg->setContentSize(Sprite::createWithSpriteFrameName(heroidstr)->getContentSize());

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

	int hour = g_nature->getTime() / 60;
	int minute = (int)g_nature->getTime() % 60;
	str = StringUtils::format("%02d:%02d", hour, minute);

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
	outinjuryBar->setBarChangeRate(Vec2(0, 1));
	outinjuryBar->setMidpoint(Vec2(0, 0));
	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	outinjuryBar->setPosition(outinjury->getPosition());
	csbnode->addChild(outinjuryBar);

	Sprite* sprite1 = Sprite::createWithSpriteFrameName("ui/topinnerinjurybar.png");
	innerinjuryBar = ProgressTimer::create(sprite1);
	innerinjuryBar->setType(ProgressTimer::Type::BAR);
	innerinjuryBar->setBarChangeRate(Vec2(0, 1));
	innerinjuryBar->setMidpoint(Vec2(0, 0));
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	innerinjuryBar->setPosition(innerinjury->getPosition());
	csbnode->addChild(innerinjuryBar);

	Sprite* sprite2 = Sprite::createWithSpriteFrameName("ui/tophungerbar.png");
	hungerBar = ProgressTimer::create(sprite2);
	hungerBar->setType(ProgressTimer::Type::BAR);
	hungerBar->setBarChangeRate(Vec2(0, 1));
	hungerBar->setMidpoint(Vec2(0, 0));
	hungerBar->setPercentage(g_hero->getHungerValue());
	hungerBar->setPosition(hunger->getPosition());
	csbnode->addChild(hungerBar);

	Sprite* sprite3 = Sprite::createWithSpriteFrameName("ui/topspiritbar.png");
	spiritBar = ProgressTimer::create(sprite3);
	spiritBar->setType(ProgressTimer::Type::BAR);
	spiritBar->setBarChangeRate(Vec2(0, 1));
	spiritBar->setMidpoint(Vec2(0, 0));
	spiritBar->setPercentage(g_hero->getSpiritValue());
	spiritBar->setPosition(spirit->getPosition());
	csbnode->addChild(spiritBar);

	Sprite* sprite4 = Sprite::createWithSpriteFrameName("ui/toplifebar.png");
	lifeBar = ProgressTimer::create(sprite4);
	lifeBar->setType(ProgressTimer::Type::BAR);
	lifeBar->setBarChangeRate(Vec2(0, 1));
	lifeBar->setMidpoint(Vec2(0, 0));
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());
	lifeBar->setPosition(life->getPosition());
	csbnode->addChild(lifeBar);
	outinjuryRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topoutinjuryred");
	outinjuryRed->setLocalZOrder(1);
	innerinjuryRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topinnerinjuryred");
	innerinjuryRed->setLocalZOrder(1);
	hungerRed = (cocos2d::ui::Widget*)csbnode->getChildByName("tophungerred");
	hungerRed->setLocalZOrder(1);
	spiritRed = (cocos2d::ui::Widget*)csbnode->getChildByName("topspiritred");
	spiritRed->setLocalZOrder(1);
	lifeRed = (cocos2d::ui::Widget*)csbnode->getChildByName("toplifered");
	lifeRed->setLocalZOrder(1);

	m_lastinnerinjury = g_hero->getInnerinjuryValue();
	m_lastoutinjury = g_hero->getOutinjuryValue();
	m_lasthunger = g_hero->getHungerValue();
	m_lastspirit = g_hero->getSpiritValue();
	m_lastlife = g_hero->getLifeValue();

	m_lastweather = g_nature->getWeather();

	m_lastDayOrNigth = g_nature->getDayOrNight();

	schedule(schedule_selector(TopBar::updataUI), NORMAL_TIMEINTERVAL * 1.0f/TIMESCALE);
	return true;
}

void TopBar::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* cnode = (Node*)pSender;
		SysSmallBox* sbox = NULL;
		if (cnode->getName().compare("hero") == 0)
		{
			Director::getInstance()->getRunningScene()->addChild(HeroStateUILayer::create());
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
			int hour = g_nature->getTime() / 60;
			int minute = (int)g_nature->getTime() % 60;
			std::string str = StringUtils::format("%02d:%02d", hour, minute);
			sbox = SysSmallBox::create(BoxType::TIME, "ui/toptimeicon.png", str, "", timeDesc);
		}
		else if (cnode->getName().compare("temperature") == 0)
		{
			std::string str = StringUtils::format("%d℃", g_nature->getTemperature());
			sbox = SysSmallBox::create(BoxType::TEMPERATURE, "ui/toptemperature.png", str, "", tempeDesc);
		}
		else if (cnode->getName().compare("outinjury") == 0)
		{
			std::string str = "外伤";
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getOutinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}
			sbox = SysSmallBox::create(BoxType::OUTERINJURY, "ui/topoutinjurybg.png", str, outInjurydesc1[index], outInjurydesc);
		}
		else if (cnode->getName().compare("innerinjury") == 0)
		{
			std::string str = "内伤";

			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getInnerinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			sbox = SysSmallBox::create(BoxType::INNERINJURY, "ui/topinnerinjurybg.png", str, innerInjurydesc1[index], innerInjurydesc);
		}
		else if (cnode->getName().compare("hunger") == 0)
		{
			std::string str = "饱食度";

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

			sbox = SysSmallBox::create(BoxType::HUNGER, "ui/tophungerbg.png", str, hungerdesc1[index], hungerdesc);
		}
		else if (cnode->getName().compare("spirit") == 0)
		{
			std::string str = "精神";


			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getSpiritValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			sbox = SysSmallBox::create(BoxType::SPIRIT, "ui/topspiritbg.png", str, spiritInjurydesc1[index], spiritInjurydesc);
		}
		else if (cnode->getName().compare("life") == 0)
		{
			std::string str = "生命";
			std::string livevaluestr = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
			sbox = SysSmallBox::create(BoxType::LIFE, "ui/toplifebg.png", str, livevaluestr, lifedesc);
		}
		if (sbox != NULL)
			Director::getInstance()->getRunningScene()->addChild(sbox, 4);
	}
}

void TopBar::updataUI(float dt)
{
	std::string str;
	pastmin += g_nature->getTimeInterval();
	if (pastmin >= 1440.0f)
	{
		pastmin = 0.0f;
		int livedays = g_nature->getPastDays();
		str = StringUtils::format("%d", livedays);
		livedayslbl->setString(str);

		str = StringUtils::format("ui/top_r_season%d.png", g_nature->getReason());
		reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		GameDataSave::getInstance()->setLiveDays(livedays);
		GameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	}

	if (m_lastweather != g_nature->getWeather())
	{
		str = StringUtils::format("ui/top_weather%d.png", g_nature->getWeather());
		weather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		weather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		if (g_nature->getWeather() == Rainy)
		{
			auto transition = TransitionCrossFade::create(1.0f, ActivitScene::createScene("images/cweatherrain.jpg", CommonFuncs::gbk2utf("下雨了...")));
			Director::getInstance()->pushScene(transition);
		}
		else if (g_nature->getWeather() == Snowy)
		{
			auto transition = TransitionCrossFade::create(1.0f, ActivitScene::createScene("images/cweathersnow.jpg", CommonFuncs::gbk2utf("下雪了...")));
			Director::getInstance()->pushScene(transition);
		}
		m_lastweather = g_nature->getWeather();
	}

	if (m_lastDayOrNigth != g_nature->getDayOrNight())
	{
		if (g_nature->getDayOrNight() == Day)
		{
			auto transition = TransitionCrossFade::create(1.0f, ActivitScene::createScene("images/cday.jpg", CommonFuncs::gbk2utf("天亮了...")));
			Director::getInstance()->pushScene(transition);
		}
		else
		{
			auto transition = TransitionCrossFade::create(1.0f, ActivitScene::createScene("images/cday.jpg", CommonFuncs::gbk2utf("黑夜降临...")));
			Director::getInstance()->pushScene(transition);
		}
		m_lastDayOrNigth = g_nature->getDayOrNight();
	}

	GameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	int hour = pastmin / 60;
	int minute = (int)pastmin % 60;

	str = StringUtils::format("%02d:%02d", hour, minute);
	timelbl->setString(str);
	str = StringUtils::format("%d", g_nature->getTemperature());
	templbl->setString(str);

	outinjuryBar->setPercentage(g_hero->getOutinjuryValue());
	innerinjuryBar->setPercentage(g_hero->getInnerinjuryValue());
	hungerBar->setPercentage(g_hero->getHungerValue());
	spiritBar->setPercentage(g_hero->getSpiritValue());
	lifeBar->setPercentage(g_hero->getLifeValue() * 100.0f / g_hero->getMaxLifeValue());

	if (m_lastinnerinjury > g_hero->getInnerinjuryValue())
	{
		innerinjuryRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(10)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastinnerinjury = g_hero->getInnerinjuryValue();
	}
	if (m_lastoutinjury > g_hero->getOutinjuryValue())
	{
		outinjuryRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(10)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastoutinjury = g_hero->getOutinjuryValue();
	}
	if (m_lasthunger > g_hero->getHungerValue())
	{
		hungerRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(10)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lasthunger = g_hero->getHungerValue();
	}
	if (m_lastspirit > g_hero->getSpiritValue())
	{
		spiritRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(10)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastspirit = g_hero->getSpiritValue();
	}
	if (m_lastlife > g_hero->getLifeValue())
	{
		lifeRed->runAction(Sequence::create(DelayTime::create(GlobalData::createRandomNum(10)), Show::create(), Blink::create(2.0f, 3), Hide::create(), NULL));
		m_lastlife = g_hero->getLifeValue();
	}
}
