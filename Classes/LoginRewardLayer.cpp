#include "LoginRewardLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "Const.h"
#include "StorageRoom.h"

LoginRewardLayer::LoginRewardLayer()
{

}


LoginRewardLayer::~LoginRewardLayer()
{

}

bool LoginRewardLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("loginRewardLayer.csb");
	this->addChild(csbnode);
	int logindays = GlobalData::continueLoginDays;


	for (int i = 1; i<= 7; i++)
	{
		std::string resid = LOGINREWARDS[i - 1];
		int res = atoi(resid.c_str());
		int count = 1;

		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}

		std::string str = StringUtils::format("item%d", i);
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);

		cocos2d::ui::ImageView* getimg = (cocos2d::ui::ImageView*)item->getChildByName("getimg");
		getimg->addTouchEventListener(CC_CALLBACK_2(LoginRewardLayer::onGet, this));

		cocos2d::ui::Text* statutext = (cocos2d::ui::Text*)getimg->getChildByName("statutext");

		cocos2d::ui::ImageView* rwdboximg = (cocos2d::ui::ImageView*)item->getChildByName("rwdboximg");
		cocos2d::ui::ImageView* rwdimg = (cocos2d::ui::ImageView*)item->getChildByName("rwdimg");

		cocos2d::ui::Text* name = (cocos2d::ui::Text*)item->getChildByName("name");

		std::string namsstr;
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			namsstr = StringUtils::format("%sx%d", CommonFuncs::gbk2utf("金元宝").c_str(), count);
			resid = "gd0";
		}

		else
			namsstr = StringUtils::format("%sx%d", GlobalData::map_allResource[resid].cname.c_str(), count);
		name->setString(namsstr.c_str());

		str = "ui/buildsmall.png";
		int type = GlobalData::getResType(resid);
		if (type == WEAPON || type == PROTECT_EQU)
		{
			str = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[resid].qu);
		}
		else if (type == N_GONG || type == W_GONG)
		{
			str = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[resid].qu);
		}
		rwdboximg->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);

		str = StringUtils::format("ui/%s.png", resid.c_str());
		rwdimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
		

		if (i < logindays)
		{
			item->loadTexture("ui/loginritem0.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/disnpcbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->setEnabled(false);
			statutext->setString(CommonFuncs::gbk2utf("已领取"));
			cocos2d::ui::Scale9Sprite* _9sprite = (cocos2d::ui::Scale9Sprite*)rwdboximg->getVirtualRenderer();
			CommonFuncs::changeGray(_9sprite->getSprite());
			_9sprite = (cocos2d::ui::Scale9Sprite*)rwdimg->getVirtualRenderer();
			CommonFuncs::changeGray(_9sprite->getSprite());
		}
		else if (i == logindays)
		{
			item->loadTexture("ui/loginritem1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/loginrbtn.png", cocos2d::ui::Widget::TextureResType::PLIST);
			statutext->setString(CommonFuncs::gbk2utf("领取"));
		}
		else
		{
			getimg->setEnabled(false);
			const std::string cndays[] = { "一", "二", "三", "四", "五", "六", "七" };
			str = StringUtils::format("第%s天", cndays[i - 1].c_str());
			statutext->setString(CommonFuncs::gbk2utf(str.c_str()));
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

LoginRewardLayer* LoginRewardLayer::create()
{
	LoginRewardLayer *pRet = new LoginRewardLayer();
	if (pRet && pRet->init())
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

void LoginRewardLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		std::string resid = LOGINREWARDS[GlobalData::continueLoginDays - 1];
		int res = atoi(resid.c_str());
		int count = 1;
		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + count);

		}
		else
		{
			PackageData pdata;
			pdata.strid = resid;
			pdata.count = count;
			pdata.type = GlobalData::getResType(resid);
			pdata.extype = GlobalData::getResExType(resid);
			StorageRoom::add(pdata);
		}

		GlobalData::continueLoginDays = 0;
		this->removeFromParentAndCleanup(true);
	}
}