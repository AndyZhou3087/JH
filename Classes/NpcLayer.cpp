﻿#include "NpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "FightLayer.h"
#include "GameScene.h"
#include "MapLayer.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "GameDataSave.h"
#include "ExchangeLayer.h"
#include "HintBox.h"
#include "CannotTouchLayer.h"
#include "MyActionProgressTimer.h"

std::string replacestr[] = {"少侠","小子","小兄弟","小伙子", "兄台"};
std::string areplacestr[] = {"女侠","小娘子","小姑娘","小姑娘","姑娘"};
NpcLayer::NpcLayer()
{
	isShowWord = false;
	m_wordcount = 0; 
	m_wordindex = 0;
}


NpcLayer::~NpcLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);

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
	m_csbnode = CSLoader::createNode("npcLayer.csb");
	this->addChild(m_csbnode);

	m_npctalkbg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("npctalkbg");
	m_npctalkbg->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onTalkbg, this));
	m_npctalkbg->setOpacity(0);

	m_addrstr = addrid;

	MapData mdata = GlobalData::map_maps[addrid];

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(mdata.cname);

	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.jpg", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onBack, this));

	m_scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");

	m_scrollview->setScrollBarEnabled(false);
	m_scrollview->setBounceEnabled(true);

	int ncpsize = mdata.npcs.size();
	int itemheight = 153;
	int innerheight = itemheight * ncpsize;
	int contentheight = m_scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollview->setInnerContainerSize(Size(650, innerheight));

	cocos2d::ui::Widget* hintdown = (cocos2d::ui::Widget*)m_csbnode->getChildByName("hintdown");
	if (ncpsize > 3)
	{
		hintdown->runAction(RepeatForever::create(Blink::create(2, 2)));
	}
	else
	{
		hintdown->setVisible(false);
	}

	for (int i = 0; i < ncpsize; i++)
	{
		Node* npcitem = CSLoader::createNode("npcNode.csb");
		npcitem->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		m_scrollview->addChild(npcitem, 0, mdata.npcs[i]);

		cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)npcitem->getChildByName("npcname");
		npcname->setString(GlobalData::map_npcs[mdata.npcs[i]].name);

		cocos2d::ui::ImageView* npcrsi = (cocos2d::ui::ImageView*)npcitem->getChildByName("npcrsi");
		npcrsi->setPositionX(npcname->getPositionX() + npcname->getContentSize().width + 25);

		int count = checkFightCount(mdata.npcs[i]);

		std::string rsistr;
		if (count > 0)
		{
			if (count <= 3)
				rsistr = "ui/npcrsi0.png";
			else
				rsistr = "ui/npcrsi1.png";
		}
		else
		{
			if (count >= -3)
				rsistr = "ui/npcrsi2.png";
			else
				rsistr = "ui/npcrsi1.png";
		}

		npcrsi->loadTexture(rsistr, cocos2d::ui::TextureResType::PLIST);

		cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
		talkbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemTalk, this));
		talkbtn->setTag(i);

		cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
		onFight->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemFight, this));

		cocos2d::ui::Button* onExchange = (cocos2d::ui::Button*)npcitem->getChildByName("exchgbtn");
		onExchange->setTag(i);
		onExchange->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemExchange, this));
		if (GlobalData::map_npcs[mdata.npcs[i]].exchgres.size() <= 0)
			onExchange->setVisible(false);

		if (mdata.npcs[i].compare("n009") == 0)
		{
			talkbtn->setTitleText(CommonFuncs::gbk2utf("吃饭"));
			talkbtn->setTag( 10 * i);
			talkbtn->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
			onFight->setTitleText(CommonFuncs::gbk2utf("睡觉"));
			onFight->setTag(10*i+1);
			onFight->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
			onExchange->setVisible(true);
			onExchange->setTitleText(CommonFuncs::gbk2utf("喝酒"));
			onExchange->setTag(10*i+2);
			onExchange->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onHostelAction, this));
		}
		if (ncpsize == 10)
			onExchange->setVisible(false);
	}

	for (int i = 1; i >= 0;i--)
		updatePlotUI(i);


	m_talkScroll = UIScroll::create(610.0f, 250);
	m_talkScroll->setPosition(Vec2(360, 652));
	addChild(m_talkScroll);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
	this->scheduleOnce(schedule_selector(NpcLayer::delayShowNewerGuide), 0.2f);
	return true;
}

void NpcLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void NpcLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void NpcLayer::onTalkbg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int size = vec_wordstr.size();

		if (size > 0 && m_wordindex >= size)
			return;

		if (isShowWord)
		{
			if (vec_wordstr.size() <= 0)
				return;

			if (vec_wordstr[m_wordindex].length() > 0)
			{
				fastShowWord();
			}
		}
	}
}

void NpcLayer::onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		NpcData npc = GlobalData::map_npcs[GlobalData::map_maps[m_addrstr].npcs[node->getTag()]];

		if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc.compare(npc.id) == 0 && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].words.size() <= 0)
			return;
		else if (GlobalData::vec_BranchPlotMissionData[GlobalData::getBranchPlotMissionIndex()].snpc.compare(npc.id) == 0 && GlobalData::vec_BranchPlotMissionData[GlobalData::getBranchPlotMissionIndex()].words.size() <= 0)
			return;

		int size = vec_wordstr.size();
		if (m_wordindex >= size && size > 0)
			return;
		if (isShowWord)
		{
			if (size <= 0)
			{
				int index = 0;
				while (m_wordlbl->getLetter(index) != NULL)
				{
					m_wordlbl->getLetter(index)->setScale(1);
					index++;
				}
				this->scheduleOnce(schedule_selector(NpcLayer::removeNpcWord), 0.5f);
				return;
			}
			if (vec_wordstr[m_wordindex].length() > 0)
			{
				fastShowWord();
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
		PlotMissionData pdata = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()];
		int plottype = 1;
		if (pdata.snpc.compare(npc.id) == 0 || (pdata.dnpc.compare(npc.id) == 0))
			plottype = 0;

		isplotMissioning = doCheckPlotMisson(plottype, npc);

		if (!isplotMissioning)
		{
			wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), npc.words[0].c_str());
			if (g_hero->getHeadID() == 4)
				wordstr = replaceSexWord(wordstr);
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
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = node->getParent()->getName();

		std::string protectword;
		
		if (npcid.compare("n004") == 0 && checkFightCount("n004") < 0)
		{
			protectword = CommonFuncs::gbk2utf("田伯光：就凭你？还不够我一刀的，我不杀无名小卒，再去练几年吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！");
			
		}
		else if (npcid.compare("n005") == 0 && checkFightCount("n005") < 0)
		{
			protectword = CommonFuncs::gbk2utf("平一指：少侠，你现在还太弱，还是在修炼修炼吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！");
			if (g_hero->getHeadID() == 4)
				protectword = replaceSexWord(protectword);	
		}
		if (protectword.length() > 0)
		{
			if (isShowWord)
				return;

			m_npctalkbg->runAction(FadeIn::create(0.2f));
			checkWordLblColor(protectword);
			showTypeText(0);

			return;
		}

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(FightLayer::create(m_addrstr, npcid), 4, "fightlayer");
	}
}

void NpcLayer::onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = GlobalData::map_maps[m_addrstr].npcs[node->getTag()];
		g_gameLayer->addChild(ExchangeLayer::create(npcid), 4);
	}
}

