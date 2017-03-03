
/********************************************************************
*	 灶台
*********************************************************************/

#ifndef _COOKTABLE_H_
#define _COOKTABLE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class CookTable :public Building
{
public:
	CookTable();
	~CookTable();

public:
	virtual void build();
	virtual void action(float acsec, int exminute);

	virtual bool init();

	CREATE_FUNC(CookTable);
};
#endif

