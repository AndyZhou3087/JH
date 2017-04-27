#include "ShopLayer.h"
#include "Const.h"
#include "GlobalData.h"
#include "BuildingUILayer.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "SelectHeroScene.h"

#include "json.h"

std::vector<GoodsData> ShopLayer::vec_goods;
int ShopLayer::payindex;

ShopLayer::ShopLayer()
{
	payindex = -1;
	vec_goods.clear();
}


ShopLayer::~ShopLayer()
{
	payindex = -1;
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
	Node* csbnode = CSLoader::createNode("shopLayer.csb");
	this->addChild(csbnode);
	
	loadShopData();


	int goodsize = vec_goods.size();
	int row = goodsize % 2 == 0 ? goodsize / 2 : (goodsize / 2 + 1);


	m_scrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

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
	payindex = index;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
void ShopLayer::setMessage(PYARET ret)
{
	if (ret == PAY_SUCC)
	{
		int shopgoodsszie = vec_goods.size();
		if (payindex < 4) // 人物解锁
		{
			if (g_SelectHeroScene != NULL)
				g_SelectHeroScene->unlockSucc(payindex);
		}
		else
		{
			addBuyGoods();
		}
	}
}

void ShopLayer::addBuyGoods()
{
	int goodsindex = payindex - 4;
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
					if (atoi(rdata.strid.c_str()) == intRes / 1000)
					{
						PackageData pdata;
						pdata.strid = rdata.strid;
						pdata.count = intRes % 1000;
						pdata.type = rdata.type - 1;
						pdata.desc = rdata.desc;
						pdata.name = rdata.cname;
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
				if (payRes[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF(payRes[i]))
				{
					isfind = true;
					PackageData pdata;
					pdata.strid = gfdata.id;
					pdata.count = 1;
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
					StorageRoom::add(pdata);
					break;
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
						StorageRoom::add(pdata);
						break;
					}
				}
			}
		}
	}
}