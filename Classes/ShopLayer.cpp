#include "ShopLayer.h"
#include "Const.h"
#include "BuildingUILayer.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "SelectHeroScene.h"
#include "SoundManager.h"
#include "ReviveLayer.h"
#include "json.h"
#include "AnalyticUtil.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#include "RmbGoodsItem.h"
#include "GoldGoodsItem.h"
#include "GetVipRewardLayer.h"
#include "MapLayer.h"
#include "ServerDataSwap.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif

int ShopLayer::payindex = -1;
bool ShopLayer::isPaying = false;
ShopLayer::ShopLayer()
{
}


ShopLayer::~ShopLayer()
{

	GlobalData::g_gameStatus = GAMESTART;
	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroResumeMoving();
	}
}

ShopLayer* ShopLayer::create()
{
	ShopLayer *pRet = new ShopLayer();
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

bool ShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("shopLayer.csb");
	this->addChild(m_csbnode);

	refreshGoldCount(0);

	std::vector<GoodsData*> vec_rmbGoods;
	std::vector<GoodsData*> vec_goldGoods;

	int goodsize = GlobalData::vec_goods.size();

	for (int i = 0; i < goodsize; i++)
	{
		GoodsData* gdata = &GlobalData::vec_goods[i];
		if (gdata->type == 0)
			vec_rmbGoods.push_back(gdata);
		else if (gdata->type == 1)
			vec_goldGoods.push_back(gdata);
	}


	m_rmbScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("rmbgoodsscroll");
	m_rmbScrollview->setScrollBarEnabled(false);
	m_rmbScrollview->setBounceEnabled(true);

	int itemwidth = 160;
	int innerwidth = itemwidth * vec_rmbGoods.size();
	int contentwidth = m_rmbScrollview->getContentSize().width;
	if (innerwidth < contentwidth)
		innerwidth = contentwidth;
	m_rmbScrollview->setInnerContainerSize(Size(innerwidth, m_rmbScrollview->getContentSize().height));

	for (unsigned int i = 0; i < vec_rmbGoods.size(); i++)
	{
		RmbGoodsItem* node = RmbGoodsItem::create(vec_rmbGoods[i]);

		node->setTag(sizeof(heroprice) / sizeof(heroprice[0]) + i);
		m_rmbScrollview->addChild(node);
		node->setPosition(Vec2(itemwidth / 2  + 10 + i * itemwidth, m_rmbScrollview->getContentSize().height/2));
	}

	m_goldScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("goldgoodsscroll");
	m_goldScrollview->setScrollBarEnabled(false);
	m_goldScrollview->setBounceEnabled(true);

	int itemheight = 205;
	int innerheight = itemheight * vec_goldGoods.size();
	int contentheight = m_goldScrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_goldScrollview->setInnerContainerSize(Size(m_goldScrollview->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < vec_goldGoods.size(); i++)
	{
		GoldGoodsItem* node = GoldGoodsItem::create(vec_goldGoods[i]);
		m_goldScrollview->addChild(node);
		node->setPosition(Vec2(360, innerheight - itemheight / 2 - i * itemheight));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onBack, this));

	cocos2d::ui::Widget* qq = (cocos2d::ui::Widget*)m_csbnode->getChildByName("qq");
	qq->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onQQ, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (GlobalData::g_gameStatus == GAMESTART)
		GlobalData::g_gameStatus = GAMEPAUSE;

	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroPauseMoving();
	}

	this->schedule(schedule_selector(ShopLayer::refreshGoldCount), 1);
	return true;
}

void ShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}



void ShopLayer::beginPay(int index)
{
	if (isPaying)
		return;

	isPaying = true;
	payindex = index;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//payCode
	buy((char*)payCode[payindex].c_str());
#endif
}
void ShopLayer::setMessage(PYARET ret)
{
	if (ret == PAY_SUCC)
	{
		int herocount = sizeof(heroprice) / sizeof(heroprice[0]);
		int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);
		int vipcount = sizeof(vipgoldcount) / sizeof(vipgoldcount[0]);
		if (payindex < herocount) // 人物解锁
		{
			if (g_SelectHeroScene != NULL)
				g_SelectHeroScene->unlockSucc(payindex);
#ifdef ANALYTICS
			std::string heroname[] = { "bxym", "bssy", "bjxb", "baq" };
			AnalyticUtil::onEvent(heroname[payindex].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount)//买元宝
		{
			//addBuyGoods();
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() + goldcount[payindex - herocount]);
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUYOK);
#ifdef ANALYTICS
			std::string name[] = { "b6", "b12", "b30", "b68"};
			AnalyticUtil::onEvent(name[payindex - 4].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount + vipcount)//买VIP
		{
			GlobalData::vec_buyVipIds.push_back(GlobalData::vec_goods[payindex - herocount].icon);

			ServerDataSwap::getInstance()->setDelegate(NULL);
			ServerDataSwap::getInstance()->vipSuccNotice(GlobalData::vec_goods[payindex - herocount].icon);
			GetVipRewardLayer* layer = GetVipRewardLayer::create();
			if(g_gameLayer != NULL)
				g_gameLayer->addChild(layer, 10);
		}
#ifdef ANALYTICS
		AnalyticUtil::pay("pay", buyprice[payindex], 1);
#endif
	}
	isPaying = false;
}

void ShopLayer::refreshGoldCount(float dt)
{
	mygoldlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("mygoldlbl");
	std::string countstr = StringUtils::format("%d", GlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void ShopLayer::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		copytoclipboard((char*)qq->getString().c_str());
#endif
	}

}
