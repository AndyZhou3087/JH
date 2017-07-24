﻿#include "HeroProperNode.h"
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "StorageRoom.h"
#include "GameDataSave.h"
#include "SoundManager.h"
#include "ActionGetLayer.h"
#include "TempStorageLayer.h"
#include "Winlayer.h"
#include "OutDoor.h"
#include "HeroStateUILayer.h"
#include "MyMenu.h"
#include "BuyComfirmLayer.h"
#include "NewerGuideLayer.h"
#include "SpecialHintLayer.h"
#include "HintBox.h"

//装备栏类型显示文字
const std::string name[] = { "武功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

//装备栏的类型
const HeroAtrType Atrytpe[] = { H_WG, H_NG, H_WEAPON, H_ARMOR, H_GATHER, H_FELL, H_EXCAVATE, H_MOUNT };

HeroProperNode::HeroProperNode()
{
	lastclickindex = -1;
	m_lastSelectedData = NULL;
	m_step = 3;
}


HeroProperNode::~HeroProperNode()
{

}

bool HeroProperNode::init()
{
	csbroot = CSLoader::createNode("heroAttribNode.csb");
	this->addChild(csbroot);
	for (int i = 0; i < 8; i++)
	{
		std::string str = StringUtils::format("Image_%d", i);
		propeImages[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);

		str = StringUtils::format("lvtext_%d", i);
		lvtext[i] = (cocos2d::ui::Text*)csbroot->getChildByName(str);

		str = StringUtils::format("box_%d", i);
		imgbtn[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		imgbtn[i]->setTag(i);
		imgbtn[i]->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onImageClick, this));
		addCarryData(Atrytpe[i]);

		//获取每个装备栏装备的数据，如果没有hpdata->count = 0
		PackageData* hpdata = g_hero->getAtrByType(Atrytpe[i]);
		if (hpdata->count > 0)
		{
			str = StringUtils::format("ui/%s.png", hpdata->strid.c_str());
			propeImages[i]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			propeImages[i]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
			 
			if (Atrytpe[i] == H_WG || Atrytpe[i] == H_NG)
			{
				int lv = hpdata->lv + 1;
				str = StringUtils::format("Lv.%d", lv);
				if (lv >= GlobalData::map_wgngs[hpdata->strid].maxlv)
					str = StringUtils::format("Lv.%d(满级)", lv);

				std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[hpdata->strid].qu);
				imgbtn[i]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
				imgbtn[i]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

			}
			else if (Atrytpe[i] == H_GATHER || Atrytpe[i] == H_FELL || Atrytpe[i] == H_EXCAVATE || Atrytpe[i] == H_ARMOR || Atrytpe[i] == H_WEAPON)
			{
				str = StringUtils::format("耐久度%d%%", hpdata->goodvalue);
				if (Atrytpe[i] == H_ARMOR || Atrytpe[i] == H_WEAPON)
				{
					std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[hpdata->strid].qu);
					imgbtn[i]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
					imgbtn[i]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());
				}
			}
			else if (Atrytpe[i] == H_MOUNT)
			{
				if (hpdata->strid.compare("74") == 0)
				{
					str = StringUtils::format("生命%d", hpdata->goodvalue);
				}
				else
					str = "永久";
			}

			else
			{
				str = "";
			}
			lvtext[i]->setString(CommonFuncs::gbk2utf(str.c_str()));
		}
		else
		{
			lvtext[i]->setString("");
		}

		if (hpdata->goodvalue < 20)
			lvtext[i]->setColor(Color3B(204, 4, 4));
		else
			lvtext[i]->setColor(Color3B(255, 255, 255));
	}

	heroselectbg = (cocos2d::ui::Widget*)csbroot->getChildByName("heroselectbg");
	heroppoint = (cocos2d::ui::Widget*)csbroot->getChildByName("heroppoint");

	m_scrollView = (cocos2d::ui::ScrollView*)heroselectbg->getChildByName("ScrollView");
	m_scrollView->setScrollBarEnabled(false);
	m_scrollView->setBounceEnabled(true);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)heroselectbg->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onOK, this));

	title = (cocos2d::ui::Text*)heroselectbg->getChildByName("title");

	isout = g_hero->getIsOut();

	m_select = Sprite::createWithSpriteFrameName("ui/atrselected.png");
	m_select->setAnchorPoint(Vec2(0, 1));
	m_scrollView->addChild(m_select, 1);
	m_select->setVisible(false);

	return true;
}

