#include "SettingLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#endif
SettingLayer::SettingLayer()
{
	clicktitlecount = 0;
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

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onTitle, this));

	//关闭按钮
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onBack, this));
	backbtn->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//ios 恢复购买按钮
	cocos2d::ui::Button* resumebuybtn = (cocos2d::ui::Button*)csbnode->getChildByName("resumebuybtn");
	resumebuybtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::onResumeBuy, this));
	resumebuybtn->setVisible(true);
#endif

	//checkbox
	m_soundCheckBox = (cocos2d::ui::CheckBox*)csbnode->getChildByName("soundcheck");
	m_soundCheckBox->addEventListener(CC_CALLBACK_2(SettingLayer::soundCheckBoxCallback, this));

	m_soundOnOffText = (cocos2d::ui::Text*)csbnode->getChildByName("settingdesc");

	updateSoundStatus();

	cocos2d::ui::Text* idlbl = (cocos2d::ui::Text*)csbnode->getChildByName("id");
	idlbl->setString(GlobalData::getMyID());

	cocos2d::ui::TextField* name = (cocos2d::ui::TextField*)csbnode->getChildByName("name");
	name->setString(GlobalData::getMyNickName());
	name->addEventListener(CC_CALLBACK_2(SettingLayer::textFieldEvent, this));
	name->setVisible(false);

	cocos2d::ui::EditBox* editName = cocos2d::ui::EditBox::create(Size(380, 45), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	editName->setPosition(Point(230, 875));
	editName->setAnchorPoint(Vec2(0, 0.5));
	editName->setFontColor(Color3B::BLACK);
	editName->setPlaceHolder("请输入昵称:");
	editName->setPlaceholderFontSize(45);
	editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	editName->setPlaceholderFontColor(Color3B::WHITE);
	editName->setMaxLength(14);
	editName->setText(GlobalData::getMyNickName().c_str());
	//editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	editName->setDelegate(this);
	csbnode->addChild(editName);

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
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
		m_soundCheckBox->setSelected(false);
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
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void SettingLayer::onResumeBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		restoreBuy();
	}
}
#endif

void SettingLayer::onTitle(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		clicktitlecount++;
		if (clicktitlecount > 5 && !GameDataSave::getInstance()->getIsJustData())
		{
			GameDataSave::getInstance()->setIsJustData(true);
		}
	}
}


void SettingLayer::textFieldEvent(Ref * pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		cocos2d::ui::TextField * textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		std::string str = textField->getString();
	}
			break;
		case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
			break;
		case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			break;
		case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			break;
	}
}


void SettingLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void SettingLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	GlobalData::setMyNickName(editBox->getText());
}

void SettingLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void SettingLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}