#include "Hero.h"
#include "Const.h"
#include "Nature.h"
#include "GameScene.h"
#include "NewerGuideLayer.h"
float Hero::MAXInnerinjuryValue = 100.0f;
float Hero::MAXOutinjuryValue = 100.0f;
float Hero::MAXHungerValue = 100.0f;
float Hero::MAXSpiritValue = 100.0f;

#define HungerSpeed 2//饱食度下降速度
#define InnerinjurySpeed 1//内伤下降速度
#define OutinjurySpeed 1//外伤下降速度
#define SpiritSpeed 1.5f//精神下降速度
#define LifeLostSpeed 2//生命下降速度（按百分比）

#define SeriousInnerinjury 40.0f//严重内伤界限
#define SeriousOutinjury 40.0f//严重外伤界限
#define SeriousHunger 40.0f//严重饥饿界限

Hero::Hero()
{
	maxlifepercent = 1.0f;
	liferecoverpercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;
	m_maxHunger = MAXHungerValue;
	m_maxInnerinjury = MAXInnerinjuryValue;
	m_maxOutinjury = MAXOutinjuryValue;
	m_maxSpirit = MAXSpiritValue;

	injuryrecoverpercent = 1.0f;
	outjuryrecoverpercent = 1.0f;
	hungerrecoverpercent = 1.0f;
	sleepLostPercent = 1.0f;

	m_pastmin = 0;

	m_totalAtkBonusPercent = 0.0f;
	m_totalDfBonusPercent = 0.0f;
	m_isWDChallenge = false;
	m_isMoving = false;
}


Hero::~Hero()
{

}

bool Hero::init()
{
	this->schedule(schedule_selector(Hero::updateData), 1.0f/*60.0f / TIMESCALE*/);
	this->schedule(schedule_selector(Hero::checkMaxVaule), 1.0f);
	return true;
}

void Hero::updateData(float dt)
{
	if (GlobalData::g_gameStatus != GAMESTART)
		return;

	if (m_isWDChallenge)
		return;

	//12s，（游戏时间1小时更新一次）
	m_pastmin += g_nature->getTimeInterval();

	if (m_pastmin >= 60)
	{
		int hour = m_pastmin / 60;
		m_pastmin = m_pastmin % 60;

		//新手引导不扣属性消耗
		if (g_NewerGuideLayer != NULL)
			return;

		//闭关没有分身符不扣属性消耗， 有分身符要扣
		if (GlobalData::isExercising() && !GlobalData::isHasFSF())
			return;

		if (m_outinjury < SeriousOutinjury)//严重外伤，内伤3倍下降
		{
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_hunger < SeriousHunger)//过度饥饿 2倍外伤，2倍内伤下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 2.0f;
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_innerinjury < SeriousInnerinjury)//严重内伤，外伤2倍下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		//接上严重界限的消耗
		m_hunger -= hour * HungerSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < MAXInnerinjuryValue)
		{
			if (m_innerinjury > 90)
				m_innerinjury -= hour * InnerinjurySpeed * 0.3f;
			else if (m_innerinjury > 70)
				m_innerinjury -= hour * InnerinjurySpeed * 0.5f;
			else
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
		}
		if (m_outinjury < MAXOutinjuryValue)
		{
			if (m_outinjury > 90)
				m_outinjury -= hour * OutinjurySpeed * 0.3f;
			else if (m_outinjury > 70)
				m_outinjury -= hour * OutinjurySpeed * 0.5f;
			else
				m_outinjury -= hour * OutinjurySpeed * 2.0f;

			if (g_nature->getTemperature() > 20)
			{
				m_outinjury -= hour * OutinjurySpeed * 1.0f;
			}
		}

		else if (g_nature->getTemperature() < -5)
		{
			m_life -= hour * LifeLostSpeed * 1.0f * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		m_spirit -= hour * SpiritSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < 0.0f)
			m_innerinjury = 0.0f;
		if (m_outinjury < 0.0f)
			m_outinjury = 0.0f;
		if (m_hunger < 0.0f)
			m_hunger = 0.0f;
		if (m_life < 0.0f)
			m_life = 0.0f;
		if (m_spirit < 0.0f)
			m_spirit = 0.0f;
	}

}

void Hero::sleep(int losttime, int hour)
{
	//按次恢复生命
	float delay = 0.0f;
	if (losttime > TIMESCALE * ACTION_BAR_TIME)
		delay = losttime / TIMESCALE;
	else
		delay = ACTION_BAR_TIME;

	float pertime = delay / (TIMESCALE* ACTION_BAR_TIME);
	sleephour = hour;
	this->schedule(schedule_selector(Hero::sleepbystep), pertime, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
	sleepLostPercent = 0.5f;


	this->scheduleOnce(schedule_selector(Hero::sleepDone), delay);
}

void Hero::sleepbystep(float dt)
{
	//每次恢复的生命值
	m_life += GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] * sleephour * 0.1f * liferecoverpercent / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > getMaxLifeValue())
	{
		m_life = getMaxLifeValue();
		this->unschedule(schedule_selector(Hero::sleepbystep));
	}
}

void Hero::sleepDone(float dt)
{
	sleepLostPercent = 1.0f;
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

float Hero::getMaxLifeValue()
{
	float flife = maxlifepercent * GlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()];
	return flife;
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
	liferecoverpercent = 1.0f;
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
bool Hero::checkifHasGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	//装备栏是否有
	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return true;
	}

	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare(gfeid) == 0)
			return true;
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU };

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < StorageRoom::map_storageData[stype].size(); i++)
		{
			if (StorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return true;
		}
	}
	return false;
}