void HeroProperNode::onEnterTransitionDidFinish()
{
	Node::onEnterTransitionDidFinish();
	showNewerGuide(m_step);
}

void HeroProperNode::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heroselectbg->setVisible(false);
		heroppoint->setVisible(false);

		HeroStateUILayer* heroStateUILayer = (HeroStateUILayer*)this->getParent()->getParent();
		if (heroStateUILayer != NULL)
			heroStateUILayer->showNewerGuide(12);
	}
}

void HeroProperNode::onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		Node* node = (Node*)pSender;
		int tag = node->getTag();
		//点击相同的一个不做操作
		if (lastclickindex == tag && heroselectbg->isVisible())
			return;

		removeitem();
		showSelectFrame(Atrytpe[tag]);

		lastclickindex = tag;
		title->setString(CommonFuncs::gbk2utf(name[tag].c_str()));
		heroppoint->setVisible(true);
		heroselectbg->setVisible(true);
		heroppoint->setPosition(Vec2(propeImages[tag]->getPositionX(), propeImages[tag]->getPositionY() - propeImages[tag]->getContentSize().height / 2 - 5));

		heroselectbg->setPositionY(heroppoint->getPositionY() - heroppoint->getContentSize().height + 3);

		showNewerGuide(++m_step);
	}
}

