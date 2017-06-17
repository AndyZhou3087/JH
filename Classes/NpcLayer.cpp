#include "NpcLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "GlobalData.h"
#include "FightLayer.h"
#include "GameScene.h"
#include "MapLayer.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"

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

	std::string snpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc;
	std::string dnpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].dnpc;

	int ncpsize = mdata.npcs.size();
	int itemheight = 153;
	int innerheight = itemheight * ncpsize;
	int contentheight = m_scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollview->setInnerContainerSize(Size(650, innerheight));

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
		onExchange->addTouchEventListener(CC_CALLBACK_2(NpcLayer::onItemExchange, this));
		if (GlobalData::map_npcs[mdata.npcs[i]].exchgres.size() <= 0)
			onExchange->setVisible(false);

		if (snpc.compare(mdata.npcs[i]) == 0 && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].status == M_NONE)
		{
			if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].words.size() > 0)
			{
				Sprite* micon = Sprite::createWithSpriteFrameName("ui/mapmission0.png");
				micon->setScale(0.6f);
				micon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
				talkbtn->addChild(micon, 0, "m0");
			}
		}
		if (dnpc.compare(mdata.npcs[i]) == 0 && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].status == M_DOING)
		{
			Sprite* dicon = Sprite::createWithSpriteFrameName("ui/mapmission1.png");
			dicon->setScale(0.6f);
			if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].type == 1)
			{
				dicon->setPosition(Vec2(onFight->getContentSize().width - 10, onFight->getContentSize().height - 10));
				onFight->addChild(dicon, 0, "m1");
			}
			else
			{
				dicon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
				talkbtn->addChild(dicon, 0, "m1");
			}
		}
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
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
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
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* node = (Node*)pSender;
		NpcData npc = GlobalData::map_npcs[GlobalData::map_maps[m_addrstr].npcs[node->getTag()]];

		if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc.compare(npc.id) == 0 && GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].words.size() <= 0)
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

		int curplot = GlobalData::getPlotMissionIndex();
		if (GlobalData::vec_PlotMissionData[curplot].snpc.compare(npc.id) == 0 && GlobalData::vec_PlotMissionData[curplot].status != M_DONE)
		{
			isplotMissioning = true;
			GlobalData::vec_PlotMissionData[curplot].status = M_DOING;
			updatePlotUI();
			for (unsigned int m = 0; m < GlobalData::vec_PlotMissionData[curplot].words.size(); m++)
			{
				wordstr = StringUtils::format("%s%s%s", npc.name, CommonFuncs::gbk2utf("：").c_str(), GlobalData::vec_PlotMissionData[curplot].words[m].c_str());
				vec_wordstr.push_back(wordstr);
				wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), CommonFuncs::gbk2utf("：").c_str(), GlobalData::vec_PlotMissionData[curplot].mywords[m].c_str());
				vec_wordstr.push_back(wordstr);
			}
			if (g_maplayer != NULL)
				g_maplayer->updataPlotMissionIcon();
		}

		if (GlobalData::vec_PlotMissionData[curplot].status == M_DOING)
		{
			if (GlobalData::vec_PlotMissionData[curplot].dnpc.compare(npc.id) == 0)
			{
				isplotMissioning = true;

				if (GlobalData::vec_PlotMissionData[curplot].type == 0)
				{
					GlobalData::vec_PlotMissionData[curplot].status = M_DONE;
					updatePlotUI();

					getWinRes();

					int unlockchapter = GlobalData::vec_PlotMissionData[curplot].unlockchapter;
					GlobalData::setUnlockChapter(GlobalData::vec_PlotMissionData[curplot].unlockchapter);

					GlobalData::setPlotMissionIndex(curplot + 1);

					if (g_maplayer != NULL)
					{
						g_maplayer->updataPlotMissionIcon();
						if (unlockchapter > 0)
							g_maplayer->scheduleOnce(schedule_selector(MapLayer::showUnlockLayer), 3.0f);
					}
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
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* node = (Node*)pSender;
		std::string npcid = node->getParent()->getName();

		if (g_hero->getLVValue() + 1 < 10)
		{
			std::string protectword;
			if (npcid.compare("n004") == 0)
			{
				protectword = CommonFuncs::gbk2utf("田伯光：就凭你？还不够我一刀的，我不杀无名小卒，再去练几年吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！");
			}
			else if (npcid.compare("n005") == 0)
			{
				protectword = CommonFuncs::gbk2utf("平一指：少侠，你现在还太弱，还是在修炼修炼吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！");

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
		}

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(FightLayer::create(m_addrstr, npcid), 4, "fightlayer");
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

void NpcLayer::updatePlotUI()
{
	std::string snpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].snpc;
	std::string dnpc = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].dnpc;
	int ncpsize = GlobalData::map_maps[m_addrstr].npcs.size();
	for (int i = 0; i < ncpsize; i++)
	{
		Node* npcitem = m_scrollview->getChildByName(GlobalData::map_maps[m_addrstr].npcs[i]);
		cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
		cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

		talkbtn->removeChildByName("m0");
		if (snpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0)
		{
			if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].status == M_NONE)
			{
				if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].words.size() > 0)
				{

					Sprite* micon = Sprite::createWithSpriteFrameName("ui/mapmission0.png");
					micon->setScale(0.6f);
					micon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
					talkbtn->addChild(micon, 0, "m0");
				}
			}
		}
		onFight->removeChildByName("m1");
		talkbtn->removeChildByName("m1");
		if (dnpc.compare(GlobalData::map_maps[m_addrstr].npcs[i]) == 0)
		{
			if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].status == M_DOING)
			{
				Sprite* dicon = Sprite::createWithSpriteFrameName("ui/mapmission1.png");
				dicon->setScale(0.6f);
				if (GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].type == 1)
				{
					dicon->setPosition(Vec2(onFight->getContentSize().width - 10, onFight->getContentSize().height - 10));
					onFight->addChild(dicon, 0 , "m1");
				}
				else
				{
					dicon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
					talkbtn->addChild(dicon, 0, "m1");
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
	if (m_wordindex >= vec_wordstr.size())
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

void NpcLayer::getWinRes()
{
	std::vector<std::string> winres = GlobalData::vec_PlotMissionData[GlobalData::getPlotMissionIndex()].rewords;
	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());

		if (res != 0)
		{
			PackageData data;
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
					data.name = rdata.cname;
					data.desc = rdata.desc;
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
							data.desc = bdata.desc;
							data.name = bdata.cname;
							break;
						}
					}
					if (isfind)
						break;
				}
			}
			MyPackage::add(data);
		}
		else
		{
			PackageData data;
			std::string strid = winres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = GlobalData::map_wgngs.begin(); it != GlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = GlobalData::map_wgngs[it->first];
				if (winres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF(winres[i]))
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.lv = 0;
					if (data.strid.substr(0, 1).compare("w") == 0)
					{
						data.type = W_GONG;
					}
					else if (data.strid.substr(0, 1).compare("x") == 0)
					{
						data.type = N_GONG;
					}
					data.desc = gfdata.desc;
					data.name = gfdata.cname;
					MyPackage::add(data);
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
						data.desc = edata.desc;
						data.name = edata.cname;
						data.type = edata.type - 1;
						data.goodvalue = 100;
						data.extype = edata.extype;
						MyPackage::add(data);
						break;
					}
				}
			}
		}

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