PackageData* Hero::getGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return getAtrByType(heroproptype[i]);
	}

	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare(gfeid) == 0)
			return &MyPackage::vec_packages[i];
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU};

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < StorageRoom::map_storageData[stype].size(); i++)
		{
			if (StorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return &StorageRoom::map_storageData[stype][i];
		}
	}

	return NULL;
}

PackageData* Hero::getMeHas(std::string strid)
{
	//装备栏是否有
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		if (getAtrByType((HeroAtrType)i)->count > 0 && getAtrByType((HeroAtrType)i)->strid.compare(strid) == 0)
		{
			return getAtrByType((HeroAtrType)i);
		}
	}
	//背包中是否有
	for (int i = 0; i < MyPackage::getSize(); i++)
	{
		if (MyPackage::vec_packages[i].strid.compare(strid) == 0)
			return &MyPackage::vec_packages[i];
	}
	//仓库中是否有

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = StorageRoom::map_storageData.begin(); it != StorageRoom::map_storageData.end(); ++it)
	{
		int size = StorageRoom::map_storageData[it->first].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = StorageRoom::map_storageData[it->first][j];
			if (sdata.strid.compare(strid) == 0)
				return &StorageRoom::map_storageData[it->first][j];
		}
	}
	return NULL;
}

void Hero::checkMaxVaule(float dt)
{
	float mlife = 1.0f;

	float matk = 1.0f;
	float mdf = 1.0f;
	if (m_spirit <= 10.0f)
	{
		liferecoverpercent = 0.3f;
		injuryrecoverpercent = 0.3f;
		outjuryrecoverpercent = 0.3f;
		hungerrecoverpercent = 0.3f;
		matk = 0.7f;
		mdf = 0.7f;
	}
	else if (m_spirit <= 20.0f)
	{
		liferecoverpercent = 0.4f;
		injuryrecoverpercent = 0.4f;
		outjuryrecoverpercent = 0.4f;
		hungerrecoverpercent = 0.4f;
		matk = 0.9f;
		mdf = 0.9f;
	}

	else if (m_spirit <= 30.0f)
	{
		liferecoverpercent = 0.7f;
		injuryrecoverpercent = 0.7f;
		outjuryrecoverpercent = 0.7f;
		hungerrecoverpercent = 0.7f;
		matk =  1.0f;
		mdf = 1.0f;
	}

	else if (m_spirit <= 40.0f)
	{
		liferecoverpercent = 0.9f;
		injuryrecoverpercent = 0.9f;
		outjuryrecoverpercent = 0.9f;
		hungerrecoverpercent = 0.9f;
		matk = 1.0f;
		mdf = 1.0f;
	}
	else
	{
		liferecoverpercent = 1.0f;
		injuryrecoverpercent = 1.0f;
		outjuryrecoverpercent = 1.0f;
		hungerrecoverpercent = 1.0f;
		matk = 1.0f;
		mdf = 1.0f;
	}

	if (m_hunger <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
		matk = MIN(matk, 0.3f);
		mdf = MIN(mdf, 0.3);
	}
	else if (m_hunger <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
		matk = MIN(matk, 0.4f);
		mdf = MIN(mdf, 0.4);
	}
	else if (m_hunger <= 30.0f)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.7f);
		mdf = MIN(mdf, 0.7);
	}
	else if (m_hunger <= 40.0f)
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

	if (m_innerinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_innerinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_innerinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_innerinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	if (m_outinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_outinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_outinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_outinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	maxlifepercent = mlife;

	if (m_life > getMaxLifeValue())
		setLifeValue(getMaxLifeValue());

	setAtkPercent(matk);
	setDfPercent(mdf);
}

void Hero::recoverOutjury(int val)
{
	float fval = val * outjuryrecoverpercent;
	m_outinjury += fval;
}

void Hero::recoverInjury(int val)
{
	float fval = val * injuryrecoverpercent;
	m_innerinjury += fval;
}

void Hero::recoverHunger(int val)
{
	float fval = val* hungerrecoverpercent;
	m_hunger += fval;
}

int Hero::getTotalDf()
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防
	if (g_hero->getAtrByType(H_NG)->count > 0)
	{
		std::string gfname = g_hero->getAtrByType(H_NG)->strid;
		ngdf = GlobalData::map_wgngs[gfname].vec_bns[g_hero->getAtrByType(H_NG)->lv];
	}

	if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->goodvalue > 0)
	{
		std::string aname = g_hero->getAtrByType(H_ARMOR)->strid;
		adf = GlobalData::map_equips[aname].df;
		int slv = g_hero->getAtrByType(H_ARMOR)->slv;
		slvdf = slv * (slv + adf / 10);
	}
	//防御属性
	float fdf = g_hero->getDfPercent() *(g_hero->getDfValue() + ngdf + adf);
	fdf += slvdf;
	adf = int(fdf*(1 + m_totalDfBonusPercent) + 0.5f);
	return adf;
}

int Hero::getTotalAtck()
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻
	if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WEAPON)->strid;
		weaponAtk = GlobalData::map_equips[strid].atk;
		int slv = g_hero->getAtrByType(H_WEAPON)->slv;
		slvAtk = slv * (slv + weaponAtk / 10);
	}
	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WG)->strid;
		wgAtk = GlobalData::map_wgngs[strid].vec_bns[g_hero->getAtrByType(H_WG)->lv];
	}
	//攻击属性
	float fack = g_hero->getAtkPercent() * (g_hero->getAtkValue() + weaponAtk + wgAtk);

	if (g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		if (GlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].extype == GlobalData::map_equips[g_hero->getAtrByType(H_WEAPON)->strid].extype)
		{
			float back = fack * 0.1f;
			fack += back;
		}
	}
	fack += slvAtk;
	int tatk = int(fack*(1 + m_totalAtkBonusPercent) + 0.5f);
	return tatk;
}