#include "StartScene.h"
#include "SelectHeroScene.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "SettingLayer.h"
#include "SoundManager.h"
#include "SelectSaveLayer.h"
#include "WaitingProgress.h"
#include "GameDataSave.h"
#include "HintBox.h"
#include "NoticeLayer.h"
#include "Const.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

USING_NS_CC;

StartScene::StartScene()
{

}
StartScene::~StartScene()
{

}

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	StartScene* startlayer = StartScene::create();

    // add layer as a child to scene
	scene->addChild(startlayer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Node* csbnode = CSLoader::createNode("startLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* newstartbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("newstartbtn");
	newstartbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onNewStart, this));


	cocos2d::ui::Widget* savedbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("savedbtn");
	savedbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onLoadSaved, this));

	m_continuebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("continuebtn");
	m_continuebtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onContinue, this));
	std::string uid = GlobalData::getUId();

	m_continuebtn->setEnabled(uid.length() <= 0 ? false : true);

	cocos2d::ui::Widget* setbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("setbtn");
	setbtn->addTouchEventListener(CC_CALLBACK_2(StartScene::onSet, this));

	cocos2d::ui::ImageView* logo = (cocos2d::ui::ImageView*)csbnode->getChildByName("logo");
	logo->addTouchEventListener(CC_CALLBACK_2(StartScene::onLogo, this));

	cocos2d::ui::Text* vesiontxt = (cocos2d::ui::Text*)csbnode->getChildByName("version");
	vesiontxt->setString(GlobalData::getVersion());

	int rqq = GlobalData::createRandomNum(2);
	cocos2d::ui::Text* qq1 = (cocos2d::ui::Text*)csbnode->getChildByName("qq");
	qq1->setString(QQNUM[rqq]);
	qq1->addTouchEventListener(CC_CALLBACK_2(StartScene::onQQ, this));

	cocos2d::ui::Text* qq2 = (cocos2d::ui::Text*)csbnode->getChildByName("qq_1");
	qq2->setString(QQNUM[1-rqq]);
	qq2->addTouchEventListener(CC_CALLBACK_2(StartScene::onQQ, this));

	clicklogocount = 0;
	isdouserdata = false;
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_START);


	this->scheduleOnce(schedule_selector(StartScene::checkServerData), 0.05f);
	
    return true;
}

void StartScene::onExit()
{
	Layer::onExit();
}

void StartScene::onNewStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{	
		Scene* scene = SelectHeroScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void StartScene::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		WaitingProgress* waitbox = WaitingProgress::create("进入游戏...");
		this->addChild(waitbox, 1, "waitbox");
		std::string uid = GlobalData::getUId();
		GlobalData::setUId(uid);

		Scene* scene = GameScene::createScene();

		Director::getInstance()->replaceScene(scene);
	}
}

void StartScene::onLoadSaved(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(SelectSaveLayer::create());
	}
}

void StartScene::onSet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SettingLayer* layer = SettingLayer::create();
		addChild(layer);
	}
}

void StartScene::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		copytoclipboard((char*)qq->getString().c_str());
#endif
	}

}

void StartScene::checkServerData(float dt)
{
	if (GlobalData::getUId().length() > 0)
	{
		if (!GameDataSave::getInstance()->getIsPostAllData())
		{
			WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

			std::vector<std::string> vec_ids = GlobalData::getSaveListId();

			std::vector<std::string> vec_userid;
			for (unsigned int i = 0; i < vec_ids.size(); i++)
			{
				std::string saveuid = vec_ids[i];
				if (saveuid.length() > 0)
				{
					vec_userid.push_back(saveuid);
				}
			}

			int tag = 0;
			for (unsigned int i = 0; i < vec_userid.size(); i++)
			{
				if (i == vec_userid.size() - 1)
					tag = 1;
				ServerDataSwap::init(this)->postOneData(vec_userid[i], tag);
			}

			//ServerDataSwap::init(this)->postAllData();
			isdouserdata = true;
		}
	}
	else
	{
		WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getAllData();
		isdouserdata = true;
	}

	if (!isdouserdata)
	{
		if (GlobalData::getNoPopNoticeDay() != GlobalData::getDayOfYear())
		{
			WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

			ServerDataSwap::init(this)->getannouncement();
		}
	}
}

void StartScene::onLogo(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		clicklogocount++;
		if (clicklogocount == 5)
		{
			WaitingProgress* waitbox = WaitingProgress::create("数据处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			ServerDataSwap::init(this)->propadjust();
		}
	}
}

void StartScene::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	if (clicklogocount >= 5)
	{
		HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf("数据处理完毕，请确认。感谢您的支持！"));
		this->addChild(hbox);
	}
	else
	{
		if (isdouserdata)
		{
			isdouserdata = false;
			GameDataSave::getInstance()->setIsPostAllData(true);

			m_continuebtn->setEnabled(GlobalData::getUId().length() <= 0 ? false : true);

			if (GameDataSave::getInstance()->getHeroLV() >= 1)
			{
				for (int i = 0; i < 50; i++)
				{
					GameDataSave::getInstance()->setIsNewerGuide(i, 0);
				}
			}
			GlobalData::init();
			if (GlobalData::getNoPopNoticeDay() != GlobalData::getDayOfYear())
			{
				WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
				ServerDataSwap::init(this)->getannouncement();
			}
		}
		else
		{
			Director::getInstance()->getRunningScene()->addChild(NoticeLayer::create(GlobalData::noticecontent), 1);
		}
	}
}

void StartScene::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (clicklogocount >= 5)
	{
		std::string descstr = "请与客服联系，稍后重试！";
		if (errcode == -2)
			descstr = "没有数据，请与客服确认！";
		HintBox* hbox = HintBox::create(CommonFuncs::gbk2utf(descstr.c_str()));
		this->addChild(hbox);
	}
}