void NpcLayer::onHostelAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int itemindex = node->getTag() / 10;
		int actionIndex = node->getTag()%10;
		int lostval[] = { 1, 2, 2 };
		int silver = 0;

		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			if (MyPackage::vec_packages[i].strid.compare("80") == 0)
			{
				silver = MyPackage::vec_packages[i].count;
				break;
			}
		}

		if (silver < lostval[actionIndex])
		{
			HintBox* hintBox = HintBox::create(CommonFuncs::gbk2utf("我靠。。。银子没带够！！"));
			addChild(hintBox);
		}
		else
		{
			MyPackage::cutone("80", lostval[actionIndex]);

			CannotTouchLayer* layer = CannotTouchLayer::create();
			g_gameLayer->addChild(layer, 5, "notouchlayer");

			std::string itemname = GlobalData::map_maps[m_addrstr].npcs[itemindex];
			Node* npcitem = m_scrollview->getChildByName(itemname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			talkbtn->setVisible(false);

			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
			onFight->setVisible(false);

			cocos2d::ui::Button* onExchange = (cocos2d::ui::Button*)npcitem->getChildByName("exchgbtn");
			onExchange->setVisible(false);

			cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)npcitem->getChildByName("progressbg");
			pbg->setVisible(true);
			cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)npcitem->getChildByName("loadingbar");
			pbar->setVisible(true);

			cocos2d::ui::Text* ptext = (cocos2d::ui::Text*)npcitem->getChildByName("actiontext");
			ptext->setVisible(true);

			if (actionIndex == 0)
			{
				ptext->setString(CommonFuncs::gbk2utf("吃饭中..."));
				float hungervale = g_hero->getHungerValue();
				int addvalue = 40;
				if (addvalue + hungervale > g_hero->getMaxHungerValue())
					g_hero->setHungerValue(g_hero->getMaxHungerValue());
				else
					g_hero->recoverHunger(addvalue);
			}
			else if (actionIndex == 1)
			{
				ptext->setString(CommonFuncs::gbk2utf("睡觉八小时"));
				int exminute = 480;
				g_nature->setTimeInterval(exminute * NORMAL_TIMEINTERVAL * 1.0f / (ACTION_BAR_TIME * TIMESCALE));
				g_hero->sleep(TIMESCALE * ACTION_BAR_TIME, exminute / 60);
			}

			else if (actionIndex == 2)
			{
				ptext->setString(CommonFuncs::gbk2utf("喝酒中..."));
				float value = g_hero->getSpiritValue();
				float addvalue = 40;
				if (addvalue + value > g_hero->getMaxSpiritValue())
					g_hero->setSpiritValue(g_hero->getMaxSpiritValue());
				else
					g_hero->setSpiritValue(addvalue + value);
			}
			pbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(NpcLayer::actionOver, this, npcitem)), NULL));
		}
	}
}

void NpcLayer::actionOver(Ref* pSender, Node* item)
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)item->getChildByName("talkbtn");
	talkbtn->setVisible(true);

	cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)item->getChildByName("fightbtn");
	onFight->setVisible(true);

	cocos2d::ui::Button* onExchange = (cocos2d::ui::Button*)item->getChildByName("exchgbtn");
	onExchange->setVisible(true);

	cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)item->getChildByName("progressbg");
	pbg->setVisible(false);
	cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
	pbar->setPercent(0);
	pbar->setVisible(false);

	cocos2d::ui::Widget* ptext = (cocos2d::ui::Widget*)item->getChildByName("actiontext");
	ptext->setVisible(false);

	g_gameLayer->removeChildByName("notouchlayer");
}

