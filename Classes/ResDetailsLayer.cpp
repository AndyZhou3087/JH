#include "ResDetailsLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Hero.h"
#include "StorageUILayer.h"
#include "SoundManager.h"
#include "BuyComfirmLayer.h"

#define WINESTRID "23"
#define GRASSRID "5"

int ResDetailsLayer::whereClick = 0;//0--仓库，1其他

std::string gfqudesc[] = {"（入门）", "（一流）", "（上乘）", "（传世）", "（绝世）"};

ResDetailsLayer::ResDetailsLayer()
{

}

ResDetailsLayer::~ResDetailsLayer()
{
	whereClick = 0;
}

bool ResDetailsLayer::init(PackageData* pdata)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	m_packageData = pdata;
	m_csbnode = CSLoader::createNode("resDetailsLayer.csb");
	this->addChild(m_csbnode);

	m_okbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("okbtn");
	m_okbtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onOk, this));

	cocos2d::ui::Button* usebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("usebtn");
	usebtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onUse, this));

	uselbl = (cocos2d::ui::Text*)usebtn->getChildByName("valuelbl");

	if ((pdata->type == FOOD || (pdata->type == MEDICINAL && pdata->strid.compare(WINESTRID) != 0)) && whereClick == 0)
		m_okbtn->setTitleText(CommonFuncs::gbk2utf("使用"));
	else if (pdata->type == RES_1 && pdata->strid.compare(GRASSRID) == 0 && whereClick == 0)
	{
		m_okbtn->setTitleText(CommonFuncs::gbk2utf("喂马"));
		updateHorseData(0);
	}

	cocos2d::ui::Text* resname = (cocos2d::ui::Text*)m_csbnode->getChildByName("namelbl");

	resname->setString(pdata->name);

	cocos2d::ui::Text* qulbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("qulbl");

	cocos2d::ui::ImageView* resbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("buildsmall");
	std::string qustr = "ui/buildsmall.png";
	int qu = -1;
	if (pdata->type == WEAPON || pdata->type == PROTECT_EQU)
	{
		qu = GlobalData::map_equips[pdata->strid].qu;
	}
	else if (pdata->type == N_GONG || pdata->type == W_GONG)
	{
		qu = GlobalData::map_wgngs[pdata->strid].qu;
	}
	if (qu > 0)
	{
		qustr = StringUtils::format("ui/qubox%d.png", qu);
		qulbl->setVisible(true);
		qulbl->setString(CommonFuncs::gbk2utf(gfqudesc[qu - 1].c_str()));
	}

	resbox->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
	resbox->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());

	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)resbox->getChildByName("Image");

	std::string str = StringUtils::format("ui/%s.png", pdata->strid.c_str());
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* atkdftext = (cocos2d::ui::Text*)m_csbnode->getChildByName("atkdftext");

	cocos2d::ui::Text* valuelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("valuelbl");
	int count = StorageRoom::getCountById(pdata->strid);
	std::string countstr;

	if (pdata->type == FOOD || pdata->type == MEDICINAL || pdata->type == RES_1)
		countstr = StringUtils::format("库存%d", count);
	else if (pdata->type == WEAPON || pdata->type == PROTECT_EQU || pdata->type == TOOLS)
	{
		countstr = StringUtils::format("耐久度%d%%", pdata->goodvalue);
		std::string tmpstr;
		if (pdata->type == WEAPON)
		{
			tmpstr = StringUtils::format("攻击力增加%d", GlobalData::map_equips[pdata->strid].atk);
		}
		else if (pdata->type == PROTECT_EQU)
		{
			tmpstr = StringUtils::format("防御力增加%d", GlobalData::map_equips[pdata->strid].df);
		}
		if (tmpstr.length() > 0)
			atkdftext->setString(CommonFuncs::gbk2utf(tmpstr.c_str()));
	}
	else if (pdata->type == N_GONG || pdata->type == W_GONG)
	{
		resname->setString(pdata->name);

		int lv = pdata->lv + 1;
		countstr = StringUtils::format("功法等级 %d/%d", lv, GlobalData::map_wgngs[pdata->strid].maxlv);

		usebtn->setVisible(true);
		m_okbtn->setPositionX(460);

		std::string uselblstr = StringUtils::format("大力丸 x%d", StorageRoom::getCountById("71"));
		uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
		
	}
	else if (pdata->type == RES_2)
	{
		if (pdata->strid.compare("70") == 0)
		{
			usebtn->setVisible(true);
			m_okbtn->setPositionX(460);
			std::string uselblstr = StringUtils::format("经验药水 x%d", StorageRoom::getCountById("70"));
			uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
		}

		else if (pdata->strid.compare("74") == 0)
		{
			countstr = StringUtils::format("生命%d", pdata->goodvalue);
		}
		else if (pdata->extype == 4)
			countstr = "永久";
	}

	valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

	cocos2d::ui::Text* resdesc = (cocos2d::ui::Text*)m_csbnode->getChildByName("desclbl");
	resdesc->setString(pdata->desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

ResDetailsLayer* ResDetailsLayer::create(PackageData* pdata)
{
	ResDetailsLayer *pRet = new ResDetailsLayer();
	if (pRet && pRet->init(pdata))
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

ResDetailsLayer* ResDetailsLayer::createByResId(std::string resid)
{
	PackageData sdata;
	bool isInRes = false;
	for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
	{
		if (resid.compare(GlobalData::vec_resData[i].strid) == 0)
		{
			isInRes = true;
			sdata.desc = GlobalData::vec_resData[i].desc;
			sdata.name = GlobalData::vec_resData[i].cname;
			sdata.strid = resid;
			sdata.type = GlobalData::vec_resData[i].type - 1;
			break;
		}
	}
	if (!isInRes)
	{
		std::map<std::string, std::vector<BuildActionData>>::iterator it;
		for (it = GlobalData::map_buidACData.begin(); it != GlobalData::map_buidACData.end(); ++it)
		{
			for (unsigned int i = 0; i < GlobalData::map_buidACData[it->first].size(); i++)
			{
				if (resid.compare(GlobalData::map_buidACData[it->first][i].icon) == 0)
				{
					isInRes = true;
					sdata.desc = GlobalData::map_buidACData[it->first][i].desc;
					sdata.name = GlobalData::map_buidACData[it->first][i].cname;
					sdata.strid = resid;
					sdata.type = GlobalData::map_buidACData[it->first][i].type - 1;
					break;
				}
			}
		}
	}

	if (isInRes)
	{
		return create(&sdata);
	}
	return NULL;
}

void ResDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		StorageUILayer* storageUI = (StorageUILayer*)this->getParent();

		if (whereClick == 0)
		{
			if (GlobalData::isExercising() && !GlobalData::isHasFSF() && m_okbtn->getTitleText().compare(CommonFuncs::gbk2utf("使用")) == 0)
			{
				BuyComfirmLayer* layer = BuyComfirmLayer::create(6);
				g_gameLayer->addChild(layer, 4, "buycomfirmlayer");
				return;
			}

			if (m_packageData->type == FOOD)
			{
				bool isInres = false;
				for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
				{
					if (m_packageData->strid.compare(GlobalData::vec_resData[i].strid) == 0)
					{
						isInres = true;
						int addvalue = GlobalData::vec_resData[i].ep[0];
						recoveHungerValue(addvalue);
						int r = 0;
						if (m_packageData->strid.compare("6") == 0 || m_packageData->strid.compare("9") == 0)
						{
							r = GlobalData::createRandomNum(100);
						}
						
						if (r >= 50)
						{
							g_hero->setInnerinjuryValue(g_hero->getInnerinjuryValue() - 10.0f);
						}

						StorageRoom::use(m_packageData->strid);
						break;
					}
				}

				if (!isInres)
				{
					for (unsigned int i = 0; i < GlobalData::map_buidACData["cooktable"].size(); i++)
					{
						if (m_packageData->strid.compare(GlobalData::map_buidACData["cooktable"][i].icon) == 0)
						{
							int addvalue = GlobalData::map_buidACData["cooktable"][i].ep[0];
							recoveHungerValue(addvalue);

							StorageRoom::use(m_packageData->strid);
							break;
						}
					}
				}
				storageUI->updateResContent();
			}
			else if (m_packageData->type == MEDICINAL)
			{
				bool isInRes = false;
				for (unsigned int i = 0; i < GlobalData::vec_resData.size(); i++)
				{
					if (m_packageData->strid.compare(GlobalData::vec_resData[i].strid) == 0)
					{
						isInRes = true;
						int wvalue = GlobalData::vec_resData[i].ep[0];
						int nvalue = GlobalData::vec_resData[i].ep[1];
						recoveInjuryValue(wvalue, nvalue);
						StorageRoom::use(m_packageData->strid);
						break;
					}
				}
				if (!isInRes)
				{
					for (unsigned int i = 0; i < GlobalData::map_buidACData["medicinekit"].size(); i++)
					{
						if (m_packageData->strid.compare(GlobalData::map_buidACData["medicinekit"][i].icon) == 0)
						{
							int wvalue = GlobalData::map_buidACData["medicinekit"][i].ep[0];
							int nvalue = GlobalData::map_buidACData["medicinekit"][i].ep[1];
							recoveInjuryValue(wvalue, nvalue);
							StorageRoom::use(m_packageData->strid);
							break;
						}
					}
				}
				storageUI->updateResContent();
			}
			else if (m_packageData->type == RES_1 && m_packageData->strid.compare(GRASSRID) == 0)
			{
				StorageRoom::use(m_packageData->strid);

				int count = StorageRoom::getCountById(GRASSRID);
				if (count <= 0)
				{
					m_okbtn->setEnabled(false);
				}

				cocos2d::ui::Text* valuelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("valuelbl");
				std::string countstr = StringUtils::format("库存%d", count);
				valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

				updateHorseData(1);
				storageUI->updateResContent();
				return;
			}

		}
		removSelf();
	}
}

