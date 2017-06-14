#include "GoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
GoodsItem::GoodsItem()
{
	isDraging = false;
}


GoodsItem::~GoodsItem()
{
}

bool GoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	Node* csbnode = CSLoader::createNode("goodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	Node* bgnode = csbnode->getChildByName("bg");

	icon = (cocos2d::ui::ImageView*)bgnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)bgnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)bgnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*)bgnode->getChildByName("price");

	//图标
	std::string imagepath = StringUtils::format("ui/%s.png", gdata->icon.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	descTxt->setString(gdata->desc);
	std::string pricestr = StringUtils::format("%d.00", gdata->price);
	priceTxt->setString(pricestr);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("bg");//整块节点击
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
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		if (type == ui::Widget::TouchEventType::MOVED)
			isDraging = true;
		((cocos2d::ui::Widget*)pSender)->runAction(Sequence::create(ScaleTo::create(0.05f, 0.95f), NULL));
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Sequence::create(ScaleTo::create(0.05f, 1), NULL));
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			((cocos2d::ui::Widget*)pSender)->runAction(Sequence::create(ScaleTo::create(0.05f, 1), NULL));

			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
			//购买
			ShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}