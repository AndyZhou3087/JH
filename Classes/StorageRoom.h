/********************************************************************
* 仓库
*********************************************************************/

#ifndef _STORAGE_H_
#define _STORAGE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class StorageRoom :public Building
{
public:
	StorageRoom();
	~StorageRoom();

public:

	virtual bool init();

	CREATE_FUNC(StorageRoom);
};
#endif