void HeroProperNode::addCarryData(HeroAtrType index)
{
	if (g_hero->getIsOut())//在家以外，取背包数据
	{
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			PackageData data = MyPackage::vec_packages[i];
			if (index == H_WEAPON && data.type == WEAPON)//武器
				map_carryData[index].push_back(data);
			else if (index == H_GATHER && data.extype == 1 && data.type == TOOLS)//采集工具
				map_carryData[index].push_back(data);
			else if (index == H_FELL && data.extype == 2 && data.type == TOOLS)//砍伐工具
				map_carryData[index].push_back(data);
			else if (index == H_EXCAVATE && data.extype == 3 && data.type == TOOLS)//挖掘工具
				map_carryData[index].push_back(data);
			else if (index == H_WG && data.type == W_GONG)//外功
				map_carryData[index].push_back(data);
			else if (index == H_NG && data.type == N_GONG)//内功
				map_carryData[index].push_back(data);
			else if (index == H_ARMOR && data.type == PROTECT_EQU)//防具
				map_carryData[index].push_back(data);
			else if (index == H_MOUNT && data.type == RES_2 && data.extype == 4)
			{
				map_carryData[index].push_back(data);
			}
		}
		if (g_hero->getAtrByType(index)->count > 0)//已经装备上的放在最前面。hpdata->count == -1没有装备
		{
			PackageData sdata = *g_hero->getAtrByType(index);
			map_carryData[index].insert(map_carryData[index].begin(), sdata);
		}

	}
	else//在家取仓库数据
	{
		if (index == H_WEAPON)//武器
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[WEAPON].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[WEAPON][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_WG)//外功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[W_GONG][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_NG)//内功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[N_GONG][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_ARMOR)//防具
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[PROTECT_EQU].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[PROTECT_EQU][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_MOUNT)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[RES_2].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[RES_2][m];
				if (data.extype == 4)
					map_carryData[index].push_back(data);
			}
		}
		else//工具
		{
			int toolsize = StorageRoom::map_storageData[TOOLS].size();

			if (toolsize > 0)
			{

				for (unsigned int m = 0; m < StorageRoom::map_storageData[TOOLS].size(); m++)
				{
					PackageData data = StorageRoom::map_storageData[TOOLS][m];
					if (data.extype == 1 && index == H_GATHER)//采集
						map_carryData[index].push_back(data);
					else if (data.extype == 2 && index == H_FELL)//砍伐
						map_carryData[index].push_back(data);
					else if (data.extype == 3 && index == H_EXCAVATE)//挖掘
						map_carryData[index].push_back(data);
				}
			}
		}
		if (g_hero->getAtrByType(index)->count > 0) // 已经装备上的放在最前面。hpdata->count == -1没有装备
		{
			PackageData sdata = *g_hero->getAtrByType(index);
			map_carryData[index].insert(map_carryData[index].begin(), sdata);
		}
	}
}
void HeroProperNode::showSelectFrame(HeroAtrType index)
{
	refreshCarryData();
	int tempsize = map_carryData[index].size();
	int itemheight = 150;
	int row = tempsize % 4 == 0 ? tempsize / 4 : (tempsize / 4 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	m_select->setVisible(false);

	for (int i = 0; i < tempsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = map_carryData[index][i];
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
			CC_CALLBACK_1(HeroProperNode::onItem, this));
		boxItem->setTag(index);
		boxItem->setUserData(&map_carryData[index][i]);
		if (i == 0 && g_hero->getAtrByType(index)->count > 0)
		{
			m_lastSelectedData = &map_carryData[index][i];
		}

		boxItem->setPosition(Vec2(80 + i % 4 * 145, innerheight - i / 4 * itemheight - itemheight / 2));
		MyMenu* menu = MyMenu::create();
		menu->setTouchlimit(m_scrollView);
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string rstrid = map_carryData[index][i].strid;
		std::string str = StringUtils::format("ui/%s.png", rstrid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%s", GlobalData::map_allResource[rstrid].cname.c_str());
		Label * namelbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 24);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getContentSize().width / 2, - 10));
		box->addChild(namelbl);

		Label * lvlbl = Label::createWithSystemFont("", "", 15);
		lvlbl->setColor(Color3B(255, 255, 255));
		lvlbl->setAnchorPoint(Vec2(1, 0));
		lvlbl->setPosition(Vec2(box->getContentSize().width - 10, 8));
		box->addChild(lvlbl);

		if (index == H_WG || index == H_NG )
		{
			int lv = map_carryData[index][i].lv + 1;
			str = StringUtils::format("Lv.%d", lv);

			if (lv >= GlobalData::map_wgngs[map_carryData[index][i].strid].maxlv)
				str = StringUtils::format("Lv.%d(满级)", lv);
		}
		else if (index == H_GATHER || index == H_FELL || index == H_EXCAVATE || index == H_ARMOR || index == H_WEAPON)
		{
			str = StringUtils::format("耐久度%d%%", map_carryData[index][i].goodvalue);
		}
		else if (index == H_MOUNT)
		{
			if (map_carryData[index][i].strid.compare("74") == 0)
			{
				str = StringUtils::format("生命%d", map_carryData[index][i].goodvalue);
			}
			else
				str = "永久";
		}
		else
		{
			str = "";
		}
		if (map_carryData[index][i].goodvalue < 20)
			lvlbl->setColor(Color3B(204, 4, 4));
		else
			lvlbl->setColor(Color3B(255, 255, 255));

		lvlbl->setString(CommonFuncs::gbk2utf(str.c_str()));

		if (g_hero->getAtrByType(index)->count > 0)//是否装备了hpdata->count ==-1没有装备
		{
			if (i == 0)
			{
				m_select->setPosition(Vec2(boxItem->getPositionX() - boxItem->getContentSize().width / 2, boxItem->getPositionY() + boxItem->getContentSize().height / 2));
				m_select->setVisible(true);
			}
		}
	}
}

