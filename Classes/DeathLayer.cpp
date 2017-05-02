﻿#include "DeathLayer.h"
#include "CommonFuncs.h"
#include "StartScene.h"
DeathLayer::DeathLayer()
{

}


DeathLayer::~DeathLayer()
{
}

bool DeathLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("deathLayer.csb");
	this->addChild(csbnode);

	csbnode->runAction(CCFadeIn::create(3.0f));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		Director::getInstance()->replaceScene(StartScene::createScene());
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

DeathLayer* DeathLayer::create()
{
	DeathLayer *pRet = new DeathLayer();
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