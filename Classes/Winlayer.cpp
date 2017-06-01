#include "Winlayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameDataSave.h"
#include "GameScene.h"
#include "StorageRoom.h"
#include "MapLayer.h"
#include "SoundManager.h"
#include "NpcLayer.h"
#include "HomeHill.h"
#include "NewerGuideLayer.h"

Winlayer::Winlayer()
{
}


Winlayer::~Winlayer()
{
}


Winlayer* Winlayer::create(std::string addrid, std::string npcid)
{
	Winlayer *pRet = new Winlayer();
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

bool Winlayer::init(std::string addrid, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("winLayer.csb");
	this->addChild(csbnode);

	m_addrid = addrid;
	m_npcid = npcid;

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(Winlayer::onBack, this));

	m_getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	m_getallbtn->addTouchEventListener(CC_CALLBACK_2(Winlayer::onAllGet, this));

	cocos2d::ui::Text* addrname = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrname->setString(GlobalData::map_maps[m_addrid].cname);
	
	int winexp = GlobalData::map_npcs[m_npcid].exp;
	explbl = (cocos2d::ui::Text*)csbnode->getChildByName("explbl");
	std::string lblstr = StringUtils::format("+%d", winexp);
	explbl->setString(lblstr);
	gfexplbl = (cocos2d::ui::Text*)csbnode->getChildByName("gfexplbl");
	if (g_hero->getAtrByType(H_WG)->count > 0 || g_hero->getAtrByType(H_NG)->count > 0)
	{
		lblstr = StringUtils::format("+%d", winexp * 3 / 2);
		gfexplbl->setString(lblstr);
	}
	else
	{
		gfexplbl->setVisible(false);
		csbnode->getChildByName("gftext")->setVisible(false);
	}

	loadTempData();

	std::vector<std::string> winres = GlobalData::map_npcs[npcid].winres;
	int curplot = GlobalData::getPlotMissionIndex();
	if (GlobalData::vec_PlotMissionData[curplot].dnpc.compare(m_npcid) == 0 && GlobalData::vec_PlotMissionData[curplot].status == M_DOING)
	{
		if (GlobalData::vec_PlotMissionData[curplot].type == 1)
		{
			GlobalData::vec_PlotMissionData[curplot].status = M_DONE;
			int unlockchapter = GlobalData::vec_PlotMissionData[curplot].unlockchapter;
			GlobalData::setUnlockChapter(unlockchapter);
			winres = GlobalData::vec_PlotMissionData[curplot].rewords;
			for (unsigned int i = 0; i < winres.size(); i++)
			{
				GlobalData::map_npcs[npcid].winresrnd[i] = 100;
			}
			GlobalData::setPlotMissionIndex(curplot + 1);
			GlobalData::savePlotMissionStatus();

			if (g_gameLayer != NULL)
			{
				NpcLayer * npclayer = (NpcLayer*)g_gameLayer->getChildByName("npclayer");
				if (npclayer != NULL)
					npclayer->updatePlotUI();
			}

			if (g_maplayer != NULL && unlockchapter > 0)
				g_maplayer->scheduleOnce(schedule_selector(MapLayer::showUnlockLayer), 0.5f);
		}
	}

	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());

		int r = GlobalData::createRandomNum(100)+ 1;
		if (r <= GlobalData::map_npcs[npcid].winresrnd[i])
		{
			
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
				getRewardData.push_back(data);
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
						getRewardData.push_back(data);
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
							getRewardData.push_back(data);
							break;
						}
					}
				}
			}
		}
	}
	updata();
	updataLV();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleOnce(schedule_selector(Winlayer::delayShowNewerGuide), 0.2f);
	return true;
}

void Winlayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

}

void Winlayer::updataLV()
{
	int winexp = GlobalData::map_npcs[m_npcid].exp;
	g_hero->setExpValue(g_hero->getExpValue() + winexp);
	int curlv = g_hero->getLVValue();
	unsigned int i = 0;
	int lv = 0;
	std::vector<int> vec_heroExp = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp;
	for (i = curlv; i < vec_heroExp.size(); i++)
	{
		if (g_hero->getExpValue() > vec_heroExp[i])
		{
			lv = i + 1;
			g_hero->setExpValue(g_hero->getExpValue() - vec_heroExp[i]);
		}
	}
	if (lv > curlv)
	{
		if (lv >= vec_heroExp.size())
			lv = vec_heroExp.size() - 1;
		g_hero->setLVValue(lv);
		g_hero->setLifeValue(g_hero->getMaxLifeValue());

		showLvUpText();
	}

	for (int m = H_WG; m <= H_NG; m++)
	{
		PackageData* gfData = g_hero->getAtrByType((HeroAtrType)m);
		if (gfData->count > 0)
		{
			std::string gfname = gfData->strid;
			std::vector<int> vec_gfExp = GlobalData::map_wgngs[gfname].vec_exp;
			curlv = GlobalData::map_wgngs[gfname].lv;
			gfData->exp += winexp * 3 / 2;
			for (i = curlv; i < vec_gfExp.size(); i++)
			{
				if (gfData->exp > vec_gfExp[i])
				{
					lv = i + 1;
					gfData->exp = gfData->exp - vec_gfExp[i];
				}
			}
			if (lv > curlv)
			{
				if (lv >= vec_gfExp.size())
					lv = vec_gfExp.size() - 1;
				gfData->lv = lv;
				GlobalData::map_wgngs[gfname].lv = lv;
			}
		}

	}

}

