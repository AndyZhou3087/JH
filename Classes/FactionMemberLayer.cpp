#include "FactionMemberLayer.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "GameScene.h"
#include "PromotionLayer.h"
#include "FactionMainLayer.h"
#include "MD5.h"

const std::string positionstr[] = { "", "帮主", "副帮主", "长老", "帮众" };
FactionMemberLayer::FactionMemberLayer()
{
	f_action = F_NONE;
	contribution = 0;
}


FactionMemberLayer::~FactionMemberLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
	f_action = F_NONE;
	contribution = 0;
}


FactionMemberLayer* FactionMemberLayer::create(FactionListData *fldata)
{
	FactionMemberLayer *pRet = new FactionMemberLayer();
	if (pRet && pRet->init(fldata))
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

bool FactionMemberLayer::init(FactionListData *fldata)
{
	Node* csbnode = CSLoader::createNode("factionMemberLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onBack, this));

	cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onAction, this));
	if (GlobalData::mytitle == 1)
		actionbtn->setTitleText(CommonFuncs::gbk2utf("解散帮派"));

	cocos2d::ui::Widget *sliserContriBtn = (cocos2d::ui::Widget*)csbnode->getChildByName("sliverbtn");
	sliserContriBtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onContribution, this));
	sliserContriBtn->setTag(0);

	cocos2d::ui::Widget *goldContriBtn = (cocos2d::ui::Widget*)csbnode->getChildByName("goldbtn");
	goldContriBtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberLayer::onContribution, this));
	goldContriBtn->setTag(1);

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	namelbl->setString(fldata->factionname);
	
	std::string str;
	int lv = fldata->lv + 1;
	lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	str = StringUtils::format("%d", lv);
	lvlbl->setString(str);

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");
	str = StringUtils::format("%d/%d", fldata->membercount, fldata->maxcount);
	countlbl->setString(str);

	explbl = (cocos2d::ui::Text*)csbnode->getChildByName("exp");
	str = StringUtils::format("%d/%d", fldata->exp, lv*lv*fldata->maxcount*100);
	explbl->setString(str);

	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desclbl->setString(fldata->desc);

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	m_fldata = fldata;

	GlobalData::g_gameStatus = GAMEPAUSE;

	getFactionMemberData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FactionMemberLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void FactionMemberLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void FactionMemberLayer::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::mytitle == 1)
		{
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

				f_action = F_RELEASE;
				WaitingProgress* waitbox = WaitingProgress::create("处理中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
				ServerDataSwap::init(this)->leaveFaction(1, m_fldata->id, g_hero->getHeadID());
			}
			else
			{
				HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！！"));
				this->addChild(hint);
			}
		}
		else
		{
			f_action = F_LEAVE;

			WaitingProgress* waitbox = WaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->leaveFaction(0, m_fldata->id, g_hero->getHeadID());
		}
	}
}

void FactionMemberLayer::onContribution(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();
		contribution = 0;
		bool isok = false;
		if (tag == 0)
		{
			if (StorageRoom::getCountById("80") < 10)
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("银两不足！"));
				this->addChild(hbox);
			}
			else
			{
				StorageRoom::use("80", 10);
				contribution += 10;
				isok = true;
			}
		}
		else
		{
			int mygold = GlobalData::getMyGoldCount();

			if (mygold >= 10)
			{
				if (GlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					GlobalData::dataIsModified = true;
					GlobalData::setMyGoldCount(0);
					HintBox* hint = HintBox::create(CommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				GlobalData::setMyGoldCount(GlobalData::getMyGoldCount() - 10);
				contribution += 100;
				isok = true;
			}
			else
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("金元宝不足！"));
				this->addChild(hbox);
			}
		}
		if (isok)
		{
			WaitingProgress* waitbox = WaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->contributionFaction(m_fldata->id, contribution, g_hero->getHeadID());
			f_action = F_CONTRIB;
		}
	}
}

void FactionMemberLayer::getFactionMemberData()
{
	WaitingProgress* waitbox = WaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getFactionMembers(m_fldata->id);
}

