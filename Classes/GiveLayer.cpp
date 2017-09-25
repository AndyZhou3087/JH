#include "GiveLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "MyPackage.h"
#include "Const.h"
#include "GameScene.h"
#include "StorageRoom.h"
#include "SoundManager.h"
#include "NpcLayer.h"
#include "MyMenu.h"

GiveLayer::GiveLayer()
{
	lastMyGoodsSrollViewHeight = -1;
	lastGiveGoodsSrollViewHeight = -1;
}


GiveLayer::~GiveLayer()
{

}


GiveLayer* GiveLayer::create(std::string npcid)
{
	GiveLayer *pRet = new GiveLayer();
	if (pRet && pRet->init(npcid))
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

bool GiveLayer::init(std::string npcid)
{
	Node* csbnode = CSLoader::createNode("giveLayer.csb");
	this->addChild(csbnode);

	m_npcid = npcid;

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(GiveLayer::onBack, this));

	m_givebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("givebtn");
	m_givebtn->addTouchEventListener(CC_CALLBACK_2(GiveLayer::onGive, this));

	cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::Widget::TextureResType::PLIST);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(GlobalData::map_npcs[npcid].name);

	m_giveGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("givescroll");
	m_giveGoodsSrollView->setScrollBarEnabled(false);
	m_myGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("mygoodsscroll");
	m_myGoodsSrollView->setScrollBarEnabled(false);

	giveval = 0;
	friendly = GlobalData::map_myfriendly[m_npcid].friendly;
	friendlylbl = (cocos2d::ui::Text*)csbnode->getChildByName("friendly");

	if (friendly < -100000 || friendly > 100000)
		friendly = 0;

	std::string friendstr = StringUtils::format("%d", friendly);
	friendlylbl->setString(friendstr);


	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		myGoodsData.push_back(MyPackage::vec_packages[i]);
	}
	updataMyGoodsUI();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GiveLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void GiveLayer::onGiveGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGiveData.erase(myGiveData.begin() + datatag);
	}
	updateMyGoods(udata);

	updata();
}

void GiveLayer::updateMyGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGoodsData.size(); i++)
	{
		if (data.strid.compare(myGoodsData[i].strid) == 0 && (myGoodsData[i].type == FOOD || myGoodsData[i].type == MEDICINAL || myGoodsData[i].type == RES_1 || myGoodsData[i].type == RES_2))
		{
			if (myGoodsData[i].count < 10)
			{
				myGoodsData[i].count++;
				break;
			}
			else
				continue;
		}
	}
	if (i == myGoodsData.size())
	{
		data.count = 1;
		myGoodsData.push_back(data);
	}

}

void GiveLayer::updateGiveGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGiveData.size(); i++)
	{
		if (data.strid.compare(myGiveData[i].strid) == 0 && (myGiveData[i].type == FOOD || myGiveData[i].type == MEDICINAL || myGiveData[i].type == RES_1 || myGiveData[i].type == RES_2))
		{
			myGiveData[i].count++;
			break;
		}
	}
	if (i == myGiveData.size())
	{
		data.count = 1;
		myGiveData.push_back(data);
	}

}

void GiveLayer::onMyGoodsItem(cocos2d::Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGoodsData.erase(myGoodsData.begin() + datatag);
	}
	updateGiveGoods(udata);
	updata();
}

void GiveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void GiveLayer::onGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		MyPackage::vec_packages.clear();

		for (unsigned int i = 0; i < myGoodsData.size(); i++)
		{
			MyPackage::vec_packages.push_back(myGoodsData[i]);
		}
		GlobalData::map_myfriendly[m_npcid].friendly = friendly + giveval;
		GlobalData::saveFriendly();
		MyPackage::save();
		NpcLayer* npclayer = (NpcLayer*)this->getParent();
		npclayer->reFreshFriendlyUI();
		this->removeFromParentAndCleanup(true);
	}
}
void GiveLayer::checkValue()
{
	giveval = 0;

	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		std::string resid = myGiveData[i].strid;
		giveval += GlobalData::map_allResource[resid].fval * myGiveData[i].count;
	}
	std::string friendstr = StringUtils::format("%d", friendly + giveval);
	friendlylbl->setString(friendstr);
}

void GiveLayer::updata()
{
	updataMyGoodsUI();
	updataGiveGoodsUI();
	checkValue();
}

void GiveLayer::updataMyGoodsUI()
{
	int size = myGoodsData.size();

	m_myGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = row * 140;

	if (lastMyGoodsSrollViewHeight != innerheight)
	{
		lastMyGoodsSrollViewHeight = innerheight;
		int contentheight = m_myGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		m_myGoodsSrollView->setInnerContainerSize(Size(m_myGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allMydata;
	for (unsigned int i = 0; i < myGoodsData.size(); i++)
	{
		allMydata.push_back(&myGoodsData[i]);
	}

	int allsize = allMydata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData* tmpdata = allMydata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(GiveLayer::onMyGoodsItem, this));
		boxItem->setUserData(allMydata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_myGoodsSrollView);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_myGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void GiveLayer::updataGiveGoodsUI()
{
	int size = myGiveData.size();

	m_giveGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_giveGoodsSrollView->getInnerContainerSize().height;
	if (lastGiveGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_giveGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastGiveGoodsSrollViewHeight = innerheight;
		m_giveGoodsSrollView->setInnerContainerSize(Size(m_giveGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allNpcdata;
	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		allNpcdata.push_back(&myGiveData[i]);
	}

	int allsize = allNpcdata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData *tmpdata = allNpcdata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", GlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(GiveLayer::onGiveGoodsItem, this));
		boxItem->setUserData(allNpcdata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		MyMenu* menu = MyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_giveGoodsSrollView);

		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_giveGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}
