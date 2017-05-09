#include "Hero.h"
#include "Const.h"
int Hero::MAXInnerinjuryValue = 100;
int Hero::MAXOutinjuryValue = 100;
int Hero::MAXHungerValue = 100;
int Hero::MAXSpiritValue = 100.0f;

#define HungerSpeed 2
#define InnerinjurySpeed 1
#define OutinjurySpeed 1
#define SpiritSpeed 2
#define LifeLostSpeed 2//百分比

#define SeriousInnerinjury 40
#define SeriousOutinjury 40
#define SeriousHunger 40

Hero::Hero()
{

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
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
		m_innerinjury -= InnerinjurySpeed * 3;
	}
	if (m_hunger < SeriousHunger)
	{
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
		m_outinjury -= OutinjurySpeed * 2;
		m_innerinjury -= InnerinjurySpeed * 2;
		m_spirit -= SpiritSpeed;
	}
	if (m_innerinjury < SeriousInnerinjury)
	{
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
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
	m_life += getMaxLifeValue() * 10 * sleephour / 100 / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > getMaxLifeValue())
	{
		m_life = getMaxLifeValue();
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

int Hero::getAtkValue()
{
	return GlobalData::map_heroAtr[getHeadID()].vec_atk[getLVValue()];
}

int Hero::getDfValue()
{
	return GlobalData::map_heroAtr[getHeadID()].vec_df[getLVValue()];
}

int Hero::getMaxLifeValue()
{
	return GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()];
}

void Hero::setAtrByType(HeroAtrType type, PackageData pData)
{
	map_heroAtr[type] = pData;
}

PackageData* Hero::getAtrByType(HeroAtrType type)
{
	return &map_heroAtr[type];
}
void Hero::revive()
{
	setOutinjuryValue(MAXOutinjuryValue);
	setInnerinjuryValue(MAXInnerinjuryValue);
	setLifeValue(getMaxLifeValue());
	setHungerValue(MAXHungerValue);
	setSpiritValue(MAXSpiritValue);
}

bool Hero::checkifHasGF(std::string gfid)
{
	if ((getAtrByType(H_WG)->count > 0 && getAtrByType(H_WG)->strid.compare(gfid) == 0) || (getAtrByType(H_NG)->count > 0 && getAtrByType(H_NG)->strid.compare(gfid) == 0))
		return true;
	else
	{
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			if (MyPackage::vec_packages[i].strid.compare(gfid) == 0)
				return true;
		}
		for (unsigned i = 0; i < StorageRoom::map_storageData[N_GONG].size(); i++)
		{
			if (StorageRoom::map_storageData[N_GONG][i].strid.compare(gfid) == 0)
				return true;
			else if (StorageRoom::map_storageData[W_GONG][i].strid.compare(gfid) == 0)
				return true;
		}
	}
	return false;
}