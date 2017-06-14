#include "StoryScene.h"
#include "CommonFuncs.h"
#include "GameScene.h"
StoryScene::StoryScene()
{
	m_wordlbl = NULL;
	isCanClick = false;
}


StoryScene::~StoryScene()
{
}

Scene* StoryScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StoryScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StoryScene::init()
{
	clickcount = 0;
	m_csbnode = CSLoader::createNode("story1.csb");
	m_csbnode->setPosition(Vec2(360, 640));
	this->addChild(m_csbnode);

	m_action = CSLoader::createTimeline("story1.csb");
	m_csbnode->runAction(m_action);
	m_action->gotoFrameAndPlay(0, 105, false);

	this->scheduleOnce(schedule_selector(StoryScene::showClickText), 10.5f);

		////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (!isCanClick)
		{
			this->unschedule(schedule_selector(StoryScene::showClickText));
			showClickText(0);
			m_action->gotoFrameAndPlay(m_action->getEndFrame(), false);
			return true;
		}
		if (clickcount == 1)
		{
			isCanClick = false;
			this->scheduleOnce(schedule_selector(StoryScene::delayShowNextStory), 0.1f);
		}
		else if (clickcount == 2)
		{
			isCanClick = false;
			this->scheduleOnce(schedule_selector(StoryScene::showNextScene), 0.1f);
		}
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
	return true;
}

StoryScene* StoryScene::create()
{
	StoryScene *pRet = new StoryScene();
	if (pRet && pRet->init())
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

void StoryScene::delayShowNextStory(float dt)
{
	m_csbnode->removeFromParentAndCleanup(true);
	if (m_wordlbl != NULL)
		m_wordlbl->removeFromParentAndCleanup(true);
	m_csbnode = CSLoader::createNode("story2.csb");
	m_csbnode->setPosition(Vec2(360, 640));
	this->addChild(m_csbnode);

	m_action = CSLoader::createTimeline("story2.csb");
	m_csbnode->runAction(m_action);
	m_action->gotoFrameAndPlay(0, 90, false);
	this->scheduleOnce(schedule_selector(StoryScene::showClickText), 10.0f);
}

void StoryScene::showClickText(float dt)
{
	m_wordlbl = Label::createWithTTF(CommonFuncs::gbk2utf("点击屏幕继续"), "fonts/STXINGKA.TTF", 30);
	m_wordlbl->setPosition(Vec2(360, 20));
	m_wordlbl->runAction(RepeatForever::create(Blink::create(2, 2)));
	this->addChild(m_wordlbl);
	clickcount++;
	isCanClick = true;
}
void StoryScene::showNextScene(float dt)
{
	auto transition = TransitionCrossFade::create(0.5f, GameScene::createScene());
	Director::getInstance()->replaceScene(transition);
}