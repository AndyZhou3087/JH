#include "VipGoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Const.h"
#include "AnalyticUtil.h"

VipGoodsItem::VipGoodsItem()
{
	isDraging = false;
}


VipGoodsItem::~VipGoodsItem()
{
}

bool VipGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_goodData = gdata;

	Node* csbnode = CSLoader::createNode("vipGoodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	Node* bgnode = csbnode->getChildByName("itembg");

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
	std::string pricestr = StringUtils::format("%d元", gdata->price);
	priceTxt->setString(CommonFuncs::gbk2utf(pricestr.c_str()));

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(VipGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(VipGoodsItem::onBuyBtn, this));
	buybtn->setSwallowTouches(false);
	return true;
}

VipGoodsItem* VipGoodsItem::create(GoodsData* gdata)
{
	VipGoodsItem *pRet = new VipGoodsItem();
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
void VipGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
}

void VipGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
			ShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}