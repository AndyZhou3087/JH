#include "SysSmallBox.h"
#include "CommonFuncs.h"
#include "GameScene.h"
SysSmallBox::SysSmallBox()
{
}


SysSmallBox::~SysSmallBox()
{
}

bool SysSmallBox::init(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text)
{
	mType = type;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("sysSmallLayer.csb");
	this->addChild(csbnode);
	image = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	titleTxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title1Txt = (cocos2d::ui::Text*)csbnode->getChildByName("title1");
	textTxt = (cocos2d::ui::Text*) csbnode->getChildByName("text");

	image->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	image->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());
	image->setScale(1.5f);
	titleTxt->setString(CommonFuncs::gbk2utf(title.c_str()));
	title1Txt->setString(CommonFuncs::gbk2utf(title1.c_str()));
	textTxt->setString(CommonFuncs::gbk2utf(text.c_str()));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		this->schedule(schedule_selector(SysSmallBox::removeself), 0.05f);
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(SysSmallBox::updataUI), 0.2f);
	return true;
}

SysSmallBox* SysSmallBox::create(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text)
{
	SysSmallBox *pRet = new SysSmallBox();
	if (pRet && pRet->init(type, imagepath, title, title1, text))
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

void SysSmallBox::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void SysSmallBox::updataUI(float dt)
{
	if (mType == REASON)
	{
		int v = g_nature->getReason();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getReason();
		}
		std::string str = StringUtils::format("ui/top_r_season%d.png", v);

		image->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		image->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		image->setScale(1.5f);

		titleTxt->setString(CommonFuncs::gbk2utf(reasonname[v].c_str()));
		title1Txt->setString(CommonFuncs::gbk2utf(reasondesc1[v].c_str()));
		textTxt->setString(CommonFuncs::gbk2utf(reasondesc[v].c_str()));
	}
	if (mType == WEATHER)
	{
		int v = g_nature->getWeather();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getWeather();
			std::string str = StringUtils::format("ui/top_weather%d.png", v);

			image->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			image->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
			image->setScale(1.5f);

			titleTxt->setString(CommonFuncs::gbk2utf(weathername[v].c_str()));
			title1Txt->setString(CommonFuncs::gbk2utf(weatherdesc1[v].c_str()));
			textTxt->setString(CommonFuncs::gbk2utf(weatherdesc[v].c_str()));
		}
	}

	if (mType == LIVEDAYS)
	{
		int v = g_nature->getPastDays();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getPastDays();
			std::string str = StringUtils::format("%d天", g_nature->getPastDays());
			titleTxt->setString(CommonFuncs::gbk2utf(str.c_str()));
		}

	}

	if (mType == TIME)
	{
		int v = g_nature->getTime();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getTime();
			int hour = lastvalue / 60;
			int minute = (int)lastvalue % 60;
			std::string str = StringUtils::format("%02d:%02d", hour, minute);
			titleTxt->setString(CommonFuncs::gbk2utf(str.c_str()));
		}

	}

	if (mType == TEMPERATURE)
	{
		int v = g_nature->getTemperature();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getTemperature();
			std::string str = StringUtils::format("%d℃", g_nature->getTemperature());
			titleTxt->setString(CommonFuncs::gbk2utf(str.c_str()));
		}

	}
}
