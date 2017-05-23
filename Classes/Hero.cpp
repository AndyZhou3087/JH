#include "Hero.h"
#include "Const.h"
#include "Nature.h"
int Hero::MAXInnerinjuryValue = 100;
int Hero::MAXOutinjuryValue = 100;
int Hero::MAXHungerValue = 100;
float Hero::MAXSpiritValue = 100.0f;

#define HungerSpeed 2//饱食度下降速度
#define InnerinjurySpeed 1//内伤下降速度
#define OutinjurySpeed 1//外伤下降速度
#define SpiritSpeed 2//精神下降速度
#define LifeLostSpeed 2//生命下降速度（按百分比）

#define SeriousInnerinjury 40//严重内伤界限
#define SeriousOutinjury 40//严重外伤界限
#define SeriousHunger 40//严重饥饿界限

Hero::Hero()
{
	maxlifepercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;
	m_maxHunger = MAXHungerValue;
	m_maxInnerinjury = MAXInnerinjuryValue;
	m_outinjury = MAXOutinjuryValue;
	m_maxSpirit = MAXSpiritValue;
}


Hero::~Hero()
{

}

bool Hero::init()
{
	//12s，（游戏时间1小时更新一次）
	this->schedule(schedule_selector(Hero::updateData), 60.0f / TIMESCALE);
	this->schedule(schedule_selector(Hero::checkMaxVaule), 1.0f);
	return true;
}

void Hero::updateData(float dt)
{
	if (m_outinjury < SeriousOutinjury)//严重外伤，内伤3倍下降
	{
		m_innerinjury -= InnerinjurySpeed * 2;
		m_life -= LifeLostSpeed * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100;
	}
	if (m_hunger < SeriousHunger)//过度饥饿 2倍外伤，2倍内伤下降
	{
		m_outinjury -= OutinjurySpeed * 1;
		m_innerinjury -= InnerinjurySpeed * 1;
		m_life -= LifeLostSpeed * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100;
	}
	if (m_innerinjury < SeriousInnerinjury)//严重内伤，外伤3倍下降
	{
		m_outinjury -= OutinjurySpeed * 2;
		m_life -= LifeLostSpeed * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100;
	}

	//接上严重界限的消耗
	m_hunger -= HungerSpeed;
	
	if (m_innerinjury < MAXInnerinjuryValue)
		m_innerinjury -= InnerinjurySpeed;
	if (m_outinjury < MAXOutinjuryValue)
		m_outinjury -= OutinjurySpeed;
	m_spirit -= SpiritSpeed;

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
	//按次恢复生命
	sleephour = hour;
	this->schedule(schedule_selector(Hero::sleepbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}

void Hero::sleepbystep(float dt)
{
	//每次恢复的生命值
	m_life += GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] * 10 * sleephour / 100 / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > getMaxLifeValue())
	{
		m_life = getMaxLifeValue();
		this->unschedule(schedule_selector(Hero::sleepbystep));
	}
}

