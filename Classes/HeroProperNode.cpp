#include "HeroProperNode.h"
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

//装备栏类型显示文字
const std::string name[] = { "武功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

//装备栏的类型
const HeroAtrType Atrytpe[] = { H_WG, H_NG, H_WEAPON, H_ARMOR, H_GATHER, H_FELL, H_EXCAVATE, H_MOUNT };

HeroProperNode::HeroProperNode()
{
	lastclickindex = -1;
	m_lastSelectedData = NULL;
	m_listener = NULL;
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
		cocos2d::ui::Button* imgbtn = (cocos2d::ui::Button*)csbroot->getChildByName(str);
		imgbtn->setTag(i);
		imgbtn->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onImageClick, this));
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
				str = StringUtils::format("Lv.%d", hpdata->lv + 1);
			}
			else if (Atrytpe[i] == H_GATHER || Atrytpe[i] == H_FELL || Atrytpe[i] == H_EXCAVATE || Atrytpe[i] == H_ARMOR || Atrytpe[i] == H_WEAPON)
			{
				str = StringUtils::format("耐久度%d", hpdata->goodvalue);
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
	}


	heroselectbg = (cocos2d::ui::Widget*)csbroot->getChildByName("heroselectbg");
	heroppoint = (cocos2d::ui::Widget*)csbroot->getChildByName("heroppoint");

	m_scrollView = (cocos2d::ui::ScrollView*)heroselectbg->getChildByName("ScrollView");

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
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		heroselectbg->setVisible(false);
		heroppoint->setVisible(false);

		_eventDispatcher->removeEventListener(m_listener);
		m_listener = NULL;

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

		if (m_listener == NULL)
		{
			m_listener = EventListenerTouchOneByOne::create();
			m_listener->onTouchBegan = [=](Touch *touch, Event *event)
			{
				return true;
			};

			m_listener->setSwallowTouches(true);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
		}

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
			else if (index == H_GATHER && data.extype == 1)//采集工具
				map_carryData[index].push_back(data);
			else if (index == H_FELL && data.extype == 2)//砍伐工具
				map_carryData[index].push_back(data);
			else if (index == H_EXCAVATE && data.extype == 3)//挖掘工具
				map_carryData[index].push_back(data);
			else if (index == H_WG && data.type == W_GONG)//外功
				map_carryData[index].push_back(data);
			else if (index == H_NG && data.type == N_GONG)//内功
				map_carryData[index].push_back(data);
			else if (index == H_ARMOR && data.type == PROTECT_EQU)//防具
				map_carryData[index].push_back(data);
		}
		if (g_hero->getAtrByType(index)->count > 0)//已经装备上的放在最前面。hpdata->count == -1没有装备
		{
			map_carryData[index].insert(map_carryData[index].begin(), *g_hero->getAtrByType(index));
		}

	}
	else//在家取仓库数据
	{
		if (index == H_WEAPON)//武器
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[WEAPON].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[WEAPON][m]);
		}
		else if (index == H_WG)//外功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[W_GONG][m]);
		}
		else if (index == H_NG)//内功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[N_GONG][m]);
		}
		else if (index == H_ARMOR)//防具
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[PROTECT_EQU].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[PROTECT_EQU][m]);
		}
		else//工具
		{
			int toolsize = StorageRoom::map_storageData[TOOLS].size();

			if (toolsize > 0)
			{

				for (unsigned int m = 0; m < StorageRoom::map_storageData[TOOLS].size(); m++)
				{
					if (StorageRoom::map_storageData[TOOLS][m].extype == 1 && index == H_GATHER)//采集
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 2 && index == H_FELL)//砍伐
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 3 && index == H_EXCAVATE)//挖掘
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
				}
			}
		}
		if (g_hero->getAtrByType(index)->count > 0) // 已经装备上的放在最前面。hpdata->count == -1没有装备
		{
			map_carryData[index].insert(map_carryData[index].begin(), *g_hero->getAtrByType(index));
		}
	}
}
void HeroProperNode::showSelectFrame(HeroAtrType index)
{
	int tempsize = map_carryData[index].size();
	int itemheight = 150;
	int row = tempsize % 4 == 0 ? tempsize / 4 : (tempsize / 4 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < tempsize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(HeroProperNode::onItem, this));
		boxItem->setTag(index);
		boxItem->setUserData(&map_carryData[index][i]);
		boxItem->setPosition(Vec2(70 + i % 4 * 135, innerheight - i / 4 * itemheight - itemheight / 2));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", map_carryData[index][i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%s", map_carryData[index][i].name.c_str());
		Label * namelbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 24);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getContentSize().width / 2, - 10));
		box->addChild(namelbl);

		if (index == H_WG || index == H_NG )
		{
			str = StringUtils::format("Lv.%d", map_carryData[index][i].lv + 1);
		}
		else if (index == H_GATHER || index == H_FELL || index == H_EXCAVATE || index == H_ARMOR || index == H_WEAPON)
		{
			str = StringUtils::format("耐久度%d", map_carryData[index][i].goodvalue);
		}
		else
		{
			str = "";
		}
		Label * lvlbl = Label::createWithSystemFont(CommonFuncs::gbk2utf(str.c_str()), "", 15);
		lvlbl->setColor(Color3B(255, 255, 255));
		lvlbl->setAnchorPoint(Vec2(1, 0));
		lvlbl->setPosition(Vec2(box->getContentSize().width - 10, 8));
		box->addChild(lvlbl);
	}
	m_select->setVisible(false);
	if (tempsize > 0)
	{
		PackageData *hpdata = g_hero->getAtrByType(index);
		if (hpdata->count > 0)//是否装备了hpdata->count ==-1没有装备
		{
			for (int i = 0; i < tempsize; i++)
			{
				PackageData carrydata = map_carryData[index][i];
				if (carrydata.strid.compare(hpdata->strid) == 0 && carrydata.goodvalue == hpdata->goodvalue)
				{
					std::string name = StringUtils::format("resitem%d", i);
					Node* node = m_scrollView->getChildByName(name)->getChildren().at(0);
					m_select->setPosition(Vec2(node->getPositionX() - node->getContentSize().width / 2, node->getPositionY() + node->getContentSize().height / 2));
					m_select->setVisible(true);
					break;
				}
			}
			m_lastSelectedData = hpdata;
		}
	}
}

