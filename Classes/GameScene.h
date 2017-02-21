#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	CREATE_FUNC(GameScene);

private:
	Node* topBar;
	Node* homeLayer;
};

#endif // __GAME_SCENE_H__
