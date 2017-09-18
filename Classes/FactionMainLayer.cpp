﻿#include "FactionMainLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "FactionCreateLayer.h"
#include "Const.h"
#include "GameScene.h"
#include "FactionMemberLayer.h"

FactionMainLayer::FactionMainLayer()
{
}


FactionMainLayer::~FactionMainLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


FactionMainLayer* FactionMainLayer::create()
{
	FactionMainLayer *pRet = new FactionMainLayer();
	if (pRet && pRet->init())
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

bool FactionMainLayer::init()
{
	Node* csbnode = CSLoader::createNode("factionMainLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(FactionMainLayer::onBack, this));

	enterbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("enterbtn");
	enterbtn->addTouchEventListener(CC_CALLBACK_2(FactionMainLayer::onEnterFaction, this));

	createbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("createbtn");
	createbtn->addTouchEventListener(CC_CALLBACK_2(FactionMainLayer::onCreateFaction, this));

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	GlobalData::g_gameStatus = GAMEPAUSE;

	getFactionListData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FactionMainLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FactionMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void FactionMainLayer::onEnterFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FactionListData* fldata = NULL;
		for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
		{
			if (GlobalData::vec_factionListData[i].id == GlobalData::myFaction)
			{
				fldata = &GlobalData::vec_factionListData[i];

			}
		}
		if (fldata != NULL)
		{
			FactionMemberLayer * fmayer = FactionMemberLayer::create(fldata);
			g_gameLayer->addChild(fmayer, 5, "factionmemberlayer");
		}
	}
}

void FactionMainLayer::onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FactionCreateLayer* flayer = FactionCreateLayer::create();
		this->addChild(flayer);
	}
}

void FactionMainLayer::getFactionListData()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getFactionList();
}

void FactionMainLayer::delayShowData(float dt)
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_factionListData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
	{
		FactionListItem* node = FactionListItem::create(&GlobalData::vec_factionListData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		std::string nodestr = StringUtils::format("flitem%d", i);
		srollView->addChild(node, 0, nodestr);
	}

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void FactionMainLayer::updateLvAndMember()
{
	for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
	{
		std::string nodestr = StringUtils::format("flitem%d", i);
		FactionListItem* itemnode = (FactionListItem*)srollView->getChildByName(nodestr);
		itemnode->updateLvAndPeopleCount();
	}
	
}

void FactionMainLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	if (GlobalData::myFaction > 0)
	{
		createbtn->setEnabled(false);
		if (GlobalData::mytitle == 0)
			enterbtn->setEnabled(false);
		else
			enterbtn->setEnabled(true);
	}
	else
	{
		enterbtn->setEnabled(false);
		createbtn->setEnabled(true);
	}
	this->scheduleOnce(schedule_selector(FactionMainLayer::delayShowData), 0.1f);
}

void FactionMainLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
void FactionMainLayer::disBtn()
{
	createbtn->setEnabled(false);
	enterbtn->setEnabled(false);
}

FactionListItem::FactionListItem()
{

}
FactionListItem::~FactionListItem()
{

}

bool FactionListItem::init(FactionListData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;
	Node* csbnode = CSLoader::createNode("factionlistNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank + 1;

	std::string itemstr;
	if (rank % 2 == 0)
	{
		itemstr = "ui/factlistitem0.png";
	}
	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	cocos2d::ui::Text* ranknumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("num");
	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	if (rank <= 3)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	namelbl->setString(data->factionname);

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");
	lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");

	updateLvAndPeopleCount();

	cocos2d::ui::Text* ownerlbl = (cocos2d::ui::Text*)csbnode->getChildByName("owner");
	ownerlbl->setString(data->owner);

	actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionListItem::onAction, this));
	if (GlobalData::myFaction == data->id)
	{
		if (GlobalData::mytitle == 0)
			actionbtn->setTitleText(CommonFuncs::gbk2utf("申请中..."));
		else if (GlobalData::mytitle == 1)
			actionbtn->setTitleText(CommonFuncs::gbk2utf("已创建"));
		else if (GlobalData::mytitle == 2 || GlobalData::mytitle == 3 || GlobalData::mytitle == 4)
			actionbtn->setTitleText(CommonFuncs::gbk2utf("已加入"));
		actionbtn->setEnabled(false);
	}

	return true;
}
void FactionListItem::updateLvAndPeopleCount()
{
	std::string str = StringUtils::format("%d/%d", m_data->membercount, m_data->maxcount);
	countlbl->setString(str);

	int lv = 0;
	for (int i = 99; i >= 0; i--)
	{
		if (m_data->exp >= i*i*m_data->maxcount * 100)
		{
			lv = i;
			break;
		}
	}
	m_data->lv = lv;
	str = StringUtils::format("%d", m_data->lv + 1);
	lvlbl->setString(str);
}

FactionListItem* FactionListItem::create(FactionListData *data)
{
	FactionListItem *pRet = new FactionListItem();
	if (pRet && pRet->init(data))
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
void FactionListItem::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::myFaction > 0)
		{
			std::string descstr;
			if (GlobalData::mytitle == 0)
				descstr = "已申请帮派！！";
			else if (GlobalData::mytitle == 1)
				descstr = "已创建帮派！！";
			else if (GlobalData::mytitle == 2 || GlobalData::mytitle == 3 || GlobalData::mytitle == 4)
				descstr = "已加入帮派！！";

			if (descstr.length() > 0)
			{
				HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf(descstr.c_str()));
				Director::getInstance()->getRunningScene()->addChild(hintbox);
			}
		}
		else
		{
			if (g_hero->getLVValue() < m_data->lvlimit - 1)
			{
				std::string str = StringUtils::format("%s%s%d%s", m_data->factionname.c_str(), CommonFuncs::gbk2utf("需要申请者达到").c_str(), m_data->lvlimit, CommonFuncs::gbk2utf("级").c_str());
				HintBox* hintbox = HintBox::create(str);
				Director::getInstance()->getRunningScene()->addChild(hintbox);
				return;
			}
			int mysex = g_hero->getSex();
			if (mysex != m_data->sexlimit && g_hero->getSex() < 4)
			{
				std::string sexstr[] = { "只收自宫者", "只收男性", "只收女性", "只收男性和女性" };
				std::string str = StringUtils::format("%s%s", m_data->factionname.c_str(), CommonFuncs::gbk2utf(sexstr[mysex].c_str()).c_str());
				HintBox* hintbox = HintBox::create(str);
				Director::getInstance()->getRunningScene()->addChild(hintbox);
				return;
			}
			WaitingProgress* waitbox = WaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->requestFaction(m_data->id);
		}
	}
}

void FactionListItem::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	actionbtn->setTitleText(CommonFuncs::gbk2utf("申请中..."));
	actionbtn->setEnabled(false);
	GlobalData::myFaction = m_data->id;
	GlobalData::mytitle = 0;
	FactionMainLayer* fmainlayer = (FactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmainlayer != NULL)
		fmainlayer->disBtn();
}

void FactionListItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("申请失败，请检查网络设置稍后重试！！"));
	Director::getInstance()->getRunningScene()->addChild(box);
}