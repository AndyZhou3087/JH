#include "ExchangeLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameScene.h"
#include "StorageRoom.h"
#include "SoundManager.h"
#include "NpcLayer.h"

ExchangeLayer::ExchangeLayer()
{
	lastMyGoodsSrollViewHeight = -1;
	lastNpcGoodsSrollViewHeight = -1;
	npcGoodsMax = 0;
	myGoodMax = 0;
}


ExchangeLayer::~ExchangeLayer()
{

}


ExchangeLayer* ExchangeLayer::create(std::string npcid)
{
	ExchangeLayer *pRet = new ExchangeLayer();
	if (pRet && pRet->init(npcid))
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

bool ExchangeLayer::init(std::string npcid)
{
	Node* csbnode = CSLoader::createNode("exgLayer.csb");
	this->addChild(csbnode);

	m_npcid = npcid;

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ExchangeLayer::onBack, this));

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcgoodstext");
	std::string namestr = StringUtils::format("%s%s", GlobalData::map_maps[npcid].cname, CommonFuncs::gbk2utf("的物品").c_str());
	npcname->setString(namestr);

	m_npcWordLbl = (cocos2d::ui::Text*)csbnode->getChildByName("npcword");
	
	m_npcGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("npcexgscroll");

	m_myGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("myexgscroll");

	std::vector<std::string> exchgres = GlobalData::map_npcs[npcid].exchgres;

	for (unsigned int i = 0; i < exchgres.size(); i++)
	{
		int res = atoi(exchgres[i].c_str());
		
		if (res != 0)
		{
			PackageData data;
			bool isfind = false;
			std::string strid = StringUtils::format("%d", res / 1000);
			data.strid = strid;
			data.count = res % 1000;

			for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
			{
				ResData rdata = GlobalData::vec_resData[i];
				if (atoi(rdata.strid.c_str()) == res / 1000)
				{
					isfind = true;
					data.type = rdata.type - 1;
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, std::vector<BuildActionData>>::iterator it;
				for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
				{
					std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

					for (unsigned int m = 0; m < vec_bactData.size(); m++)
					{
						BuildActionData bdata = vec_bactData[m];
						if (atoi(bdata.icon) == res / 1000)
						{
							isfind = true;
							data.strid = bdata.icon;
							data.count = res % 1000;
							data.type = bdata.type - 1;
							break;
						}
					}
					if (isfind)
						break;
				}
			}
			npcGoodsData.push_back(data);
		}
		else
		{
			PackageData data;
			std::string strid = exchgres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (exchgres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF(exchgres[i]) && !GlobalData::tempHasgf(exchgres[i]))
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.lv = 0;
					data.type = gfdata.type - 1;
					npcGoodsData.push_back(data);
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = GlobalData::map_equips[ite->first];
					if (exchgres[i].compare(edata.id) == 0)
					{
						data.strid = edata.id;
						data.count = 1;
						data.type = edata.type - 1;
						data.goodvalue = 100;
						data.extype = edata.extype;
						npcGoodsData.push_back(data);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		myGoodsData.push_back(MyPackage::vec_packages[i]);
	}

	npcGoodsMax = npcGoodsData.size();
	myGoodMax = myGoodsData.size();
	updata();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ExchangeLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}


void ExchangeLayer::onNpcGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = npcGoodsData.begin(); it != npcGoodsData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata;
				pdata.type = data->type;
				pdata.strid = data->strid;
				pdata.lv = data->lv;
				pdata.extype = data->extype;
				pdata.count = 1;
				pdata.exp = data->exp;
				pdata.goodvalue = data->goodvalue;

				addToMyGoods(pdata);

				npcGoodsData.erase(it);

				break;
			}
		}
	}
	else
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.strid = data->strid;
		pdata.lv = data->lv;
		pdata.extype = data->extype;
		pdata.count = 1;
		pdata.exp = data->exp;
		pdata.goodvalue = data->goodvalue;
		addToMyGoods(pdata);

		data->count--;
	}
	updata();
}

