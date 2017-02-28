
/********************************************************************
*	 ����
*********************************************************************/

#ifndef _WINETABLE_H_
#define _WINETABLE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class WineTable :public Building
{
public:
	WineTable();
	~WineTable();

public:
	virtual void build();
	virtual void repair();

	virtual bool init();

	CREATE_FUNC(WineTable);
};
#endif

