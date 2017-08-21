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

	srollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	GlobalData::g_gameStatus = GAMEPAUSE;

	WaitingProgress* waitbox = WaitingProgress::create("排名中，请稍等...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	GlobalData::isGetServerData = true;
	ServerDataSwap::getInstance()->setDelegate(this);
	std::string orderstr = StringUtils::format("days=%d", g_nature->getPastDays());
	ServerDataSwap::getInstance()->getRankData(orderstr);

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

void RankLayer::delayShowData(float dt)
{

	int size = GlobalData::map_NPCFriendData.size();

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

	m_loadlbl->setVisible(false);
	RankData myrankdata;
	myrankdata.rank = GlobalData::myrank;
	myrankdata.nickname = GlobalData::getMyNickName();
	myrankdata.herotype = g_hero->getHeadID();
	myrankdata.herolv = g_hero->getLVValue();
	myrankdata.herosex = g_hero->getSex();
	myrankdata.heroval = g_nature->getPastDays();
	RankItem* node = RankItem::create(&myrankdata);
	node->setPosition(Vec2(360, 130));
	this->addChild(node);
}
void RankLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	m_loadlbl = Label::createWithTTF(CommonFuncs::gbk2utf("加载中..."), "fonts/STXINGKA.TTF", 28);
	m_loadlbl->setColor(Color3B(0, 0, 0));
	m_loadlbl->setPosition(Vec2(320, 600));
	this->addChild(m_loadlbl);
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
	str = StringUtils::format("%d天", data->heroval);
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