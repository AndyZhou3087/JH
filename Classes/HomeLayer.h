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

	CREATE_FUNC(HomeLayer);
	void updateBuilding();
private:
	void onclick(Ref* pSender);
	void onStorageRoom(Ref* pSender);
	void onFence(Ref* pSender);
	
	void loadJsonData();

private:
	Building* m_storageroom;
	std::vector<Building*> Vec_Buildings;
};
#endif

