/********************************************************************
* 战斗UI界面
*********************************************************************/
#ifndef _FIGHT_LAYER_H_
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

	/****************************
	初始化
	@param addrname 地点名称
	@param npcid NPC ID
	****************************/
	bool init(std::string addrname, std::string npcid);

	static FightLayer* create(std::string addrid, std::string npcid);
private:
	/****************************
	逃跑按钮回调
	****************************/
	void onEscape (cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	NPC 回击
	定时器执行
	****************************/
	void delayBossFight(float dt);

	/****************************
	显示 战斗文字
	@param type type:0-hero word;type:1-boss word;
	@param value 伤害数值
	****************************/
	void showFightWord(int type, int value);

	/****************************
	更改特殊字的颜色
	@param wordstr 文字
	****************************/
	void checkWordLblColor(std::string wordstr);
private:
	UIScroll* m_fihgtScorll;//文字滚动控件
	cocos2d::ui::Button* m_escapebtn;//逃跑按钮控件
	cocos2d::ui::Text* herohpvaluetext;//角色血量控件
	cocos2d::ui::Text* npchpvaluetext;//NCP血量控件
	cocos2d::ui::LoadingBar* herohpbar;//角色血量进度条控件
	cocos2d::ui::LoadingBar* npchpbar;//NPC血量进度条控件
	int npchp;//NPC 血量
	int npcmaxhp;// NPC 最大血量
	int npcatk;//NPC 攻击值
	int npcdf;//NPC 防御值

	/****************************
	定时更新UI
	****************************/
	void updata(float dt);

	std::string m_addrid;//地点ID
	std::string m_npcid;//NPC ID
};
#endif

