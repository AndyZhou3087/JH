#ifndef _BEDUI_LAYER_H_
#define _BEDUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Building.h"
USING_NS_CC;
class BedUILayer :public Layer
{
public:
	BedUILayer();
	~BedUILayer();

	virtual bool init();

	CREATE_FUNC(BedUILayer);
private:
};
#endif

