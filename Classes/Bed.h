/********************************************************************
* 床
*********************************************************************/

#ifndef _BED_H_
#define _BED_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class Bed :public Building
{
public:
	Bed();
	~Bed();

public:
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(Bed);
};
#endif

