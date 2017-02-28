#ifndef _HOME_LAYER_H_
#define _HOME_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
class HomeLayer:public Layer
{
public:
	HomeLayer();
	~HomeLayer();

	virtual bool init();

	CREATE_FUNC(HomeLayer);
private:
	void onclick(Ref* pSender);
};
#endif

