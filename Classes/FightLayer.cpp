﻿#include "FightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "GameScene.h"
#include "Winlayer.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "AnalyticUtil.h"
#include "MapLayer.h"

FightLayer::FightLayer()
{
	isecapeok = false;
	isUseWg = false;
}


FightLayer::~FightLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

FightLayer* FightLayer::create(std::string addrid, std::string npcid)
{
	FightLayer *pRet = new FightLayer();
	if (pRet && pRet->init(addrid, npcid))
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

bool FightLayer::init(std::string addrid, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	this->addChild(csbnode);

	m_addrid = addrid;
	m_npcid = npcid;

	//地点名称
	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(GlobalData::map_maps[m_addrid].cname);

	// NPC 图标
	cocos2d::ui::ImageView* npcicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");

	//兔子和狼换一下
	std::string str = StringUtils::format("ui/%s.png", npcid.c_str());
	SpriteFrame* sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
	if (sf != NULL)
	{
		npcicon->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		npcicon->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	}

	//NPC名称
	cocos2d::ui::Text* npcnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcnametxt->setString(GlobalData::map_npcs[npcid].name);
	
	//角色名
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	heronametxt->setString(g_hero->getMyName());

	int maxlife = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_maxhp[g_hero->getLVValue()];
	//角色血量显示
	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), maxlife);
	herohpvaluetext->setString(hpstr);


	//角色血量进度
	float herohppercent = 100 * g_hero->getLifeValue() / maxlife;

	herohpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");
	herohpbar->setPercent(herohppercent);

	npcmaxhp = GlobalData::map_npcs[npcid].life;
	npchp = npcmaxhp;

	npcatk = GlobalData::map_npcs[npcid].atk;
	npcdf = GlobalData::map_npcs[npcid].df;

	//NPC血量显示
	npchpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");
	hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	//NCP血量进度
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");
	npchpbar->setPercent(npchppercent);

	//逃跑按钮
	m_escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	if (NewerGuideLayer::checkifNewerGuide(39))
		m_escapebtn->setVisible(false);
	// 滚动文字
	m_fihgtScorll = UIScroll::create(610.0f, 420.0f);
	m_fihgtScorll->setPosition(Vec2(360, 350));
	csbnode->addChild(m_fihgtScorll);

	if (npcid.compare("n001") != 0)
		this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 0.8f);//0.8s，hero->npc
	else
	{
		addrnametxt->setString(CommonFuncs::gbk2utf("路上"));
		m_fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("fightbtn");
		m_fightbtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onFihgt, this));
		m_fightbtn->setVisible(true);
		m_escapebtn->setPositionX(480);
		this->schedule(schedule_selector(FightLayer::checkHeroLife), 0.5f);
	}

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	int r = GlobalData::createRandomNum(4);
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_FIGHT_0 + r);
	return true;
}

void FightLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_npcid.compare("n001") == 0)
	{
		g_maplayer->heroPauseMoving();
	}

}

void FightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (node->getTag() == 0)
		{
			int r = GlobalData::createRandomNum(100);
			if (r > 50)
			{
				HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("逃跑成功！"));
				addChild(hbox);

				isecapeok = true;
			}
			else
			{
				g_uiScroll->addEventText(CommonFuncs::gbk2utf("你乘机逃跑，可惜失败了！！"), 25, Color3B(204, 4, 4));
				m_escapebtn->setEnabled(false);
				if (m_npcid.compare("n001") == 0)
				{
					fightRobber();
				}
			}
			m_escapebtn->setTitleText(CommonFuncs::gbk2utf("返回"));
			m_escapebtn->setTag(1);
			if (isecapeok && m_npcid.compare("n001") == 0)//山贼
			{
				g_maplayer->heroResumeMoving();
				this->removeFromParentAndCleanup(true);
			}
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}

void FightLayer::onFihgt(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_escapebtn->setEnabled(false);
		fightRobber();
	}
}

void FightLayer::fightRobber()
{
	m_fightbtn->setEnabled(false);
	this->unschedule(schedule_selector(FightLayer::checkHeroLife));
	this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 0.5f);
}

void FightLayer::delayHeroFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	npchp -= npchurt;

	if (npchp < 0)
		npchp = 0;


	std::string hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar->setPercent(npchppercent);
	showFightWord(0, npchurt);

	if (npchp <= 0)// NPC dead 胜利
	{
		this->scheduleOnce(schedule_selector(FightLayer::delayShowWinLayer), 1.5f);
		return;
	}
	this->scheduleOnce(schedule_selector(FightLayer::delayBossFight), 1.2f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
}

