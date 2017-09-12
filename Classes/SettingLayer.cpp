#include "SettingLayer.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameDataSave.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif
#include "HintBox.h"
#include "WaitingProgress.h"

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

    mynamestr = GlobalData::getMyNickName();
    
	cocos2d::ui::TextField* name = (cocos2d::ui::TextField*)csbnode->getChildByName("name");
	name->setString(mynamestr);
	name->addEventListener(CC_CALLBACK_2(SettingLayer::textFieldEvent, this));
	name->setVisible(false);

	m_editName = cocos2d::ui::EditBox::create(Size(380, 44), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	m_editName->setPosition(Point(225, 875));
	m_editName->setAnchorPoint(Vec2(0, 0.5));
	m_editName->setFontColor(Color3B::BLACK);
	m_editName->setPlaceHolder(CommonFuncs::gbk2utf("请输入昵称:").c_str());
	m_editName->setPlaceholderFontSize(30);
	m_editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	m_editName->setPlaceholderFontColor(Color3B::WHITE);
	m_editName->setMaxLength(12);
	m_editName->setText(mynamestr.c_str());
	//editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_editName->setDelegate(this);
	csbnode->addChild(m_editName);

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
    editboxstr = editBox->getText();
    std::string utf8str = editboxstr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    utf8str = gbkToUTF8(editboxstr.c_str());
#endif
    
    WaitingProgress* waitbox = WaitingProgress::create("处理中...");
    Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
    
	ServerDataSwap::init(this)->modifyNickName(utf8str);
}

void SettingLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void SettingLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}

void SettingLayer::onSuccess()
{

    GlobalData::setMyNickName(editboxstr);
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void SettingLayer::onErr(int errcode)
{
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	HintBox* hintbox;
	if (errcode == 2)
	{
		hintbox = HintBox::create(CommonFuncs::gbk2utf("抱歉！输入中包含敏感关键字，修改失败！"));
	}
	else if (errcode == 3)
	{
		hintbox = HintBox::create(CommonFuncs::gbk2utf("抱歉！该昵称已存在，大侠换一个吧！"));
	}
	else
	{
		hintbox = HintBox::create(CommonFuncs::gbk2utf("网络异常，修改失败！"));
	}
	this->addChild(hintbox);

	m_editName->setText(mynamestr.c_str());

}
