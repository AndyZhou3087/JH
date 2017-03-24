
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
	virtual void action(int minute, int exminute);
	virtual bool init();



	CREATE_FUNC(Furnace);

private:
	int extime;
	void warm(float dt);
	void warmover(float dt);
};
#endif

