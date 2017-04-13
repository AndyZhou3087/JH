#include "ActionGetLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "GameScene.h"

ActionGetLayer::ActionGetLayer()
{
}


ActionGetLayer::~ActionGetLayer()
{
}

bool ActionGetLayer::init(int rid, std::vector<int> res_ids, int type, int actype)
{
	Node* csbnode = CSLoader::createNode("actionGetLayer.csb");
	this->addChild(csbnode);

	mtype = type;
	mrid = rid;
	m_actype = actype;
	cocos2d::ui::Text* actiontext = (cocos2d::ui::Text*)csbnode->getChildByName("actiontext");
	actiontext->setString(acname[actype]);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onBack, this));

	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onGet, this));
	std::string str = StringUtils::format("继续%s", acname[actype].c_str());
	getbtn->setTitleText(CommonFuncs::gbk2utf(str.c_str()));

	cocos2d::ui::Button* getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onAllGet, this));

	rewardids = res_ids;

	doAction();
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

void ActionGetLayer::doAction()
{
	for (unsigned int i = 0; i < rewardids.size(); i++)
	{
		unsigned int m = 0;
		for (m = 0; m < getResData.size(); m++)
		{
			if (rewardids[i] == atoi(getResData[m].strid.c_str()))
			{
				getResData[m].count++;
				break;
			}
		}

		if (m == getResData.size())
		{
			PackageData data;
			std::string idstr = StringUtils::format("%d", rewardids[i]);
			data.strid = idstr;
			data.type = mtype - 1;
			data.count = 1;
			data.lv = 0;
			data.extype = 0;
			data.exp = 0;
			data.goodvalue = 100;
			getResData.push_back(data);
		}
	}
}

void ActionGetLayer::onRewardItem(cocos2d::Ref* pSender)
{
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	removeitem();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = getResData.begin(); it != getResData.end(); ++it)
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
					getResData.erase(it);
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

void ActionGetLayer::onPackageItem(cocos2d::Ref* pSender)
{
	removeitem();
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getResData.size(); i++)
	{
		if (data.strid.compare(getResData[i].strid) == 0)
		{
			getResData[i].count++;
			break;
		}
	}

	if (i == getResData.size())
	{
		getResData.push_back(data);
	}
	saveTempData();
	MyPackage::cutone(data.strid);
	updata();
}

ActionGetLayer* ActionGetLayer::create(int rid, std::vector<int> res_ids, int type, int actype)
{
	ActionGetLayer *pRet = new ActionGetLayer();
	if (pRet && pRet->init(rid, res_ids, type, actype))
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

void ActionGetLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void ActionGetLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::vec_resData[mrid].count > 0)
		{
			GlobalData::vec_resData[mrid].count--;
			removeitem();
			doAction();
			updata();

			std::string desc;
			if (g_hero->getAtrByType((HeroAtrType)m_actype).count > 0)
			{
				desc = CommonFuncs::gbk2utf(acdesc1[m_actype].c_str());
			}
			else
			{
				desc = CommonFuncs::gbk2utf(acdesc[m_actype].c_str());
			}
			desc.append(GlobalData::vec_resData[mrid].unitname);
			g_uiScroll->addEventText(desc);
		}
	}
}

void ActionGetLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeitem();

		for (unsigned int i = 0; i < getResData.size(); i++)
		{
			int count = getResData[i].count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = getResData[i];
				data.count = 1;
				if (MyPackage::add(data) == 0)
				{
					if (--getResData[i].count <= 0)
					{
						getResData.erase(getResData.begin() + i);
						break;
					}
				}
			}

		}

		updata();
	}
}

void ActionGetLayer::saveTempData()
{
	std::string str;
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d;", getResData[i].strid.c_str(), getResData[i].type, getResData[i].count, getResData[i].extype, getResData[i].lv);
		str.append(onestr);
	}
	GameDataSave::getInstance()->setTempStorage("m1-2", str.substr(0, str.length() - 1));
}

void ActionGetLayer::updata()
{
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(ActionGetLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getResData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 512));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getResData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getResData[i].count);
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
			CC_CALLBACK_1(ActionGetLayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 305));
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


void ActionGetLayer::removeitem()
{
	for (unsigned int i = 0; i < getResData.size(); i++)
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

void ActionGetLayer::onExit()
{
	saveTempData();
	Layer::onExit();
}