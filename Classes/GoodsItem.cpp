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

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("bg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(GoodsItem::buyClick, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)bgnode->getChildByName("buybtn");//整块节点击
	buybtn->addTouchEventListener(CC_CALLBACK_2(GoodsItem::onBuy, this));
	buybtn->setSwallowTouches(false);
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
	CommonFuncs::BtnAction(pSender, type);
	buyClick(pSender, type);
}

void GoodsItem::buyClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		startPos = node->convertToWorldSpace(this->getParent()->getPosition());
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 pos = node->convertToWorldSpace(this->getParent()->getPosition());
		if (fabsf(pos.y - startPos.y) > 20)
			isDraging = true;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			//购买
			ShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}