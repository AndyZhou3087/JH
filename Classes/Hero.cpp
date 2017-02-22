#include "Hero.h"

int Hero::MAXInnerinjuryValue = 100;
int Hero::MAXOutinjuryValue = 100;
int Hero::MAXHungerValue = 100;
int Hero::MAXSpiritValue = 100;

Hero::Hero()
{
	m_pic = 0;
	m_innerinjury = MAXInnerinjuryValue;
	m_outinjury = MAXOutinjuryValue;
	m_hunger = MAXHungerValue;
	m_spirit = MAXSpiritValue;
	m_livedays = 0;
}


Hero::~Hero()
{
}
