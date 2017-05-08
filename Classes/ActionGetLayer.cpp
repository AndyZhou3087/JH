﻿#include "ActionGetLayer.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "SoundManager.h"

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

	//操作的文字显示控件1："采集", 2："砍伐", 3："挖掘"
	cocos2d::ui::Text* actiontext = (cocos2d::ui::Text*)csbnode->getChildByName("actiontext");
	actiontext->setString(acname[actype]);

	//返回按钮
	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onBack, this));

	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onGet, this));
	std::string str = StringUtils::format("继续%s", acname[actype].c_str());
	getbtn->setTitleText(CommonFuncs::gbk2utf(str.c_str()));

	//全部拾取按钮
	cocos2d::ui::Button* getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	getallbtn->addTouchEventListener(CC_CALLBACK_2(ActionGetLayer::onAllGet, this));

	rewardids = res_ids;

	//点击后山列表中的操作获取一次资源
	doAction();
	updata();

	////layer 点击事件，屏蔽下层事件
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
			//奖励栏中数据跟产出的ID比较，相同数量+1
			if (rewardids[i] == atoi(getResData[m].strid.c_str()))
			{
				getResData[m].count++;
				break;
			}
		}
		//奖励栏中数据跟产出的ID比较，产出ID不在奖励栏内加到奖励栏中
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

			for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
			{
				if (GlobalData::vec_resData[i].strid.compare(idstr) == 0)
				{
					data.name = GlobalData::vec_resData[i].cname;
					data.desc = GlobalData::vec_resData[i].desc;
					break;
				}
			}
			getResData.push_back(data);
		}
	}
}

void ActionGetLayer::onRewardItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	//点击奖励栏的资源
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	removeitem();

	int count = data->count - 1;
	if (count <= 0)//数量为0，全部加到背包中了，移除掉
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
				pdata.name = data->name;
				pdata.desc = data->desc;
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					getResData.erase(it);
				}
				break;
			}
		}
	}
	else//不为0，数量-1
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.strid = data->strid;
		pdata.lv = data->lv;
		pdata.extype = data->extype;
		pdata.count = 1;
		pdata.exp = data->exp;
		pdata.goodvalue = data->goodvalue;
		pdata.name = data->name;
		pdata.desc = data->desc;
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}
	//保存临时数据
	saveTempData();

	//更新UI
	updata();
}

void ActionGetLayer::onPackageItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	//点击背包栏
	removeitem();
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getResData.size(); i++)
	{
		//奖励栏有背包中的资源，背包-1，奖励栏数据-1
		if (data.strid.compare(getResData[i].strid) == 0)
		{
			getResData[i].count++;
			break;
		}
	}

	//奖励栏没有背包中的资源，添加
	if (i == getResData.size())
	{
		data.count = 1;
		getResData.push_back(data);
	}

	//保存临时数据
	saveTempData();

	//背包 - 1,<0,会从背包中移除掉
	MyPackage::cutone(data.strid);
	//更新UI
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
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
	}
}

void ActionGetLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		//更新后山资源列表中的数据，为0，就不出产出资源了，需等待资源恢复
		if (GlobalData::vec_resData[mrid].count > 0)
		{
			GlobalData::vec_resData[mrid].count--;
			removeitem();
			doAction();
			updata();

			std::string desc;
			if (g_hero->getAtrByType((HeroAtrType)m_actype)->count > 0)//是否有工具m_actype：1："采集", 2："砍伐", 3："挖掘"
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
	//全部拾取
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
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

void ActionGetLayer::loadTempData()
{
	//临时存放的数据，保存在本地文件中，解析
	std::string datastr = GameDataSave::getInstance()->getTempStorage("m1-2");
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
		tempResData.push_back(data);
	}
}

void ActionGetLayer::saveTempData()
{
	//临时存放数据保持到本地文件
	loadTempData();
	std::vector<PackageData> allResData = tempResData;

	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		int tmpsize = tempResData.size();
		int j = 0;
		for (j = 0; j < tmpsize; j++)
		{
			if (getResData[i].strid.compare(tempResData[j].strid) == 0)
			{
				allResData[j].count += getResData[i].count;
				break;
			}
		}
		if (j == tmpsize)
		{
			allResData.push_back(getResData[i]);
		}
	}
	std::string str;
	for (unsigned int i = 0; i < allResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", allResData[i].strid.c_str(), allResData[i].type, allResData[i].count, allResData[i].extype, allResData[i].lv, allResData[i].exp, allResData[i].goodvalue, allResData[i].name.c_str(), allResData[i].desc.c_str());
		str.append(onestr);
	}
	GameDataSave::getInstance()->setTempStorage("m1-2", str.substr(0, str.length() - 1));
}

void ActionGetLayer::updata()
{
	//奖励栏资源UI更新
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

	//背包栏资源UI更新
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

//退出也要保存下临时存放的数据
void ActionGetLayer::onExit()
{
	saveTempData();
	Layer::onExit();
}