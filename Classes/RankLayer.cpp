#include "RankLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"

RankLayer::RankLayer()
{

}


RankLayer::~RankLayer()
{
	GlobalData::g_gameStatus = GAMESTART;
}


RankLayer* RankLayer::create()
{
	RankLayer *pRet = new RankLayer();
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

bool RankLayer::init()
{
	Node* csbnode = CSLoader::createNode("rankLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(RankLayer::onBack, this));

	tag0btn = (cocos2d::ui::Button*)csbnode->getChildByName("tagbtn0");
	tag0btn->addTouchEventListener(CC_CALLBACK_2(RankLayer::onRank, this));
	tag0btn->setTag(0);
	tag0btn->setBright(false);

	tag1btn = (cocos2d::ui::Button*)csbnode->getChildByName("tagbtn1");
	tag1btn->addTouchEventListener(CC_CALLBACK_2(RankLayer::onRank, this));
	tag1btn->setTag(1);
	tag1btn->setBright(true);

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	text6 = (cocos2d::ui::Text*) csbnode->getChildByName("rankcatabox")->getChildByName("text6");

	GlobalData::g_gameStatus = GAMEPAUSE;

	selectrank = 0;
	myfightingpower = 0;

	getRankData(0);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RankLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void RankLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void RankLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnnode = (cocos2d::ui::Button*)pSender;
		btnnode->setBright(false);
		int tag = btnnode->getTag();

		if (selectrank == tag)
			return;

		if (tag == 0)
		{
			tag1btn->setBright(true);
			text6->setString(CommonFuncs::gbk2utf("天数"));
		}
		else
		{
			tag0btn->setBright(true);
			text6->setString(CommonFuncs::gbk2utf("战斗力"));
		}

		selectrank = btnnode->getTag();
		getRankData(selectrank);
	}
}

void RankLayer::getRankData(int type)
{
	WaitingProgress* waitbox = WaitingProgress::create("排名中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	GlobalData::isGetServerData = true;

	std::string orderstr = StringUtils::format("days=%d", g_nature->getPastDays());
	if (type == 1)
	{
		myfightingpower = 0;
		if (g_hero != NULL)
		{
			myfightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
		}
		orderstr = StringUtils::format("fightingpower=%d", myfightingpower);
	}
	else
	{
		myfightingpower = 0;
	}
	std::string addtypestr = StringUtils::format("&ranktype=%d", type);
	orderstr.append(addtypestr);

	ServerDataSwap::getInstance()->setDelegate(this);
	ServerDataSwap::getInstance()->getRankData(orderstr);
}

void RankLayer::delayShowData(float dt)
{

	srollView->removeAllChildrenWithCleanup(true);
	int size = GlobalData::vec_rankData.size();

	int itemheight = 78;
	int innerheight = itemheight * size;
	int contentheight = srollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < GlobalData::vec_rankData.size(); i++)
	{
		RankItem* node = RankItem::create(&GlobalData::vec_rankData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}

	RankData myrankdata;
	myrankdata.rank = GlobalData::myrank;
	myrankdata.nickname = GlobalData::getMyNickName();
	myrankdata.herotype = g_hero->getHeadID();
	myrankdata.herolv = g_hero->getLVValue();
	myrankdata.herosex = g_hero->getSex();

	if (myfightingpower > 0)
		myrankdata.heroval = myfightingpower;
	else
		myrankdata.heroval = g_nature->getPastDays();
	RankItem* node = RankItem::create(&myrankdata);
	node->setPosition(Vec2(360, 130));
	this->addChild(node);

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}
void RankLayer::onSuccess()
{
	this->scheduleOnce(schedule_selector(RankLayer::delayShowData), 0.1f);
	GlobalData::isGetServerData = false;
}

void RankLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
	GlobalData::isGetServerData = false;
}

RankItem::RankItem()
{

}
RankItem::~RankItem()
{

}

bool RankItem::init(RankData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	Node* csbnode = CSLoader::createNode("rankNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank + 1;

	std::string itemstr;
	if (data->nickname.compare(GlobalData::getMyNickName()) == 0)
	{
		itemstr = "ui/myrankitem.png";
	}
	else if (rank % 2 == 0)
	{
		itemstr = "ui/rankitem0.png";
	}
	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	cocos2d::ui::Text* ranknumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("ranknum");
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

	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("nickname");
	nicknamelbl->setString(data->nickname);

	cocos2d::ui::Text* heronamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	heronamelbl->setString(CommonFuncs::gbk2utf(heroname[(data->herotype - 1)].c_str()));

	cocos2d::ui::Text* herolvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herolv");
	str = StringUtils::format("%d", data->herolv + 1);
	herolvlbl->setString(str);

	const std::string sexstr[] = { "不详", "男", "女" };
	cocos2d::ui::Text* herosexlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herosex");
	herosexlbl->setString(CommonFuncs::gbk2utf(sexstr[data->herosex].c_str()));
	if (data->herosex == 0)
	{
		herosexlbl->setTextColor(Color4B(204, 4, 4,255));
	}

	cocos2d::ui::Text* herovallbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroval");
	str = StringUtils::format("%d", data->heroval);
	herovallbl->setString(CommonFuncs::gbk2utf(str.c_str()));
	return true;
}

RankItem* RankItem::create(RankData *data)
{
	RankItem *pRet = new RankItem();
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