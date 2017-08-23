#ifndef _RANK_LAYER_H_
#define _RANK_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ServerDataSwap.h"
#include "GlobalData.h"
USING_NS_CC;
class RankLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	RankLayer();
	~RankLayer();

	bool init();
	virtual void onEnterTransitionDidFinish();
	static RankLayer* create();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);
	void getRankData(int type);
private:
	cocos2d::ui::ScrollView* srollView;
	cocos2d::ui::Button *tag0btn;
	cocos2d::ui::Button *tag1btn;
	cocos2d::ui::Text* text6;
	int selectrank;
	int myfightingpower;
};

class RankItem :public Sprite
{
public:
	RankItem();
	~RankItem();

	bool init(RankData *data);
	static RankItem* create(RankData *data);

private:
};
#endif