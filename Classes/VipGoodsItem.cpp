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
	//descTxt->setString(gdata->desc);
	std::string pricestr = StringUtils::format("%d元", gdata->price);
	priceTxt->setString(CommonFuncs::gbk2utf(pricestr.c_str()));

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(VipGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);
	int index = 0;
	for (unsigned int i = 0; i < GlobalData::vec_goods.size(); i++)
	{
		if (GlobalData::vec_goods[i].icon.compare(m_goodData->icon) == 0)
		{
			index = i;
			break;
		}
	}

	int rgoldcount = vipgoldcount[index - golditemcount];

	std::vector<std::string> vec_rewardres = m_goodData->vec_res;

	int startx = -100;
	int spacex = 68;
	int starty = 58;

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		std::string resstr;
		std::string strcount;
		if (i == 0)
		{
			resstr = "ui/gd0.png";
			strcount = StringUtils::format("x%d", rgoldcount);
		}
		else
		{
			std::string resid = vec_rewardres[i - 1];
			int intresid = atoi(resid.c_str());
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
		}
		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setScale(0.6f);
		res->setPosition(Vec2(startx + i*spacex, starty));
		this->addChild(res);

		Label * coutlbl = Label::createWithSystemFont(strcount, "", 21);
		coutlbl->setColor(Color3B(0, 0, 0));
		coutlbl->setPosition(Vec2(res->getPositionX(), 20));
		this->addChild(coutlbl);
	}

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