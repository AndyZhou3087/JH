#include "ReviveLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "DeathLayer.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "Const.h"
#ifdef UMENG_SHARE
#include "UmengShare/Common/CCUMSocialSDK.h"
USING_NS_UM_SOCIAL;
#include "iosfunc.h"
#endif
#define REVIVEGOODSID 14
bool ReviveLayer::isBuyRevive = false;

ReviveLayer::ReviveLayer()
{
	isBuyRevive = false;
}


ReviveLayer::~ReviveLayer()
{
	isBuyRevive = false;
}

bool ReviveLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("reviveLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("cacelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onCancel, this));

	cocos2d::ui::Text* sharetext = (cocos2d::ui::Text*)m_csbnode->getChildByName("sharetext");

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("revivebtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onRevive, this));
#ifdef UMENG_SHARE
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onCancel, this));

	cocos2d::ui::Button* sharebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("sharebtn");
	sharebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onShare, this));

	if (GlobalData::getDayOfYear() != GlobalData::getShareDay())
	{
		closebtn->setVisible(true);
		cancelbtn->setVisible(false);
		sharetext->setVisible(true);
		sharebtn->setVisible(true);
	}
	else
	{
		closebtn->setVisible(false);
		cancelbtn->setVisible(true);
		sharebtn->setVisible(false);
		sharetext->setVisible(false);
	}
#else
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onCancel, this));

	cocos2d::ui::Button* freeRevivebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("freerevivebtn");
	freeRevivebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onFreeRevive, this));

	if (GlobalData::getDayOfYear() != GlobalData::getShareDay())
	{
		closebtn->setVisible(true);
		cancelbtn->setVisible(false);
		sharetext->setString(CommonFuncs::gbk2utf("（每天免费复活一次！）"));
		sharetext->setVisible(true);
		freeRevivebtn->setVisible(true);
	}
	else
	{
		closebtn->setVisible(false);
		cancelbtn->setVisible(true);
		freeRevivebtn->setVisible(false);
		sharetext->setVisible(false);
	}
#endif


	revivecount = StorageRoom::getCountById("73");

	for (unsigned int i = 0; i < MyPackage::vec_packages.size(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			revivecount += MyPackage::vec_packages[i].count;
			break;
		}
	}

	std::string strcount = StringUtils::format("x%d", revivecount);

	cocos2d::ui::Text* revivecountlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("revivecount");
	revivecountlbl->setString(strcount);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

ReviveLayer* ReviveLayer::create()
{
	ReviveLayer *pRet = new ReviveLayer();
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
#ifdef UMENG_SHARE
void ReviveLayer::onShare(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		CCUMSocialSDK *sdk = CCUMSocialSDK::create();
		vector<int>* platforms = new vector<int>();
		//platforms->push_back(QZONE);
		//platforms->push_back(QQ);
		//platforms->push_back(WEIXIN);
		platforms->push_back(WEIXIN_CIRCLE);
        sdk->openShare(platforms, "来自分享面板", "良心独立游戏！装备全靠刷！刚刷出自宫神功就被打死了！快来帮我！", "Icon-87.png", 
		"https://itunes.apple.com/cn/app/%E6%AD%A6%E6%9E%97%E7%BE%A4%E4%BE%A0%E4%BC%A0-%E9%AB%98%E8%87%AA%E7%94%B1%E5%BA%A6%E6%AD%A6%E4%BE%A0%E5%85%BB%E6%88%90%E6%B8%B8%E6%88%8F/id1243387739?mt=8", 
		share_selector(ReviveLayer::shareCallback));
        //UMShare();
	}
}

/*
* 分享回调
* @param platform 要分享到的目标平台
* @param stCode 返回码, 200代表分享成功, 100代表开始分享
* @param errorMsg 分享失败时的错误信息,android平台没有错误信息
*/
void ReviveLayer::shareCallback(int platform, int stCode, string& errorMsg) {

	log("#### share callback!!!!!! stCode = %d", stCode);

	string result = "";
	if (stCode == 200) {
		result = "分享成功";

		reviveOk();
		GlobalData::setShareDay(GlobalData::getDayOfYear());
	}
	else if (stCode == -1) {
		result = "分享取消";
	}
	else {
		result = "分享失败";
	}

}
#endif

void ReviveLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_gameLayer != NULL)
		{
			g_gameLayer->removeChildByName("fightlayer");
			g_gameLayer->saveAllData();
		}
		Director::getInstance()->resume();
		DeathLayer* layer = DeathLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}

void ReviveLayer::onFreeRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		reviveOk();
		GlobalData::setShareDay(GlobalData::getDayOfYear());
	}
}

void ReviveLayer::onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (revivecount > 0)
		{
			doRevive();
		}
		else
		{
			isBuyRevive = true;
			ShopLayer::beginPay(REVIVEGOODSID);
		}
	}
}

void ReviveLayer::reviveOk()
{
	if (g_gameLayer != NULL)
	{
		g_gameLayer->removeChildByName("revivelayer");
		g_gameLayer->removeChildByName("fightlayer");

		g_gameLayer->heroRevive();
	}
}
void ReviveLayer::doRevive()
{
	reviveOk();
	for (unsigned int i = 0; i < MyPackage::vec_packages.size(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			MyPackage::cutone("73");
			return;
		}
	}

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < StorageRoom::map_storageData[it->first].size(); i++)
		{
			if (StorageRoom::map_storageData[it->first][i].strid.compare("73") == 0)
			{
				StorageRoom::use("73");
				return;
			}
		}
	}
}