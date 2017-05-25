#include "SettingLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
SettingLayer::SettingLayer()
{

}


SettingLayer::~SettingLayer()
{

}

bool SettingLayer::init()
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("settingLayer.csb");
	this->addChild(csbnode);

	//关闭按钮
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onBack, this));

	//checkbox
	m_soundCheckBox = (cocos2d::ui::CheckBox*)csbnode->getChildByName("soundcheck");
	m_soundCheckBox->addEventListener(CC_CALLBACK_2(SettingLayer::soundCheckBoxCallback, this));

	m_soundOnOffText = (cocos2d::ui::Text*)csbnode->getChildByName("settingdesc");
	updateSoundStatus();

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		this->removeFromParentAndCleanup(true);
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void SettingLayer::updateSoundStatus()
{
	if (SoundManager::getInstance()->getSoundIsOn())
	{
		m_soundCheckBox->setSelected(true);
		m_soundOnOffText->setString(CommonFuncs::gbk2utf("开"));
	}
	else
	{
		m_soundCheckBox->setSelected(true);
		m_soundOnOffText->setString(CommonFuncs::gbk2utf("关"));
	}
}

void SettingLayer::soundCheckBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	switch(type)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			SoundManager::getInstance()->setIsSoundOn(true);
			SoundManager::getInstance()->setIsMusicOn(true);
			SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_START);
			//updateSoundStatus();
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			SoundManager::getInstance()->stopAllEffectSound();
			SoundManager::getInstance()->stopBackMusic();
			SoundManager::getInstance()->setIsSoundOn(false);
			SoundManager::getInstance()->setIsMusicOn(false);
			//updateSoundStatus();
			break;
		default:
			break;
	}
}

void SettingLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		this->removeFromParentAndCleanup(true);
	}
}