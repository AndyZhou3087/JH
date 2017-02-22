#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"

class Hero
{
public:
	Hero();
	~Hero();
	CC_SYNTHESIZE(int, m_pic, HeadImg);
	CC_SYNTHESIZE(int, m_innerinjury, InnerinjuryValue);
	CC_SYNTHESIZE(int, m_outinjury, OutinjuryValue);
	CC_SYNTHESIZE(int, m_hunger, HungerValue);
	CC_SYNTHESIZE(int, m_spirit, SpiritValue);
	CC_SYNTHESIZE(int, m_life, LifeValue);
	CC_SYNTHESIZE(int, m_maxlife, MaxLifeValue);
	CC_SYNTHESIZE(int, m_livedays, LiveDays);
public:
	static int MAXInnerinjuryValue;
	static int MAXOutinjuryValue;
	static int MAXHungerValue;
	static int MAXSpiritValue;
private:

};
#endif

