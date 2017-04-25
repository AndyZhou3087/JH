#include "BuyDetailsLayer.h"
#include "CommonFuncs.h"
#include "SelectHeroScene.h"
BuyDetailsLayer::BuyDetailsLayer()
{

}


BuyDetailsLayer::~BuyDetailsLayer()
{
}

bool BuyDetailsLayer::init(std::string imagepath, std::string name, std::string desc, int price)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("buyDetailsLayer.csb");
	this->addChild(csbnode);
	image = (cocos2d::ui::ImageView*)csbnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*) csbnode->getChildByName("price");

	image->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	image->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(CommonFuncs::gbk2utf(name.c_str()));
	descTxt->setString(CommonFuncs::gbk2utf(desc.c_str()));
	std::string pricestr = StringUtils::format("￥%d.00", price);
	priceTxt->setString(CommonFuncs::gbk2utf(pricestr.c_str()));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(BuyDetailsLayer::onBack, this));

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(BuyDetailsLayer::onBuy, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

BuyDetailsLayer* BuyDetailsLayer::create(std::string imagepath, std::string name, std::string desc, int price)
{
	BuyDetailsLayer *pRet = new BuyDetailsLayer();
	if (pRet && pRet->init(imagepath, name, desc, price))
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

void BuyDetailsLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void BuyDetailsLayer::onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SelectHeroScene* layer = (SelectHeroScene*)this->getParent();
		if (layer != NULL)
			layer->unlockSucc(this->getTag());
		this->removeFromParentAndCleanup(true);
	}
}