bool NpcLayer::doCheckPlotMisson(int type, NpcData npcdata)
{
	bool isplotMissioning = false;
	std::string wordstr;
	int curplot = 0;
	PlotMissionData* plotData = NULL;
	if (type == 0)
	{
		curplot = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[curplot];
	}
	else
	{
		curplot = GlobalData::getBranchPlotMissionIndex();
		if (GlobalData::vec_BranchPlotMissionData[curplot].unlockchapter <= GlobalData::getUnlockChapter())
			plotData = &GlobalData::vec_BranchPlotMissionData[curplot];
	}
	
	if (plotData != NULL)
	{
		if (plotData->snpc.compare(npcdata.id) == 0 && plotData->status != M_DONE)
		{
			isplotMissioning = true;
			plotData->status = M_DOING;
			updatePlotUI(type);
			for (unsigned int m = 0; m < plotData->words.size(); m++)
			{
				wordstr = StringUtils::format("%s%s%s", npcdata.name, CommonFuncs::gbk2utf("：").c_str(), plotData->words[m].c_str());
				if (g_hero->getHeadID() == 4)
					wordstr = replaceSexWord(wordstr);
				vec_wordstr.push_back(wordstr);

				wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), CommonFuncs::gbk2utf("：").c_str(), plotData->mywords[m].c_str());
				vec_wordstr.push_back(wordstr);
			}
			if (g_maplayer != NULL)
				g_maplayer->updataPlotMissionIcon(type);
		}

		if (plotData->status == M_DOING)
		{
			if (plotData->dnpc.compare(npcdata.id) == 0)
			{
				isplotMissioning = true;

				if (plotData->type == 0)
				{
					plotData->status = M_DONE;
					getWinRes(type);

					int unlockchapter = 0;
					if (type == 0)
					{
						unlockchapter = plotData->unlockchapter;
						GlobalData::setUnlockChapter(unlockchapter);

						GlobalData::setPlotMissionIndex(curplot + 1);
					}
					else
					{
						GlobalData::setBranchPlotMissionIndex(curplot + 1);
					}

					updatePlotUI(type);

					if (g_maplayer != NULL)
					{
						g_maplayer->updataPlotMissionIcon(type);
						if (unlockchapter > 0 && type == 0 && unlockchapter <= MAXCHAPTER)
						{
							g_maplayer->updataPlotMissionIcon(1);
							g_maplayer->scheduleOnce(schedule_selector(MapLayer::showUnlockLayer), 0.5f);
						}

						if (plotData->dnpc.compare("n089") == 0)
						{
							g_maplayer->scheduleOnce(schedule_selector(MapLayer::showEndAnim), 1.0f);
						}
					}

				}

				for (unsigned int m = 0; m < plotData->bossword.size(); m++)
				{
					wordstr = StringUtils::format("%s%s%s", npcdata.name, CommonFuncs::gbk2utf("：").c_str(), plotData->bossword[m].c_str());
					if (g_hero->getHeadID() == 4)
						wordstr = replaceSexWord(wordstr);
					vec_wordstr.push_back(wordstr);

				}
			}
		}
		if (isplotMissioning)
		{
			if (type == 0)
			{
				GlobalData::savePlotMissionStatus();
				if (plotData->status == M_DONE)
					updatePlotUI(1);
			}
			else
				GlobalData::saveBranchPlotMissionStatus();
		}
	}

	return isplotMissioning;
}

void NpcLayer::showTypeText(float dt)
{
	m_wordlbl->schedule([&](float dt){
		isShowWord = true;
		m_wordcount += 3;
		int letterindex = m_wordcount / 3 - 1;
		m_wordlbl->getLetter(letterindex)->setScale(1.0f);
		int len = m_wordlbl->getString().length();
		if (m_wordcount >= len)
		{
			m_wordindex++;
			int size = vec_wordstr.size();
			if (m_wordindex < size)
			{
				checkWordLblColor(vec_wordstr[m_wordindex]);
			}
			else
			{
				this->scheduleOnce(schedule_selector(NpcLayer::removeNpcWord), 2.0f);
			}
			m_wordcount = 0;
			//isShowWord = false;
			m_wordlbl->unschedule("schedule_typecallback");
		}

	}, 0.1f, "schedule_typecallback");
}
void NpcLayer::checkWordLblColor(std::string wordstr)
{
	m_wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 26);
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
			int sindex = findpos / 3;
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
		int sindex = findpos / 3;
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
	m_wordcount = 0;
	m_wordindex= 0;
	m_npctalkbg->runAction(FadeOut::create(0.2f));
	vec_wordstr.clear();
	m_talkScroll->clean();
}

