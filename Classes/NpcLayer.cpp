#include "NpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "FightLayer.h"

NpcLayer::NpcLayer()
{
	isShowWord = false;
	wordstr = "";
	m_wordcount = 0; 
}


NpcLayer::~NpcLayer()
{
}

NpcLayer* NpcLayer::create(std::string addrname)
{
	NpcLayer *pRet = new NpcLayer();
	if (pRet && pRet->init(addrname))
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

bool NpcLayer::init(std::string addrid)
{
	Node* csbnode = CSLoader::createNode("npcLayer.csb");
	this->addChild(csbnode);

	m_addrstr = addrid;

	MapData mdata = GlobalData::map_maps[addrid];

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("ui/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	typeimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onBack, this));

	int ncpsize = mdata.npcs.size();
	for (int i = 0; i < ncpsize; i++)
	{
		Node* npcitem = CSLoader::createNode("npcNode.csb");
		npcitem->setName(mdata.npcs[i]);
		npcitem->setPosition(Vec2(360, 415 - 155 * i));
		csbnode->addChild(npcitem);

		cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)npcitem->getChildByName("npcname");
		npcname->setString(GlobalData::map_npcs[mdata.npcs[i]].name);

		cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
		talkbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemTalk, this));
		talkbtn->setTag(i);

		cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
		onFight->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemFight, this));

		cocos2d::ui::Button* onExchange = (cocos2d::ui::Button*)npcitem->getChildByName("exchgbtn");
		onExchange->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemExchange, this));
		if (GlobalData::map_npcs[mdata.npcs[i]].exchgres.size() <= 0)
			onExchange->setVisible(false);
	}


	m_talkScroll = UIScroll::create(610.0f, 260.f);
	m_talkScroll->setPosition(Vec2(360, 615));
	addChild(m_talkScroll);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void NpcLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void NpcLayer::onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		NpcData npc = GlobalData::map_npcs[GlobalData::map_maps[m_addrstr].npcs[node->getTag()]];

		if (isShowWord && wordstr.length() > 0)
		{
			m_wordlbl->unscheduleAllSelectors();
			m_wordlbl->setString(wordstr);
		}

		m_wordcount = 0;
		wordstr = StringUtils::format("%s%s%s", npc.name,CommonFuncs::gbk2utf("：").c_str(),  npc.words[0].c_str());

		m_wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 25);
		m_wordlbl->setColor(Color3B::BLACK);
		std::string npcname = npc.name;

		for (int i = 0; i < npcname.size() / 3; i++)
		{
			//vec_cColorIndex.push_back(i);
		}
		m_wordlbl->setVisible(false);
		m_talkScroll->addEventLabel(m_wordlbl);
		showTypeText();
	}
}

void NpcLayer::onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = node->getParent()->getName();
		Director::getInstance()->getRunningScene()->addChild(FightLayer::create(m_addrstr, npcid), 1, "fightlayer");
	}
}

void NpcLayer::onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

	}
}

void NpcLayer::showTypeText()
{
	m_wordlbl->schedule([&](float dt){
		isShowWord = true;
		m_wordcount += 3;
		std::string str = wordstr.substr(0, m_wordcount);
		m_wordlbl->setString(str);
		m_wordlbl->setVisible(true);
		
		if (m_wordcount >= wordstr.length())
		{
			isShowWord = false;
			m_wordlbl->unschedule("schedule_typecallback");
		}
	}, 0.03f, "schedule_typecallback");
}