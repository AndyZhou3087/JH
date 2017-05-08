#include "NpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "FightLayer.h"
#include "GameScene.h"
#include "MapLayer.h"

NpcLayer::NpcLayer()
{
	isShowWord = false;
	m_wordcount = 0; 
	m_wordindex = 0;
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

	m_npctalkbg = (cocos2d::ui::Widget*)csbnode->getChildByName("npctalkbg");
	m_npctalkbg->setOpacity(0);

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

		if (vec_wordstr.size() > 0 && m_wordindex >= vec_wordstr.size())
			return;

		if (isShowWord)
		{
			if (vec_wordstr[m_wordindex].length() > 0)
			{
				m_wordcount = 0;
				m_wordindex++;

				if (m_wordindex >= vec_wordstr.size())
					return;

				this->unscheduleAllCallbacks();
				m_wordlbl->unscheduleAllCallbacks();
				int index = 0;
				while (m_wordlbl->getLetter(index) != NULL)
				{
					m_wordlbl->getLetter(index)->setScale(1);
					index++;
				}

				checkWordLblColor(vec_wordstr[m_wordindex]);
				float dt = 0.0f;
				for (unsigned int i = m_wordindex; i < vec_wordstr.size(); i++)
				{
					this->scheduleOnce(schedule_selector(NpcLayer::showTypeText), dt);
					dt += vec_wordstr[i].size() / 3 * 0.1f + 1.0f;
				}
				return;
			}
		}
		else
		{
			m_npctalkbg->runAction(FadeIn::create(0.2f));
		}

		m_wordcount = 0;
		std::string wordstr;

		bool isplotMissioning = false;

		int curplot = GlobalData::getPlotMissionIndex();
		if (GlobalData::vec_PlotMissionData[curplot].snpc.compare(npc.id) == 0 && GlobalData::vec_PlotMissionData[curplot].status != M_DONE)
		{
			isplotMissioning = true;
			GlobalData::vec_PlotMissionData[curplot].status = M_DOING;

			for (unsigned int m = 0; m < GlobalData::vec_PlotMissionData[curplot].words.size(); m++)
			{
				wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), GlobalData::vec_PlotMissionData[curplot].words[m].c_str());
				vec_wordstr.push_back(wordstr);
				wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), CommonFuncs::gbk2utf("：").c_str(), GlobalData::vec_PlotMissionData[curplot].mywords[m].c_str());
				vec_wordstr.push_back(wordstr);
			}
		}

		if (GlobalData::vec_PlotMissionData[curplot].status == M_DOING)
		{
			if (GlobalData::vec_PlotMissionData[curplot].dnpc.compare(npc.id) == 0)
			{
				isplotMissioning = true;

				if (GlobalData::vec_PlotMissionData[curplot].type == 0)
				{
					GlobalData::vec_PlotMissionData[curplot].status = M_DONE;

					GlobalData::setUnlockChapter(GlobalData::vec_PlotMissionData[curplot].unlockchapter);

					GlobalData::setPlotMissionIndex(curplot + 1);
					if (g_maplayer != NULL)
						g_maplayer->updateUnlockChapter();
				}

				for (unsigned int m = 0; m < GlobalData::vec_PlotMissionData[curplot].bossword.size(); m++)
				{
					wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), GlobalData::vec_PlotMissionData[curplot].bossword[m].c_str());
					vec_wordstr.push_back(wordstr);
				}
			}
		}
		GlobalData::savePlotMissionStatus();
		if (!isplotMissioning)
		{
			wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), npc.words[0].c_str());
			vec_wordstr.push_back(wordstr);
		}

		if (vec_wordstr.size() > 0)
		{
			checkWordLblColor(vec_wordstr[0]);

			float dt = 0.0f;
			for (unsigned int i = 0; i < vec_wordstr.size(); i++)
			{
				this->scheduleOnce(schedule_selector(NpcLayer::showTypeText), dt);
				dt += vec_wordstr[i].size() / 3 * 0.1f + 1.0f;
			}
		}
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

void NpcLayer::showTypeText(float dt)
{
	m_wordlbl->schedule([&](float dt){
		isShowWord = true;
		m_wordcount += 3;
		int letterindex = m_wordcount / 3 - 1;
		m_wordlbl->getLetter(letterindex)->setScale(1.0f);
		if (m_wordcount >= m_wordlbl->getString().length())
		{
			m_wordindex++;
			if (m_wordindex < vec_wordstr.size())
			{
				checkWordLblColor(vec_wordstr[m_wordindex]);
			}
			else
			{
				this->scheduleOnce(schedule_selector(NpcLayer::removeNpcWord), 2.0f);
			}
			m_wordcount = 0;
			isShowWord = false;
			m_wordlbl->unschedule("schedule_typecallback");
		}

	}, 0.1f, "schedule_typecallback");
}
void NpcLayer::checkWordLblColor(std::string wordstr)
{
	m_wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 25);
	m_wordlbl->setLineBreakWithoutSpace(true);
	m_wordlbl->setMaxLineWidth(610);
	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		m_wordlbl->getLetter(index)->setScale(0);
		index++;
	}

	std::map<std::string, NpcData>::iterator it;
	for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = GlobalData::map_npcs[it->first].name;
		std::size_t findpos = wordstr.find(npcname);
		if (findpos != std::string::npos)
		{
			int sindex = (findpos + 1) / 3;
			int len = npcname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				m_wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
			}
		}
	}
	std::size_t findpos = wordstr.find(g_hero->getMyName());
	if (findpos != std::string::npos)
	{
		int sindex = (findpos + 1) / 3;
		int len = g_hero->getMyName().size() / 3;
		for (int i = sindex; i < sindex + len; i++)
		{
			m_wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
		}
	}

	m_talkScroll->addEventLabel(m_wordlbl);
}

void NpcLayer::removeNpcWord(float dt)
{
	isShowWord = false;
	m_wordindex= 0;
	m_npctalkbg->runAction(FadeOut::create(0.2f));
	vec_wordstr.clear();
	m_talkScroll->clean();
}