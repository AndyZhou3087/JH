﻿#include "GoldGoodsItem.h"
#include "CommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "GameDataSave.h"
#include "BuyComfirmLayer.h"

GoldGoodsItem::GoldGoodsItem()
{
	isDraging = false;
}


GoldGoodsItem::~GoldGoodsItem()
{
}

bool GoldGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_goodData = gdata;

	Node* csbnode = CSLoader::createNode("goldGoodsNode.csb");//物品节点
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
	std::string pricestr = StringUtils::format("%d", gdata->price);
	priceTxt->setString(pricestr);

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(GoldGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)bgnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(GoldGoodsItem::onBuyBtn, this));
	buybtn->setSwallowTouches(false);
	return true;
}

GoldGoodsItem* GoldGoodsItem::create(GoodsData* gdata)
{
	GoldGoodsItem *pRet = new GoldGoodsItem();
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
void GoldGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
}

void GoldGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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
			//addBuyGoods(m_goodData);

			BuyComfirmLayer* layer = BuyComfirmLayer::create(m_goodData);
			Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");
#ifdef ANALYTICS
			std::string name = StringUtils::format("b%s", m_goodData->icon.c_str());
			AnalyticUtil::onEvent(name);
#endif
		}
		isDraging = false;
	}
}


void GoldGoodsItem::addBuyGoods(GoodsData* gdata)
{
	std::vector<std::string> payRes = gdata->vec_res;
	for (unsigned int i = 0; i < payRes.size(); i++)
	{
		int intRes = atoi(payRes[i].c_str());
		if (intRes != 0)
		{
			bool isfind = false;
			std::map<std::string, std::vector<BuildActionData>>::iterator it;
			for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
			{
				std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

				for (unsigned int m = 0; m < vec_bactData.size(); m++)
				{
					BuildActionData bdata = vec_bactData[m];
					if (atoi(bdata.icon) == intRes / 1000)
					{
						isfind = true;
						PackageData pdata;
						pdata.strid = bdata.icon;
						pdata.count = intRes % 1000;
						pdata.type = bdata.type - 1;
						StorageRoom::add(pdata);
						break;
					}
				}
				if (isfind)
					break;
			}
			if (!isfind)
			{
				for (unsigned int n = 0; n < GlobalData::vec_resData.size(); n++)
				{
					ResData rdata = GlobalData::vec_resData[n];
					int rint = atoi(rdata.strid.c_str());
					if (rint == intRes / 1000)
					{
						PackageData pdata;
						pdata.strid = rdata.strid;
						pdata.count = intRes % 1000;
						pdata.type = rdata.type - 1;
						pdata.extype = rdata.actype;

						if (rint >= 75 && rint <= 78)
							updateDefaultStorage(pdata);
						StorageRoom::add(pdata);
						break;
					}
				}
			}
		}
		else
		{
			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (payRes[i].compare(gfdata.id) == 0)
				{
					isfind = true;
					PackageData pdata;
					pdata.strid = gfdata.id;
					pdata.count = 1;
					pdata.lv = 0;
					pdata.type = gfdata.type - 1;
					updateDefaultStorage(pdata);
					if (!g_hero->checkifHasGF(payRes[i]) && !GlobalData::tempHasgf(payRes[i]))
					{
						StorageRoom::add(pdata);
						break;
					}
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = GlobalData::map_equips[ite->first];
					if (payRes[i].compare(edata.id) == 0)
					{
						PackageData pdata;
						pdata.strid = edata.id;
						pdata.count = 1;
						pdata.type = edata.type - 1;
						pdata.goodvalue = 100;
						pdata.extype = edata.extype;
						StorageRoom::add(pdata);
						updateDefaultStorage(pdata);
						break;
					}
				}
			}
		}
	}
}

void GoldGoodsItem::updateDefaultStorage(PackageData pdata)
{
	std::vector<PackageData> vec_defaultStorage;
	std::string datastr = GlobalData::getDefaultStorage(g_hero->getHeadID());

	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		vec_defaultStorage.push_back(data);
	}

	bool ishas = false;
	for (unsigned int i = 0; i < vec_defaultStorage.size(); i++)
	{
		if (vec_defaultStorage[i].strid.compare(pdata.strid) == 0)
		{
			ishas = true;
			break;
		}
	}

	if (!ishas)
		vec_defaultStorage.push_back(pdata);

	std::string str;
	for (unsigned int i = 0; i < vec_defaultStorage.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d;", vec_defaultStorage[i].strid.c_str(), vec_defaultStorage[i].type, vec_defaultStorage[i].count, vec_defaultStorage[i].extype, vec_defaultStorage[i].lv, vec_defaultStorage[i].exp, vec_defaultStorage[i].goodvalue);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setModifyDefaultStorage(g_hero->getHeadID(), str.substr(0, str.length() - 1));

}