void ResDetailsLayer::onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_packageData->type == N_GONG || m_packageData->type == W_GONG)
		{
			if (StorageRoom::getCountById("71") > 0)
			{
				StorageRoom::use("71");
				std::string uselblstr = StringUtils::format("大力丸 x%d", StorageRoom::getCountById("71"));
				uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
			}
			else
			{
				return;
			}

			ResData resdata;
			for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
			{
				if (GlobalData::vec_resData[m].strid.compare("70") == 0)
				{
					resdata = GlobalData::vec_resData[m];
					break;
				}
			}
			unsigned int i = 0;
			int lv = 0;
			int curlv = 0;
			for (unsigned int m = 0; m < StorageRoom::map_storageData[m_packageData->type].size(); m++)
			{
				PackageData* gfData = &StorageRoom::map_storageData[m_packageData->type][m];
				if (gfData->count > 0)
				{
					std::string gfname = gfData->strid;
					std::vector<int> vec_gfExp = GlobalData::map_wgngs[gfname].vec_exp;
					curlv = gfData->lv;
					gfData->exp += resdata.ep[0];
					for (i = curlv; i < vec_gfExp.size(); i++)
					{
						if (gfData->exp >= vec_gfExp[i])
						{
							lv = i + 1;
							gfData->exp = gfData->exp - vec_gfExp[i];
						}
					}
					if (lv > curlv)
					{
						int gfmaxlv = GlobalData::map_wgngs[gfname].maxlv;
						if (lv >= gfmaxlv)
							lv = gfmaxlv - 1;
						gfData->lv = lv;
					}
				}

			}
		}
		else if (m_packageData->type == RES_2)
		{
			if (StorageRoom::getCountById("70") > 0)
			{
				StorageRoom::use("70");
				std::string uselblstr = StringUtils::format("经验药水 x%d", StorageRoom::getCountById("70"));
				uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));

			}
			else
			{
				return;
			}

			ResData resdata;
			for (unsigned int m = 0; m < GlobalData::vec_resData.size(); m++)
			{
				if (GlobalData::vec_resData[m].strid.compare("70") == 0)
				{
					resdata = GlobalData::vec_resData[m];
					break;
				}
			}

			int exp = resdata.ep[0];
			g_hero->setExpValue(g_hero->getExpValue() + exp);
			int curlv = g_hero->getLVValue();
			unsigned int i = 0;
			int lv = 0;
			std::vector<int> vec_heroExp = GlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp;
			for (i = curlv; i < vec_heroExp.size(); i++)
			{
				if (g_hero->getExpValue() >= vec_heroExp[i])
				{
					lv = i + 1;
					g_hero->setExpValue(g_hero->getExpValue() - vec_heroExp[i]);
				}
			}
			if (lv > curlv)
			{
				int heromaxlv = vec_heroExp.size();
				if (lv >= heromaxlv)
				{
					g_hero->setExpValue(vec_heroExp[heromaxlv - 1]);
					lv = heromaxlv - 1;
				}
				g_hero->setLVValue(lv);
				
			}
		}
		StorageUILayer* storageUI = (StorageUILayer*)this->getParent();
		storageUI->updateResContent();
		removSelf();
	}
}
void ResDetailsLayer::recoveInjuryValue(int addwvalue, int addnvalue)
{
	float outvalue = g_hero->getOutinjuryValue();
	if (addwvalue + outvalue > g_hero->getMaxOutinjuryValue())
		g_hero->setOutinjuryValue(g_hero->getMaxOutinjuryValue());
	else
		g_hero->recoverOutjury(addwvalue);
	float invalue = g_hero->getInnerinjuryValue();
	if (invalue + addnvalue > g_hero->getMaxInnerinjuryValue())
		g_hero->setInnerinjuryValue(g_hero->getMaxInnerinjuryValue());
	else
		g_hero->recoverInjury(addnvalue);
}