void Winlayer::onRewardItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = getRewardData.begin(); it != getRewardData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata;
				pdata.type = data->type;
				pdata.strid = data->strid;
				pdata.lv = data->lv;
				pdata.extype = data->extype;
				pdata.count = 1;
				pdata.exp = data->exp;
				pdata.goodvalue = data->goodvalue;
				pdata.name = data->name;
				pdata.desc = data->desc;
				if (MyPackage::add(pdata) == 0)
				{
					data->count--;
					getRewardData.erase(it);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata;
		pdata.type = data->type;
		pdata.strid = data->strid;
		pdata.lv = data->lv;
		pdata.extype = data->extype;
		pdata.count = 1;
		pdata.exp = data->exp;
		pdata.goodvalue = data->goodvalue;
		pdata.name = data->name;
		pdata.desc = data->desc;
		if (MyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}
	saveTempData();
	updata();
	showNewerGuide(35);
}

void Winlayer::onPackageItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = MyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getRewardData.size(); i++)
	{
		if (data.strid.compare(getRewardData[i].strid) == 0)
		{
			getRewardData[i].count++;
			break;
		}
	}

	if (i == getRewardData.size())
	{
		data.count = 1;
		getRewardData.push_back(data);
	}
	saveTempData();
	MyPackage::cutone(data.strid);

	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}

	updata();
}

void Winlayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		HomeHill* homehill = (HomeHill*)g_gameLayer->getChildByName("homehill");
		if (homehill != NULL)
		{
			if (NewerGuideLayer::checkifNewerGuide(36))
				homehill->showNewerGuide(36);
			else if (NewerGuideLayer::checkifNewerGuide(39))
				homehill->showNewerGuide(39);
		}
		this->removeFromParentAndCleanup(true);
	}
}

void Winlayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		for (unsigned int i = 0; i < getRewardData.size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			this->removeChildByName(name);
		}
		std::vector<PackageData>::iterator it;
		bool isfull = false;
		for (it = getRewardData.begin(); it != getRewardData.end();)
		{
			bool isOver = false;
			int count = it->count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = *it;
				data.count = 1;
				if (MyPackage::add(data) == 0)
				{
					if (--it->count <= 0)
					{
						it = getRewardData.erase(it);
						isOver = true;
						break;
					}
				}
				else
				{
					isfull = true;
					break;
				}
			}
			if (!isOver)
				it++;
			if (isfull)
				break;
		}
		saveTempData();
		updata();
		showNewerGuide(38);
	}
}


void Winlayer::loadTempData()
{
	tempResData.clear();
	std::string datastr = GameDataSave::getInstance()->getTempStorage("m1-2");
	std::vector<std::string> vec_retstr;
	CommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		CommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		data.name = tmp[7];
		data.desc = tmp[8];
		tempResData.push_back(data);
	}
}

void Winlayer::saveTempData()
{
	std::vector<PackageData> allResData = tempResData;

	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		int tmpsize = tempResData.size();
		int j = 0;
		for (j = 0; j < tmpsize; j++)
		{
			if (getRewardData[i].strid.compare(tempResData[j].strid) == 0)
			{
				allResData[j].count += getRewardData[i].count;
				break;
			}
		}
		if (j == tmpsize)
		{
			allResData.push_back(getRewardData[i]);
		}
	}
	std::string str;
	for (unsigned int i = 0; i < allResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%s-%s;", allResData[i].strid.c_str(), allResData[i].type, allResData[i].count, allResData[i].extype, allResData[i].lv, allResData[i].exp, allResData[i].goodvalue, allResData[i].name.c_str(), allResData[i].desc.c_str());
		str.append(onestr);
	}
	GameDataSave::getInstance()->setTempStorage(m_addrid, str.substr(0, str.length() - 1));
}

void Winlayer::updata()
{
	//更新奖励栏
	updataRewardUI();
	//更新背包栏
	updataMyPackageUI();
}

void Winlayer::updataMyPackageUI()
{
	for (int i = 0; i < MyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(Winlayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 220));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", MyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", MyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void Winlayer::updataRewardUI()
{
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/buildsmall.png");

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(Winlayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getRewardData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 420));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getRewardData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getRewardData[i].count);
		Label * reslbl = Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void Winlayer::onExit()
{
	saveTempData();
	Layer::onExit();
}

void Winlayer::showLvUpText()
{
	Sprite* lvUpSprite = Sprite::createWithSpriteFrameName("ui/herolvuptext.png");
	lvUpSprite->setPosition(Vec2(360, 400));
	this->addChild(lvUpSprite);
	lvUpSprite->runAction(Spawn::create(MoveTo::create(3.0f, Vec2(360, 700)), FadeOut::create(3.0f), NULL));
}

void Winlayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 34)
	{
		Node* resnode = this->getChildByName("resitem0");
		nodes.push_back(resnode->getChildren().at(0));
	}
	else if (step == 35 || step == 38)
	{
		nodes.push_back(m_backbtn);
	}
	else if (step == 37)
	{
		nodes.push_back(m_getallbtn);
	}
	g_gameLayer->showNewerGuide(step, nodes);
}


void Winlayer::delayShowNewerGuide(float dt)
{
	if (NewerGuideLayer::checkifNewerGuide(34))
		showNewerGuide(34);
	else if (NewerGuideLayer::checkifNewerGuide(37))
		showNewerGuide(37);
}