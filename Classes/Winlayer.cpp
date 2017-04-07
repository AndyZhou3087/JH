#include "Winlayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "GameScene.h"

Winlayer::Winlayer()
{
}


Winlayer::~Winlayer()
{
}


Winlayer* Winlayer::create(std::string addr, std::string npcid)
{
	Winlayer *pRet = new Winlayer();
	if (pRet && pRet->init(addr, npcid))
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

bool Winlayer::init(std::string addr, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("winLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(Winlayer::onBack, this));

	cocos2d::ui::Button* getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(Winlayer::onAllGet, this));

	cocos2d::ui::Text* addrname = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrname->setString(addr);

	m_npcid = npcid;

	std::vector<std::string> winres = GlobalData::map_npcs[npcid].winres;

	static int randsed = 60 * 1000;
	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());
		randsed += systime();
		srand(randsed);
		int r = rand() % 100 + 1;
		if (r <= GlobalData::map_npcs[npcid].winresrnd[i])
		{
			PackageData data;
			if (res != 0)
			{
				std::string strid = StringUtils::format("%d", res / 1000);
				data.strid = strid;
				data.count = res % 1000;

				for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
				{
					ResData rdata = GlobalData::vec_resData[i];
					if (atoi(rdata.strid.c_str()) == res / 1000)
					{
						data.type = rdata.type;
						break;
					}
				}

			}
			else
			{
				std::string strid = winres[i];
				data.strid = strid;
				data.count = 1;
				if (strid.substr(0, 1).compare("a") == 0)
				{
					data.type = WEAPON;
				}
				else if (strid.substr(0, 1).compare("e") == 0)
				{
					data.type = PROTECT_EQU;
				}
				else if (strid.substr(0, 1).compare("w") == 0)
				{
					data.type = W_GONG;
				}
				else if (strid.substr(0, 1).compare("x") == 0)
				{
					data.type = N_GONG;
				}
			}
			data.lv = 0;
			data.extype = 0;
			getRewardData.push_back(data);
		}
	}
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

void Winlayer::updataLV()
{
	int winexp = GlobalData::map_npcs[m_npcid].exp;
	g_hero->setExpValue(g_hero->getExpValue() + winexp);
	int curlv = g_hero->getLVValue();
	unsigned int i = 0;
	int lv = 0;
	std::vector<int> vec_heroExp = GlobalData::map_heroAtr[g_hero->getMyID()].vec_exp;
	for (i = 0; i < vec_heroExp.size(); i++)
	{
		if (g_hero->getExpValue() > vec_heroExp[i])
		{
			lv = i;
		}
	}
	if (lv != curlv)
	{
		g_hero->setLVValue(lv);
		g_hero->setExpValue(g_hero->getExpValue() - vec_heroExp[lv]);
	}

	for (int m = H_WG; m <= H_NG; m++)
	{
		PackageData* gfData = g_hero->getAtrByType((HeroAtrType)m);
		if (gfData != NULL)
		{
			std::string gfname = gfData->strid;
			std::vector<int> vec_gfExp = GlobalData::map_wgngs[gfname].vec_exp;
			gfData->exp += winexp * 3 / 2;
			for (i = 0; i < vec_gfExp.size(); i++)
			{
				if (gfData->exp > vec_gfExp[i])
				{
					lv = i;
				}
			}
			if (lv != curlv)
			{
				gfData->lv = lv;
				gfData->exp = gfData->exp - vec_gfExp[lv];
			}
		}

	}

}

void Winlayer::onRewardItem(cocos2d::Ref* pSender)
{
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	removeitem();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = getRewardData.begin(); it != getRewardData.end(); ++it)
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
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					getRewardData.erase(it);
				}
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
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}
	saveTempData();
	updata();
}

void Winlayer::onPackageItem(cocos2d::Ref* pSender)
{
	removeitem();
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getRewardData.size(); i++)
	{
		if (data.strid.compare(getRewardData[i].strid) == 0)
		{
			getRewardData[i].count++;
			break;
		}
	}

	if (i == getRewardData.size())
	{
		getRewardData.push_back(data);
	}
	saveTempData();
	MyPackage::cutone(index);
	updata();
}

void Winlayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void Winlayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeitem();

		for (unsigned int i = 0; i < getRewardData.size(); i++)
		{
			int count = getRewardData[i].count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = getRewardData[i];
				data.count = 1;
				if (MyPackage::add(data) == 0)
				{
					if (--getRewardData[i].count <= 0)
					{
						getRewardData.erase(getRewardData.begin() + i);
						break;
					}
				}
			}

		}

		updata();
	}
}

void Winlayer::saveTempData()
{
	std::string str;
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d;", getRewardData[i].strid.c_str(), getRewardData[i].type, getRewardData[i].count, getRewardData[i].extype, getRewardData[i].lv);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setTempStorage("m1-2", str.substr(0, str.length() - 1));
}

void Winlayer::updata()
{
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(Winlayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getRewardData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 440));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getRewardData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getRewardData[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(Winlayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 240));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}


void Winlayer::removeitem()
{
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}
}

void Winlayer::onExit()
{
	saveTempData();
	Layer::onExit();
}

int Winlayer::systime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}