void ResDetailsLayer::recoveHungerValue(int addvalue)
{
	float hungervale = g_hero->getHungerValue();
	if (addvalue + hungervale > g_hero->getMaxHungerValue())
		g_hero->setHungerValue(g_hero->getMaxHungerValue());
	else
		g_hero->recoverHunger(addvalue);
}

void ResDetailsLayer::removSelf()
{
	StorageUILayer* storagelayer = (StorageUILayer*)g_gameLayer->getChildByName("storageuilayer");
	if (storagelayer != NULL)
		storagelayer->showNewerGuide(47);
	this->removeFromParentAndCleanup(true);
}

void ResDetailsLayer::updateHorseData(int addvalue)
{
	PackageData* horseData = NULL;
	if (g_hero->getAtrByType(H_MOUNT)->count > 0)
		horseData = g_hero->getAtrByType(H_MOUNT);
	else
	{
		bool isfind = false;
		std::map<int, std::vector<PackageData>>::iterator it;
		for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
		{
			for (unsigned int i = 0; i < StorageRoom::map_storageData[it->first].size(); i++)
			{
				PackageData *sdata = &StorageRoom::map_storageData[it->first][i];
				if (sdata->strid.compare("74") == 0)
				{
					isfind = true;
					horseData = sdata;
					break;
				}
			}
		}
		if (!isfind)
		{
			for (int i = 0; i < MyPackage::getSize(); i++)
			{
				if (MyPackage::vec_packages[i].strid.compare("74") == 0)
				{
					horseData =  &MyPackage::vec_packages[i];
					break;
				}
			}
		}
	}
	if (horseData != NULL)
	{
		cocos2d::ui::Text* horselbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("horselbl");
		horselbl->setVisible(true);
		horseData->goodvalue += addvalue;
		if (horseData->goodvalue > 100)
			horseData->goodvalue = 100;
		std::string temp = horselbl->getString().c_str();
		std::string horesstr = StringUtils::format(CommonFuncs::gbk2utf("白马（生命%d）").c_str(), horseData->goodvalue);
		horselbl->setString(horesstr);
	}
}

