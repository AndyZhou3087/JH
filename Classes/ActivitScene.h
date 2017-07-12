/********************************************************************
* 过场场景
*********************************************************************/
#ifndef _ACTIVITY_SCENE_H_
#define _ACTIVITY_SCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ActivitScene :public Layer
{
public:
	ActivitScene();
	~ActivitScene();

	bool init(std::string imagepath, std::string content);
	static cocos2d::Scene* createScene(std::string imagepath, std::string content);
private:
	static ActivitScene* create(std::string imagepath, std::string content);
	void popself(float dt);
	void getRndRes(float dt);
	int getResType(std::string strid);

private:
};
#endif

