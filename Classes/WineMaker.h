
/********************************************************************
*	 �����
*********************************************************************/

#ifndef _WineMaker_H_
#define _WineMaker_H_
#include "Building.h"
#include "cocos2d.h"
USING_NS_CC;

class WineMaker :public Building
{
public:
	WineMaker();
	~WineMaker();

public:
	virtual void build();
	virtual void repair();

	virtual bool init();

	CREATE_FUNC(WineMaker);
};
#endif
