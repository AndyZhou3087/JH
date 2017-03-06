#ifndef _STORAGEUI_LAYER_H_
#define _STORAGEUI_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Building.h"
USING_NS_CC;
typedef struct
{
	int id;
	int type;
	int count;

}StorageData;

typedef enum
{
	FOOD = 0,
	MEDICINAL,
	WEAPON,
	PROTECT_EQU,
	N_GONG,//内功
	W_GONG,//外功
	RES_1,
	RES_2

}StorageType;

class StorageUILayer :public Layer
{
public:
	StorageUILayer();
	~StorageUILayer();

	virtual bool init();
	CREATE_FUNC(StorageUILayer);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	Node* m_csbnode;

	cocos2d::ui::ScrollView* scrollview;
	std::map<int,std::vector<StorageData>> map_storageData;
private:
	int typecount[RES_2 + 1];
	int typerow[RES_2 + 1];
	int getCountByType(int type);
};
#endif

