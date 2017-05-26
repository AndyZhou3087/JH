#include "ResDetailsLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Hero.h"
#include "StorageUILayer.h"
#include "SoundManager.h"

#define WINESTRID "23"

int ResDetailsLayer::whereClick = 0;//0--仓库，1其他

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
	Node* csbnode = CSLoader::createNode("resDetailsLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onOk, this));

	cocos2d::ui::Button* usebtn = (cocos2d::ui::Button*)csbnode->getChildByName("usebtn");
	usebtn->addTouchEventListener(CC_CALLBACK_2(ResDetailsLayer::onUse, this));

	uselbl = (cocos2d::ui::Text*)usebtn->getChildByName("valuelbl");

	if ((pdata->type == FOOD || (pdata->type == MEDICINAL && pdata->strid.compare(WINESTRID) != 0)) && whereClick == 0)
		okbtn->setTitleText(CommonFuncs::gbk2utf("使用"));
	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("buildsmall")->getChildByName("Image");

	std::string str = StringUtils::format("ui/%s.png", pdata->strid.c_str());
	resimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	resimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	cocos2d::ui::Text* resname = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");

	resname->setString(pdata->name);

	cocos2d::ui::Text* valuelbl = (cocos2d::ui::Text*)csbnode->getChildByName("valuelbl");
	int count = StorageRoom::getCountById(pdata->strid);
	std::string countstr;

	if (pdata->type == FOOD || pdata->type == MEDICINAL || pdata->type == RES_1)
		countstr = StringUtils::format("库存%d", count);
	else if (pdata->type == WEAPON || pdata->type == PROTECT_EQU || pdata->type == TOOLS)
		countstr = StringUtils::format("耐久度%d%%", pdata->goodvalue);
	else if (pdata->type == N_GONG || pdata->type == W_GONG)
	{
		countstr = StringUtils::format("功法等级%d", pdata->lv);
		usebtn->setVisible(true);
		okbtn->setPositionX(460);

		std::string uselblstr = StringUtils::format("大力丸 x%d", StorageRoom::getCountById("71"));
		uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
		
	}
	else if (pdata->type == RES_2)
	{
		if (pdata->strid.compare("70") == 0)
		{
			usebtn->setVisible(true);
			okbtn->setPositionX(460);
			std::string uselblstr = StringUtils::format("经验药水 x%d", StorageRoom::getCountById("70"));
			uselbl->setString(CommonFuncs::gbk2utf(uselblstr.c_str()));
		}
	}

	valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

	cocos2d::ui::Text* resdesc = (cocos2d::ui::Text*)csbnode->getChildByName("desclbl");
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
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		if (whereClick == 0)
		{
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
							g_hero->setOutinjuryValue(g_hero->getOutinjuryValue() - 10);
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

			}

			if (m_packageData->type == FOOD || m_packageData->type == MEDICINAL)
			{
				StorageUILayer* storageUI = (StorageUILayer*)this->getParent();
				storageUI->updateResContent();
			}
		}
		removSelf();
	}
}

void ResDetailsLayer::onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
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
					curlv = GlobalData::map_wgngs[gfname].lv;
					gfData->exp += resdata.ep[0];
					for (i = 0; i < vec_gfExp.size(); i++)
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
			for (i = 0; i < vec_heroExp.size(); i++)
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
				
			}
		}
		StorageUILayer* storageUI = (StorageUILayer*)this->getParent();
		storageUI->updateResContent();
		removSelf();
	}
}
void ResDetailsLayer::recoveInjuryValue(int addwvalue, int addnvalue)
{
	int outvalue = g_hero->getOutinjuryValue();
	if (addwvalue + outvalue > g_hero->getMaxOutinjuryValue())
		g_hero->setOutinjuryValue(g_hero->getMaxOutinjuryValue());
	else
		g_hero->recoverOutjury(addwvalue);
	int invalue = g_hero->getInnerinjuryValue();
	if (invalue + addnvalue > g_hero->getMaxInnerinjuryValue())
		g_hero->setInnerinjuryValue(g_hero->getMaxInnerinjuryValue());
	else
		g_hero->recoverInjury(addnvalue);
}

void ResDetailsLayer::recoveHungerValue(int addvalue)
{
	int hungervale = g_hero->getHungerValue();
	if (addvalue + hungervale > g_hero->getMaxHungerValue())
		g_hero->setHungerValue(g_hero->getMaxHungerValue());
	else
		g_hero->recoverHunger(addvalue);
}

void ResDetailsLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

