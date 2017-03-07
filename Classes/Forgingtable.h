
/********************************************************************
*	 锻造台
*********************************************************************/

#ifndef _FORGINGTABLE_H_
#define _FORGINGTABLE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class Forgingtable :public Building
{
public:
	Forgingtable();
	~Forgingtable();

public:
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(Forgingtable);
};
#endif

