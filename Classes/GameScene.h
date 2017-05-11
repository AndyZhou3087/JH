
/********************************************************************
*	 游戏主场景
*********************************************************************/
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "UIScroll.h"
#include "TopBar.h"
#include "Nature.h"
#include "Hero.h"
class GameScene : public cocos2d::Layer
{
public:
	GameScene();
	~GameScene();
    static cocos2d::Scene* createScene();

	virtual void onExit();
    virtual bool init();
	CREATE_FUNC(GameScene);
	/****************************
	角色复活更新UI
	*****************************/
	void heroRevive();
private:
	TopBar* topBar;//角色属性和天气NODE
	Node* homeLayer;//角色家
	
private:
	/****************************
	读取保存到本地的数据
	*****************************/
	void loadSaveData();

	/****************************
	读取保存到本地的角色属性数据
	*****************************/
	void loadSavedHeroPropData();

	/****************************
	保存数据到本地文件
	*****************************/
	void saveAllData();

	/****************************
	定时更新资源列表数据
	*****************************/
	void updata(float dt);

	/****************************
	定时保存后山资源列表的数据
	*****************************/
	void timerSaveResData(float dt);

	/****************************
	定时查看角色是否死亡，生命为0死掉
	*****************************/
	void checkiflive(float dt);


};
extern Nature* g_nature;//天气数据
extern Hero* g_hero;//角色数据
extern GameScene* g_gameLayer;//游戏层
extern UIScroll* g_uiScroll;//topbar文件事件滚动控件
#endif // __GAME_SCENE_H__
