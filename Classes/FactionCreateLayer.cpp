#include "FactionCreateLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "FactionMainLayer.h"
#include "MD5.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

FactionCreateLayer::FactionCreateLayer()
{

}


FactionCreateLayer::~FactionCreateLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


FactionCreateLayer* FactionCreateLayer::create()
{
	FactionCreateLayer *pRet = new FactionCreateLayer();
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

bool FactionCreateLayer::init()
{
	Node* csbnode = CSLoader::createNode("factionCreateLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(FactionCreateLayer::onBack, this));

	cocos2d::ui::Widget *createbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("createbtn");
	createbtn->addTouchEventListener(CC_CALLBACK_2(FactionCreateLayer::onCreateFaction, this));

	cocos2d::ui::TextField* nameInput = (cocos2d::ui::TextField*)csbnode->getChildByName("name");
	nameInput->setString("");
	nameInput->addEventListener(CC_CALLBACK_2(FactionCreateLayer::textFieldEvent, this));
	nameInput->setVisible(false);

	descinput = (cocos2d::ui::TextField*)csbnode->getChildByName("descinput");
	descinput->setString("");
	descinput->addEventListener(CC_CALLBACK_2(FactionCreateLayer::textFieldEvent, this));

	m_factionNameEdit = cocos2d::ui::EditBox::create(Size(380, 40), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	m_factionNameEdit->setPosition(Point(165, 1110));
	m_factionNameEdit->setAnchorPoint(Vec2(0, 0.5));
	m_factionNameEdit->setFontColor(Color3B::BLACK);
	m_factionNameEdit->setPlaceHolder(CommonFuncs::gbk2utf("请输入帮派名称").c_str());
	m_factionNameEdit->setPlaceholderFontSize(30);
	m_factionNameEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	m_factionNameEdit->setPlaceholderFontColor(Color3B(112,116,109));
	m_factionNameEdit->setMaxLength(8);
	m_factionNameEdit->setText("");
	m_factionNameEdit->setDelegate(this);
	csbnode->addChild(m_factionNameEdit);

	for (int i = 0; i < 4; i++)
	{
		std::string nodestr = StringUtils::format("btn%d", i);
		lvlimitbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName("lvbtnnode")->getChildByName(nodestr);
		lvlimitbtn[i]->addTouchEventListener(CC_CALLBACK_2(FactionCreateLayer::onSelectLv, this));
		lvlimitbtn[i]->setTag(i);
	}
	lvlimitbtn[3]->setBright(false);
	for (int i = 0; i < 5; i++)
	{
		std::string nodestr = StringUtils::format("btn%d", i);
		sexlimitbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName("sexbtnnode")->getChildByName(nodestr);
		sexlimitbtn[i]->addTouchEventListener(CC_CALLBACK_2(FactionCreateLayer::onSelectSex, this));
		sexlimitbtn[i]->setTag(i);
	}
	sexlimitbtn[4]->setBright(false);

	selectlv = 100;
	selectsex = 4;

	GlobalData::g_gameStatus = GAMEPAUSE;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FactionCreateLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FactionCreateLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void FactionCreateLayer::onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string factionname = m_factionNameEdit->getText();
		if (factionname.length() <= 0)
		{
			HintBox* hintbox = HintBox::create(CommonFuncs::gbk2utf("请输入帮派名称！！"));
			this->addChild(hintbox);
			return;
		}

		int mygold = GlobalData::getMyGoldCount();

		if (mygold >= 50)
		{
			if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				GlobalData::dataIsModified = true;
				GlobalData::setMyGoldCount(0);
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}
			WaitingProgress* waitbox = WaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			std::string utf8name = factionname;
			std::string utf8desc = descinput->getString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			utf8name = gbkToUTF8(factionname.c_str());
			utf8desc = gbkToUTF8(descinput->getString().c_str());
#endif
			ServerDataSwap::init(this)->createFaciton(utf8name, selectlv, selectsex, utf8desc);
		}
		else
		{
			HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！"));
			this->addChild(hint);
		}
	}
}

void FactionCreateLayer::onSuccess()
{
	GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 50);
	int usegold = GlobalData::getUseGold() + 50;
	GlobalData::setUseGold(usegold);


	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	FactionMainLayer* parent = (FactionMainLayer*)this->getParent();
	parent->getFactionListData();
	this->removeFromParentAndCleanup(true);
}

void FactionCreateLayer::onErr(int errcode)
{
	if (errcode == -1 || errcode == -2)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		HintBox * box = HintBox::create(CommonFuncs::gbk2utf("创建帮派失败，请检查网络设置或者联系客服！！"));
		this->addChild(box);
	}
	else if (errcode == -3)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		HintBox * box = HintBox::create(CommonFuncs::gbk2utf("输入中包含敏感词！！"));
		this->addChild(box);
	}
	else if (errcode == -4)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		HintBox * box = HintBox::create(CommonFuncs::gbk2utf("已创建过帮派！！"));
		this->addChild(box);
	}
	else if (errcode == -5)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		HintBox * box = HintBox::create(CommonFuncs::gbk2utf("帮派名称已存在！！"));
		this->addChild(box);
	}
}

void FactionCreateLayer::onSelectLv(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnode = (cocos2d::ui::Button*)pSender;
		int tag = btnode->getTag();
		int lvl[] = {30,50,80,100};
		for (int i = 0; i < 4; i++)
		{
			if (i == tag)
				lvlimitbtn[i]->setBright(false);
			else
				lvlimitbtn[i]->setBright(true);
		}
		selectlv = lvl[tag];
	}

}

void FactionCreateLayer::onSelectSex(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnode = (cocos2d::ui::Button*)pSender;
		int tag = btnode->getTag();
		int sexl[] = {1,2,3,0,4};
		for (int i = 0; i < 5; i++)
		{
			if (i == tag)
				sexlimitbtn[i]->setBright(false);
			else
				sexlimitbtn[i]->setBright(true);
		}
		selectsex = sexl[tag];
	}
}

void FactionCreateLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void FactionCreateLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{

}

void FactionCreateLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void FactionCreateLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}

void FactionCreateLayer::textFieldEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		cocos2d::ui::TextField * textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		std::string str = textField->getString();
	}
	break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		break;
	}
}