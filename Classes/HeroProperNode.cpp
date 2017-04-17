#include "HeroProperNode.h"
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "StorageRoom.h"
#include "GameDataSave.h"

const std::string name[] = { "外功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

const HeroAtrType Atrytpe[] = { H_WG, H_NG, H_WEAPON, H_ARMOR, H_GATHER, H_FELL, H_EXCAVATE, H_MOUNT };

HeroProperNode::HeroProperNode()
{
	lastclickindex = -1;
	m_lastSelectedItem = NULL;
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
		str = StringUtils::format("box_%d", i);

		cocos2d::ui::Button* imgbtn = (cocos2d::ui::Button*)csbroot->getChildByName(str);
		imgbtn->setTag(i);
		imgbtn->addTouchEventListener(CC_CALLBACK_2(HeroProperNode::onImageClick, this));
		addCarryData(Atrytpe[i]);

		PackageData hpdata = g_hero->getAtrByType(Atrytpe[i]);
		if (hpdata.count > 0)
		{
			for (unsigned int m = 0; m < map_carryData[Atrytpe[i]].size(); m++)
			{
				PackageData carrydata = map_carryData[Atrytpe[i]][m];
				if (carrydata.strid.length() > 0 && carrydata.strid.compare(hpdata.strid) == 0 && carrydata.goodvalue == hpdata.goodvalue)
				{
					str = StringUtils::format("ui/%s.png", hpdata.strid.c_str());
					propeImages[i]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
					propeImages[i]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
				}
			}
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

void HeroProperNode::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		lastclickindex = -1;
		heroselectbg->setVisible(false);
		heroppoint->setVisible(false);
	}
}

void HeroProperNode::onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();

		if (lastclickindex == tag)
			return;
		removeitem();
		showSelectFrame(Atrytpe[tag]);

		lastclickindex = tag;
		title->setString(CommonFuncs::gbk2utf(name[tag].c_str()));
		heroppoint->setVisible(true);
		heroselectbg->setVisible(true);
		heroppoint->setPosition(Vec2(propeImages[tag]->getPositionX(), propeImages[tag]->getPositionY() - propeImages[tag]->getContentSize().height / 2 - 5));

		heroselectbg->setPositionY(heroppoint->getPositionY() - heroppoint->getContentSize().height + 3);
	}
}

void HeroProperNode::addCarryData(HeroAtrType index)
{
	if (g_hero->getIsOut())
	{
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			PackageData data = MyPackage::vec_packages[i];
			if (index == H_WEAPON && data.type == WEAPON)
				map_carryData[index].push_back(data);
			else if (index == H_GATHER && data.extype == 1)
				map_carryData[index].push_back(data);
			else if (index == H_FELL && data.extype == 2)
				map_carryData[index].push_back(data);
			else if (index == H_EXCAVATE && data.extype == 3)
				map_carryData[index].push_back(data);
			else if (index == H_WG && data.type == W_GONG)
				map_carryData[index].push_back(data);
			else if (index == H_NG && data.type == N_GONG)
				map_carryData[index].push_back(data);
			else if (index == H_ARMOR && data.type == PROTECT_EQU)
				map_carryData[index].push_back(data);
		}
		if (g_hero->getAtrByType(index).count > 0)
		{
			map_carryData[index].insert(map_carryData[index].begin(), g_hero->getAtrByType(index));
		}

	}
	else
	{
		if (index == H_WEAPON)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[WEAPON].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[WEAPON][m]);
		}
		else if (index == H_WG)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[W_GONG][m]);
		}
		else if (index == H_NG)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[N_GONG][m]);
		}
		else if (index == H_ARMOR)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[PROTECT_EQU].size(); m++)
				map_carryData[index].push_back(StorageRoom::map_storageData[PROTECT_EQU][m]);
		}
		else
		{
			int toolsize = StorageRoom::map_storageData[TOOLS].size();

			if (toolsize > 0)
			{

				for (unsigned int m = 0; m < StorageRoom::map_storageData[TOOLS].size(); m++)
				{
					if (StorageRoom::map_storageData[TOOLS][m].extype == 1 && index == H_GATHER)
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 2 && index == H_FELL)
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 3 && index == H_EXCAVATE)
						map_carryData[index].push_back(StorageRoom::map_storageData[TOOLS][m]);
				}
			}
		}
		if (g_hero->getAtrByType(index).count > 0)
		{
			map_carryData[index].insert(map_carryData[index].begin(), g_hero->getAtrByType(index));
		}
	}
}
void HeroProperNode::showSelectFrame(HeroAtrType index)
{
	int tempsize = map_carryData[index].size();
	int itemheight = 135;
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

		//str = StringUtils::format("%d", tempResData[i].count);
		//Label * reslbl = Label::createWithSystemFont(str, "", 18);
		//reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		//box->addChild(reslbl);
	}

	m_select->setVisible(false);
	if (tempsize > 0)
	{
		PackageData hpdata = g_hero->getAtrByType(index);
		if (hpdata.count > 0)
		{
			for (int i = 0; i < tempsize; i++)
			{
				PackageData carrydata = map_carryData[index][i];
				if (carrydata.strid.compare(hpdata.strid) == 0 && carrydata.goodvalue == hpdata.goodvalue)
				{
					std::string name = StringUtils::format("resitem%d", i);
					Node* node = m_scrollView->getChildByName(name)->getChildren().at(0);
					m_select->setPosition(Vec2(node->getPositionX() - node->getContentSize().width / 2, node->getPositionY() + node->getContentSize().height / 2));
					m_select->setVisible(true);
					break;
				}
			}
		}
	}
}

void HeroProperNode::onItem(Ref* pSender)
{
	Node* node = (Node*)pSender;

	HeroAtrType atrype = (HeroAtrType)node->getTag();
	PackageData* udata = (PackageData*)node->getUserData();
	m_select->setPosition(Vec2(node->getPositionX() - node->getContentSize().width/2, node->getPositionY() + node->getContentSize().height/2));
	
	std::string str;

	if (m_select->isVisible())
	{
		if (udata->type >= TOOLS)
			str = StringUtils::format("ui/hp%d-%d.png", udata->type+1, udata->extype);
		else
			str = StringUtils::format("ui/hp%d.png", udata->type+1);

		m_select->setVisible(false);
	}
	else
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
		}
		else
		{
			MyPackage::add(g_hero->getAtrByType(atrype));
			PackageData data;
			data.count = -1;
			g_hero->setAtrByType(atrype, data);
		}
		
	}
	else
	{
		if (m_select->isVisible())
		{
			StorageRoom::use(udata->strid);
			g_hero->setAtrByType(atrype, *udata);
		}
		else
		{
			StorageRoom::add(g_hero->getAtrByType(atrype));
			PackageData data;
			data.count = -1;
			g_hero->setAtrByType(atrype, data);
		}
	}
	m_lastSelectedItem = node;
}

void HeroProperNode::removeitem()
{
	if (lastclickindex > 0)
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
	std::string str;
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		PackageData sdata =	g_hero->getAtrByType((HeroAtrType)i);
	
		std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", sdata.strid.c_str(), sdata.type, sdata.count, sdata.extype, sdata.lv, sdata.exp, sdata.goodvalue,sdata.name.c_str(), sdata.desc.c_str());
		str.append(idstr);
	}
	GameDataSave::getInstance()->setHeroProperData(str.substr(0, str.length() - 1));
}

void HeroProperNode::onExit()
{
	saveData();
	Node::onExit();
}