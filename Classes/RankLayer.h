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

	void delayShowData(float dt);
	void onSuccess();
	void onErr(int errcode);
private:
	cocos2d::ui::ScrollView* srollView;
	Label * m_loadlbl;
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