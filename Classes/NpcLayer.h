#ifndef _NPC_LAYER_H_
#define _NPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
#include "GlobalData.h"
USING_NS_CC;

class NpcLayer :public Layer
{
public:
	NpcLayer();
	~NpcLayer();

	bool init(std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static NpcLayer* create(std::string addrid);
	/****************************
	更新任务的图标显示
	@param 任务类型 0：主线；1：支线
	*****************************/
	void updatePlotUI(int type);

	/****************************
	执行任务
	@param 任务类型 0：主线；1：支线
	@param npcdata
	@return 是否有任务
	*****************************/
	bool doCheckPlotMisson(int type, NpcData npcdata);

	/****************************
	更新好友度
	*****************************/
	void reFreshFriendly();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemMaster(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onItemFriend(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onHostelAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRepair(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showTypeText(float dt);
	void removeNpcWord(float dt);
	void checkWordLblColor(std::string wordstr);
	void fastShowWord();
	void onTalkbg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void checkUpateNpc(float dt);
	void refreshNpcNode();
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	/****************************
	延迟新手引导
	*****************************/
	void delayShowNewerGuide(float dt);
	void getWinRes(int type);

	int checkFightCount(std::string npcid);

	std::string replaceSexWord(std::string dstr);

	void actionOver(Ref* pSender, Node* item, int actionindex);
private:
	Node* m_csbnode;
	std::string m_addrstr;
	UIScroll* m_talkScroll;
	Label* m_wordlbl;
	int m_wordcount;
	bool isShowWord;
	cocos2d::ui::ImageView* m_npctalkbg;
	cocos2d::ui::ScrollView* m_scrollview;
	std::vector<std::string> vec_wordstr;
	int m_wordindex;
	int m_plotindex;
	int m_lastWxpPos;
	int m_lastDgqbPos;
};
#endif

