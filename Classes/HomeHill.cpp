﻿#include "HomeHill.h"
#include "json.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "ActionGetLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "CommonFuncs.h"
#include "FightLayer.h"


HomeHill::HomeHill()
{
}


HomeHill::~HomeHill()
{
}

bool HomeHill::init()
{
	Node* csbnode = CSLoader::createNode("homeHillLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onBack, this));
	
	int ressize = GlobalData::vec_hillResid.size();
	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	int itemheight = 140;
	int innerheight = itemheight * ressize;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < ressize; i++)
	{
		Node* resnode = CSLoader::createNode("resNode.csb");
		std::string namestr = StringUtils::format("node%d", i);
		scrollView->addChild(resnode,0, namestr);
		resnode->setPosition(Vec2(scrollView->getContentSize().width/2, innerheight - i * itemheight - itemheight / 2));

		cocos2d::ui::ImageView* iconimg = (cocos2d::ui::ImageView*)resnode->getChildByName("icon");
		cocos2d::ui::Text* name = (cocos2d::ui::Text*)resnode->getChildByName("name");
		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)resnode->getChildByName("desc");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		actionbtn->addTouchEventListener(CC_CALLBACK_2(HomeHill::onclick, this));
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData data = GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data.strid) == 0)
			{
				actionbtn->setName(data.strid);
				std::string str = StringUtils::format("ui/%s.png", data.strid.c_str());
				iconimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				iconimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

				name->setString(data.cname);
				desc->setString(data.desc);
				str = StringUtils::format("%d", data.count);
				count->setString(str);

				float fs = ((float)data.speed) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data.max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);

				if (data.count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					actionbtn->setEnabled(false);
					actionbtn->setBright(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					actionbtn->setEnabled(true);
					actionbtn->setBright(true);
				}

				actionbtn->setTitleText(CommonFuncs::gbk2utf(acname[data.actype].c_str()));
				
			}
		}

	}
	this->schedule(schedule_selector(HomeHill::updateUI),0.2f);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void HomeHill::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HomeHill::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		unsigned int i = 0;
		ResData* data = NULL;
		for (i = 0; i<GlobalData::vec_resData.size(); i++)
		{
			data = &GlobalData::vec_resData[i];
			if (data->strid.compare(node->getName()) == 0)
				break;
		}

		if (GlobalData::vec_resData[i].count > 0)
		{
			GlobalData::vec_resData[i].count--;

			if (atoi(data->strid.c_str()) != 0)//资源采集挖掘
			{
				ActionGetLayer* layer = ActionGetLayer::create(i, data->res, data->type, data->actype);
				this->addChild(layer);

				std::string desc;
				if (g_hero->getAtrByType((HeroAtrType)data->actype) != NULL)
				{
					desc = CommonFuncs::gbk2utf(acdesc1[data->actype].c_str());
				}
				else
				{
					desc = CommonFuncs::gbk2utf(acdesc[data->actype].c_str());
				}
				desc.append(data->unitname);
				g_uiScroll->addEventText(desc);
			}
			else//兔子，狼战斗界面
			{
				FightLayer* layer = FightLayer::create(GlobalData::map_npcs["m1-2"].name, data->strid);
				this->addChild(layer);
			}

		}
	}
}

void HomeHill::updateUI(float dt)
{
	for (unsigned int i = 0; i < GlobalData::vec_hillResid.size(); i++)
	{
		std::string namestr = StringUtils::format("node%d", i);
		Node* resnode = scrollView->getChildByName(namestr);

		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
		{
			ResData* data = &GlobalData::vec_resData[m];
			if (GlobalData::vec_hillResid[i].compare(data->strid) == 0)
			{
				std::string str = StringUtils::format("%d", data->count);
				count->setString(str);
				str = StringUtils::format("%.1fh", data->speed / 60.f);
				speed->setString(str);

				str = StringUtils::format("%.1fh", (data->speed* data->max - data->waittime) / 60.f);
				waittime->setString(str);
	
				if (data->count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					count->setColor(Color3B::RED);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					count->setColor(Color3B::BLACK);
				}
			}
		}
	}
}