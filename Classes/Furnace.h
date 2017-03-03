
/********************************************************************
*	 暖炉
*********************************************************************/

#ifndef _FURNACE_H_
#define _FURNACE_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class Furnace :public Building
{
public:
	Furnace();
	~Furnace();

public:
	virtual void build();
	virtual void action(float acsec, int exminute);
	virtual bool init();
	void warmover(float dt);

	CREATE_FUNC(Furnace);
};
#endif

