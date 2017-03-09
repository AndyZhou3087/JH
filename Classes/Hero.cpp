#include "Hero.h"
#include "Const.h"
int Hero::MAXInnerinjuryValue = 100;
int Hero::MAXOutinjuryValue = 100;
int Hero::MAXHungerValue = 100;
int Hero::MAXSpiritValue = 100.0f;

#define HungerSpeed 5
#define InnerinjurySpeed 1
#define OutinjurySpeed 1
#define SpiritSpeed 2
#define LifeLostSpeed 2//百分比

#define SeriousInnerinjury 40
#define SeriousOutinjury 40
#define SeriousHunger 40

Hero::Hero()
{
	m_pic = 0;
	m_innerinjury = MAXInnerinjuryValue;
	m_outinjury = MAXOutinjuryValue;
	m_hunger = MAXHungerValue;
	m_spirit = MAXSpiritValue;
}


Hero::~Hero()
{

}

bool Hero::init()
{
	this->schedule(schedule_selector(Hero::updateData), 60.0f / TIMESCALE);
	return true;
}

void Hero::updateData(float dt)
{
	if (m_outinjury < SeriousOutinjury)
	{
		m_life -= LifeLostSpeed * m_maxlife / 100;
		m_innerinjury -= InnerinjurySpeed * 3;
	}
	if (m_hunger < SeriousHunger)
	{
		m_life -= LifeLostSpeed * m_maxlife / 100;
		m_outinjury -= OutinjurySpeed * 2;
		m_innerinjury -= InnerinjurySpeed * 2;
		m_spirit -= SpiritSpeed;
	}
	if (m_innerinjury < SeriousInnerinjury)
	{
		m_life -= LifeLostSpeed * m_maxlife / 100;
		m_outinjury -= OutinjurySpeed * 3;
	}

	m_hunger -= HungerSpeed;

	if (m_innerinjury < MAXInnerinjuryValue)
		m_innerinjury -= InnerinjurySpeed;
	if (m_outinjury < MAXOutinjuryValue)
		m_outinjury -= OutinjurySpeed;

	if (m_innerinjury < 0)
		m_innerinjury = 0;
	if (m_outinjury < 0)
		m_outinjury = 0;
	if (m_hunger < 0)
		m_hunger = 0;
	if (m_life < 0)
		m_hunger = 0;
	if (m_spirit < 0)
		m_spirit = 0;

}

void Hero::sleep(int hour)
{
	sleephour = hour;
	this->schedule(schedule_selector(Hero::sleepbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}

void Hero::sleepbystep(float dt)
{
	m_life += m_maxlife * 10 * sleephour / 100 / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > m_maxlife)
	{
		m_life = m_maxlife;
		this->unschedule(schedule_selector(Hero::sleepbystep));
	}
}

void Hero::drinking()
{
	this->schedule(schedule_selector(Hero::drinkbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}

void Hero::drinkbystep(float dt)
{
	m_spirit += 0.5f;
	if (m_spirit > MAXSpiritValue)
	{
		m_spirit = MAXSpiritValue;
		this->unschedule(schedule_selector(Hero::drinkbystep));
	}
}
