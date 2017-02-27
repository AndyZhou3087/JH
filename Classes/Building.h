#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "cocos2d.h"
#include "Goods.h"
USING_NS_CC;

class Building :public Goods
{
public:
	Building();
	~Building();

public:

protected:
	int goodvaule;
	virtual void build();
	virtual void repair();

};
#endif

