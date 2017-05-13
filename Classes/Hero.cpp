#include "Hero.h"
#include "Const.h"
int Hero::MAXInnerinjuryValue = 100;
int Hero::MAXOutinjuryValue = 100;
int Hero::MAXHungerValue = 100;
int Hero::MAXSpiritValue = 100.0f;

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

}


Hero::~Hero()
{

}

bool Hero::init()
{
	//12s，（游戏时间1小时更新一次）
	this->schedule(schedule_selector(Hero::updateData), 60.0f / TIMESCALE);
	return true;
}

void Hero::updateData(float dt)
{
	if (m_outinjury < SeriousOutinjury)//严重外伤，内伤3倍下降
	{
		m_innerinjury -= InnerinjurySpeed * 2;
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
	}
	if (m_hunger < SeriousHunger)//过度饥饿 2倍外伤，2倍内伤下降
	{
		m_outinjury -= OutinjurySpeed * 1;
		m_innerinjury -= InnerinjurySpeed * 1;
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
	}
	if (m_innerinjury < SeriousInnerinjury)//严重内伤，外伤3倍下降
	{
		m_outinjury -= OutinjurySpeed * 2;
		m_life -= LifeLostSpeed * getMaxLifeValue() / 100;
	}

	//接上严重界限的消耗
	m_hunger -= HungerSpeed;
	m_innerinjury -= InnerinjurySpeed;
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
	m_life += getMaxLifeValue() * 10 * sleephour / 100 / (TIMESCALE* ACTION_BAR_TIME);
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
	//复活-满状态复活
	setOutinjuryValue(MAXOutinjuryValue);
	setInnerinjuryValue(MAXInnerinjuryValue);
	setLifeValue(getMaxLifeValue());
	setHungerValue(MAXHungerValue);
	setSpiritValue(MAXSpiritValue);
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
			else if (StorageRoom::map_storageData[W_GONG][i].strid.compare(gfid) == 0)
				return true;
		}
	}
	return false;
}