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
private:
	TopBar* topBar;
	Node* homeLayer;
	
private:
	void loadSaveData();
	void saveAllData();
	void updata(float dt);
	void timerSaveResData(float dt);

};
extern Nature* g_nature;
extern Hero* g_hero;
extern GameScene* g_gameLayer;
extern UIScroll* g_uiScroll;
#endif // __GAME_SCENE_H__
