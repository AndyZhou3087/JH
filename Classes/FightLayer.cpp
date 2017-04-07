#include "FightLayer.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "CommonFuncs.h"
#include "HintBox.h"
#include "GameScene.h"
#include "Winlayer.h"

FightLayer::FightLayer()
{
}


FightLayer::~FightLayer()
{
}

FightLayer* FightLayer::create(std::string addrname, std::string npcname)
{
	FightLayer *pRet = new FightLayer();
	if (pRet && pRet->init(addrname, npcname))
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

bool FightLayer::init(std::string addrname, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("fightLayer.csb");
	this->addChild(csbnode);

	m_addrname = addrname;
	m_npcid = npcid;

	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(addrname);

	cocos2d::ui::ImageView* npcicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");

	std::string str = StringUtils::format("ui/%s.png", npcid.c_str());
	SpriteFrame* sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
	if (sf != NULL)
	{
		npcicon->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		npcicon->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	}

	cocos2d::ui::Text* npcnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcnametxt->setString(GlobalData::map_npcs[npcid].name);
	
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	heronametxt->setString(g_hero->getMyName());

	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);

	int herohppercent = 100 *  g_hero->getLifeValue() / g_hero->getMaxLifeValue();

	herohpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");
	herohpbar->setPercent(herohppercent);

	npcmaxhp = GlobalData::map_npcs[npcid].life;
	npchp = npcmaxhp;

	npcatk = GlobalData::map_npcs[npcid].atk;
	npcdf = GlobalData::map_npcs[npcid].df;

	npchpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");
	hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");
	npchpbar->setPercent(npchppercent);

	m_escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(FightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	m_fihgtScorll = UIScroll::create(650.0f, 450.0f);
	m_fihgtScorll->setPosition(Vec2(37, 128));
	csbnode->addChild(m_fihgtScorll);

	this->schedule(schedule_selector(FightLayer::updata), 1.0f);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void FightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (node->getTag() == 0)
		{
			HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("逃跑成功！"));
			addChild(hbox);
			m_escapebtn->setTitleText(CommonFuncs::gbk2utf("返回"));
			m_escapebtn->setTag(1);
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}

void FightLayer::updata(float dt)
{
	int gfBonusAck = 0;
	if (g_hero->getAtrByType(H_WG) != NULL)
	{
		std::string gfname = g_hero->getAtrByType(H_WG)->strid;
		gfBonusAck = GlobalData::map_wgngs[gfname].vec_bns[GlobalData::map_wgngs[gfname].lv];
	}

	int heroCurAck = g_hero->getAtkValue() + gfBonusAck;
	int npchurt = heroCurAck - npcdf;
	if (npchurt < heroCurAck * 10 / 100)
		npchp -= heroCurAck * 10 / 100;
	else
		npchp -= npchurt;

	if (npchp < 0)
		npchp = 0;


	std::string hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar->setPercent(npchppercent);

	if (npchp <= 0)
	{
		this->unschedule(schedule_selector(FightLayer::updata));

		Winlayer* layer = Winlayer::create(m_addrname, m_npcid);
		Director::getInstance()->getRunningScene()->addChild(layer);
		this->removeFromParentAndCleanup(true);
		return;
	}

	int gfBonusDf = 0;
	if (g_hero->getAtrByType(H_NG) != NULL)
	{
		std::string gfname = g_hero->getAtrByType(H_NG)->strid;
		gfBonusDf = GlobalData::map_wgngs[gfname].vec_bns[GlobalData::map_wgngs[gfname].lv];
	}

	int curheroHp = 0;
	int curheroDf = g_hero->getDfValue() + gfBonusDf;
	int herohurt = npcatk - curheroDf;

	if (herohurt < npcatk * 10 / 100)
		curheroHp = g_hero->getLifeValue() - npcatk * 10 / 100;
	else
		curheroHp -= herohurt;

	if (curheroHp < 0)
		curheroHp = 0;
	g_hero->setLifeValue(curheroHp);

	hpstr = StringUtils::format("%d/%d", g_hero->getLifeValue(), g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	int herohppercent = 100 * g_hero->getLifeValue() / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);

	if (g_hero->getLifeValue() <= 0)
	{
		this->unschedule(schedule_selector(FightLayer::updata));
	}

}