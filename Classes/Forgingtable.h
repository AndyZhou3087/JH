
/********************************************************************
*	 ∂Õ‘ÏÃ®
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
	virtual void build();
	virtual void repair();

	virtual bool init();

	CREATE_FUNC(Forgingtable);
};
#endif