void HeroProperNode::onItem(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;

	HeroAtrType atrype = (HeroAtrType)node->getTag();
	PackageData* udata = (PackageData*)node->getUserData();
	m_select->setPosition(Vec2(node->getPositionX() - node->getContentSize().width/2, node->getPositionY() + node->getContentSize().height/2));
	
	if (m_lastSelectedData != NULL)
	{
		if (m_lastSelectedData->strid.compare(udata->strid) != 0)//是否再次点击
		{

			//是否在同一种类型中切换装备，如果是先卸下，在装备上选中的
			if (m_lastSelectedData->type == udata->type && m_lastSelectedData->extype == udata->extype)
			{
				if (isout)
				{
					if (m_select->isVisible())
						MyPackage::add(*m_lastSelectedData);
				}
				else
				{
					if (m_select->isVisible())
						StorageRoom::add(*m_lastSelectedData);
				}
				m_select->setVisible(false);
			}
		}

	}

	std::string str;

	if (m_select->isVisible())//之前是选中m_select可见，现在点了就是没选中
	{
		if (udata->type >= TOOLS)
			str = StringUtils::format("ui/hp%d-%d.png", udata->type + 1, udata->extype);
		else
			str = StringUtils::format("ui/hp%d.png", udata->type + 1);

		m_select->setVisible(false);
	}
	else//选中
	{
		m_select->setVisible(true);
		str = StringUtils::format("ui/%s.png", udata->strid.c_str());
	}


	propeImages[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	propeImages[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	if (isout)
	{
		if (m_select->isVisible())
		{
			MyPackage::cutone(udata->strid);
			g_hero->setAtrByType(atrype, *udata);

			if (Atrytpe[lastclickindex] == H_WEAPON || Atrytpe[lastclickindex] == H_WG || Atrytpe[lastclickindex] == H_NG || Atrytpe[lastclickindex] == H_ARMOR)
			{
				str = StringUtils::format("Lv.%d", udata->lv + 1);
			}
			else if (Atrytpe[lastclickindex] == H_GATHER || Atrytpe[lastclickindex] == H_FELL || Atrytpe[lastclickindex] == H_EXCAVATE)
			{
				str = StringUtils::format("耐久度%d", udata->goodvalue);
			}
			else
			{
				str = "";
			}

			lvtext[lastclickindex]->setString(CommonFuncs::gbk2utf(str.c_str()));
		}
		else//卸掉装备 设置count = -1
		{
			MyPackage::add(*g_hero->getAtrByType(atrype));
			PackageData data;
			data.count = -1;
			g_hero->setAtrByType(atrype, data);
			lvtext[lastclickindex]->setString("");
		}
		
	}
	else
	{
		if (m_select->isVisible())
		{
			StorageRoom::use(udata->strid);
			g_hero->setAtrByType(atrype, *udata);
			if (Atrytpe[lastclickindex] == H_WEAPON || Atrytpe[lastclickindex] == H_WG || Atrytpe[lastclickindex] == H_NG || Atrytpe[lastclickindex] == H_ARMOR)
			{
				str = StringUtils::format("Lv.%d", udata->lv + 1);
			}
			else if (Atrytpe[lastclickindex] == H_GATHER || Atrytpe[lastclickindex] == H_FELL || Atrytpe[lastclickindex] == H_EXCAVATE)
			{
				str = StringUtils::format("耐久度%d", udata->goodvalue);
			}
			else
			{
				str = "";
			}

			lvtext[lastclickindex]->setString(CommonFuncs::gbk2utf(str.c_str()));
		}
		else
		{
			StorageRoom::add(*g_hero->getAtrByType(atrype));
			PackageData data;
			data.count = -1;
			g_hero->setAtrByType(atrype, data);
			lvtext[lastclickindex]->setString("");
		}
	}
	m_lastSelectedData = (PackageData*)node->getUserData();

	updataMyPackageUI();
	showNewerGuide(++m_step);
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
	
		std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", sdata->strid.c_str(), sdata->type, sdata->count, sdata->extype, sdata->lv, sdata->exp, sdata->goodvalue,sdata->name.c_str(), sdata->desc.c_str());
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
			nodes.push_back(resItemNode->getChildren().at(0));
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