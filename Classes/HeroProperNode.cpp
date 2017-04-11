#include "HeroProperNode.h"
#include "json.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "StorageRoom.h"

const std::string name[] = { "外功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

const HeroAtrType Atrytpe[] = { H_WG, H_NG, H_WEAPON, H_ARMOR, H_GATHER, H_FELL, H_EXCAVATE, H_MOUNT };

HeroProperNode::HeroProperNode()
{
	lastclickindex = -1;
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

void HeroProperNode::showSelectFrame(HeroAtrType index)
{
	vec_carryData.clear();
	if (g_hero->getIsOut())
	{
		for (unsigned int i = 0; i < MyPackage::getSize(); i++)
		{
			PackageData* data = &MyPackage::vec_packages[i];
			if (index == H_WEAPON && data->type == WEAPON)
				vec_carryData.push_back(data);
			else if (index == H_GATHER && data->extype == 1)
				vec_carryData.push_back(data);
			else if (index == H_FELL && data->extype == 2)
				vec_carryData.push_back(data);
			else if (index == H_EXCAVATE && data->extype == 3)
				vec_carryData.push_back(data);
			else if (index == H_WG && data->type == W_GONG)
				vec_carryData.push_back(data);
			else if (index == H_NG && data->type == N_GONG)
				vec_carryData.push_back(data);
			else if (index == H_ARMOR && data->type == PROTECT_EQU)
				vec_carryData.push_back(data);
		}
	}
	else
	{
		if (index == H_WEAPON)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[WEAPON].size(); m++)
				vec_carryData.push_back(&StorageRoom::map_storageData[WEAPON][m]);
		}
		else if (index == H_WG)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
				vec_carryData.push_back(&StorageRoom::map_storageData[W_GONG][m]);
		}
		else if (index == H_NG)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
				vec_carryData.push_back(&StorageRoom::map_storageData[N_GONG][m]);
		}
		else if (index == H_ARMOR)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[PROTECT_EQU].size(); m++)
				vec_carryData.push_back(&StorageRoom::map_storageData[PROTECT_EQU][m]);
		}
		else
		{
			int toolsize = StorageRoom::map_storageData[TOOLS].size();

			if (toolsize > 0)
			{

				for (unsigned int m = 0; m < StorageRoom::map_storageData[TOOLS].size(); m++)
				{
					if (StorageRoom::map_storageData[TOOLS][m].extype == 1 && index == H_GATHER)
						vec_carryData.push_back(&StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 2 && index == H_FELL)
						vec_carryData.push_back(&StorageRoom::map_storageData[TOOLS][m]);
					else if (StorageRoom::map_storageData[TOOLS][m].extype == 3 && index == H_EXCAVATE)
						vec_carryData.push_back(&StorageRoom::map_storageData[TOOLS][m]);
				}
			}
		}

	}

	int tempsize = vec_carryData.size();
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
		boxItem->setUserData(vec_carryData[i]);
		boxItem->setPosition(Vec2(70 + i % 4 * 135, innerheight - i / 4 * itemheight - itemheight / 2));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", vec_carryData[i]->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		//str = StringUtils::format("%d", tempResData[i].count);
		//Label * reslbl = Label::createWithSystemFont(str, "", 18);
		//reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		//box->addChild(reslbl);
	}
	if (tempsize <= 0)
	{
		m_select->setVisible(false);
	}
}

void HeroProperNode::onItem(Ref* pSender)
{
	Node* node = (Node*)pSender;
	HeroAtrType atrype = (HeroAtrType)node->getTag();
	PackageData* udata = (PackageData*)node->getUserData();
	g_hero->setAtrByType(atrype, udata);
	m_select->setPosition(Vec2(node->getPositionX() - node->getContentSize().width/2, node->getPositionY() + node->getContentSize().height/2));
	m_select->setVisible(true);
	std::string str = StringUtils::format("ui/%s.png", udata->strid.c_str());
	propeImages[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	propeImages[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	if (isout)
	{
		int i = 0;
		for (i = 0; i < MyPackage::getSize(); i++)
		{
			if (udata->strid.compare(MyPackage::vec_packages[i].strid) == 0)
			{
				break;
			}
		}
		MyPackage::cutone(i);
	}
}

void HeroProperNode::removeitem()
{
	for (unsigned int i = 0; i < vec_carryData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->removeChildByName(name);
	}
}