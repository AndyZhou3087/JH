#include "FightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "GameScene.h"
#include "Winlayer.h"
#include "SoundManager.h"

FightLayer::FightLayer()
{
	isecapeok = false;
}


FightLayer::~FightLayer()
{
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

	//角色血量显示
	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);


	//角色血量进度
	int herohppercent = 100 *  g_hero->getLifeValue() / g_hero->getMaxLifeValue();

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

	// 滚动文字
	m_fihgtScorll = UIScroll::create(610.0f, 400.0f);
	m_fihgtScorll->setPosition(Vec2(360, 370));
	csbnode->addChild(m_fihgtScorll);

	this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 0.8f);//0.8s，hero->npc

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_FIGHT);
	return true;
}

void FightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* node = (Node*)pSender;
		if (node->getTag() == 0)
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("逃跑成功！"));
			addChild(hbox);
			m_escapebtn->setTitleText(CommonFuncs::gbk2utf("返回"));
			m_escapebtn->setTag(1);
			isecapeok = true;
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}

void FightLayer::delayHeroFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	int gfBonusAck = 0;
	int weaponAck = 0;
	if (g_hero->getAtrByType(H_WG)->count > 0)//是否有外功--加攻
	{
		std::string gfname = g_hero->getAtrByType(H_WG)->strid;
		gfBonusAck = GlobalData::map_wgngs[gfname].vec_bns[GlobalData::map_wgngs[gfname].lv];
	}

	if (g_hero->getAtrByType(H_WEAPON)->count > 0)//是否有武器--加攻
	{
		std::string wname = g_hero->getAtrByType(H_WEAPON)->strid;
		weaponAck = GlobalData::map_equips[wname].atk;
	}
	int heroCurAck = g_hero->getAtkValue() + gfBonusAck + weaponAck;
	float fack = g_hero->getAtkPercent() * heroCurAck;
	heroCurAck = ceil(fack);

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = ceil(fminack);
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

	int gfBonusDf = 0;
	int adf = 0;
	if (g_hero->getAtrByType(H_NG)->count > 0)////是否有内功--加防
	{
		std::string gfname = g_hero->getAtrByType(H_NG)->strid;
		gfBonusDf = GlobalData::map_wgngs[gfname].vec_bns[GlobalData::map_wgngs[gfname].lv];
	}

	if (g_hero->getAtrByType(H_ARMOR)->count > 0)////是否有防具--加防
	{
		std::string aname = g_hero->getAtrByType(H_ARMOR)->strid;
		adf = GlobalData::map_equips[aname].df;
	}

	int curheroHp = g_hero->getLifeValue();
	int curheroDf = g_hero->getDfValue() + gfBonusDf + adf;
	float fdf = g_hero->getDfPercent() * curheroDf;
	curheroDf = ceil(fdf);
	int herohurt = npcatk - curheroDf;

	float fminack = 0.1f * npcatk;
	int intminack = ceil(fminack);

	if (herohurt < intminack)
		herohurt = intminack;
	curheroHp -= herohurt;

	if (curheroHp < 0)
		curheroHp = 0;
	g_hero->setLifeValue(curheroHp);

	std::string hpstr = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	int herohppercent = 100 * g_hero->getLifeValue() / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);
	showFightWord(1, herohurt);

	if (g_hero->getLifeValue() > 0)
	{
		this->scheduleOnce(schedule_selector(FightLayer::delayHeroFight), 1.2f);

		float f1maxlife = g_hero->getMaxLifeValue() * 0.1f;
		if (herohurt >= (int)f1maxlife)
		{
			int r = GlobalData::createRandomNum(100);
			if (r < 30)
			{
				int curvalue = g_hero->getInnerinjuryValue() - 5;
				if (curvalue < 0)
					curvalue = 0;
				g_hero->setInnerinjuryValue(curvalue);

			}
			else
			{
				int curvalue = g_hero->getOutinjuryValue() - 5;
				if (curvalue < 0)
					curvalue = 0;
				g_hero->setOutinjuryValue(curvalue);
			}
		}
	}

	
}

void FightLayer::delayShowWinLayer(float dt)
{
	Winlayer* layer = Winlayer::create(m_addrid, m_npcid);
	Director::getInstance()->getRunningScene()->addChild(layer);
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
			size = sizeof(herofightdesc1) / sizeof(herofightdesc1[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = herofightdesc1[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), g_hero->getAtrByType(H_WEAPON)->name.c_str(), GlobalData::map_npcs[m_npcid].name);
		}
		else//没有武器
		{
			size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
			r = GlobalData::createRandomNum(size);
			wordstr = herofightdesc[r];
			herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), GlobalData::map_npcs[m_npcid].name);
		}
		
		checkWordLblColor(herowordstr);

		size = sizeof(herofightdesc2) / sizeof(herofightdesc2[0]);
		r = GlobalData::createRandomNum(size);
		wordstr = herofightdesc2[r];
		herowordstr = StringUtils::format(CommonFuncs::gbk2utf(wordstr.c_str()).c_str(), GlobalData::map_npcs[m_npcid].name, value);
		checkWordLblColor(herowordstr);
	}
	else//需要战斗
	{
		std::string bosswordstr;
		if (g_hero->getAtrByType(H_ARMOR)->count > 0)//是有有防具
		{
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

		checkWordLblColor(bosswordstr);
	}
}

void FightLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 25);
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
		std::size_t findpos = wordstr.find(npcname);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = npcname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(230,35,35));
			}
		}
	}
	//角色名称颜色 绿色
	std::size_t findpos = wordstr.find(g_hero->getMyName());
	if (findpos != std::string::npos)
	{
		int sindex = findpos / 3;
		int len = g_hero->getMyName().size() / 3;
		for (int i = sindex; i < sindex + len; i++)
		{
			wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
		}
	}
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = GlobalData::map_equips.begin(); ite != GlobalData::map_equips.end(); ++ite)
	{
		std::string ename = GlobalData::map_equips[ite->first].cname;
		std::size_t findpos = wordstr.find(ename);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = ename.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
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