void HeroProperNode::onItem(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	if (GlobalData::isExercising() && !GlobalData::isHasFSF())
	{
		int index = -1;
		for (unsigned int i = 0; i < GlobalData::vec_goods.size(); i++)
		{
			if (GlobalData::vec_goods[i].icon.compare("72") == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			BuyComfirmLayer* layer = BuyComfirmLayer::create(&GlobalData::vec_goods[index]);
			Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");
		}
		return;
	}

	m_select_itemnode = (Node*)pSender;
	m_select_atrype = (HeroAtrType)m_select_itemnode->getTag();
	m_select_udata = (PackageData*)m_select_itemnode->getUserData();


	if (g_hero->getSex() == S_MAN && (m_select_udata->strid.compare("w022") == 0 || m_select_udata->strid.compare("w040") == 0))
	{
		SpecialHintLayer* layer = SpecialHintLayer::create();
		g_gameLayer->addChild(layer, 11);
		return;
	}

	selectCarryData();

	showNewerGuide(++m_step);
}

void HeroProperNode::selectCarryData()
{
	if (m_lastSelectedData != NULL)
	{
		if (m_lastSelectedData != m_select_udata)//是否再次点击
		{
			//是否在同一种类型中切换装备，如果是先卸下，在装备上选中的
			bool issametool = false;
			bool issameother = false;
			if (m_lastSelectedData->type == TOOLS)
			{
				issametool = m_lastSelectedData->type == m_select_udata->type &&  m_lastSelectedData->extype == m_select_udata->extype;
			}
			else
			{
				issameother = m_lastSelectedData->type == m_select_udata->type;
			}
			if (issametool || issameother)
			{
				if (isout)
				{
					if (m_select->isVisible())
					{
						if (MyPackage::add(*m_lastSelectedData) < 0)
						{
							HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("背包已满！！"));
							Director::getInstance()->getRunningScene()->addChild(hint, 4);
							return;
						}
					}
				}
				else
				{
					if (m_select->isVisible())
						StorageRoom::add(*m_lastSelectedData);
				}
			}
			m_select->setVisible(false);
		}

	}

	m_select->setPosition(Vec2(m_select_itemnode->getPositionX() - m_select_itemnode->getContentSize().width / 2, m_select_itemnode->getPositionY() + m_select_itemnode->getContentSize().height / 2));

	std::string str;

	if (m_select->isVisible())//之前是选中m_select可见，现在点了就是没选中
	{
		if (m_select_udata->type >= TOOLS)
			str = StringUtils::format("ui/hp%d-%d.png", m_select_udata->type + 1, m_select_udata->extype);
		else
			str = StringUtils::format("ui/hp%d.png", m_select_udata->type + 1);

		m_select->setVisible(false);
	}
	else//选中
	{
		m_select->setVisible(true);
		str = StringUtils::format("ui/%s.png", m_select_udata->strid.c_str());
	}

	if (m_select->isVisible())
	{
		takeon(m_select_atrype, m_select_udata);
	}
	else
	{
		if (!takeoff(m_select_atrype))
		{
			m_select->setVisible(!m_select->isVisible());
			return;
		}
	}

	propeImages[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	propeImages[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());


	m_lastSelectedData = m_select_udata;

	updataMyPackageUI();
}

void HeroProperNode::takeon(HeroAtrType atrype, PackageData* pdata)
{
	if (g_hero->getIsOut())
		MyPackage::cutone(*pdata);
	else
		StorageRoom::use(*pdata);

	g_hero->setAtrByType(atrype, *pdata);
	std::string str;
	if (atrype == H_WG || atrype == H_NG)
	{
		int lv = pdata->lv + 1;
		str = StringUtils::format("Lv.%d", lv);

		if (lv >= GlobalData::map_wgngs[pdata->strid].maxlv)
			str = StringUtils::format("Lv.%d(满级)", lv);

		std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[pdata->strid].qu);
		imgbtn[lastclickindex]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
		imgbtn[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());
	}
	else if (atrype == H_GATHER || atrype == H_FELL || atrype == H_EXCAVATE || atrype == H_WEAPON || atrype == H_ARMOR)
	{
		str = StringUtils::format("耐久度%d%%", pdata->goodvalue);

		if (atrype == H_WEAPON || atrype == H_ARMOR)
		{
			std::string qustr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[pdata->strid].qu);
			imgbtn[lastclickindex]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
			imgbtn[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());
		}
	}
	else if (atrype == H_MOUNT)
	{
		if (pdata->strid.compare("74") == 0)
		{
			str = StringUtils::format("生命%d", pdata->goodvalue);
		}
		else
			str = "永久";
	}
	else
	{
		str = "";
	}

	if (pdata->goodvalue < 20)
		lvtext[lastclickindex]->setColor(Color3B(204, 4, 4));
	else
		lvtext[lastclickindex]->setColor(Color3B(255, 255, 255));

	lvtext[lastclickindex]->setString(CommonFuncs::gbk2utf(str.c_str()));
}