void NpcLayer::updatePlotUI(int type)
{
	int plotIndex = 0;
	PlotMissionData* plotData;

	if (type == 0)
	{
		plotIndex = GlobalData::getPlotMissionIndex();
		plotData = &GlobalData::vec_PlotMissionData[plotIndex];
	}
	else
	{
		plotIndex = GlobalData::getBranchPlotMissionIndex();
		plotData = &GlobalData::vec_BranchPlotMissionData[plotIndex];
	}

	std::string snpc = plotData->snpc;
	std::string dnpc = plotData->dnpc;
	int ncpsize = GlobalData::map_maps[m_addrstr].npcs.size();
	for (int i = 0; i < ncpsize; i++)
	{
		Node* npcitem = m_scrollview->getChildByName(GlobalData::map_maps[m_addrstr].npcs[i]);
		cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
		cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

		std::string smissionname = StringUtils::format("m%d_0", type);
		if (talkbtn->getChildByName(smissionname) != NULL)
			talkbtn->removeChildByName(smissionname);
		if (snpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0)
		{
			if (plotData->status == M_NONE)
			{
				if (plotData->words.size() <= 0)
				{
					plotData->status = M_DOING;
				}
				else
				{
					std::string miconstr = StringUtils::format("ui/mapmission%d_0.png", type);
					Sprite* micon = Sprite::createWithSpriteFrameName(miconstr);
					micon->setScale(0.6f);
					micon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
					talkbtn->addChild(micon, 0, smissionname);
				}
			}
		}

		std::string dmissionname = StringUtils::format("m%d_1", type);

		if (onFight->getChildByName(dmissionname) != NULL)
			onFight->removeChildByName(dmissionname);

		if (talkbtn->getChildByName(dmissionname) != NULL)
			talkbtn->removeChildByName(dmissionname);

		bool ret = false;
		if (plotData->mapid.length() > 0)
		{
			if (plotData->mapid.compare(m_addrstr) == 0)
				ret = true;
		}
		else
		{
			ret = true;
		}

		if (dnpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0 && ret)
		{

			if (plotData->status == M_DOING)
			{
				std::string diconstr = StringUtils::format("ui/mapmission%d_1.png", type);
				Sprite* dicon = Sprite::createWithSpriteFrameName(diconstr);
				dicon->setScale(0.6f);
				if (plotData->type == 1)
				{
					dicon->setPosition(Vec2(onFight->getContentSize().width - 10, onFight->getContentSize().height - 10));
					onFight->addChild(dicon, 0, dmissionname);
				}
				else
				{
					dicon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
					talkbtn->addChild(dicon, 0, dmissionname);
				}
			}
		}
	}
}

void NpcLayer::fastShowWord()
{
	m_wordcount = 0;
	m_wordindex++;

	this->unscheduleAllCallbacks();
	m_wordlbl->unscheduleAllCallbacks();
	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setScale(1);
		index++;
	}
	int wordsize = vec_wordstr.size();
	if (m_wordindex >= wordsize)
	{
		this->scheduleOnce(schedule_selector(NpcLayer::removeNpcWord), 2.0f);
		return;
	}

	checkWordLblColor(vec_wordstr[m_wordindex]);
	float dt = 0.0f;
	for (unsigned int i = m_wordindex; i < vec_wordstr.size(); i++)
	{
		this->scheduleOnce(schedule_selector(NpcLayer::showTypeText), dt);
		dt += vec_wordstr[i].size() / 3 * 0.1f + 1.0f;
	}
}

