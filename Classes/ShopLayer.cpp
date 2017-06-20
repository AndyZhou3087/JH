#include "ShopLayer.h"
#include "Const.h"
#include "GlobalData.h"
#include "BuildingUILayer.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "SelectHeroScene.h"
#include "SoundManager.h"
#include "ReviveLayer.h"
#include "json.h"
#include "AnalyticUtil.h"
#include "GameDataSave.h"
#include "CommonFuncs.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif

std::vector<GoodsData> ShopLayer::vec_goods;
int ShopLayer::payindex = -1;
bool ShopLayer::isPaying = false;
ShopLayer::ShopLayer()
{
	payindex = -1;
	//isPaying = false;
	vec_goods.clear();
}


ShopLayer::~ShopLayer()
{
	payindex = -1;
	//isPaying = false;
	vec_goods.clear();
}

ShopLayer* ShopLayer::create()
{
	ShopLayer *pRet = new ShopLayer();
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

bool ShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("shopLayer.csb");
	this->addChild(csbnode);
	
	loadShopData();


	int goodsize = vec_goods.size();
	int row = goodsize % 2 == 0 ? goodsize / 2 : (goodsize / 2 + 1);


	m_scrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	m_scrollview->setScrollBarEnabled(false);
	m_scrollview->setBounceEnabled(true);

	int itemheight = 350;
	int innerheight = itemheight * row;
	int contentheight = m_scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollview->setInnerContainerSize(Size(600, innerheight));

	for (int i = 0; i < goodsize; i++)
	{
		GoodsItem* node = GoodsItem::create(&vec_goods[i]);
		node->setTag(4 + i);
		m_scrollview->addChild(node);
		node->setPosition(Vec2(165 + i % 2 * 330, innerheight - itemheight / 2 - i / 2 * itemheight));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onBack, this));

	cocos2d::ui::Widget* qq = (cocos2d::ui::Widget*)csbnode->getChildByName("qq");
	qq->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onQQ, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void ShopLayer::loadShopData()
{
	rapidjson::Document doc = ReadJsonFile("data/shop.json");
	rapidjson::Value& values = doc["gs"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		GoodsData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["icon"];
		data.icon = v.GetString();

		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.vec_res.push_back(v[m].GetString());
		}
		v = item["name"];
		data.name = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();

		v = item["count"];
		data.count = atoi(v.GetString());
		v = item["price"];
		data.price = atoi(v.GetString());
		vec_goods.push_back(data);
	}
}

void ShopLayer::beginPay(int index)
{
	if (isPaying)
		return;

	isPaying = true;
	payindex = index;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//payCode
	buy((char*)payCode[payindex].c_str());
#endif
}
void ShopLayer::setMessage(PYARET ret)
{
	isPaying = false;

	if (ret == PAY_SUCC)
	{
		int shopgoodsszie = vec_goods.size();
		if (payindex < 4) // 人物解锁
		{
			if (g_SelectHeroScene != NULL)
				g_SelectHeroScene->unlockSucc(payindex);
#ifdef ANALYTICS
			std::string heroname[] = { "bxym", "bssy", "bjxb", "baq" };
			AnalyticUtil::onEvent(heroname[payindex].c_str());
#endif
		}
		else
		{
			addBuyGoods();
			if (ReviveLayer::isBuyRevive)
				ReviveLayer::doRevive();
#ifdef ANALYTICS
			std::string heroname[] = { "bzyys", "bdlw", "bfsf", "bfh","bbasepkg", "badvpkg","bemergpkg", "bfoodpkg","bjhgspkg"};
			AnalyticUtil::onEvent(heroname[payindex - 4].c_str());
#endif
			if (g_gameLayer->getChildByName("buycomfirmlayer") != NULL)
				g_gameLayer->removeChildByName("buycomfirmlayer");
		}
#ifdef ANALYTICS
		AnalyticUtil::pay("pay", buyprice[payindex], 1);
#endif
	}
}

void ShopLayer::addBuyGoods()
{
	int goodsindex = payindex - 4;
	if (vec_goods.size() <= 0)
		loadShopData();
	std::vector<std::string> payRes = vec_goods[goodsindex].vec_res;
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
						pdata.desc = bdata.desc;
						pdata.name = bdata.cname;
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
						pdata.desc = rdata.desc;
						pdata.name = rdata.cname;
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
					if (pdata.strid.substr(0, 1).compare("w") == 0)
					{
						pdata.type = W_GONG;
					}
					else if (pdata.strid.substr(0, 1).compare("x") == 0)
					{
						pdata.type = N_GONG;
					}
					pdata.desc = gfdata.desc;
					pdata.name = gfdata.cname;
					updateDefaultStorage(pdata);
					if (!g_hero->checkifHasGF(payRes[i]))
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
						pdata.desc = edata.desc;
						pdata.name = edata.cname;
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
void ShopLayer::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		copytoclipboard((char*)qq->getString().c_str());
#endif
	}

}

void ShopLayer::updateDefaultStorage(PackageData pdata)
{
	vector<PackageData> vec_defaultStorage;
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
		data.name = tmp[7];
		data.desc = tmp[8];
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
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", vec_defaultStorage[i].strid.c_str(), vec_defaultStorage[i].type, vec_defaultStorage[i].count, vec_defaultStorage[i].extype, vec_defaultStorage[i].lv, vec_defaultStorage[i].exp, vec_defaultStorage[i].goodvalue, vec_defaultStorage[i].name.c_str(), vec_defaultStorage[i].desc.c_str());
		str.append(onestr);
	}
	GameDataSave::getInstance()->setModifyDefaultStorage(g_hero->getHeadID(), str.substr(0, str.length() - 1));

}