void FightLayer::delayBossFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	float curheroHp = g_hero->getLifeValue();

	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	float fminack = 0.1f * npcatk;
	int intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;
	curheroHp -= herohurt;

	if (curheroHp < 0.0f)
		curheroHp = 0.0f;
	g_hero->setLifeValue(curheroHp);

	checkHeroLife(0);

	showFightWord(1, herohurt);

	if (g_hero->getLifeValue() > 0.0f)
	{
		this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.2f);

		float f1maxlife = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_maxhp[g_hero->getLVValue()] * 0.05f;
		if (herohurt >= (int)f1maxlife)//受到大于10%伤害
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 30)//30%概率收内伤
			{
				float curvalue = g_hero->getInnerinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setInnerinjuryValue(curvalue);

			}
			else//70%概率收外伤
			{
				float curvalue = g_hero->getOutinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setOutinjuryValue(curvalue);
			}
		}
	}
	else
	{
		m_escapebtn->setTitleText(CommonFuncs::gbk2utf("返回"));
		m_escapebtn->setTag(1);

#ifdef ANALYTICS
		std::string deathstr = StringUtils::format("d-%s", m_npcid.c_str());
		AnalyticUtil::onEvent(m_npcid.c_str());
#endif
	}
	
}

void FightLayer::delayShowWinLayer(float dt)
{
	Winlayer* layer = Winlayer::create(m_addrid, m_npcid);
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(layer, 10, "Winlayer");
	this->removeFromParentAndCleanup(true);
}

