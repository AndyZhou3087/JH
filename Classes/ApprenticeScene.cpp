﻿#include "ApprenticeScene.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "HintBox.h"
#include "GameDataSave.h"
#include "MD5.h"

ApprenticeScene::ApprenticeScene()
{

}

ApprenticeScene::~ApprenticeScene()
{
	GlobalData::isPopingScene = false;
}

Scene* ApprenticeScene::createScene(int type)
{
	if (GlobalData::isPopingScene)
		return NULL;
	GlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = ApprenticeScene::create(type);

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

ApprenticeScene* ApprenticeScene::create(int type)
{
	ApprenticeScene *pRet = new ApprenticeScene();
	if (pRet && pRet->init(type))
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

bool ApprenticeScene::init(int type)
{
	Node* csbnode = CSLoader::createNode("apprenticeLayer.csb");
	this->addChild(csbnode);

	m_type = type;

	std::string descstr = "有人慕名远道而来，想向你拜师，是否收他为徒？徒弟会帮你收集后山资源！";
	std::string typeimgstr = "images/apprentice0.jpg";
	std::string closebtntext = "不收";
	std::string getbtntext = "收徒";
	if (type == 1)
	{
		descstr = "你的徒儿为你累死累活收集资源，现在向你讨要生活费来了，是否赏赐生活费？太抠门徒弟可能离你而去！";
		typeimgstr = "images/apprentice1.jpg";
		closebtntext = "不给";
		getbtntext = "赏赐";
	}
	else
	{
		int aday = GameDataSave::getInstance()->getApprenticeDay();
		if (aday < 0)
		{
			descstr = "由于你抠门的名声已经传遍江湖，想收徒要先支付生活费！";
			csbnode->getChildByName("gold")->setVisible(true);
			csbnode->getChildByName("goldcount")->setVisible(true);
		}
	}

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(CommonFuncs::gbk2utf(descstr.c_str()));


	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture(typeimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("nobtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(ApprenticeScene::onClose, this));
	closebtn->setTitleText(CommonFuncs::gbk2utf(closebtntext.c_str()));

	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(ApprenticeScene::onGet, this));
	getbtn->setTitleText(CommonFuncs::gbk2utf(getbtntext.c_str()));

	if (m_type ==  1)
	{
		std::string boxstr = "ui/buildsmall.png";

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(360, 220));
		this->addChild(box);

		std::string str = "ui/gd0.png";
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		
		Label * namelbl = Label::createWithTTF(CommonFuncs::gbk2utf("金元宝"), "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 140));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", 20);
		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(1, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 50, 180));
		this->addChild(coutlbl);
	}


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void ApprenticeScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void ApprenticeScene::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 1)
		{
			GameDataSave::getInstance()->setApprenticeDay(-1);
		}
		Director::getInstance()->popScene();
	}
}

void ApprenticeScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 0)
		{
			int curday = GlobalData::getDayOfYear();
			int aday = GameDataSave::getInstance()->getApprenticeDay();
			if (aday < 0)
			{
				giveGold();
			}
			else
			{
				GameDataSave::getInstance()->setApprenticeDay(curday);
			}
			Director::getInstance()->popScene();
		}
		else
		{
			giveGold();
		}
	}
}
void ApprenticeScene::giveGold()
{
	int curday = GlobalData::getDayOfYear();
	int mygold = GlobalData::getMyGoldCount();

	if (mygold >= 20)
	{
		if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
		{
			GlobalData::dataIsModified = true;
			GlobalData::setMyGoldCount(0);
			HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
			this->addChild(hint);
			return;
		}
		GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 20);
		GameDataSave::getInstance()->setApprenticeDay(curday);
	}
	else
	{
		HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
		this->addChild(hbox);
	}
}