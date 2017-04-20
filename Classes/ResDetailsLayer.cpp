#include "ResDetailsLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "StorageRoom.h"
#include "GameScene.h"
#include "Hero.h"
#include "BuildingUILayer.h"
#include "StorageUILayer.h"
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

	if (pdata->type == FOOD || pdata->type == MEDICINAL)
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
		countstr = StringUtils::format("功法等级%d", pdata->lv);
	valuelbl->setString(CommonFuncs::gbk2utf(countstr.c_str()));

	cocos2d::ui::Text* resdesc = (cocos2d::ui::Text*)csbnode->getChildByName("desclbl");
	resdesc->setString(pdata->desc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		removSelf();
		return true;
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

void ResDetailsLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
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
					int hungervale = g_hero->getHungerValue();
					if (addvalue + hungervale > Hero::MAXHungerValue)
						g_hero->setHungerValue(Hero::MAXHungerValue);
					else
						g_hero->setHungerValue(addvalue + hungervale);

					StorageRoom::use(m_packageData->strid);
					break;
				}
			}

			if (!isInres)
			{
				for (unsigned int i = 0; i < BuildingUILayer::map_buidACData["cooktable"].size(); i++)
				{
					if (m_packageData->strid.compare(BuildingUILayer::map_buidACData["cooktable"][i].icon) == 0)
					{
						int addvalue = BuildingUILayer::map_buidACData["cooktable"][i].ep[0];

						int hungervale = g_hero->getHungerValue();
						if (addvalue + hungervale > Hero::MAXHungerValue)
							g_hero->setHungerValue(Hero::MAXHungerValue);
						else
							g_hero->setHungerValue(addvalue + hungervale);

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
					int outvalue = g_hero->getOutinjuryValue();
					if (wvalue + outvalue > Hero::MAXOutinjuryValue)
						g_hero->setOutinjuryValue(Hero::MAXOutinjuryValue);
					else
						g_hero->setOutinjuryValue(wvalue + outvalue);
					int invalue = g_hero->getInnerinjuryValue();
					if (invalue + nvalue > Hero::MAXInnerinjuryValue)
						g_hero->setInnerinjuryValue(Hero::MAXInnerinjuryValue);
					else
						g_hero->setInnerinjuryValue(invalue + nvalue);
					StorageRoom::use(m_packageData->strid);
					break;
				}
			}
			if (!isInRes)
			{
				for (unsigned int i = 0; i < BuildingUILayer::map_buidACData["medicinekit"].size(); i++)
				{
					if (m_packageData->strid.compare(BuildingUILayer::map_buidACData["medicinekit"][i].icon) == 0)
					{
						int wvalue = BuildingUILayer::map_buidACData["medicinekit"][i].ep[0];
						int nvalue = BuildingUILayer::map_buidACData["medicinekit"][i].ep[1];
						int outvalue = g_hero->getOutinjuryValue();
						if (wvalue + outvalue > Hero::MAXOutinjuryValue)
							g_hero->setOutinjuryValue(Hero::MAXOutinjuryValue);
						else
							g_hero->setOutinjuryValue(wvalue + outvalue);
						int invalue = g_hero->getInnerinjuryValue();
						if (invalue + nvalue > Hero::MAXInnerinjuryValue)
							g_hero->setInnerinjuryValue(Hero::MAXInnerinjuryValue);
						else
							g_hero->setInnerinjuryValue(invalue + nvalue);
						StorageRoom::use(m_packageData->strid);
						break;
					}
				}
			}

		}
		StorageUILayer* storageUI = (StorageUILayer*)this->getParent();
		storageUI->updateResContent();
		removSelf();
	}
}

void ResDetailsLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