void Hero::drinking()
{
	//按次恢复精神值
	this->schedule(schedule_selector(Hero::drinkbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}

void Hero::drinkbystep(float dt)
{
	//每次恢复精神值
	m_spirit += 2.0f;
	if (m_spirit > getMaxSpiritValue())
	{
		m_spirit = getMaxSpiritValue();
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
	float flife = maxlifepercent * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()];
	return (int)flife;
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
	//复活-满状态复活

	maxlifepercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;

	setOutinjuryValue(MAXOutinjuryValue);
	setMaxOutinjuryValue(MAXOutinjuryValue);

	setInnerinjuryValue(MAXInnerinjuryValue);
	setMaxInnerinjuryValue(MAXInnerinjuryValue);

	setLifeValue(getMaxLifeValue());
	
	setHungerValue(MAXHungerValue);
	setMaxHungerValue(MAXHungerValue);

	setSpiritValue(MAXSpiritValue);
	setMaxSpiritValue(MAXSpiritValue);
}

//功法每种只能有一本，不能重复
bool Hero::checkifHasGF(std::string gfid)
{
	//装备栏是否有
	if ((getAtrByType(H_WG)->count > 0 && getAtrByType(H_WG)->strid.compare(gfid) == 0) || (getAtrByType(H_NG)->count > 0 && getAtrByType(H_NG)->strid.compare(gfid) == 0))
		return true;
	else
	{
		//背包中是否有
		for (int i = 0; i < MyPackage::getSize(); i++)
		{
			if (MyPackage::vec_packages[i].strid.compare(gfid) == 0)
				return true;
		}
		//仓库中是否有
		for (unsigned i = 0; i < StorageRoom::map_storageData[N_GONG].size(); i++)
		{
			if (StorageRoom::map_storageData[N_GONG][i].strid.compare(gfid) == 0)
				return true;
		}

		for (unsigned i = 0; i < StorageRoom::map_storageData[W_GONG].size(); i++)
		{
			if (StorageRoom::map_storageData[W_GONG][i].strid.compare(gfid) == 0)
				return true;
		}
	}
	return false;
}

void Hero::checkMaxVaule(float dt)
{
	float mlife = 1.0f;
	int mhvalue = MAXHungerValue;
	int mivalue = MAXInnerinjuryValue;
	int movalue = MAXOutinjuryValue;
	float matk = 1.0f;
	float mdf = 1.0f;
	if (m_spirit <= 10)
	{
		mlife = 0.3f;
		mhvalue = 30;
		mivalue = 30;
		movalue = 30;
		matk = 0.7f;
		mdf = 0.7f;
	}
	else if (m_spirit <= 20)
	{
		mlife = 0.4f;
		mhvalue = 40;
		mivalue = 40;
		movalue = 40;
		matk = 0.9f;
		mdf = 0.9f;
	}

	else if (m_spirit <= 30)
	{
		mlife = 0.7f;
		mhvalue = 70;
		mivalue = 70;
		movalue = 70;
		matk =  1.0f;
		mdf = 1.0f;
	}

	else if (m_spirit <= 40)
	{
		mlife = 0.9f;
		mhvalue = 90;
		mivalue = 90;
		movalue = 90;
		matk = 1.0f;
		mdf = 1.0f;
	}
	else
	{
		mlife = 1.0f;
		mhvalue = MAXHungerValue;
		mivalue = MAXInnerinjuryValue;
		movalue = MAXOutinjuryValue;
		matk = 1.0f;
		mdf = 1.0f;
	}

	if (m_hunger <= 10)
	{
		mlife = MIN(mlife, 0.3f);
		matk = MIN(matk, 0.3f);
		mdf = MIN(mdf, 0.3);
	}
	else if (m_hunger <= 20)
	{
		mlife = MIN(mlife, 0.4f);
		matk = MIN(matk, 0.4f);
		mdf = MIN(mdf, 0.4);
	}
	else if (m_hunger <= 30)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.7f);
		mdf = MIN(mdf, 0.7);
	}
	else if (m_hunger <= 40)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.9f);
		mdf = MIN(mdf, 0.9);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 1.0f);
		mdf = MIN(mdf, 1.0);
	}

	if (m_innerinjury <= 10)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_innerinjury <= 20)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_innerinjury <= 30)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_innerinjury <= 40)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	if (m_outinjury <= 10)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_outinjury <= 20)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_outinjury <= 30)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_outinjury <= 40)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	maxlifepercent = mlife;

	setMaxHungerValue(mhvalue);
	if (m_life > getMaxLifeValue())
		setLifeValue(getMaxLifeValue());

	setMaxInnerinjuryValue(mivalue);
	if (m_innerinjury > getMaxInnerinjuryValue())
		m_innerinjury = getMaxInnerinjuryValue();

	setMaxOutinjuryValue(movalue);

	if (m_outinjury > getMaxOutinjuryValue())
		setMaxOutinjuryValue(getMaxOutinjuryValue());

	setAtkPercent(matk);
	setDfPercent(mdf);
}