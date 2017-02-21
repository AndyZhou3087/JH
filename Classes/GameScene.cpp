#include "GameScene.h"
#include "TopBar.h"
#include "HomeLayer.h"
#include "UIScroll.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	homeLayer = HomeLayer::create();
	addChild(homeLayer);

	topBar = TopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1063));
	addChild(topBar);

	Sprite* bg = Sprite::createWithSpriteFrameName("ui/topeventwordbox.png");
	bg->setPosition(Vec2(visibleSize.width / 2, 980));
	this->addChild(bg);

	UIScroll* uiScroll = UIScroll::create(720.0f, 200.0f);
	uiScroll->setPosition(Vec2(visibleSize.width / 2, 980));
	uiScroll->addEventText("AAAAAAAA");
	//uiScroll->addEventText("bbbbbbbb");
	addChild(uiScroll);

    return true;
}