void FactionMemberLayer::delayShowData(float dt)
{
	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_factionMemberData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < GlobalData::vec_factionMemberData.size(); i++)
	{
		FactionMemberItem* node = FactionMemberItem::create(&GlobalData::vec_factionMemberData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		std::string nodestr = StringUtils::format("fmitem%d", i);
		srollView->addChild(node,0, nodestr);
	}

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void FactionMemberLayer::updateUi()
{
	std::string str = StringUtils::format("%d/%d", m_fldata->membercount, m_fldata->maxcount);
	countlbl->setString(str);

	int lv = 0;
	for (int i = 99; i >= 0; i--)
	{
		if (m_fldata->exp >= i*i*m_fldata->maxcount * 100)
		{
			lv = i;
			break;
		}
	}
	m_fldata->lv = lv;

	lv = lv + 1;
	str = StringUtils::format("%d", lv);
	lvlbl->setString(str);

	str = StringUtils::format("%d/%d", m_fldata->exp, lv*lv*m_fldata->maxcount * 100);
	explbl->setString(str);
	FactionMainLayer* fmlayer = (FactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->updateLvAndMember();
	}

}

void FactionMemberLayer::onSuccess()
{
	if (f_action == F_LEAVE || f_action == F_RELEASE)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		GlobalData::myFaction = 0;
		GlobalData::mytitle = 0;
		f_action = F_NONE;
		this->removeFromParentAndCleanup(true);

		FactionMainLayer* fmlayer = (FactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
		if (fmlayer != NULL)
		{
			fmlayer->getFactionListData();
		}
	}
	else if (f_action == F_CONTRIB)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		f_action = F_NONE;
		m_fldata->exp += contribution / 10;
		updateUi();
		for (unsigned int i = 0; i < GlobalData::vec_factionMemberData.size(); i++)
		{
			std::string nodestr = StringUtils::format("fmitem%d", i);
			FactionMemberItem* node = (FactionMemberItem*)srollView->getChildByName(nodestr);
			node->updateContribution(contribution);
		}
	}
	else
	{
		this->scheduleOnce(schedule_selector(FactionMemberLayer::delayShowData), 0.1f);
	}


}

void FactionMemberLayer::onErr(int errcode)
{
	f_action = F_NONE;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

FactionMemberItem::FactionMemberItem()
{

}
FactionMemberItem::~FactionMemberItem()
{

}

bool FactionMemberItem::init(FactionMemberData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;
	Node* csbnode = CSLoader::createNode("factionMemberNode.csb");
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	std::string itemstr;
	if (data->id % 2 == 0)
	{
		itemstr = "ui/factionmemberitem0.png";
	}
	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	std::string headstr = StringUtils::format("ui/fhero%d.png", m_data->herotype);
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
	headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);
	headimg->setScale(0.52f);

	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	nicknamelbl->setString(data->nickname);

	cocos2d::ui::Text* lvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	std::string str = StringUtils::format("%d", data->herolv + 1);
	lvlbl->setString(str);

	contributionlbl = (cocos2d::ui::Text*)csbnode->getChildByName("contribution");
	str = StringUtils::format("%d", data->contribution);
	contributionlbl->setString(str);

	postionlbl = (cocos2d::ui::Text*)csbnode->getChildByName("postion");
	postionlbl->setString(CommonFuncs::gbk2utf(positionstr[data->position].c_str()));

	actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberItem::onAction, this));

	modifybtn = (cocos2d::ui::Button*)csbnode->getChildByName("modifybtn");
	modifybtn->addTouchEventListener(CC_CALLBACK_2(FactionMemberItem::onModify, this));

	if (atoi(GlobalData::getMyID().c_str()) == data->userid && g_hero->getHeadID() == data->herotype)
	{
		actionbtn->setVisible(false);
		modifybtn->setVisible(false);
	}
	else
	{
		if (GlobalData::mytitle == 0 || GlobalData::mytitle == 4)
		{
			actionbtn->setVisible(false);
			modifybtn->setVisible(false);
		}
		else if (GlobalData::mytitle == 1)
		{
			if (data->position == 0)
			{
				actionbtn->setTitleText(CommonFuncs::gbk2utf("同意加入"));
				modifybtn->setVisible(false);
			}
			else
				actionbtn->setTitleText(CommonFuncs::gbk2utf("逐出"));
		}
		else if (GlobalData::mytitle == 2)
		{
			if (data->position == 0)
			{
				actionbtn->setTitleText(CommonFuncs::gbk2utf("同意加入"));
				modifybtn->setVisible(false);
			}
			else if (data->position == 1 || data->position == 2)
			{
				actionbtn->setVisible(false);
				modifybtn->setVisible(false);
			}
			else
				actionbtn->setTitleText(CommonFuncs::gbk2utf("逐出"));
		}
		else if (GlobalData::mytitle == 3)
		{
			if (data->position == 0)
			{
				actionbtn->setTitleText(CommonFuncs::gbk2utf("同意加入"));
				modifybtn->setVisible(false);
			}
			else if (data->position == 1 || data->position == 2 || data->position == 3)
			{
				actionbtn->setVisible(false);
				modifybtn->setVisible(false);
			}
			else
				actionbtn->setTitleText(CommonFuncs::gbk2utf("逐出"));
		}
		else
		{
			actionbtn->setVisible(false);
			modifybtn->setVisible(false);
		}
	}

	return true;
}

