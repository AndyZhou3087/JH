#include "PlayerChallengeResultLayer.h"
#include "CommonFuncs.h"
#include "GameScene.h"
#include "HintBox.h"
#include "WaitingProgress.h"
#include "Const.h"
#include "SoundManager.h"
#include "GameDataSave.h"

PlayerChallengeResultLayer::PlayerChallengeResultLayer()
{
	GlobalData::isPlayerChallenging = false;
	GameDataSave::getInstance()->setPlayerChallengeData("");
}


PlayerChallengeResultLayer::~PlayerChallengeResultLayer()
{

}


PlayerChallengeResultLayer* PlayerChallengeResultLayer::create(RankData* fightPlayerData, int win)
{
	PlayerChallengeResultLayer *pRet = new PlayerChallengeResultLayer();
	if (pRet && pRet->init(fightPlayerData, win))
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

bool PlayerChallengeResultLayer::init(RankData* fightPlayerData, int win)
{
	m_csbnode = CSLoader::createNode("playerChallengeResultLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(PlayerChallengeResultLayer::onBack, this));

	std::string str = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("heroimg");
	headimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintitle%d.png", win);
	cocos2d::ui::ImageView* wintitle = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintitle");
	wintitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/winicon%d.png", win);
	cocos2d::ui::ImageView* winicon = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winicon");
	winicon->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/winbox%d.png", win);
	cocos2d::ui::ImageView* winbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winbox");
	winbox->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintext%d.png", win);
	cocos2d::ui::ImageView* wintext = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintext");
	wintext->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("explbl");

	cocos2d::ui::TextBMFont* ranknum = (cocos2d::ui::TextBMFont*)m_csbnode->getChildByName("ranknum");
	str = StringUtils::format("%d", GlobalData::myrank);

	cocos2d::ui::Text* rankup = (cocos2d::ui::Text*)m_csbnode->getChildByName("rankup");

	if (win == 1)
	{
		explbl->setString("+3");
		if (fightPlayerData->rank < GlobalData::myrank)
		{
			str = StringUtils::format("%d", fightPlayerData->rank);
			ranknum->setString(str);

			std::string upstr = StringUtils::format("%d", GlobalData::myrank - fightPlayerData->rank);
			rankup->setString(upstr);
			GlobalData::myrank = fightPlayerData->rank;
		}
		GlobalData::myFihgtexp += 3;
	}
	else
	{
		explbl->setString("-1");
		GlobalData::myFihgtexp -= 1;
		rankup->setVisible(false);
		winicon->setVisible(false);
	}

	GlobalData::myFihgtCount--;
	
	
	ServerDataSwap::init(this)->getChallengeResult(fightPlayerData->playerid, fightPlayerData->herotype, fightPlayerData->rank, win);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void PlayerChallengeResultLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void PlayerChallengeResultLayer::onSuccess()
{
	
}

void PlayerChallengeResultLayer::onErr(int errcode)
{

	HintBox * box = HintBox::create(CommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
