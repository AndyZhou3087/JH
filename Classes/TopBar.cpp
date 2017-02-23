#include "TopBar.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "GameScene.h"

TopBar::TopBar()
{
	pastmin = g_natureData->getTime();
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

	std::string str = StringUtils::format("ui/top_r_season%d.png", GameDataSave::getInstance()->getNatureReason());
	reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	wheather = (cocos2d::ui::ImageView*)csbnode->getChildByName("wheather");
	wheather->setName("wheather");
	wheather->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("ui/top_weather%d.png", GameDataSave::getInstance()->getNatureWheather());
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

	str = StringUtils::format("%d", GameDataSave::getInstance()->getLiveDays());
	livedayslbl->setString(str);

	timelbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("timelbl");
	timelbl->setName("time");
	timelbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));
	str = StringUtils::format("%d", GameDataSave::getInstance()->getNatureTime());
	timelbl->setString(str);

	templbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("templbl");
	templbl->setName("temperature");
	templbl->addTouchEventListener(CC_CALLBACK_2(TopBar::onclick, this));

	str = StringUtils::format("%d", GameDataSave::getInstance()->getNatureTemperature());
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
		int livedays = g_heroData->getLiveDays() + 1;

		g_heroData->setLiveDays(livedays);
		str = StringUtils::format("%d", livedays);
		livedayslbl->setString(str);
		g_natureData->ChangeReason(livedays);
		g_natureData->ChangeWheather();

		str = StringUtils::format("ui/top_r_season%d.png", g_natureData->getReason());
		reason->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		reason->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		str = StringUtils::format("ui/top_weather%d.png", g_natureData->getWheather());
		wheather->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		wheather->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		GameDataSave::getInstance()->setLiveDays(livedays);
		GameDataSave::getInstance()->setNatureReason(g_natureData->getReason());
		GameDataSave::getInstance()->setNatureWheather(g_natureData->getWheather());
	}

	GameDataSave::getInstance()->setNatureTemperature(g_natureData->getTemperature());
	g_natureData->setTime(pastmin);
	g_natureData->ChangeDayNight(pastmin);
	str = StringUtils::format("%02d:%02d", pastmin / 60, pastmin % 60);
	timelbl->setString(str);
	str = StringUtils::format("%d", g_natureData->getTemperature());
	templbl->setString(str);
}
