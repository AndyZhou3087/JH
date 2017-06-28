#include "BuyComfirmLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "ShopLayer.h"

BuyComfirmLayer::BuyComfirmLayer()
{
	m_goodsIndex = 0;
}


BuyComfirmLayer::~BuyComfirmLayer()
{

}

bool BuyComfirmLayer::init(int shopGoodsIndex)
{
	m_goodsIndex = shopGoodsIndex;
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

	if (m_goodsIndex == FSFGOODSID)
		desc = "闭关中！没有分身符无法进行此操作。是否立即购买分身符？";

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

BuyComfirmLayer* BuyComfirmLayer::create(int shopGoodsIndex)
{
	BuyComfirmLayer *pRet = new BuyComfirmLayer();
	if (pRet && pRet->init(shopGoodsIndex))
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
		ShopLayer::beginPay(FSFGOODSID);
	}
}