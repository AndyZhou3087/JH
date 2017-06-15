#include "ExerciseDoneLayer.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "Const.h"
ExerciseDoneLayer::ExerciseDoneLayer()
{

}


ExerciseDoneLayer::~ExerciseDoneLayer()
{
}

bool ExerciseDoneLayer::init(int hour)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("exerciseDoneLayer.csb");
	this->addChild(csbnode);

	m_unlockbg = (cocos2d::ui::Widget*)csbnode->getChildByName("unlockbg");
	m_unlockbg->setScale(0);
	//m_unlockbg->runAction(Sequence::create(ScaleTo::create(0.3f, 0.7f), ScaleTo::create(0.7f, 1.0f), NULL));
	m_unlockbg->runAction(ScaleTo::create(0.5f, 1.0f));
	m_title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	m_title->setVisible(false);

	m_herotext = (cocos2d::ui::Text*)csbnode->getChildByName("exptext");
	m_herotext->setVisible(false);
	std::string str = StringUtils::format("角色经验值： +%d", EXSERCISE_DONE_HEROEXP);
	m_herotext->setString(CommonFuncs::gbk2utf(str.c_str()));

	m_wgtext = (cocos2d::ui::Text*)csbnode->getChildByName("wgtext");
	m_wgtext->setVisible(false);
	str = StringUtils::format("武功经验值： +%d", EXSERCISE_DONE_GFEXP * hour);
	m_wgtext->setString(CommonFuncs::gbk2utf(str.c_str()));

	m_ngtext = (cocos2d::ui::Text*)csbnode->getChildByName("ngtext");
	m_ngtext->setVisible(false);
	str = StringUtils::format("内功经验值： +%d", EXSERCISE_DONE_GFEXP * hour);
	m_ngtext->setString(CommonFuncs::gbk2utf(str.c_str()));

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(ExerciseDoneLayer::showText), 0.6f);

	return true;
}

ExerciseDoneLayer* ExerciseDoneLayer::create(int hour)
{
	ExerciseDoneLayer *pRet = new ExerciseDoneLayer();
	if (pRet && pRet->init(hour))
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

void ExerciseDoneLayer::showText(float dt)
{
	m_title->setOpacity(0);
	m_title->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_herotext->setOpacity(0);
	m_herotext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));
	m_wgtext->setOpacity(0);
	m_wgtext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	m_ngtext->setOpacity(0);
	m_ngtext->runAction(Sequence::create(Show::create(), FadeIn::create(1.0f), NULL));

	this->scheduleOnce(schedule_selector(ExerciseDoneLayer::removeself), 3.5f);
}

void ExerciseDoneLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}