FactionMemberItem* FactionMemberItem::create(FactionMemberData *data)
{
	FactionMemberItem *pRet = new FactionMemberItem();
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
void FactionMemberItem::onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)pSender;
		if (actionbtn->getTitleText().compare(CommonFuncs::gbk2utf("同意加入")) == 0)
		{
			WaitingProgress* waitbox = WaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->joinFaction(m_data->factionid, m_data->userid, m_data->herotype);
		}
		else if (actionbtn->getTitleText().compare(CommonFuncs::gbk2utf("逐出")) == 0)
		{
			WaitingProgress* waitbox = WaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->kickFaction(m_data->factionid, m_data->userid, m_data->herotype);
		}
	}
}

void FactionMemberItem::onModify(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		PromotionLayer* player = PromotionLayer::create(m_data, this);
		g_gameLayer->addChild(player, 5, "promotionlayer");
	}
}

void FactionMemberItem::onSuccess()
{
	FactionMemberLayer* mlayer = (FactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");

	if (actionbtn->getTitleText().compare(CommonFuncs::gbk2utf("同意加入")) == 0)
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
		m_data->position = 4;
		modifybtn->setVisible(true);
		actionbtn->setTitleText(CommonFuncs::gbk2utf("逐出"));
		postionlbl->setString(CommonFuncs::gbk2utf("帮众"));

		for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
		{
			if (GlobalData::vec_factionListData[i].id == m_data->factionid)
			{
				GlobalData::vec_factionListData[i].membercount++;
				break;
			}
		}
		mlayer->updateUi();
	}
	else if (actionbtn->getTitleText().compare(CommonFuncs::gbk2utf("逐出")) == 0)
	{
		for (unsigned int i = 0; i < GlobalData::vec_factionListData.size(); i++)
		{
			if (GlobalData::vec_factionListData[i].id == m_data->factionid)
			{
				GlobalData::vec_factionListData[i].membercount--;
				break;
			}
		}
		for (unsigned int i = 0; i < GlobalData::vec_factionMemberData.size(); i++)
		{
			if (GlobalData::vec_factionMemberData[i].userid == m_data->userid && GlobalData::vec_factionMemberData[i].herotype == m_data->herotype)
			{
				GlobalData::vec_factionMemberData.erase(GlobalData::vec_factionMemberData.begin() + i);
				break;
			}
		}
		if (mlayer != NULL)
		{
			mlayer->updateUi();
			mlayer->delayShowData(0);
		}
	}
}

void FactionMemberItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("操作失败，请检查网络设置稍后重试！！"));
	Director::getInstance()->getRunningScene()->addChild(box);
}

void FactionMemberItem::updatePosition(int position)
{
	postionlbl->setString(CommonFuncs::gbk2utf(positionstr[position].c_str()));
}

void FactionMemberItem::updateContribution(int contribution)
{
	m_data->contribution += contribution;
	std::string str = StringUtils::format("%d", m_data->contribution);
	contributionlbl->setString(str);
}
