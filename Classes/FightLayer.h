﻿#ifndef _FIGHT_LAYER_H_
#define _FIGHT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
USING_NS_CC;
class FightLayer :public Layer
{
public:
	FightLayer();
	~FightLayer();

	bool init(std::string addrname, std::string npcid);

	static FightLayer* create(std::string addrid, std::string npcid);
private:
	void onEscape (cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void delayBossFight(float dt);
	void showFightWord(int type, int value);//type:0-hero;type:1-boss;
	void checkWordLblColor(std::string wordstr);
private:
	UIScroll* m_fihgtScorll;
	cocos2d::ui::Button* m_escapebtn;
	cocos2d::ui::Text* herohpvaluetext;
	cocos2d::ui::Text* npchpvaluetext;
	cocos2d::ui::LoadingBar* herohpbar;
	cocos2d::ui::LoadingBar* npchpbar;
	int npchp;
	int npcmaxhp;
	int npcatk;
	int npcdf;
	void updata(float dt);
	std::string m_addrid;
	std::string m_npcid;
};
#endif

