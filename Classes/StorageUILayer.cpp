﻿#include "StorageUILayer.h"
#include "json.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "Const.h"

const std::string name[] = { "食物", "药材", "武器", "防具", "内功", "外功", "资源1", "资源2"};

StorageUILayer::StorageUILayer()
{

}


StorageUILayer::~StorageUILayer()
{
}

bool StorageUILayer::init()
{

	m_csbnode = CSLoader::createNode("buidingUiLayer.csb");
	m_csbnode->setPosition(Vec2(0, -100));
	this->addChild(m_csbnode);

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(CommonFuncs::gbk2utf("仓库"));

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(StorageUILayer::onBack, this));
	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setContentSize(Size(650, 750));
	scrollview->setPositionY(200);
	
	int textheigth = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		int count = getCountByType(i);
		if (count > 0)
		{
			int row = count % 5 == 0 ? count / 5 : (count / 5 + 1);
			typerow[i] = row;
			typecount[i] = count;
			textheigth += 45;
		}
		else
		{
			typecount[i] = 0;
			typerow[i] = 0;
		}
	}
	
	int dataheigth = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		dataheigth += typerow[i] * 130;
	}
	int innerheight = textheigth + dataheigth;
	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < RES_MAX; i++)
	{
		int icount = getCountByType(i);
		if (icount > 0)
		{
			int preheight = 0;
			for (int j = 0; j < i; j++)
			{
				if (getCountByType(j)>0)
				{
					preheight += 45;
					preheight += typerow[j] * 130;
				}

			}
			Sprite * sepline = Sprite::createWithSpriteFrameName("ui/storagesepline.png");
			sepline->setPosition(Vec2(sepline->getContentSize().width / 2 + 40, innerheight - 35 - preheight));
			scrollview->addChild(sepline);
			Label* namelbl = Label::createWithTTF(CommonFuncs::gbk2utf(name[i].c_str()), "fonts/STXINGKA.TTF", 22);
			namelbl->setColor(Color3B(0, 0, 0));
			namelbl->setPosition(Vec2(25, 25));
			sepline->addChild(namelbl);

			for (int m = 0; m < icount; m++)
			{

				Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
				//box->setPosition(Vec2(box->getContentSize().width/2 + 20 + m % 5 * 120, sepline->getPositionY() - 5 - 65 - m/5*130));
				//scrollview->addChild(box);

				MenuItemSprite* boxItem = MenuItemSprite::create(
					box,
					box,
					box,
					CC_CALLBACK_1(StorageUILayer::onclick, this));
				boxItem->setTag(0);
				boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 20 + m % 5 * 120, sepline->getPositionY() - 5 - 65 - m / 5 * 130));
				Menu* menu = Menu::create();
				menu->addChild(boxItem);
				menu->setPosition(Vec2(0, 0));
				scrollview->addChild(menu);

				std::string str = StringUtils::format("ui/%d.png", StorageRoom::map_storageData[i].at(m).id);
				Sprite * res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
				box->addChild(res);

				str = StringUtils::format("%d", StorageRoom::map_storageData[i].at(m).count);
				Label * reslbl = Label::createWithSystemFont(str, "", 18);
				reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
				box->addChild(reslbl);
			}
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void StorageUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void StorageUILayer::onclick(Ref* pSender)
{
	
}

int StorageUILayer::getCountByType(int type)
{
	//int test[] = { 5, 0, 6, 2, 0, 0, 7, 3 };
	//return test[type];
	return StorageRoom::map_storageData[type].size();
}