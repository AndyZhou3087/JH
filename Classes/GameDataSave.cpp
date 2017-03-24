#include "GameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "Hero.h"
GameDataSave* GameDataSave::_Context = NULL;
GameDataSave* GameDataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new GameDataSave;
	}
	return _Context;
}

void GameDataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int GameDataSave::getLiveDays()
{
	return loadIntDataByKey("livedays");
}

void GameDataSave::setLiveDays(int val)
{
	saveIntDataByKey("livedays", val);
}

int GameDataSave::getHeroOutinjury()
{
	return loadIntDataByKey("outinjury", Hero::MAXOutinjuryValue);
}
void GameDataSave::setHeroOutinjury(int val)
{
	saveIntDataByKey("outinjury", val);
}

int GameDataSave::getHeroInnerinjury()
{
	return loadIntDataByKey("innerinjury", Hero::MAXInnerinjuryValue);
}
void GameDataSave::setHeroInnerinjury(int val)
{
	saveIntDataByKey("innerinjury", val);
}

int GameDataSave::getHeroHunger()
{
	return loadIntDataByKey("hunger", Hero::MAXHungerValue);
}
void GameDataSave::setHeroHunger(int val)
{
	saveIntDataByKey("hunger", val);
}

int GameDataSave::getHeroSpirit()
{
	return loadIntDataByKey("spirit", Hero::MAXSpiritValue);
}
void GameDataSave::setHeroSpirit(int val)
{
	saveIntDataByKey("spirit", val);
}

int GameDataSave::getHeroLife()
{
	return loadIntDataByKey("life", 100);
}
void GameDataSave::setHeroLife(int val)
{
	saveIntDataByKey("life", val);
}

int GameDataSave::getHeroMaxLife()
{
	return loadIntDataByKey("maxlife", 100);
}
void GameDataSave::setHeroMaxLife(int val)
{
	saveIntDataByKey("maxlife", val);
}


int GameDataSave::getNatureTime()
{
	return loadIntDataByKey("ndaytime");
}
void GameDataSave::setNatureTime(int val)
{
	saveIntDataByKey("ndaytime", val);
}

int GameDataSave::getNatureWeather()
{
	return loadIntDataByKey("nwhather");
}
void GameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey("nwhather", val);
}
int GameDataSave::getNatureReason()
{
	return loadIntDataByKey("nreason");
}
void GameDataSave::setNatureReason(int val)
{
	saveIntDataByKey("nreason", val);
}

int GameDataSave::getNatureTemperature()
{
	return loadIntDataByKey("ntempe");
}
void GameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey("ntempe", val);
}

std::string GameDataSave::getStorageData()
{
	return loadStringDataByKey("storage");
}
void GameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey("storage", valstr);
}

int GameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(rname);
}
void GameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(rname, val);
}

cocos2d::Vec2 GameDataSave::getHeroPos()
{
	int x = loadIntDataByKey("herox", 0);
	int y = loadIntDataByKey("heroy", 0);
	return Vec2(x, y);
}

void GameDataSave::setHeroPos(cocos2d::Vec2 val)
{
	saveIntDataByKey("herox", (int)val.x);
	saveIntDataByKey("heroy", (int)val.y);
}

std::string GameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(str);
}

void GameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(str, vstr);
}

std::string GameDataSave::getPackage()
{
	return loadStringDataByKey("packages");
}

void GameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey("packages", vstr);
}

std::string GameDataSave::getResData()
{
	return loadStringDataByKey("resdata");
}
void GameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey("resdata", vstr);
}