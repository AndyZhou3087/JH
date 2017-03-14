/********************************************************************
* 仓库
*********************************************************************/

#ifndef _STORAGE_H_
#define _STORAGE_H_
#include "Building.h"
#include "cocos2d.h"
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
	RES_MAX

}StorageType;

class StorageRoom :public Building
{
public:
	StorageRoom();
	~StorageRoom();

public:

	virtual bool init();

	CREATE_FUNC(StorageRoom);

	void loadStorageData();
	static void save();
	static std::map<int, std::vector<StorageData>> map_storageData;
	static void add(StorageData data);
	static void use(int id);
	static int getCountByTypeId(int id);
};
#endif
