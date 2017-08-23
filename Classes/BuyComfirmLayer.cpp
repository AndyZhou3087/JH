#include "BuyComfirmLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "GoldGoodsItem.h"
#include "ShopLayer.h"
#include "HintBox.h"
#include "StorageUILayer.h"
#include "Const.h"
#include "AnalyticUtil.h"
#include "MD5.h"

BuyComfirmLayer::BuyComfirmLayer()
{
}


BuyComfirmLayer::~BuyComfirmLayer()
{

}

bool BuyComfirmLayer::init(GoodsData* gdata)
{
	m_gdata = gdata;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("buyComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cacelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(BuyComfirmLayer::onCancel, this));

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(BuyComfirmLayer::onBuy, this));

	std::string desc;

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	if (GlobalData::g_gameStatus == GAMESTART)
		desc = "闭关中！没有分身符无法进行此操作。";
	desc.append(StringUtils::format("%s%d%s%s%s", CommonFuncs::gbk2utf("是否立即花费").c_str(), m_gdata->price, CommonFuncs::gbk2utf("金元宝购买").c_str(), m_gdata->name.c_str(), CommonFuncs::gbk2utf("？").c_str()));
	desctext->setString(desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

BuyComfirmLayer* BuyComfirmLayer::create(GoodsData* gdata)
{
	BuyComfirmLayer *pRet = new BuyComfirmLayer();
	if (pRet && pRet->init(gdata))
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

void BuyComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void BuyComfirmLayer::onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int mygold = GlobalData::getMyGoldCount();
		if (mygold >= m_gdata->price)
		{
			if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				GlobalData::dataIsModified = true;
				mygold = 0;
			}
			GlobalData::setMyGoldCount(mygold);

			if (GlobalData::dataIsModified)
			{
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}

			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUYOK);
			GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - m_gdata->price);
			GoldGoodsItem::addBuyGoods(m_gdata);
			int usegold = GlobalData::getUseGold() + m_gdata->price;
			GlobalData::setUseGold(usegold);

			StorageUILayer* storageUI = (StorageUILayer*)g_gameLayer->getChildByName("storageuilayer");
			if (storageUI != NULL)
				storageUI->updateResContent();

#ifdef ANALYTICS
			std::string name = StringUtils::format("b%s", m_gdata->icon.c_str());
			AnalyticUtil::onEvent(name.c_str());
#endif
		}
		else
		{
			if (GlobalData::g_gameStatus == GAMESTART)
			{
				Director::getInstance()->getRunningScene()->addChild(ShopLayer::create(), 1000);
			}
			else
			{
				Director::getInstance()->getRunningScene()->addChild(HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！")), 1000);
			}
		}
		this->removeFromParentAndCleanup(true);
	}
}