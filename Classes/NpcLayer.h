#ifndef _NPC_LAYER_H_
#define _NPC_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UIScroll.h"
USING_NS_CC;

class NpcLayer :public Layer
{
public:
	NpcLayer();
	~NpcLayer();

	bool init(std::string addrid);
	virtual void onEnterTransitionDidFinish();
	static NpcLayer* create(std::string addrid);
	void updatePlotUI();
public:
	bool isNewPlot;
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showTypeText(float dt);
	void removeNpcWord(float dt);
	void checkWordLblColor(std::string wordstr);
	void fastShowWord();
	void onTalkbg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	新手引导
	*****************************/
	void showNewerGuide(int step);

	/****************************
	延迟新手引导
	*****************************/
	void delayShowNewerGuide(float dt);
	void getWinRes();
private:
	Node* m_csbnode;
	std::string m_addrstr;
	UIScroll* m_talkScroll;
	Label* m_wordlbl;
	int m_wordcount;
	bool isShowWord;
	cocos2d::ui::ImageView* m_npctalkbg;
	std::vector<std::string> vec_wordstr;
	int m_wordindex;
	int m_plotindex;

};
#endif

