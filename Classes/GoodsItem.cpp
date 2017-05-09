#include "GoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
GoodsItem::GoodsItem()
{

}


GoodsItem::~GoodsItem()
{
}

bool GoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	Node* csbnode = CSLoader::createNode("goodsNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	icon = (cocos2d::ui::ImageView*)csbnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*) csbnode->getChildByName("price");

	std::string imagepath = StringUtils::format("ui/%s.png", gdata->icon.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	descTxt->setString(gdata->desc);
	std::string pricestr = StringUtils::format("%d.00", gdata->price);
	priceTxt->setString(pricestr);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("bg");
	backbtn->addTouchEventListener(CC_CALLBACK_2(GoodsItem::onBuy, this));

	backbtn->setSwallowTouches(false);

	return true;
}

GoodsItem* GoodsItem::create(GoodsData* gdata)
{
	GoodsItem *pRet = new GoodsItem();
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
void GoodsItem::onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		ShopLayer::beginPay(this->getTag());
	}
}