void NpcLayer::getWinRes(int type)
{
	std::vector<PackageData> tempResData;//背包满了，保存到临时存放点
	std::vector<std::string> winres;
	if (type == 0)
		winres = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].rewords;
	else
		winres = GlobalData::vec_BranchPlotMissionData[GlobalData::getBranchPlotMissionIndex()].rewords;
	int addret = 0;
	PackageData data;
	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());

		if (res != 0)
		{
			bool isfind = false;
			std::string strid = StringUtils::format("%d", res / 1000);
			data.strid = strid;
			data.count = res % 1000;

			for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
			{
				ResData rdata = GlobalData::vec_resData[i];
				if (atoi(rdata.strid.c_str()) == res / 1000)
				{
					isfind = true;
					data.type = rdata.type - 1;
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, std::vector<BuildActionData>>::iterator it;
				for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
				{
					std::vector<BuildActionData> vec_bactData = GlobalData::map_buidACData[it->first];

					for (unsigned int m = 0; m < vec_bactData.size(); m++)
					{
						BuildActionData bdata = vec_bactData[m];
						if (atoi(bdata.icon) == res / 1000)
						{
							isfind = true;
							data.strid = bdata.icon;
							data.count = res % 1000;
							data.type = bdata.type - 1;
							break;
						}
					}
					if (isfind)
						break;
				}
			}
			addret = MyPackage::add(data);
		}
		else
		{
			std::string strid = winres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (winres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF(winres[i]) && !GlobalData::tempHasgf(winres[i]))
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.lv = 0;
					data.type = gfdata.type - 1;
					addret = MyPackage::add(data);
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = GlobalData::map_equips[ite->first];
					if (winres[i].compare(edata.id) == 0)
					{
						data.strid = edata.id;
						data.count = 1;
						data.type = edata.type - 1;
						data.goodvalue = 100;
						data.extype = edata.extype;
						addret = MyPackage::add(data);
						break;
					}
				}
			}
		}
		if (addret < 0 && data.count > 0)
		{
			tempResData.push_back(data);
		}
	}

	if (tempResData.size() > 0)
	{
		std::string datastr = GameDataSave::getInstance()->getTempStorage(m_addrstr);

		for (unsigned int i = 0; i < tempResData.size(); i++)
		{
			std::string sepstr;
			if (datastr.size() <= 0)
				sepstr = "";
			else
				sepstr = ";";
			std::string onestr = StringUtils::format("%s%s-%d-%d-%d-%d-%d-%d",sepstr.c_str(), tempResData[i].strid.c_str(), tempResData[i].type, tempResData[i].count, tempResData[i].extype, tempResData[i].lv, tempResData[i].exp, tempResData[i].goodvalue);
			datastr.append(onestr);
		}
		GameDataSave::getInstance()->setTempStorage(m_addrstr, datastr);
	}

}

void NpcLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	Node* npcitem = m_scrollview->getChildByName(GlobalData::map_maps[m_addrstr].npcs[0]);
	cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
	nodes.push_back(talkbtn);
	g_gameLayer->showNewerGuide(step, nodes);
}

void NpcLayer::delayShowNewerGuide(float dt)
{
	if (NewerGuideLayer::checkifNewerGuide(49) && m_addrstr.compare("m1-4") == 0)
		showNewerGuide(49);
	else if (NewerGuideLayer::checkifNewerGuide(50) && m_addrstr.compare("m1-3") == 0)
		showNewerGuide(50);
}

int NpcLayer::checkFightCount(std::string npcid)
{

	int npcdf = GlobalData::map_npcs[npcid].df;
	int npcatk = GlobalData::map_npcs[npcid].atk;

	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	if (npchurt <= 0)
		npchurt = 1;

	int npclife = GlobalData::map_npcs[npcid].life;
	
	int heroVsBossCount = npclife % npchurt == 0 ? npclife / npchurt : (npclife / npchurt + 1);

	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	fminack = 0.1f * npcatk;
	intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;

	if (herohurt <= 0)
		herohurt = 1;

	int intlife = int(g_hero->getLifeValue() + 1);
	int bossVsHeroCount = intlife % herohurt == 0 ? intlife / herohurt : (intlife / herohurt + 1);

	if (heroVsBossCount <= bossVsHeroCount)
		return heroVsBossCount;
	else
		return -bossVsHeroCount;
}

std::string NpcLayer::replaceSexWord(std::string dstr)
{
	std::string retstr = dstr;
	int size = sizeof(replacestr) / sizeof(replacestr[0]);
	for (int i = 0; i < size; i++)
	{
		retstr = CommonFuncs::replace_all(dstr, CommonFuncs::gbk2utf(replacestr[i].c_str()), CommonFuncs::gbk2utf(areplacestr[i].c_str()));
	}
	return retstr;
}