bool HeroProperNode::takeoff(HeroAtrType atrype)
{
	PackageData mydata = *g_hero->getAtrByType(atrype);
	if (g_hero->getIsOut())
	{
		if (MyPackage::add(mydata) < 0)
		{
			HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("背包已满！！"));
			Director::getInstance()->getRunningScene()->addChild(hint, 4);
			return false;
		}
	}
	else
		StorageRoom::add(mydata);
	g_hero->getAtrByType(atrype)->count = -1;
	lvtext[lastclickindex]->setString("");

	std::string str = "ui/buildsmall.png";
	imgbtn[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	imgbtn[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	return true;
}

void HeroProperNode::removeitem()
{
	if (lastclickindex >= 0)
	{
		for (unsigned int i = 0; i < map_carryData[Atrytpe[lastclickindex]].size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			m_scrollView->removeChildByName(name);
		}
	}
}

void HeroProperNode::saveData()
{
	//保存装备栏数据
	std::string str;
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		PackageData* sdata = g_hero->getAtrByType((HeroAtrType)i);
	
		std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", sdata->strid.c_str(), sdata->type, sdata->count, sdata->extype, sdata->lv, sdata->exp, sdata->goodvalue, sdata->slv,sdata->tqu);
		str.append(idstr);
	}
	GameDataSave::getInstance()->setHeroProperData(str.substr(0, str.length() - 1));
}

void HeroProperNode::onExit()
{
	saveData();
	Node::onExit();
}

void HeroProperNode::refreshCarryData()
{
	removeitem();
	map_carryData.clear();
	for (int i = 0; i < 8; i++)
		addCarryData(Atrytpe[i]);
}

void HeroProperNode::updataMyPackageUI()
{
	if (g_gameLayer != NULL)
	{
		ActionGetLayer * Alayer = (ActionGetLayer*)g_gameLayer->getChildByName("ActionGetLayer");
		if (Alayer != NULL)
			Alayer->updataMyPackageUI();
		TempStorageLayer* Tlayer = (TempStorageLayer*)g_gameLayer->getChildByName("TempStorageLayer");
		if (Tlayer != NULL)
			Tlayer->updataMyPackageUI();

		Winlayer* Wlayer = (Winlayer*)g_gameLayer->getChildByName("Winlayer");
		if (Wlayer != NULL)
			Wlayer->updataMyPackageUI();

		OutDoor* Olayer = (OutDoor*)g_gameLayer->getChildByName("OutDoor");
		if (Olayer != NULL)
		{
			Olayer->updata();
		}
	}
}

void HeroProperNode::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	Node* resItemNode = NULL;
	if (step == 3)
		nodes.push_back(propeImages[0]);
	else if (step == 4 || step == 6 || step == 8 || step == 10)
	{
		resItemNode = m_scrollView->getChildByName("resitem0");
		if (resItemNode != NULL)
		{
			if (resItemNode->getChildrenCount() > 0)
				nodes.push_back(resItemNode->getChildren().at(0));
		}
		if (NewerGuideLayer::checkifNewerGuide(step))
			m_scrollView->setEnabled(false);
	}
	else if (step == 5)
	{
		nodes.push_back(propeImages[1]);
	}
	else if (step == 7)
	{
		nodes.push_back(propeImages[2]);
	}
	else if (step == 9)
	{
		nodes.push_back(propeImages[3]);
	}
	else if (step == 11)
	{
		nodes.push_back(heroselectbg->getChildByName("okbtn"));
	}
	if (step <= 11 && nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}