void ExchangeLayer::addToMyGoods(PackageData data)
{
	unsigned int i = 0;
	for (i = 0; i < myGoodsData.size(); i++)
	{
		if (data.strid.compare(myGoodsData[i].strid) == 0 && (myGoodsData[i].type == FOOD || myGoodsData[i].type == MEDICINAL || myGoodsData[i].type == RES_1 || myGoodsData[i].type == RES_2))
		{
			myGoodsData[i].count++;
			break;
		}

	}
	if (i == myGoodsData.size())
	{
		data.count = 1;
		myGoodsData.push_back(data);
	}

	int smax = myGoodsData.size();
	if (myGoodMax < smax)
	{
		myGoodMax = smax;
	}

}

void ExchangeLayer::addToNpcGoods(PackageData data)
{
	unsigned int i = 0;
	for (i = 0; i < npcGoodsData.size(); i++)
	{
		if (data.strid.compare(npcGoodsData[i].strid) == 0 && (npcGoodsData[i].type == FOOD || npcGoodsData[i].type == MEDICINAL || npcGoodsData[i].type == RES_1 || npcGoodsData[i].type == RES_2))
		{
			npcGoodsData[i].count++;
			break;
		}
	}
	if (i == npcGoodsData.size())
	{
		data.count = 1;
		npcGoodsData.push_back(data);
	}

	int smax = npcGoodsData.size();
	if (npcGoodsMax < smax)
	{
		npcGoodsMax = smax;
	}
}

void ExchangeLayer::onMyGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = myGoodsData.begin(); it != myGoodsData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata;
				pdata.type = data->type;
				pdata.strid = data->strid;
				pdata.lv = data->lv;
				pdata.extype = data->extype;
				pdata.count = 1;
				pdata.exp = data->exp;
				pdata.goodvalue = data->goodvalue;

				addToNpcGoods(pdata);

				myGoodsData.erase(it);

				break;
			}
		}
	}
	else
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.strid = data->strid;
		pdata.lv = data->lv;
		pdata.extype = data->extype;
		pdata.count = 1;
		pdata.exp = data->exp;
		pdata.goodvalue = data->goodvalue;
		addToNpcGoods(pdata);
		data->count--;
	}
	updata();
}

void ExchangeLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void ExchangeLayer::updata()
{
	updataMyGoodsUI();
	updataNpcGoodsUI();
}

void ExchangeLayer::updataMyGoodsUI()
{
	for (int i = 0; i < myGoodMax; i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		m_myGoodsSrollView->removeChildByName(name);
	}

	int size = myGoodsData.size();
	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_myGoodsSrollView->getInnerContainerSize().height;
	if (lastMyGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_myGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastMyGoodsSrollViewHeight = innerheight;
		m_myGoodsSrollView->setInnerContainerSize(Size(m_myGoodsSrollView->getContentSize().width, innerheight));
	}

	for (unsigned int i = 0; i < myGoodsData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = myGoodsData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ExchangeLayer::onMyGoodsItem, this));
		boxItem->setUserData(&myGoodsData[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_myGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata.strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", tmpdata.count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void ExchangeLayer::updataNpcGoodsUI()
{
	for (int i = 0; i < npcGoodsMax; i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		m_npcGoodsSrollView->removeChildByName(name);
	}
	int size = npcGoodsData.size();
	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_npcGoodsSrollView->getInnerContainerSize().height;
	if (lastNpcGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_npcGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastNpcGoodsSrollViewHeight = innerheight;
		m_npcGoodsSrollView->setInnerContainerSize(Size(m_npcGoodsSrollView->getContentSize().width, innerheight));
	}

	for (int i = 0; i < size; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = npcGoodsData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ExchangeLayer::onNpcGoodsItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&npcGoodsData[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_npcGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata.strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tmpdata.count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void ExchangeLayer::onExit()
{

	Layer::onExit();
}