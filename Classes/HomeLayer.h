#ifndef _HOME_LAYER_H_
#define _HOME_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

#include "Building.h"
USING_NS_CC;
class HomeLayer:public Layer
{
public:
	HomeLayer();
	~HomeLayer();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	CREATE_FUNC(HomeLayer);
	void updateBuilding();

	void showNewerGuide(int step);
private:
	void onclick(Ref* pSender);
	void onStorageRoom(Ref* pSender);
	void onFence(Ref* pSender);
	
	void loadJsonData();

	/****************************
	延迟显示新手引导
	*****************************/
	void delayShowNewerGuide(float dt);
private:
	Building* m_storageroom;
	Building* m_fence;
	std::vector<Building*> Vec_Buildings;
};
#endif

