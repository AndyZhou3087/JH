#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"
USING_NS_CC;

class Hero:public Node
{
public:
	Hero();
	~Hero();
	virtual bool init();

	CREATE_FUNC(Hero);

	CC_SYNTHESIZE(int, m_pic, HeadImg);
	CC_SYNTHESIZE(int, m_innerinjury, InnerinjuryValue);
	CC_SYNTHESIZE(int, m_outinjury, OutinjuryValue);
	CC_SYNTHESIZE(int, m_hunger, HungerValue);
	CC_SYNTHESIZE(int, m_spirit, SpiritValue);
	CC_SYNTHESIZE(int, m_life, LifeValue);
	CC_SYNTHESIZE(int, m_maxlife, MaxLifeValue);
	void sleep(int hour);
	
public:
	static int MAXInnerinjuryValue;
	static int MAXOutinjuryValue;
	static int MAXHungerValue;
	static int MAXSpiritValue;
private:
	void updateData(float dt);
	void sleepbystep(float dt);
private:
	int sleephour;
};

#endif