void FightLayer::showFightWord(int type, int value)
{
	std::string wordstr;
	int size = 0;
	int r = 0;
	if (type == 0)//
	{
		std::string herowordstr;
		if (g_hero->getAtrByType(H_WEAPON)->count > 0)//是否有武器
		{
			int showr = GlobalData::createRandomNum(2);

			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				if (showr == 0)
					isUseWg = true;
				if (m_npcid.compare("n003") == 0 || m_npcid.compare("n002") == 0)
					isUseWg = false;
			}
			if (isUseWg)
			{
				herowordstr = getGfFightStr();
			}
			else
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), g_hero->getAtrByType(H_WEAPON)->name.c_str(), GlobalData::map_npcs[m_npcid].name);
			}

			
			int r = GlobalData::createRandomNum(100);
			if (r < 80)
			{
				g_hero->getAtrByType(H_WEAPON)->goodvalue--;
				if (g_hero->getAtrByType(H_WEAPON)->goodvalue <= 0)
				{
					//StorageRoom::use(g_hero->getAtrByType(H_WEAPON)->strid);
					PackageData data;
					data.count = -1;
					g_hero->setAtrByType(H_WEAPON, data);
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), g_hero->getAtrByType(H_WEAPON)->name.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_WEAPON)->goodvalue == 20 || g_hero->getAtrByType(H_WEAPON)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", g_hero->getAtrByType(H_WEAPON)->name.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_WEAPON)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getGfFightStr();
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);
			}
		}
		
		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightworfword[0].c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightRabbitword[0].c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);

		checkWordLblColor(herowordstr);

		if (isUseWg)
		{
			int r1 = GlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		}
		else
		{
			if (g_hero->getAtrByType(H_WEAPON)->count > 0)
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = GlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
			}
			else
			{
				herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
			}
		}

		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightworfword2[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(herofightRabbitword2[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		checkWordLblColor(herowordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_ATTACK);
	}
	else//
	{
		std::string bosswordstr;

		if (g_hero->getAtrByType(H_ARMOR)->count > 0)//是有有防具
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 80)
			{
				g_hero->getAtrByType(H_ARMOR)->goodvalue--;

				if (g_hero->getAtrByType(H_ARMOR)->goodvalue <= 0)
				{
					//StorageRoom::use(g_hero->getAtrByType(H_ARMOR)->strid);
					PackageData data;
					data.count = -1;
					g_hero->setAtrByType(H_ARMOR, data);
					
					std::string desc = StringUtils::format("%s%s%s", CommonFuncs::gbk2utf("你的").c_str(), g_hero->getAtrByType(H_ARMOR)->name.c_str(), CommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_ARMOR)->goodvalue == 20 || g_hero->getAtrByType(H_ARMOR)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", g_hero->getAtrByType(H_ARMOR)->name.c_str(), CommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_ARMOR)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), g_hero->getAtrByType(H_ARMOR)->name.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		}

		if (m_npcid.compare("n003") == 0)
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(worffight[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		else if (m_npcid.compare("n002") == 0)
			bosswordstr = StringUtils::format(CommonFuncs::gbk2utf(rabbitfightword[0].c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		checkWordLblColor(bosswordstr);

		isUseWg = false;
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT);
	}
}

void FightLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 28);
	wordlbl->setLineBreakWithoutSpace(true);
	wordlbl->setMaxLineWidth(610);
	int index = 0;
	//getletter --是每个字，中文，英文都算一个字
	//UFT8 中文是3个字符所以除以3转换下
	while (wordlbl->getLetter(index) != NULL)
	{
		wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		index++;
	}

	//NPC名称颜色 红色
	std::map<std::string, NpcData>::iterator it;
	for (it = GlobalData::map_npcs.begin(); it != GlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = GlobalData::map_npcs[it->first].name;
		std::size_t findpos;
		std::string temp = wordstr;
		int count = 0;
		while (1){
			findpos = temp.find(npcname);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + npcname.length(), temp.length() - npcname.length());
				int sindex = (wordstr.length() - temp.length() - npcname.length()) / 3;
				int len = npcname.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
				}
			}
			else 
				break;
		}

		//std::size_t findpos = wordstr.find(npcname);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = npcname.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(230,35,35));
		//	}
		//}
	}
	//角色名称颜色 绿色

	std::size_t findpos;
	std::string temp = wordstr;
	std::string heroname = g_hero->getMyName();
	while (1){
		findpos = temp.find(g_hero->getMyName());
		if (findpos != std::string::npos){
			temp = temp.substr(findpos + heroname.length(), temp.length() - heroname.length());
			int sindex = (wordstr.length() - temp.length() - heroname.length()) / 3;
			int len = heroname.length() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
			}
		}
		else
			break;
	}
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		std::string ename = GlobalData::map_equips[ite->first].cname;
		std::size_t findpos;
		std::string temp = wordstr;
		while (1){
			findpos = temp.find(ename);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + ename.length(), temp.length() - ename.length());
				int sindex = (wordstr.length() - temp.length() - ename.length()) / 3;
				int len = ename.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
				}
			}
			else
				break;
		}

		//std::size_t findpos = wordstr.find(ename);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = ename.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
		//	}
		//}
	}
	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		GFSkillData gfskilldata = GlobalData::map_gfskills[g_hero->getAtrByType(H_WG)->strid];
		int mygfskillsize = gfskilldata.snames.size();
		
		for (int n = 0; n < mygfskillsize; n++)
		{
			std::string gfname = gfskilldata.snames[n];
			std::size_t findpos;
			std::string temp = wordstr;
			while (1){
				findpos = temp.find(gfname);
				if (findpos != std::string::npos){
					temp = temp.substr(findpos + gfname.length(), temp.length() - gfname.length());
					int sindex = (wordstr.length() - temp.length() - gfname.length()) / 3;
					int len = gfname.length() / 3;
					for (int i = sindex; i < sindex + len; i++)
					{
						wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
					}
				}
				else
					break;
			}
		}
	}

	//数字颜色 红色
	//目前只实现了一段中只能改变一组数字的颜色，并且数字要在上面 关键字的后面
	int sindex = -1;
	findpos = 0;
	for (unsigned int i = 0; i < wordstr.length(); i++)
	{
		char a = wordstr[i];
		if (wordstr[i] >= '0' && wordstr[i] <= '9')
		{
			sindex = i / 3;
			findpos = i;
			wordlbl->getLetter(sindex)->setColor(Color3B(230, 35, 35));
			break;
		}
	}

	if (sindex > 0)
	{
		//找到地一个数字后，往后再找4位
		for (int i = 1; i <= 4; i++)
		{
			char a = wordstr[findpos + i];
			if (wordstr[findpos + i] >= '0' && wordstr[findpos + i] <= '9')
			{
				wordlbl->getLetter(sindex + i)->setColor(Color3B(230, 35, 35));
			}
		}
	}

	m_fihgtScorll->addEventLabel(wordlbl);
}

std::string FightLayer::getGfFightStr()
{
	std::string retstr;

	int r1 = GlobalData::createRandomNum(2);
	int r2 = GlobalData::createRandomNum(4);

	std::string gfstr = g_hero->getAtrByType(H_WG)->strid;
	GFSkillData sdata = GlobalData::map_gfskills[g_hero->getAtrByType(H_WG)->strid];
	int r3 = GlobalData::createRandomNum(sdata.snames.size());
	std::string gfname = sdata.snames[r3];
	std::string	wordstr = CommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), gfname.c_str(), GlobalData::map_npcs[m_npcid].name);
	else
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name, gfname.c_str(), gfname.c_str(), GlobalData::map_npcs[m_npcid].name);

	return retstr;
}

void FightLayer::checkHeroLife(float dt)
{
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), GlobalData::map_heroAtr[g_hero->getHeadID()].vec_maxhp[g_hero->getLVValue()]);
	herohpvaluetext->setString(hpstr);
	float herohppercent = 100 * g_hero->getLifeValue() / GlobalData::map_heroAtr[g_hero->getHeadID()].vec_maxhp[g_hero->getLVValue()];
	herohpbar->setPercent(herohppercent);
}